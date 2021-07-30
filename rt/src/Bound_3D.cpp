/*
***************************************************************************
*                                                                         *
*                             Bob Ray Tracer                              *
*                                                                         *
*           Bound.C = generate bounding slabs and tree structure          *
*                                                                         *
*       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     *
*                                                                         *
*       ALL RIGHTS RESERVED.   This software is published, but is NOT     *
*         Public Domain and remains the propery of ALGORITHM, Inc.,       *
*   Christopher D. Watkins and Stephen B. Coy.  This software may not be  *
*  reproduced or integrated into other packages without the prior written *
*          consent of Christopher D. Watkins and Stephen B. Coy.          *
*                                                                         *
*                       Requires: defs.h, extern.h                        *
*                                                                         *
***************************************************************************
*/
#include "Bound_3D.hpp"

#include "BobMath.hpp"
#include "Bob.hpp"
#include "Stats.hpp"
#include "Exception.hpp"
#include "defs.hpp"
//#include "extern.hpp"
//#include "struct_defs.hpp"

#include <iostream>

using std::cerr;
using std::cout;
using std::endl;

extern int tickflag;
extern int bunching;

long Bound_3D::total = 0; /* # objects in main list */
double Bound_3D::Median = 0; /* 2*median value along axis */
int Bound_3D::Axis   = 0; /* axis to split along */

long Bound_3D::nPrims = 0;

void Bound_3D::FindAxis(Object *top, long count) {
    double mins[NSLABS];
    double maxs[NSLABS];
    int i, j, which;
    double d = -HUGE_NUM, e, x, y, z;
    long cnt;

    cnt = count;
    for (i = 0; i < NSLABS; i++) { /* zero out min/max */
        mins[i] = HUGE_NUM;
        maxs[i] = -HUGE_NUM;
    }
    x = y = z = 0.0;

    while (count--) {
        if (top == NULL) {
            cerr << "NULL top in FindAxis, count = " << count << endl;
            throw Exception("thrown by FindAxis");
        }
        for (j = 0; j < NSLABS; j++) {
            e = top->o_dmin[j] + top->o_dmax[j];
            if (e < mins[j])
                mins[j] = e;
            if (e > maxs[j])
                maxs[j] = e;
        }
        x += top->o_dmin[0] + top->o_dmax[0];
        y += top->o_dmin[1] + top->o_dmax[1];
        z += top->o_dmin[2] + top->o_dmax[2];
        top = top->next;
    }

    for (i = 0; i < NSLABS; i++) {
        e = maxs[i] - mins[i];
        if (e > d) {
            d = e;
            which = i;
        }
    }

    Axis = which;
    switch (Axis) {
    case 0:
        Median = x / cnt;
        break;
    case 1:
        Median = y / cnt;
        break;
    case 2:
        Median = z / cnt;
        break;
    }
} /* end of FindAxis() */

int Bound_3D::SortAndSplit(Object **top_handle, long count) {
    Object *top = nullptr, *hi = nullptr, *lo = nullptr, *cur = nullptr, *tmp = nullptr, *hi_end = nullptr, *lo_end = nullptr;

    if (count <= 0)
        return 0;

    top = *top_handle;
    if (count > bunching) { /* need to split */
        FindAxis(top, count);
        long hi_cnt = 0L;
        long lo_cnt = 0L;
        cur = *top_handle;
        hi = (Object *)NULL;
        lo = (Object *)NULL;
        for (long i = 0; i < count; i++) {
            if (Median > (cur->o_dmin[Axis] + cur->o_dmax[Axis])) {
                lo_cnt++;
                tmp = cur;
                cur = cur->next;
                tmp->next = lo;
                if (lo == NULL) {
                    lo_end = tmp;
                }
                lo = tmp;
            } else {
                hi_cnt++;
                tmp = cur;
                cur = cur->next;
                tmp->next = hi;
                if (hi == NULL) {
                    hi_end = tmp;
                }
                hi = tmp;
            }
        } /* end of i loop */

        /*
            Fix if either list is length 0 by arbitrarily
            splitting into two.  Should never happen with
            new FindAxis code.
        */

        if (lo_cnt == 0) {
            lo_end = hi;
            for (long i = 0; i < count / 2; i++) {
                tmp = hi;
                hi = hi->next;
                tmp->next = lo;
                lo = tmp;
                hi_cnt--;
                lo_cnt++;
            }
        } else if (hi_cnt == 0) {
            hi_end = lo;
            for (long i = 0; i < count / 2; i++) {
                tmp = lo;
                lo = lo->next;
                tmp->next = hi;
                hi = tmp;
                lo_cnt--;
                hi_cnt++;
            }
        }

        /* insert sub-lists into main list */

        *top_handle = hi;
        hi_end->next = lo;
        lo_end->next = cur;
        SortAndSplit(&hi_end->next, lo_cnt);
        SortAndSplit(top_handle, hi_cnt);

        return 1;
    } else if (count > 1) { /* create a composite only if more than one object */
        Object *cp;
        CompositeData *cd;

        cp = new Object();
        Stats::trackMemoryUsage(sizeof(Object));


        cp->o_type = T_COMPOSITE;
        //TODO: TCE Remove:        cp->o_procs = &NullProcs;       /* die if you call any  */
        cp->o_surf = NULL; /* no surface...        */
        cd = new CompositeData();
        Stats::trackMemoryUsage(sizeof(CompositeData));

        cd->size = count;
        total = total - count + 1;

        /*
            Replace the objects in the main list with the
            new composite object.  The objects being removed
            are added to the new objects children list.
        */

        cd->children = top;
        *top_handle = cp;

        long cnt = count;
        while (--cnt) { /* make top point to last child object */
            top = top->next;
        }
        cp->next = top->next; /* connect new comp object to remainder of list */
        top->next = NULL;     /* last child's next pointer */

        /* calc bounding slabs for new composite object */

        for (long i = 0; i < NSLABS; i++) { /* for each slab */
            double dmin = HUGE_NUM;
            double dmax = -HUGE_NUM;
            top = cd->children; /* point to first child */
            for (long j = 0; j < count; j++) {
                if (top->o_dmin[i] < dmin)
                    dmin = top->o_dmin[i];
                if (top->o_dmax[i] > dmax)
                    dmax = top->o_dmax[i];
                top = top->next;
            }
            cp->o_dmin[i] = dmin;
            cp->o_dmax[i] = dmax;
        }
        cp->o_data = (void *)cd;
        ++nPrims;
        Stats::yystats();

        return 0;
    }
    return 0; /* only happens with list of length 1 */
} /* end of SortAndSplit() */

/*
 * This function attempts to use median cut
 * to generate tighter bounding volumes than the old
 * code...
 */
void Bound_3D::BuildBoundingSlabs(void) {
    total = nPrims;
    while (SortAndSplit(&Object_3D::Root, total))
        ; /* this line intentionally left blank */
//    cout << "In BuildBoundingSlabs tickflag = " << tickflag << endl;
    if (tickflag) {
        cout << "\n\tAfter adding bounding volumes, " << nPrims << " prims.\n";
        cout << "\tExtent of scene\n";
        cout << "\tX  " << Object_3D::Root->o_dmin[0] << "--" << Object_3D::Root->o_dmax[0] << "\n";
        cout << "\tY  " << Object_3D::Root->o_dmin[1] << "--" << Object_3D::Root->o_dmax[1] << "\n";
        cout << "\tZ  " << Object_3D::Root->o_dmin[2] << "--" << Object_3D::Root->o_dmax[2] << "\n";
    }
}

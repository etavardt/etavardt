/*
�������������������������������������������
�                                                                         �
�                             Bob Ray Tracer                              �
�                                                                         �
�           Bound.C = generate bounding slabs and tree structure	      �
�                                                                         �
�       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     �
�                                                                         �
�       ALL RIGHTS RESERVED.   This software is published, but is NOT     �
�         Public Domain and remains the propery of ALGORITHM, Inc.,       �
�   Christopher D. Watkins and Stephen B. Coy.  This software may not be  �
�  reproduced or integrated into other packages without the prior written �
�          consent of Christopher D. Watkins and Stephen B. Coy.          �
�                                                                         �
�                       Requires: defs.h, extern.h                        �
�                                                                         �
�������������������������������������������
*/

#include <cstdio>
#include <cstdlib>
#include "defs.hpp"
#include "struct_defs.hpp"
#include "extern.hpp"
#include "proto.hpp"
#include "Object_3D.hpp"

static long     total;          /* # objects in main list */
static Flt      Median;         /* 2*median value along axis */
static int      Axis;           /* axis to split along */


void    FindAxis(Object *top, long count)
{
	Flt	mins[NSLABS];
	Flt	maxs[NSLABS];
	int	i, j , which;
	Flt     d = -HUGE, e, x, y, z;
	long    cnt;

	cnt = count;
	for(i=0; i<NSLABS; i++) {       /* zero out min/max */
		mins[i] = HUGE;
		maxs[i] = -HUGE;
	}
	x = y = z = 0.0;

	while(count--) {
		if(top == NULL) {
			fprintf(stderr, "NULL top in FindAxis, count = %ld\n", count);
			exit(1);
		}
		for(j=0; j<NSLABS; j++) {
			e = top->o_dmin[j] + top->o_dmax[j];
			if(e < mins[j])
				mins[j] = e;
			if(e > maxs[j])
				maxs[j] = e;
		}
		x += top->o_dmin[0] + top->o_dmax[0];
		y += top->o_dmin[1] + top->o_dmax[1];
		z += top->o_dmin[2] + top->o_dmax[2];
		top = top->next;
	}

	for(i=0; i<NSLABS; i++) {
		e = maxs[i] - mins[i];
		if(e>d) {
			d = e;
			which = i;
		}
	}

	Axis = which;
	switch(Axis) {
		case 0 : Median = x/cnt; break;
		case 1 : Median = y/cnt; break;
		case 2 : Median = z/cnt; break;
	}
}       /* end of FindAxis() */


int     SortAndSplit(Object **top_handle, long count)
{
	Object  *top, *hi, *lo, *cur, *tmp, *hi_end, *lo_end;
	long    lo_cnt, hi_cnt, i, j;
	Flt     dmin, dmax;

	if(count <= 0)
		return 0;

	top = *top_handle;
	if(count > bunching) {          /* need to split */
		FindAxis(top, count);
		hi_cnt = 0;
		lo_cnt = 0;
		cur = *top_handle;
		hi = (Object *)NULL;
		lo = (Object *)NULL;
		for(i=0; i<count; i++) {
			if(Median > (cur->o_dmin[Axis] + cur->o_dmax[Axis])) {
				lo_cnt++;
				tmp = cur;
				cur = cur->next;
				tmp->next = lo;
				if(lo == NULL) {
					lo_end = tmp;
				}
				lo = tmp;
			} else {
				hi_cnt++;
				tmp = cur;
				cur = cur->next;
				tmp->next = hi;
				if(hi == NULL) {
					hi_end = tmp;
				}
				hi = tmp;
			}
		}       /* end of i loop */

		/*
			Fix if either list is length 0 by arbitrarily
			splitting into two.  Should never happen with
			new FindAxis code.
		*/

		if(lo_cnt == 0) {
			lo_end = hi;
			for(i=0; i<count/2; i++) {
				tmp = hi;
				hi = hi->next;
				tmp->next = lo;
				lo = tmp;
				hi_cnt--;
				lo_cnt++;
			}
		} else if(hi_cnt == 0) {
			hi_end = lo;
			for(i=0; i<count/2; i++) {
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
	} else if(count > 1) {          /* create a composite only if more than one object */
		Object          *cp;
		CompositeData   *cd;

		cp = (Object *)vmalloc(sizeof(Object));
		ptrchk(cp, "composite object");

		cp->o_type = T_COMPOSITE;
//TODO: TCE Remove:		cp->o_procs = &NullProcs;       /* die if you call any  */
		cp->o_surf = NULL;              /* no surface...        */
		cd = (CompositeData *)vmalloc(sizeof(CompositeData));
		ptrchk(cd, "composite data");
		cd->size = count;
		total = total-count+1;

		/*
			Replace the objects in the main list with the
			new composite object.  The objects being removed
			are added to the new objects children list.
		*/

		cd->children = top;
		*top_handle = cp;

		i = count;
		while(--i) {            /* make top point to last child object */
			top = top->next;
		}
		cp->next = top->next;   /* connect new comp object to remainder of list */
		top->next = NULL;       /* last child's next pointer */

		/* calc bounding slabs for new composite object */

		for(i=0; i<NSLABS; i++) {       /* for each slab */
			dmin = HUGE;
			dmax = -HUGE;
			top = cd->children;     /* point to first child */
			for(j=0; j<count; j++) {
				if(top->o_dmin[i] < dmin)
					dmin = top->o_dmin[i];
				if(top->o_dmax[i] > dmax)
					dmax = top->o_dmax[i];
				top = top->next;
			}
			cp->o_dmin[i] = dmin;
			cp->o_dmax[i] = dmax;
		}
		cp->o_data = (void *) cd;
		++nPrims;
		yystats();

		return 0;
	}
	return 0;       /* only happens with list of length 1 */
}       /* end of SortAndSplit() */

/*
 * This function attempts to use median cut
 * to generate tighter bounding volumes than the old
 * code...
 */
extern Object *Root;
void BuildBoundingSlabs(void)
{
	total = nPrims;
	while(SortAndSplit(&Root, total))
		;               /* this line intentionally left blank */
	if(tickflag) {
		printf("\n\tAfter adding bounding volumes, %ld prims.\n", nPrims);
		printf("\tExtent of scene\n");
		printf("\tX  %g -- %g\n", Root->o_dmin[0], Root->o_dmax[0]);
		printf("\tY  %g -- %g\n", Root->o_dmin[1], Root->o_dmax[1]);
		printf("\tZ  %g -- %g\n", Root->o_dmin[2], Root->o_dmax[2]);
	}
}

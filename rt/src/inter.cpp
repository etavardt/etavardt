/*
***************************************************************************
*                                                                         *
*                             Bob Ray Tracer                              *
*                                                                         *
*       Inter.C = routines that shuffle through the tree of bounding      *
*          objects and try to find the nearest intersection.                  *
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
#include "RayTrace_3D.hpp"

#include <cstdio>
#include <cmath>
#include <cassert>

#include "Bob.hpp"
#include "Object_3D.hpp"
#include "Isect_3D.hpp"
#include "defs.hpp"
#include "extern.hpp"


extern void PriorityQueueNull (void);
extern int  PriorityQueueEmpty (void);
extern void PriorityQueueInsert (double key , Object *obj);
extern void PriorityQueueDelete (double *key , Object **obj);

/*
 * intersect.c
 * Much nicer now, uses the nifty priority queue search
 * as suggested by Kajiya...
 */

double    RayTrace_3D::num[NSLABS];
double    RayTrace_3D::den[NSLABS];

/*******************************************************************************************************
 * CheckAndEnqueue(obj, maxdist)
 * Check the current ray (as paramaterized with the num and den
 * arrays above) against the bounding volume of obj.
 * If we intersect the bounding volume, then insert it into the
 * priority queue.
 *
 * Note: should be broken into two separate procedures...
 *******************************************************************************************************/
void RayTrace_3D::checkAndEnqueue(Object *obj, double maxdist)
{
    int i = 0;

    double    tmin, tmax;
    double    dmin = -HUGE_NUM;
    double    dmax = maxdist;

    nChecked++;

    for(i=0; i<NSLABS; i++) {

        if(den[i] == 0.0)
            continue;

        /* enters the slab here...    */
        tmin = (obj->o_dmin[i] - num[i]) / den[i];
        /* and exits here...        */
        tmax = (obj->o_dmax[i] - num[i]) / den[i];

        /* but we may have to swap...    */
        if(tmin < tmax) {
            /* if exited closer than we thought, update     */
            if(tmax < dmax)
                dmax = tmax;
            /* if entered farther than we thought, update     */
            if(tmin > dmin)
                dmin = tmin;
        } else {
            /* if exited closer than we thought, update     */
            if(tmin < dmax)
                dmax = tmin;
            /* if entered farther than we thought, update     */
            if(tmax > dmin)
                dmin = tmax;
        }

        if(dmin>dmax || dmax<Bob::rayeps)
            return;
    }
    PriorityQueueInsert(dmin, obj);
    nEnqueued++;
}

/*******************************************************************************************************
 * Intersect(ray, hit, maxdist, lastObjHit)
 *
 * Returns true if we hit something in the root model closer than maxdist.
 * Returns the closest hit in the "hit" buffer.

    lastObjHit is a pointer to the last object hit.  If lastObjHit is NULL then
    the ray either originated at the eye or the last object may be
    lastObjHit-intersecting ie spheres and cones.  This can be used to
    eliminate doing an intersection test with the last object.

 *******************************************************************************************************/
int RayTrace_3D::intersect(Ray *ray, Isect &hit, double maxdist, Object *lastObjHit)
{
    Isect          nhit;
    int            i;
    double            min_dist = maxdist;
    Object        *cobj, *child;
    Object        *pobj = NULL;
    CompositeData *cdp;
    double            key;

    /* If the object is simple, then return the hit that it gives you */

    if(Object_3D::Root->o_type != T_COMPOSITE)
        return Object_3D::Root->intersect(ray, hit);
        //        return (Root->o_procs->intersect) (Root, ray, hit);

    for(i=0; i<3; i++) {
        num[i] = ray->P[i];
        den[i] = ray->D[i];
    }

    /* start with an empty priority queue */
    PriorityQueueNull();

    RayTrace_3D::checkAndEnqueue(Object_3D::Root, maxdist);

    for (;;) {

        if(PriorityQueueEmpty())
            break;

        PriorityQueueDelete(&key, &cobj);

        if(key > min_dist) {

            /*
             * we have already found a primitive
             * that was closer, we need look no further...
             */
             break;

        } else if(cobj->o_type == T_COMPOSITE) {
            /*
             * if it is in the queue, it got hit.
             * check each of its children to see if their
             * bounding volumes get hit.
             * if so, then push them into the priority
             * queue...
             */

            cdp = (CompositeData *) cobj->o_data;
            child = cdp->children;

            while(child) {
                if(lastObjHit != child) {
                    RayTrace_3D::checkAndEnqueue(child, maxdist);
                }
                child = child->next;
            }

        } else {

            /*
             * we have a primitive
             * intersect with the primitive, and possibly
             * update the nearest hit if it is indeed closer
             * than the one we currently have...
             */

//            if((cobj->o_procs->intersect) (cobj, ray, &nhit)) {
            if(cobj->intersect(ray, nhit)) {
                if(nhit.isect_t < min_dist) {
                    pobj = cobj;
                    hit = nhit;
                    min_dist = nhit.isect_t;
                }
            }
        }
    }

    if (pobj)
        return 1;
    else
        return 0;
}

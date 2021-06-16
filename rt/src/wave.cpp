/*
�������������������������������������������
�                                                                         �
�                             Bob Ray Tracer                              �
�                                                                         �
�                 Wave.C = totally new wave functions - dude              �
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
#include <cmath>
#include "defs.hpp"
#include "extern.hpp"
#include "proto.hpp"

void    make_waves(Vec P, Vec R, Wave *waves)
//    Vec     P,      /* the point in question */
//        R;      /* where to put the result */
//    Wave    *waves; /* top of the linked list */
{
    Vec     diff;           /* diff between point and source */
    Flt     dist;           /* dist form point to source of wave */
    Flt     amp;            /* current height of wave */

    MakeVector(0, 0, 0, R);         /* just to be sure */
    while(waves) {
        VecSub(P, waves->center, diff);
        dist = VecNormalize(diff);
        dist /= waves->wavelength;      /* where in cycle are we? */
        dist += waves->phase;           /* add on offset */

        if(waves->damp < 1.0) {         /* account for damping */
            amp = waves->amp * pow(waves->damp, dist);
        } else {
            amp = waves->amp;       /* no damping */
        }

        amp *= cos(dist * PI*2.0);

        VecAddS(amp, diff, R, R);

        waves = waves->next;
    }       /* end of while loop */
}       /* end of make_waves() */

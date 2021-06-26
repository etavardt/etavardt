/*
�������������������������������������������
�                                                                         �
�                             Bob Ray Tracer                              �
�                                                                         �
�                    Error.C = error trapping routines                    �
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
// TODO: TCE Remove file or find another use
/*
 * various routines to print error messages and die...
 */
/*
int     NullIntersect(Object *a, Ray *b, Isect *c)
{
    fprintf(stderr, "Called non-existant intersect routine for bounding box, dying...\n");
    exit(1);
    return 1;       //* keep lint/compilers quiet * /
}

void    NullNormal(Object *a, Isect *b, Flt *c, Flt *d)
{
    fprintf(stderr, "Called non-existant normal routine for bounding box, dying...\n");
    exit(1);
}

ObjectProcs NullProcs = {
    NullIntersect,
    NullNormal
};
*/
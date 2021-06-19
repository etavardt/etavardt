#pragma once
/*
�������������������������������������������
�                                                                         �
�                             Bob Ray Tracer                              �
�                                                                         �
�                        Pic.H = typedef for Pi                           �
�                                                                         �
�       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     �
�                                                                         �
�       ALL RIGHTS RESERVED.   This software is published, but is NOT     �
�         Public Domain and remains the propery of ALGORITHM, Inc.,       �
�   Christopher D. Watkins and Stephen B. Coy.  This software may not be  �
�  reproduced or integrated into other packages without the prior written �
�          consent of Christopher D. Watkins and Stephen B. Coy.          �
�                                                                         �
�������������������������������������������
*/
#ifndef PIC_ALREADY

#define PIC_ALREADY

#include "String.hpp"
#include "struct_defs.hpp"

typedef struct Pic {
//    char    *filename;
    String filename;
    FILE    *filep;
    int    x, y;
} Pic;
extern Pic    *PicOpen(String &filename, int x, int y);
extern void  PicWriteLine (Pic *pic , Pixel *buf);
extern void  PicClose (Pic *pic);

#endif

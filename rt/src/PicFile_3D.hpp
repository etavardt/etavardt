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

#include <fstream>
#include "String.hpp"
#include "struct_defs.hpp"

class PicFile_3D {
    public:
    String filename;
    std::fstream fs;
    int    x, y;

    PicFile_3D(){}
    ~PicFile_3D(){}

    bool open(String &_filename, int _x, int _y);
    void writeLine (const Pixel buf[]);
    void close ();
};

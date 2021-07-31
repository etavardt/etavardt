#pragma once
/*
***************************************************************************
*                                                                         *
*                             Bob Ray Tracer                              *
*                                                                         *
*                        Pic.H = typedef for Pi                           *
*                                                                         *
*       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     *
*                                                                         *
*       ALL RIGHTS RESERVED.   This software is published, but is NOT     *
*         Public Domain and remains the propery of ALGORITHM, Inc.,       *
*   Christopher D. Watkins and Stephen B. Coy.  This software may not be  *
*  reproduced or integrated into other packages without the prior written *
*          consent of Christopher D. Watkins and Stephen B. Coy.          *
*                                                                         *
***************************************************************************
*/

#include <fstream>
#include "String.hpp"
#include "Color.hpp"

class PicFile_3D {
  public:
    int x, y;

    String filename;
    std::fstream fs;

    PicFile_3D() : x(0), y(0) {}
    ~PicFile_3D() {}

    bool open(const String &_filename, int _x, int _y);
    void writeLine(const Pixel buf[]);
    void close();

    PicFile_3D &operator=(const PicFile_3D &pic) { x = pic.x; y = pic.y; filename = pic.filename; return *this; }
};

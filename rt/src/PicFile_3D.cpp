/*
***************************************************************************
*                                                                         *
*                             Bob Ray Tracer                              *
*                                                                         *
*         Pic.C = file output routines for creating files compatible      *
*         with Bob's run length encoding.  The paranoid mode              *
*         operation forces the program to close the output file           *
*         every TIME_OUT seconds.  This ensures that in case of           *
*         a power outage you will at most loose the 60 seconds            *
*         of work or the current scan line, whichever is greater.         *
*                                                                         *
*       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     *
*                                                                         *
*       ALL RIGHTS RESERVED.   This software is published, but is NOT     *
*         Public Domain and remains the propery of ALGORITHM, Inc.,       *
*   Christopher D. Watkins and Stephen B. Coy.  This software may not be  *
*  reproduced or integrated into other packages without the prior written *
*          consent of Christopher D. Watkins and Stephen B. Coy.          *
*                                                                         *
*                Requires: pic.h, defs.h, extern.h                        *
*                                                                         *
***************************************************************************
*/
#include "PicFile_3D.hpp"

#include <ctime>
#include <iostream>
#include <fstream>

#include "Bob.hpp"
#include "Color.hpp"
#include "Screen_3D.hpp"
#include "Exception.hpp"
#include "String.hpp"
//#include "defs.hpp"
//#include "extern.hpp"

using std::cerr;
using std::cout;
using std::endl;

#define TIME_OUT (120) //(120) //(60) /* # seconds to trigger paranoid mode */

time_t old_time, new_time;

bool PicFile_3D::open(const String &_filename, int _x, int _y) {
    Bob &bob = Bob::getApp();

    filename = _filename;
    time(&old_time); /* get current time */

    x = _x;
    y = _y;

    if (bob.resume) { /* finish a partial image */
        /* find line where interrupted */
        int line = bob.start_line; /* line to start on */
        fs.open(filename, std::ios::in | std::ios::binary);
        if (!fs.is_open()) {
            cerr << "Error.  Trying to resume generation of " << filename << "." << endl;
            cerr << "        Can't open " << filename << " for reading." << endl;
            throw Exception("thrown from PicFile_3D::open");
        }
        /* skip header */
        fs.seekg(10);
        int i = 0;
        while (!fs.eof()) {
            int c = fs.get();
            if (fs.eof()) {
                break;
            }

            fs.get();
            fs.get();
            fs.get();

            i += c;
            if (i >= x) {
                i = 0;
                line++;
            }
        }
        fs.close();
        fs.clear();

        /* re-open and set to end */
        fs.open(filename, std::ios::ate | std::ios::binary);
        if (!fs.is_open()) {
            cerr << "Error.  Trying to resume generation of " << filename << endl;
            cerr << "        Can't open " << filename << " for appending." << endl;
            throw Exception("thrown from picOpen");
        }
        bob.start_line = line; /* fake start line */
    } else {               /* start a new image */
        fs.open(filename, std::ios::out | std::ios::binary);
        if (!fs.is_open()) {
            perror(filename.c_str());
            exit(1);
        }
        fs.put(x / 256); /* image size */
        fs.put(x % 256);
        fs.put(y / 256);
        fs.put(y % 256);

        fs.put(bob.start_line / 256); /* image range */
        fs.put(bob.start_line % 256);
        fs.put(bob.stop_line / 256);
        fs.put(bob.stop_line % 256);

        fs.put(0);
        fs.put(24);
    }

    return fs.is_open();
}

void PicFile_3D::writeLine(const Pixel buf[]) {
    int i = 0,          /* which pixel? */
        total,      /* how many left in scan? */
        // count,      /* current run total */
        cr, cg, cb, /* current run color */
        r, g, b;    /* next pixel color */
    double seconds; /* another helping? */

    total = x;
    cr = buf[i].r;
    cg = buf[i].g;
    cb = buf[i].b;
    i++;
    do {
        int count = 1;
        total--;
        while (1) {
            r = buf[i].r;
            g = buf[i].g;
            b = buf[i].b;
            i++;
            if (r != cr || g != cg || b != cb || count >= 254 || total <= 0) {
                break;
            }
            total--;
            count++;
        }
        fs.put(count);
        if (fs.eof()) {
            cerr << "Error writing to disk.  Must be out of space." << endl;
            throw Exception("thrown from PicFile_3D::writeLine");
        }
        // TODO: TCE: was RGB process backwards i.e. r is really b anb b is r?
        fs.put(cb);
        fs.put(cg);
        fs.put(cr);
        // fs.put(cr);
        // fs.put(cg);
        // fs.put(cb);

        cr = r;
        cg = g;
        cb = b;

        if (total == 1) { /* if at last pixel */
            fs.put(1);
            fs.put(buf[x - 1].b);
            fs.put(buf[x - 1].g);
            fs.put(buf[x - 1].r);
            // fs.put(buf[x - 1].r);
            // fs.put(buf[x - 1].g);
            // fs.put(buf[x - 1].b);
            total--;
        }
    } while (total > 0);
    // fflush(pic.ofs);
    fs.flush();

    time(&new_time);
    /* check time for paranoid mode */
    seconds = difftime(new_time, old_time);
    if (seconds > TIME_OUT) {
        if (!fs.good() || !fs.is_open()) {
            /* close, re-open, and set to end */
            fs.close();
            fs.clear();
            fs.open(filename, std::ios::ate | std::ios::binary);
            if (!fs.is_open()) {
                cerr << "Error opening " << filename << " for appending." << endl;
                throw Exception("thrown from PicFile_3D::writeLine");
            } else {
                cerr << "Opening " << filename << " for appending." << endl;
            }
        }
    }
    old_time = new_time;

} /* end of PicWriteLine() */

void PicFile_3D::close() {
    fs.close();
}

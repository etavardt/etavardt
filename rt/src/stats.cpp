/*
�������������������������������������������
�                                                                         �
�                             Bob Ray Tracer                              �
�                                                                         �
�      Stats.C = Display info about current trace in progress.  Note      �
�              that this assumes the use of ansi.sys.                     �
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

#include "Stats.hpp"
#include "Bob.hpp"
#include "Bound_3D.hpp"
#include "Object_3D.hpp"
#include "defs.hpp"
#include "extern.hpp"
#include "struct_defs.hpp"
//#include <cstdio>
//#include <cstdlib>
//#include <cstring>
#include <time.h>

unsigned long Stats::memAllocated = 0;

void Stats::statistics(int line) {

    Bob::clearScreen();

    cout << "\t\t\t\t" << Bob::_Program << "\t\t" << Bob::_Version << "\n\t" << Bob::_Copyright << endl;
    cout << "\ninput file \"" << Bob::getApp().infilename << "\"  memory " << memAllocated << "  resolution " << Xresolution << "x" << Yresolution << "  ";
    cout << "line " << line << endl;

    cout << "total rays cast\t\t" << nRays << endl;
    cout << "\teye rays\t" << nRays - nReflected - nRefracted << endl;
    cout << "\treflected rays\t" << nReflected << endl;
    cout << "\trefracted rays\t" << nRefracted << endl << endl;

    cout << "shadow rays\t\t" << nShadows << endl;
    cout << "cache hits\t\t" << nShadowCacheHits << endl;
    if (nShadows > 0) {
        cout << "cache percent\t" << 100.0 * (Flt)nShadowCacheHits / (Flt)nShadows << endl << endl;
    } else {
        cout << "cache percent\n" << endl;
    }

    cout << "avg rays/pixel\t" << (Flt)nRays / ((Flt)(line + 1 - start_line) * (Flt)Xresolution) << endl;
    cout << "avg queues/ray\t" << (Flt)totalQueues / (Flt)totalQueueResets << endl << endl;

    cout << "bounds checked\t\t" << nChecked << endl;
    cout << "queue inserts\t\t" << totalQueues << endl;
    cout << "queue resets\t\t" << totalQueueResets << endl;
    cout << "max queue size\t\t" << maxQueueSize << endl;

    cout << "\nmax recursion depth       " << deepest + 1 << "/" << maxlevel << "  " << endl;
    cout << endl;

} /* end of statistics() */

void Stats::yystats(void) {
    static int toc = 0;
    if (tickflag && ((toc & 0x0f) == 0)) {
        Bob::clearScreen();
        cout << "\tlights " << nLights << " prims " << Bound_3D::nPrims << " memory " << memAllocated << endl;
    }
    toc++;
}

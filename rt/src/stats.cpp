/*
***********************************************************************************************************
*                                                                         *
*                             Bob Ray Tracer                              *
*                                                                         *
*      Stats.C = Display info about current trace in progress.  Note      *
*              that this assumes the use of ansi.sys.                     *
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
***********************************************************************************************************
*/

#include "Stats.hpp"

#include <time.h>

#include "Bob.hpp"
#include "Bound_3D.hpp"
#include "Object_3D.hpp"
#include "Parser.hpp"
#include "RayTrace_3D.hpp"

#include "extern.hpp"

extern int tickflag;

Bob &Stats::bob = Bob::getApp();
unsigned long Stats::memAllocated = 0;

void Stats::statistics(int line) {

    Bob::clearScreen();

    cout << "\t\t\t\t" << Bob::_Program << "\t\t" << Bob::_Version << "\n\t" << Bob::_Copyright << endl;
    cout << "\ninput file \"" << bob.infilename << "\"  memory " << memAllocated << "  resolution " << Parser::xResolution << "x" << Parser::Parser::yResolution << "  ";
    cout << "line " << line << endl;

    cout << "total rays cast\t\t" << RayTrace_3D::nRays << endl;
    cout << "\teye rays\t" << RayTrace_3D::nRays - RayTrace_3D::nReflected - RayTrace_3D::nRefracted << endl;
    cout << "\treflected rays\t" << RayTrace_3D::nReflected << endl;
    cout << "\trefracted rays\t" << RayTrace_3D::nRefracted << endl << endl;

    cout << "shadow rays\t\t" << RayTrace_3D::nShadows << endl;
    cout << "cache hits\t\t" << nShadowCacheHits << endl;
    if (RayTrace_3D::nShadows > 0) {
        cout << "cache percent\t" << 100.0 * (double)nShadowCacheHits / (double)RayTrace_3D::nShadows << endl << endl;
    } else {
        cout << "cache percent\n" << endl;
    }

    cout << "avg rays/pixel\t" << (double)RayTrace_3D::nRays / ((double)(line + 1 - bob.start_line) * (double)Parser::xResolution) << endl;
    cout << "avg queues/ray\t" << (double)totalQueues / (double)totalQueueResets << endl << endl;

    cout << "bounds checked\t\t" << nChecked << endl;
    cout << "queue inserts\t\t" << totalQueues << endl;
    cout << "queue resets\t\t" << totalQueueResets << endl;
    cout << "max queue size\t\t" << maxQueueSize << endl;

    cout << "\nmax recursion depth       " << RayTrace_3D::deepest + 1 << "/" << RayTrace_3D::maxlevel << "  " << endl;
    cout << endl;

} /* end of statistics() */

void Stats::yystats(void) {
    static int toc = 0;
    if (tickflag && ((toc & 0x0f) == 0)) {
        Bob::clearScreen();
        cout << "\tlights " << Parser::nLights << " prims " << Bound_3D::nPrims << " memory " << memAllocated << endl;
    }
    toc++;
}

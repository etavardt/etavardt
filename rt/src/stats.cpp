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

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <time.h>
#include "Bob.hpp"
#include "Stats.hpp"
#include "defs.hpp"
#include "struct_defs.hpp"
#include "extern.hpp"
#include "Object_3D.hpp"

//#define ESC     27
unsigned long Stats::memAllocated = 0;

void Stats::statistics(int line)
{
    static int      first_call = 1;
//system("CLS");
    if(first_call) {
        for(int i=0; i<30; i++) cout << endl;
        if(tickflag) {
            //system("CLS");
            //cout << char(ESC) << "[2J";
//            cout << (String(itoa(ESC))+"[2J");
//            printf("%c[2J", ESC);   /* clear screen 1st time around */
        } else {
            cout << "\n";
//            printf("\n");
        }
        first_call = 0;
    }

    system("CLS");
//    printf("%c[1;1f", ESC);         /* home cursor */

    cout << "\t\t\t\t" << Bob::_Program <<"\t\t" << Bob::_Version << "\n\t" << Bob::_Copyright << endl;
    cout << "\ninput file \"" << Bob::getApp().infilename << "\"  memory " << memAllocated << "  resolution " << Xresolution << "x" << Yresolution << "  ";
    cout << "line " << line << endl;

    cout << "total rays cast\t\t" << nRays << endl;
    cout << "\teye rays\t" << nRays-nReflected-nRefracted << endl;
    cout << "\treflected rays\t" << nReflected << endl;
    cout << "\trefracted rays\t" << nRefracted << endl << endl;

    cout << "shadow rays\t\t" << nShadows << endl;
    cout << "cache hits\t\t" << nShadowCacheHits << endl;
    if(nShadows>0) {
        cout << "cache percent\t" << 100.0*(Flt)nShadowCacheHits/(Flt)nShadows << endl << endl;
    } else {
        cout << "cache percent\n" << endl;
    }

    cout << "avg rays/pixel\t" << (Flt)nRays / ((Flt)(line+1-start_line)*(Flt)Xresolution) << endl;
    cout << "avg queues/ray\t" << (Flt)totalQueues / (Flt) totalQueueResets << endl << endl;

    cout << "bounds checked\t\t" << nChecked << endl;
    cout << "queue inserts\t\t" << totalQueues << endl;
    cout << "queue resets\t\t" << totalQueueResets << endl;
    cout << "max queue size\t\t" << maxQueueSize << endl;

    cout << "\nmax recursion depth       " << deepest+1 << "/" << maxlevel << "  " << endl;
    cout << endl;
 
/*
    printf("\n\t\t%s\t\t       %s\n\t\t%s\n", Bob::_Program, Bob::_Version, Bob::_Copyright);

    printf("\ninput file \"%s\"  memory %lu  resolution %1dx%1d  ", Bob::getApp().infilename, MemAllocated, Xresolution, Yresolution);
    printf("line %1d\n", line);

    printf("total rays cast\t\t%8lu\n", nRays);
    printf("\teye rays\t%8lu\n", nRays-nReflected-nRefracted);
    printf("\treflected rays\t%8lu\n", nReflected);
    printf("\trefracted rays\t%8lu\n\n", nRefracted);

    printf("shadow rays\t\t%8lu\n", nShadows);
    printf("cache hits\t\t%8lu\n", nShadowCacheHits);
    if(nShadows>0) {
        printf("cache percent\t%16.6f\n\n", 100.0*(Flt)nShadowCacheHits/(Flt)nShadows);
    } else {
        printf("cache percent\n\n");
    }

    printf("avg rays/pixel\t%16.6f\n", (Flt)nRays / ((Flt)(line+1-start_line)*(Flt)Xresolution));
    printf("avg queues/ray\t%16.6f\n\n", (Flt)totalQueues / (Flt) totalQueueResets);

    printf("bounds checked\t\t%8lu\n", nChecked);
    printf("queue inserts\t\t%8lu\n", totalQueues);
    printf("queue resets\t\t%8lu\n", totalQueueResets);
    printf("max queue size\t\t%8lu\n", maxQueueSize);

    printf("\nmax recursion depth       %3d/%d  ", deepest+1, maxlevel);

    fflush(stdout);
    */
}       /* end of statistics() */

void Stats::yystats(void) {
    static int toc = 0;
//    cout << "In Stats::yystats tickflag = " << tickflag << " toc = " << toc << " toc&0x0f = " << (toc & 0x0f) << endl;
    if (tickflag && ((toc & 0x0f) == 0)) {
        system("CLS");
        cout << "\tlights " << nLights << " prims " << nPrims << " memory " << memAllocated << endl;
    }
    toc++;
}


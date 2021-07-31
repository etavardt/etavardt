/*
***************************************************************************
*                                                                         *
*                             Bob Ray Tracer                              *
*                                                                         *
*        File.C = functions to handle opening of files taking into        *
*           consideration environment variables.                           *
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

#include <iostream>
//#include <cstdio>
#include "Bob.hpp"
#include "String.hpp"
#include <algorithm>
#include <cstdlib>
#include <cstring>

using std::cerr;
using std::cout;
using std::endl;
using std::replace;

/*
    qstrcat() -- just like strcat() except ignores quotes
*/

void qstrcat(char *d, char *s) {
    /* find end of dest */
    while (*d)
        d++;

    /* tack on source */
    while (*s) {
        if (*s != '"') {
            *d = *s;
            d++;
        }
        s++;
    }
    *d = *s; /* copy final NULL */
} /* end of qstrcat() */

/*
    env_fopen() -- acts just like fopen except uses the paths
        taken from the envoronment variable BOB.  Also
        handles filenames wrapped in quotes.
*/
FILE *env_fopen(const String &name, const String &mode) {
    FILE *fp;

//    cout << "In env_fopen Bob::paths.size(): " << Bob::paths.size() << endl;
    for (size_t i = 0; i < Bob::paths.size(); i++) {
        String full_path;
//        cout << "In env_fopen Bob::paths[" << i << "]: " << Bob::paths[i] << endl;
        full_path = Bob::paths[i];
//        cout << "In env_fopen full_path: " << full_path << endl;
        if (i) {
            full_path += "\\";
        }
//        cout << "In env_fopen full_path: " << full_path << endl;
        full_path += name;
#ifndef WINDOWS
        // it is not Windows so use the other slash
        replace(full_path.begin(), full_path.end(), '\\', '/');
#endif
//        cout << "In env_fopen full_path: " << full_path << endl;
        fp = fopen(full_path.c_str(), mode.c_str());
//        cout << "In env_fopen fp: " << fp << endl;
        if (fp) {
//            cout << "In env_fopen exiting with fp:" << fp << endl;
            return fp;
        }
    }
//    cout << "In env_fopen exiting with NULL" << endl;
    /* nothing worked so return NULL */
    return NULL;

} /* end of env_fopen() */

/*
�������������������������������������������
�                                                                         �
�                             Bob Ray Tracer                              �
�                                                                         �
�        File.C = functions to handle opening of files taking into        �
�           consideration environment variables.                           �
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

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "Bob.hpp"
#include "String.hpp"

using std::cout;
using std::cerr;
using std::endl;

/*
    qstrcat() -- just like strcat() except ignores quotes
*/

void qstrcat(char *d, char *s)
{
    /* find end of dest */
    while(*d)
        d++;

    /* tack on source */
    while(*s) {
        if(*s != '"') {
            *d = *s;
            d++;
        }
        s++;
    }
    *d = *s;        /* copy final NULL */
}       /* end of qstrcat() */

/*
    env_fopen() -- acts just like fopen except uses the paths
        taken from the envoronment variable BOB.  Also
        handles filenames wrapped in quotes.
*/

FILE *env_fopen(char *name, const char *mode) {
    FILE            *fp;
    int             i;
//    static char     full_path[1024];
	String full_path;
//	cout << "Bob::paths.size(): " << Bob::paths.size() << endl;
//    for(i=0; i<Bob::num_paths; i++) {
	for(i=0; i<Bob::paths.size(); i++) {
//		cout << "Bob::paths[" << i << "]: " << Bob::paths[i] << endl;
		full_path = Bob::paths[i].c_str();
//        strcpy(full_path, Bob::paths[i].c_str());
        if(i) {
			full_path.append("\\");
//            strcat(full_path, "/");
        }
//		cout << "full_path: " << full_path << endl;
		full_path.append(name);
//        qstrcat(full_path, name);
//		cout << "full_path: " << full_path << endl;
        fp = fopen(full_path.c_str(), mode);
//		cout << "fp: " << fp << endl;
        if(fp) {
            return fp;
        }
    }

    /* nothing worked so return NULL */
    return NULL;

}       /* end of env_fopen() */

/*
�������������������������������������������
�                                                                         �
�                             Bob Ray Tracer                              �
�                                                                         �
�                  YYStuff.C = additional parsing routines                �
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

    Additional parse routines put here to try and keep parse.c
    done to a reasonable size.  Doesn't help much, does it?
*/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "Exception.hpp"
#include "String.hpp"
#include "defs.hpp"
#include "extern.hpp"
#include "proto.hpp"

using std::cout;
using std::cerr;
using std::endl;

void yyerror(const char *str) {
    Infile  *iptr;

    iptr = InfileTop->what;
    cerr << "\n\nError at line " << (yylinecount+1) << endl;
    cerr << "file \"" << iptr->file_name << "\"" << endl;
    cerr << str << endl;
    cerr << "Error at or near = \"" << cur_text << "\"" << endl;
    throw Exception("thrown by yyerror");
}

void    ReadSceneFile(const char *real_name, char *tmp_name)
{
    Infile  *iptr;

    if((yyin = env_fopen(tmp_name, String("r").data())) == NULL) {
        cerr << "Error, cannot open " << tmp_name << endl;
        throw Exception("thrown by ReadSceneFile");
    }
    strcpy(Infilename, real_name);

    /* set up input file stack */
    InfileTop = (Stack *)malloc(sizeof(Stack));
    ptrchk(InfileTop, "input file stack object");
    InfileTop->prev = NULL;

    iptr = (Infile *)malloc(sizeof(Infile));
    ptrchk(iptr, "input file structure");
    InfileTop->what = iptr;

    iptr->file_name = strdup(Infilename);
    ptrchk(iptr->file_name, "input file name");
    iptr->line = 0;

    /* parse the input file */
    if(yyparse() == 1) {
        cerr << "Invalid input specification" << endl;
        throw Exception("thrown by ReadSceneFile");
    }

    /* clean up transform structures */
    while(TransTop)
        trans_pop();

    if(stop_line == -1) {
        cerr << "\n\nError, no studio structure in input file." << endl;
        throw Exception("thrown by ReadSceneFile");
    }

    if(tickflag) {
        cout << "\n\tinputfile = \"" << Infilename << "\"" << endl;
        cout << "\tlights " << nLights << " prims " << nPrims <<"\n" << endl;
        cout << "\tresolution " << Xresolution << " " << Yresolution << endl;
    }
}

void ptrchk(void *ptr, const char *str) {
    if (!ptr)
        throw Exception(String("Error allocating memory for a ").append(str).append("\n").append("thrown by ReadSceneFile"));
}

void    yystats(void)
{
    static int      toc;

    if(tickflag && ((toc&0x0f)==0)) {
        cout << "\n\tlights " << nLights << " prims " << nPrims << " memory " << MemAllocated << endl;
    }
    toc++;
}

void    yy_newfile(char *new_file)
{
    Infile  *iptr;
    Stack   *sptr;

    iptr = InfileTop->what;         /* save line number for current file */
    iptr->line = yylinecount;

    sptr = (Stack *)vmalloc(sizeof(Stack));
    ptrchk(sptr, "input file stack object");
    sptr->prev = InfileTop;
    InfileTop = sptr;

    iptr = (Infile *)vmalloc(sizeof(Infile));
    ptrchk(iptr, "input file structure");
    InfileTop->what = iptr;

    iptr->file_name = strdup(new_file);
    iptr->line = 0;
    yylinecount = -1;

}       /* end of yy_newfile() */

void    yy_popfile()
{
    Infile  *iptr;
    Stack   *sptr;

    if(InfileTop->prev) {
        iptr = InfileTop->what;
        sptr = InfileTop;
        InfileTop = InfileTop->prev;
        iptr = InfileTop->what;
        yylinecount = iptr->line;
    } else {
        cerr << "\nTemp input file corrupted.  Dying" << endl;
        throw Exception("thrown by yy_popfile");
    }
}

void    trans_pop()
{
    Transform       *tptr;

    tptr = TransTop;
    if(tptr == NULL) {
        yyerror("Trying to pop a transformation from an empty stack.");
    }
    TransTop = TransTop->next;
}



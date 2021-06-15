/*
�������������������������������������������������������������������������ͻ
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
�������������������������������������������������������������������������ͼ

	Additional parse routines put here to try and keep parse.c
	done to a reasonable size.  Doesn't help much, does it?
*/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include "defs.hpp"
#include "extern.h"

typedef std::string string;

int     yyerror(const char *str)
{
	Infile  *iptr;

	iptr = InfileTop->what;
	fprintf(stderr, "\n\nError at line %d\n", yylinecount+1);
	fprintf(stderr, "file \"%s\"\n", iptr->file_name);
	fprintf(stderr, "%s\n", str);
	fprintf(stderr, "Error at or near = \"%s\"", cur_text);
	exit(1);

	return 0;       /* keep lint/compilers quiet */
}

void    ReadSceneFile(const char *real_name, char *tmp_name)
{
	Infile  *iptr;

	if((yyin = env_fopen(tmp_name, string("r").data())) == NULL) {
		fprintf(stderr, "Error, cannot open %s\n", tmp_name);
		exit(1);
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
		fprintf(stderr, "Invalid input specification\n");
		exit(1);
	}

	/* clean up transform structures */
	while(TransTop)
		trans_pop();

	if(stop_line == -1) {
		fprintf(stderr, "\n\nError, no studio structure in input file.\n");
		exit(1);
	}

	if(tickflag) {
		fprintf(stderr, "%c\tinputfile = \"%s\"                    \n", 13, Infilename);
		fprintf(stderr, "\tlights %d, prims %lu\n", nLights, nPrims);
		fprintf(stderr, "\tresolution %d %d\n", Xresolution, Yresolution);
	}
}

void    ptrchk(void *ptr, const char *str)
{
	if(ptr)
		return;

	fprintf(stderr, "\n\nError allocating memory for a %s.\n", str);
	exit(1);
}

void    yystats(void)
{
	static int      toc;

	if(tickflag && ((toc&0x0f)==0)) {
		fprintf(stderr, "%c\tlights %d, prims %ld, memory %lu", 13, nLights, nPrims, MemAllocated);
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
		fprintf(stderr, "\nTemp input file corrupted.  Dying\n");
		exit(1);;
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



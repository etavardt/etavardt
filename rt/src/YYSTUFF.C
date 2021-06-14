/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                             Bob Ray Tracer                              บ
บ                                                                         บ
บ                  YYStuff.C = additional parsing routines                บ
บ                                                                         บ
บ       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     บ
บ                                                                         บ
บ       ALL RIGHTS RESERVED.   This software is published, but is NOT     บ
บ         Public Domain and remains the propery of ALGORITHM, Inc.,       บ
บ   Christopher D. Watkins and Stephen B. Coy.  This software may not be  บ
บ  reproduced or integrated into other packages without the prior written บ
บ          consent of Christopher D. Watkins and Stephen B. Coy.          บ
บ                                                                         บ
บ                       Requires: defs.h, extern.h                        บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

	Additional parse routines put here to try and keep parse.c
	done to a reasonable size.  Doesn't help much, does it?
*/

#include <stdio.h>
#include <string.h>
#include "defs.h"
#include "extern.h"


int     yyerror(char *str)
{
	Infile  *iptr;

	iptr = (Infile *)InfileTop->what;
	fprintf(stderr, "\n\nError at line %d\n", yylinecount+1);
	fprintf(stderr, "file \"%s\"\n", iptr->file_name);
	fprintf(stderr, "%s\n", str);
	fprintf(stderr, "Error at or near = \"%s\"", cur_text);
	last_call(1);

	return 0;       /* keep lint/compilers quiet */
}

void    ReadSceneFile(char *real_name, char *tmp_name)
{
	Infile  *iptr;

	if((yyin = env_fopen(tmp_name, "r")) == NULL) {
		fprintf(stderr, "Error, cannot open %s\n", tmp_name);
		last_call(1);
	}
	strcpy(Infilename, real_name);

	/* set up input file stack */
	InfileTop = (Stack *)malloc(sizeof(Stack));
	ptrchk(InfileTop, "input file stack object");
	InfileTop->prev = NULL;

	iptr = (Infile *)malloc(sizeof(Infile));
	ptrchk(iptr, "input file structure");
	InfileTop->what = (void *)iptr;

	iptr->file_name = strdup(Infilename);
	ptrchk(iptr->file_name, "input file name");
	iptr->line = 0;

	/* parse the input file */
	if(yyparse() == 1) {
		fprintf(stderr, "Invalid input specification\n");
		last_call(1);
	}

	/* clean up transform structures */
	while(TransTop)
		trans_pop();

	if(stop_line == -1) {
		fprintf(stderr, "\n\nError, no studio structure in input file.\n");
		last_call(1);
	}

	if(tickflag) {
		fprintf(stderr, "%c\tinputfile = \"%s\"                    \n", 13, Infilename);
		fprintf(stderr, "\tlights %d, prims %lu\n", nLights, nPrims);
		fprintf(stderr, "\tresolution %d %d\n", Xresolution, Yresolution);
	}
}

void    ptrchk(void *ptr, char *str)
{
	if(ptr)
		return;

	fprintf(stderr, "\n\nError allocating memory for a %s.\n", str);
	last_call(1);
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
	InfileTop->what = (void *)iptr;

	iptr->file_name = strdup(new_file);
	iptr->line = 0;
	yylinecount = -1;

}       /* end of yy_newfile() */

void    yy_popfile()
{
	Infile  *iptr;
	Stack   *sptr;

	if(InfileTop->prev) {
		iptr = (Infile *)InfileTop->what;
		sptr = InfileTop;
		InfileTop = InfileTop->prev;
		iptr = (Infile *)InfileTop->what;
		yylinecount = iptr->line;
	} else {
		fprintf(stderr, "\nTemp input file corrupted.  Dying\n");
		last_call(1);;
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



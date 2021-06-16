/*
�������������������������������������������
�                                                                         �
�                             Bob Ray Tracer                              �
�                                                                         �
�         Pic.C = file output routines for creating files compatible      �
�         with Bob's run length encoding.  The paranoid mode              �
�         operation forces the program to close the output file           �
�         every TIME_OUT seconds.  This ensures that in case of           �
�         a power outage you will at most loose the 60 seconds            �
�         of work or the current scan line, whichever is greater.         �
�                                                                         �
�       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     �
�                                                                         �
�       ALL RIGHTS RESERVED.   This software is published, but is NOT     �
�         Public Domain and remains the propery of ALGORITHM, Inc.,       �
�   Christopher D. Watkins and Stephen B. Coy.  This software may not be  �
�  reproduced or integrated into other packages without the prior written �
�          consent of Christopher D. Watkins and Stephen B. Coy.          �
�                                                                         �
�                Requires: pic.h, defs.h, extern.h                        �
�                                                                         �
�������������������������������������������
*/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "pic.hpp"
#include "defs.hpp"
#include "struct_defs.hpp"
#include "extern.hpp"
#include "proto.hpp"

#define TIME_OUT        (60)    /* # seconds to trigger paranoid mode */

time_t  old_time, new_time;

Pic    *PicOpen(char    *filename, int x, int y)
{
    Pic    *tmp;
    int     line;   /* line to start on */
    int     i, c;

    time(&old_time);        /* get current time */

    tmp = (Pic *)malloc(sizeof(Pic));
    ptrchk(tmp, "Pic structure");
    tmp->filename = (char *)malloc(strlen(filename)+1);
    strcpy(tmp->filename, filename);
    tmp->x = x;
    tmp->y = y;

    if(resume) {            /* finish a partial image */
        /* find line where interrupted */
        line = start_line;
        if(((tmp->filep)=fopen(filename, "rb"))==NULL) {
            fprintf(stderr, "Error.  Trying to resume generation of %s.\n", filename);
            fprintf(stderr, "        Can't open %s for reading.\n", filename);
            exit(1);
        }
        /* skip header */
        for(i=0; i<10; i++) {
            fgetc(tmp->filep);
        }
        i = 0;
        while(1) {
            c = fgetc(tmp->filep);
            if(c == EOF) {
                break;
            }
            fgetc(tmp->filep);
            fgetc(tmp->filep);
            fgetc(tmp->filep);
            i += c;
            if(i >= x) {
                i = 0;
                line++;
            }
        }
        fclose(tmp->filep);

        /* re-open and set to end */
        if(((tmp->filep)=fopen(filename, "ab"))==NULL) {
            fprintf(stderr, "Error.  Trying to resume generation of %s.\n", filename);
            fprintf(stderr, "        Can't open %s for appending.\n", filename);
            exit(1);
        }
        fseek(tmp->filep, 0L, SEEK_END);
        start_line = line;              /* fake start line */
    } else {                /* start a new image */
        if(((tmp->filep)=fopen(filename, "wb"))==NULL) {
            perror(filename);
            exit(1);
        }

        fputc(x/256, tmp->filep);       /* image size */
        fputc(x%256, tmp->filep);
        fputc(y/256, tmp->filep);
        fputc(y%256, tmp->filep);
    
        fputc(start_line/256, tmp->filep);      /* image range */
        fputc(start_line%256, tmp->filep);
        fputc(stop_line/256, tmp->filep);
        fputc(stop_line%256, tmp->filep);

        fputc(    0, tmp->filep);
        fputc(   24, tmp->filep);       /* # bitplanes */
    }

    return(tmp);
}

void    PicWriteLine(Pic *pic, Pixel *buf)
{
    int    i,        /* which pixel? */
        total,        /* how many left in scan? */
        count,        /* current run total */
        cr, cg, cb,    /* current run color */
        r, g, b;    /* next pixel color */
    double  seconds;        /* another helping? */

    i = 0;
    total = pic->x;
    cr = buf[i].r;
    cg = buf[i].g;
    cb = buf[i].b;
    i++;
    do {
        count = 1;
        total--;
        while(1) {
            r = buf[i].r;
            g = buf[i].g;
            b = buf[i].b;
            i++;
            if(r!=cr || g!=cg || b!=cb || count>=254 || total<=0) {
                break;
            }
            total--;
            count++;
        }
        if(fputc(count, pic->filep) == EOF) {
            fprintf(stderr, "Error writing to disk.  Must be out of space.\n");
            exit(1);
        }
        fputc(cb, pic->filep);
        fputc(cg, pic->filep);
        fputc(cr, pic->filep);

        cr = r;
        cg = g;
        cb = b;

        if(total==1) {        /* if at last pixel */
            fputc(1, pic->filep);
            fputc(buf[pic->x-1].b, pic->filep);
            fputc(buf[pic->x-1].g, pic->filep);
            fputc(buf[pic->x-1].r, pic->filep);
            total--;
        }
    } while(total>0);
    fflush(pic->filep);

    /* check time for paranoid mode */
    time(&new_time);
    seconds = difftime(new_time, old_time);
    if(seconds > TIME_OUT) {
        old_time = new_time;
        /* close, re-open, and set to end */
        fclose(pic->filep);
        if(((pic->filep)=fopen(pic->filename, "ab"))==NULL) {
            fprintf(stderr, "Error opening %s for appending.\n", pic->filename);
            exit(1);
        }
        fseek(pic->filep, 0L, SEEK_END);
    }

}       /* end of PicWriteLine() */
            
void    PicClose(Pic *pic)
{
    fclose(pic->filep);
}


/*
�������������������������������������������
�                                                                         �
�                             Bob Ray Tracer                              �
�                                                                         �
�                   PQueue.C = priority queue routines                    �
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
#include <cmath>
#include "defs.hpp"
#include "extern.hpp"

typedef struct t_qelem {
    double    q_key;
    Object    *q_obj;
} Qelem;

static int    Qsize;
Qelem    Q[PQSIZE];

void    PriorityQueueNull(void)
{
    Qsize = 0;
    totalQueueResets ++;
}

int     PriorityQueueEmpty(void)
{
    return (Qsize == 0);
}

void    PriorityQueueInsert(
        double key,
        Object *obj)
{
    int    i = 0;
    Qelem        tmp;

    totalQueues++;
     Qsize++;
    if(Qsize > maxQueueSize)
        maxQueueSize = Qsize;
    if(Qsize >= PQSIZE) {
        fprintf(stderr, "Exhausted priority queue space, dying...\n");
        exit(1);
    }
    Q[Qsize].q_key = key;
    Q[Qsize].q_obj = obj;

    i = Qsize;
    while(i>1 && Q[i].q_key<Q[i/2].q_key) {
        tmp = Q[i];
        Q[i] = Q[i/2];
        Q[i/2] = tmp;
        i = i / 2;
    }
}

void    PriorityQueueDelete(
        double *key,
        Object **obj)
{
    Qelem    tmp;
    int    i, j;

    if(Qsize == 0) {
        printf("Priority queue is empty, dying...\n");
        exit(1);
    }

    *key = Q[1].q_key;
    *obj = Q[1].q_obj;

    Q[1] = Q[Qsize];
    Qsize--;

    i = 1;

    while(2*i <= Qsize) {

        if(2*i == Qsize) {
            j = 2*i;
        } else if (Q[2*i].q_key < Q[2*i+1].q_key) {
            j = 2*i;
        } else {
            j = 2*i + 1;
        }

        if (Q[i].q_key > Q[j].q_key) {
            tmp = Q[i];
            Q[i] = Q[j];
            Q[j] = tmp;
            i = j;
        } else {
            break;
        }
    }
}

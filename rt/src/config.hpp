/*
*******************************************
*                                                                         *
*                             Bob Ray Tracer                              *
*                                                                         *
*                 Config.H = configuration specific material              *
*                                                                         *
*       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     *
*                                                                         *
*       ALL RIGHTS RESERVED.   This software is published, but is NOT     *
*         Public Domain and remains the propery of ALGORITHM, Inc.,       *
*   Christopher D. Watkins and Stephen B. Coy.  This software may not be  *
*  reproduced or integrated into other packages without the prior written *
*          consent of Christopher D. Watkins and Stephen B. Coy.          *
*                                                                         *
*******************************************
*/

/* OS and compiler specific garbage */

//#define index strchr

/* Bob specific garbage */

#define NSLABS (3)
#define BUNCHINGFACTOR (4)
#define PQSIZE (1000)

#define L_SAMPLES (8)     /* default # samples for spherical lights */
#define MIN_LIGHT (0.005) /* min spotlight size */

#define F_SAMPLES (8) /* default # samples for depth of field */

#define MAXLEVEL (20)      /* max recursion level, start at 0 */
#define MINWEIGHT (0.0001) /* min weight for a ray to be considered */

#define MAX_TOKEN (80)  /* max token length */
#define MAX_PARAMS (10) /* max number of parameters for file */

#define NLAMBDA (3) /* not used anywhere */

/***********************************************************************
 * If your compiler doesn't grok the void type, then define NO_VOID
 * here...
 ***********************************************************************/

#ifdef NO_VOID
#define void char
#endif /* NO_VOID */

/***********************************************************************\
*                                    *
*       random numbers anyone?  Returns a double 0.0 .. 1.0             *
*                                    *
\***********************************************************************/

#define rnd() (((double)rand()) / RAND_MAX)

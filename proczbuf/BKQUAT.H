/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ              BkQuat.H = Quaternion proto-types for BkQuat.C             บ
บ                                                                         บ
บ       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     บ
บ                                                                         บ
บ       ALL RIGHTS RESERVED.   This software is published, but is NOT     บ
บ         Public Domain and remains the propery of ALGORITHM, Inc.,       บ
บ   Christopher D. Watkins and Stephen B. Coy.  This software may not be  บ
บ  reproduced or integrated into other packages without the prior written บ
บ          consent of Christopher D. Watkins and Stephen B. Coy.          บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

extern double SqrFPQ(double x);

extern void QAdd(QA p, QA q, QA r);			/* r=p+q */

extern void QSubtract(QA p, QA q, QA r);		/* r=p-q */

extern void QMultiply(QA p, QA q, QA r);		/* r=p*q */

extern void QDivide(QA p, QA q, QA r);		        /* r=p/q */

extern void QSquareRoot(QA q, QA s);

extern void QSquare(QA q, QA r);			/* r=q^2 */

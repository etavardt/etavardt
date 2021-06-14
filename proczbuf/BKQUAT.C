/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ               BkQuat.C = Quaternion Mathematics Module		  บ
บ                                                                         บ
บ       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     บ
บ                                                                         บ
บ       ALL RIGHTS RESERVED.   This software is published, but is NOT     บ
บ         Public Domain and remains the propery of ALGORITHM, Inc.,       บ
บ   Christopher D. Watkins and Stephen B. Coy.  This software may not be  บ
บ  reproduced or integrated into other packages without the prior written บ
บ          consent of Christopher D. Watkins and Stephen B. Coy.          บ
บ                                                                         บ
บ          Requires: BkDefs.H, BkMath.H, BkGraph.H and BkQDefs.H          บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include "stdio.h"
#include "dos.h"
#include "conio.h"
#include "math.h"
#include "malloc.h"

#include "BkDefs.H"
#include "BkMath.H"
#include "BkGraph.H"

#include "BkQDefs.H"


double SqrFPQ(double x)
{
  return(x*x);
}



/*
   QAdd         - quaternion addition
   QSubtract	- quaternion subtraction
   QMultiply	- quaternion multiplication
   QDivide	- quaternion division
   QSquareRoot  - quaternion square root
   QSquare	- quaternion square

   Q = R + Ai + Bj + Ck = q[0] + q[1]i + q[2]j + q[3]k
*/

void QAdd(QA p, QA q, QA r)			/* r=p+q */
{
  r[0]=p[0]+q[0];
  r[1]=p[1]+q[1];
  r[2]=p[2]+q[2];
  r[3]=p[3]+q[3];
}

void QSubtract(QA p, QA q, QA r)		/* r=p-q */
{
  r[0]=p[0]-q[0];
  r[1]=p[1]-q[1];
  r[2]=p[2]-q[2];
  r[3]=p[3]-q[3];
}

void QMultiply(QA p, QA q, QA r)		/* r=p*q */
{
  r[0]=p[0]*q[0]-p[1]*q[1]-p[2]*q[2]-p[3]*q[3];
  r[1]=p[0]*q[1]-p[1]*q[0]-p[2]*q[3]-p[3]*q[2];
  r[2]=p[0]*q[2]-p[2]*q[0]-p[3]*q[1]-p[1]*q[3];
  r[3]=p[0]*q[3]-p[3]*q[0]-p[1]*q[2]-p[2]*q[1];
}

void QDivide(QA p, QA q, QA r)		/* r=p/q */
{
  QA t, s;
  double a;

  q[1]=-q[1];
  q[2]=-q[2];
  q[3]=-q[3];
  QMultiply(p, q, t);
  QMultiply(q, q, s);
  a=1.0/s[0];
  r[0]=t[0]*a;
  r[1]=t[1]*a;
  r[2]=t[2]*a;
  r[3]=t[3]*a;
}

void QSquareRoot(QA q, QA s)
{
  double len, l, m;
  double a, b;
  QA r;

  len=sqrt(SqrFPQ(q[0])+SqrFPQ(q[1])+SqrFPQ(q[2]));
  l=1.0/len;
  r[0]=q[0]*1.0;
  r[1]=q[1]*1.0;
  r[2]=q[2]*1.0;
  r[3]=0.0;
  m=1.0/sqrt(SqrFPQ(r[0])+SqrFPQ(r[1]));
  a=sqrt((1.0+r[2])/2.0);
  b=sqrt((1.0-r[2])/2.0);
  s[0]=sqrt(len)*b*r[0]*m;
  s[1]=sqrt(len)*b*r[1]*m;
  s[2]=sqrt(len)*a;
  s[3]=q[3];
}

void QSquare(QA q, QA r)			/* r=q^2 */
{
  double a;

  a=2.0*q[0];
  r[0]=SqrFPQ(q[0])-SqrFPQ(q[1])-SqrFPQ(q[2])-SqrFPQ(q[3]);
  r[1]=a*q[1];
  r[2]=a*q[2];
  r[3]=a*q[3];
}

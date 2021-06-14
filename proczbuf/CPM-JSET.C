/*
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                          บ
บ                          3-D Julia Set Generator                         บ
บ                                                                          บ
บ      CPM-JSET.C = generates Julia Set Terrain by Continuous Potential    บ
บ                                                                          บ
บ        Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     บ
บ                                                                          บ
บ        ALL RIGHTS RESERVED.   This software is published, but is NOT     บ
บ          Public Domain and remains the propery of ALGORITHM, Inc.,       บ
บ    Christopher D. Watkins and Stephen B. Coy.  This software may not be  บ
บ   reproduced or integrated into other packages without the prior written บ
บ           consent of Christopher D. Watkins and Stephen B. Coy.          บ
บ                                                                          บ
บ Requires : BkDefs.H, BkGlobs.H, BkMath.H, BkMath.C, BkGraph.H, BkGraph.C บ
บ                                                                          บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

/*
   125 iterations maximum
*/

#include "stdio.h"
#include "stdlib.h"
#include "dos.h"
#include "conio.h"
#include "math.h"
#include "string.h"
#include "mem.h"

#include "BkDefs.H"
#include "BkGlobs.H"

#include "BkMath.H"
#include "BkGraph.H"
#include "BkZBuf.H"

#define cp   -0.12
#define cq    0.74
#define XMin -1.4
#define XMax  1.4
#define YMin -1.4
#define YMax  1.4
#define Iter  32
#define Scal  32767

char ObjF[]="CPMJSET1";

/*
#define cp    0.31
#define cq    0.04
#define XMin -1.4
#define XMax  1.4
#define YMin -1.4
#define YMax  1.4
#define Iter  32
#define Scal  32767

char ObjF[]="CPMJSET2";
*/

int Iters;

float JSetPot(float p, float q, float cx, float cy, int MaxIter)
{
  float x, y;
  float x2, y2;
  float temp;

  x=cx;
  x2=SqrFP(x);
  y=cy;
  y2=SqrFP(y);
  Iters=0;
  while((Iters<MaxIter) && (x2+y2<20000.0))
  {
    temp=p+x2-y2;
    y=q+2*x*y;
    y2=SqrFP(y);
    x=temp;
    x2=SqrFP(x);
    ++Iters;
  }
  if(Iters<MaxIter)
    return(0.5*log10(x2+y2)/pow(2, Iters));
  else
    return(0.0);
}

void JSetCPM(int nx, int ny, int MaxIter,
	     float p, float q,
	     float Xmin, float Xmax,
	     float Ymin, float Ymax)
{
  int ix, iy;
  float cx, cy;
  float dx, dy;
  int z;

  dx=(Xmax-Xmin)/nx;
  dy=(Ymax-Ymin)/ny;
  for(ix=0; ix<=(nx/2); ix++)
  {
    cx=Xmin+ix*dx;
    for(iy=0; iy<=ny; iy++)
    {
      cy=Ymin+iy*dy;
      z=Round(JSetPot(p, q, cx, cy, MaxIter)*Scal);
      Height[ix][iy]=z;
      Height[Res-ix][Res-iy]=z;
      Put_Pixel(ix, 199-iy, Iters/7, Iters%35);
      Put_Pixel((Res-ix), 199-(Res-iy), Iters/7, Iters%35);
    }
  }
}

void InverseHeightBuffer()
{
  int i, j;
  int Max;

  Max=0;
  for(i=0; i<=MaxRes; i++)
  {
    for(j=0; j<=MaxRes; j++)
    {
      if(Height[i][j]>Max)
	Max=Height[i][j];
    }
  }
  for(i=0; i<=MaxRes; i++)
  {
    for(j=0; j<=MaxRes; j++)
    {
      Height[i][j]=Max-Height[i][j];
    }
  }
}

/* ***********************************************************************
   *									 *
   *				Main Program				 *
   *									 *
   ***********************************************************************
*/

void main()
{
  clrscr();
  Res=60;
  MaxHeight=35;
  HeightBufferScalingFactor();
  Write_RAW_Data = 1;
  Init_Graphics(0);
  ClearHeightBuffer();
  JSetCPM(Res, Res, Iter, cp, cq, XMin, XMax, YMin, YMax);
  InverseHeightBuffer();
  strcpy(ObjectFile, ObjF);
  SaveHeightBuffer(ObjectFile);
  Exit_Graphics();
}

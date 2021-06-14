/*
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                          บ
บ                        3-D Mandelbrot Set Generator                      บ
บ                                                                          บ
บ  CPM-MSET.C = generates Mandelbrot Set Terrain by Continuous Potential   บ
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

#include "stdio.h"
#include "stdlib.h"
#include "dos.h"
#include "conio.h"
#include "math.h"
#include "string.h"

#include "BkDefs.H"
#include "BkGlobs.H"

#include "BkMath.H"
#include "BkGraph.H"
#include "BkZBuf.H"

/*
   125 iterations maximum
*/

#define XMin -2.50
#define XMax  1.35
#define YMin -1.80
#define YMax  1.80
#define Iter  32
#define Scal  32767

char ObjF[]="CPMMSET1";

/*
#define XMin -0.19
#define XMax -0.13
#define YMin  1.01
#define YMax  1.06
#define Iter  125
#define Scal  3276700

char ObjF[]="CPMMSET2";
*/

float MSetPot(float cx, float cy, int MaxIter, int *Iters)
{
  float x, y;
  float x2, y2;
  float temp;

  x=cx;
  x2=SqrFP(x);
  y=cy;
  y2=SqrFP(y);
  *Iters=0;
  while((*Iters<MaxIter) && (x2+y2<20000.0))
  {
    temp=cx+x2-y2;
    y=cy+2*x*y;
    y2=SqrFP(y);
    x=temp;
    x2=SqrFP(x);
    ++*Iters;
  }
  if(*Iters<MaxIter)
    return(0.5*log10(x2+y2)/pow(2, *Iters));
  else
    return(0.0);
}

void MSetCPM(int nx, int ny, int MaxIter, float Xmin, float Xmax, float Ymin, float Ymax)
{
  int ix, iy;
  int Iters;
  float cx, cy;
  float dx, dy;

  dx=(Xmax-Xmin)/nx;
  dy=(Ymax-Ymin)/ny;
  for(iy=0; iy<=ny; iy++)
  {
    cy=Ymin+iy*dy;
    for(ix=0; ix<=nx; ix++)
    {
      cx=Xmin+ix*dx;
      Height[ix][iy]=Round(MSetPot(cx, cy, MaxIter, &Iters)*Scal);
      Put_Pixel(ix, iy, (Iters/7), (Iters%35));
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
  MSetCPM(Res, Res, Iter, XMin, XMax, YMin, YMax);
  InverseHeightBuffer();
  strcpy(ObjectFile, ObjF);
  SaveHeightBuffer(ObjectFile);
  Exit_Graphics();
}

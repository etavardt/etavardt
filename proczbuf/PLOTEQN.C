/*
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                          บ
บ                             Equation Plotter                             บ
บ                                                                          บ
บ                 PlotEqn.C = plots equations - surprise                   บ
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


/* ***********************************************************************
   *									 *
   *				 Equations				 *
   *									 *
   ***********************************************************************
*/


#define Span 5

char ObjF[]="PLOTEQN1";

float zf(float x, float y)
{
  float c;

  c=SqrFP(x)+SqrFP(y);
  return(75.0/(c+1.0));
}


/*
#define Span 5

char ObjF[]="PLOTEQN2";

float zf(float x, float y)
{
  return(6.0*(cos(x*y)+1.0));
}
*/

/*
#define Span 5

char ObjF[]="PLOTEQN3";

float zf(float x, float y)
{
  float c;

  c=SqrFP(x)+SqrFP(y);
  return(20.0*(sin(sqrt(c))+1.0));
}
*/

/*
#define Span 2.75

char ObjF[]="PLOTEQN4";

float zf(float x, float y)
{
  float c;

  c=SqrFP(x)+SqrFP(y);
  return(10.0*(1.0+sqrt(c)+sin(SqrFP(c)*0.1)+sin(x*y)));
}
*/

/* ******************************************************************** */

void CreatEquationPlotHeightBuffer(float Xlft, float Xrgt,
				   float Ybot, float Ytop)
{
  int ix, iy, iz;
  float x, y;
  float dx, dy;

  MaxHeight=MaxRes*100;
  HeightBufferScalingFactor();
  dx=(Xrgt-Xlft)/(Res-1);
  dy=(Ytop-Ybot)/(Res-1);
  for(ix=0; ix<Res; ix++)
  {
    x=Xlft+ix*dx;
    for(iy=0; iy<Res; iy++)
    {
      y=Ybot+iy*dy;
      iz=Round(zf(x, y));
      if(iz<0)
      {
	Exit_Graphics();
	printf("Adjust the equation : z value less than zero\n");
	delay(2000);
	exit(1);
      }
      Height[ix][iy]=iz;
      Cartesian_Plot_3D(ix-Res/2, iy-Res/2, iz, (iz*MaxCol)%255);
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
  printf("Creating Equation Plot Height Buffer\n\n");
  ClearHeightBuffer();
  Res=50;
  Write_RAW_Data = 1;
  Init_Perspective(false, 0, 0, 500, 500);
  Init_Plotting(240, 18);
  Init_Graphics(0);
  CreatEquationPlotHeightBuffer(-Span, Span, -Span, Span);
  strcpy(ObjectFile, ObjF);
  SaveHeightBuffer(ObjectFile);
  Exit_Graphics();
}

/*
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                          บ
บ                       Ocean from Summed Sinusoids                        บ
บ                                                                          บ
บ       Ocean.C = generates an ocean (pretty powerful software - Eh?) 	   บ
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
   *				 the Ocean 				 *
   *									 *
   ***********************************************************************
*/


#define Span 5

char ObjF[]="OCEAN";

float zf(float x, float y)
{
  float f1, f2, f3, f4;
  float Ampl, Dampen, Dampen2;
  float OceanWavePhase = 20.0;
  float OceanWaveAmpl  = 10.0;

  /* sum four sinusoids for ocean height value at a given point */

  f1=sin(Radians(21.50*x+60.70*y+OceanWavePhase));
  f2=sin(Radians(11.25*x+11.50*y+OceanWavePhase));
  f3=sin(Radians(42.50*x+20.80*y+OceanWavePhase));
  f4=sin(Radians(10.75*x+42.00*y+OceanWavePhase));

  Ampl=OceanWaveAmpl*(f1+f2+f3+f4)*0.25 + 10.0;

  return(Ampl);
}


/* ******************************************************************** */

void CreatOceanHeightBuffer(float Xlft, float Xrgt,
				   float Ybot, float Ytop)
{
  int ix, iy, iz;
  float x, y;
  float dx, dy;

  MaxHeight=MaxRes*200;
  HeightBufferScalingFactor();
  dx=(Xrgt-Xlft)/Res;
  dy=(Ytop-Ybot)/Res;
  for(ix=0; ix<=Res; ix++)
  {
    x=Xlft+ix*dx;
    for(iy=0; iy<=Res; iy++)
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
  printf("Creating Ocean Height Buffer\n\n");
  ClearHeightBuffer();
  Res=60;
  Write_RAW_Data = 1;
  Init_Perspective(false, 0, 0, 500, 500);
  Init_Plotting(240, 45);
  Init_Graphics(0);
  CreatOceanHeightBuffer(-Span, Span, -Span, Span);
  strcpy(ObjectFile, ObjF);
  SaveHeightBuffer(ObjectFile);
  Exit_Graphics();
}

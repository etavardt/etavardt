/*
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                          บ
บ                          Fractal Plasma Terrain                          บ
บ                                                                          บ
บ               Terrain.C = generates fractal plasma terrrain              บ
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

#define mcol 63

void Init_Palette_3(Palette_Register Color)
{
  Byte i;

  Color[0].Red=0;
  Color[0].Grn=Round(mcol/85);
  Color[0].Blu=mcol;
  for(i=1; i<=85; i++)
  {
    Color[i].Red=0;
    Color[i].Grn=Round((i*mcol)/85);
    Color[i].Blu=Round(((86-i)*mcol)/85);
    Color[i+85].Red=Round((i*mcol)/85);
    Color[i+85].Grn=Round(((86-i)*mcol)/85);
    Color[i+85].Blu=0;
    Color[i+170].Red=Round(((86-i)*mcol)/85);
    Color[i+170].Grn=0;
    Color[i+170].Blu=Round((i*mcol)/85);
  }
}

void UpdateHeight(int x, int y, int Color)
{
  Plot(x, y, Color&255);
  Height[Res-x][Res-y]=Color;
}

void NewCol(int xa, int ya, int x, int y, int xb, int yb)
{
  int Color;

  Color=abs(xa-xb)+abs(ya-yb);
  Color=random(Color<<1)-Color;
  Color+=(Get_Pixel(xa, ya)+Get_Pixel(xb, yb)+1) >> 1;
  if(Color<1)
    Color=1;
  else
  {
    if(Color>255)
      Color=255;
  }
  if(Get_Pixel(x, y)==0)
    UpdateHeight(x, y, Color);
}

void SubDivide(int x1, int y1, int x2, int y2)
{
  int x, y, Color;

  if(!((x2-x1<2) && (y2-y1<2)))
  {
    x=(x1+x2)>>1;
    y=(y1+y2)>>1;
    NewCol(x1, y1, x, y1, x2, y1);
    NewCol(x2, y1, x2, y, x2, y2);
    NewCol(x1, y2, x, y2, x2, y2);
    NewCol(x1, y1, x1, y, x1, y2);
    Color=(Get_Pixel(x1, y1)+Get_Pixel(x2, y1)+Get_Pixel(x2, y2)+Get_Pixel(x1, y2)+2)>>2;
    UpdateHeight(x, y, Color);
    SubDivide(x1, y1, x, y);
    SubDivide(x, y1, x2, y);
    SubDivide(x, y, x2, y2);
    SubDivide(x1, y, x, y2);
  }
}

/* ***********************************************************************
   *									 *
   *				Main Program				 *
   *									 *
   ***********************************************************************
*/

Palette_Register PalArray;

void main()
{
  ClearHeightBuffer();
  Res=60;
  MaxHeight=6000;
  HeightBufferScalingFactor();
  Write_RAW_Data = 1;
  Init_Graphics(0);
  Init_Palette_3(PalArray);
  Set_Palette(PalArray);
  randomize();

  UpdateHeight(  0,   0, 255);
  UpdateHeight(Res,   0,   1);
  UpdateHeight(Res, Res,   1);
  UpdateHeight(  0, Res, 255);

  SubDivide(0, 0, Res, Res);
  strcpy(ObjectFile, "TERRAIN");
  SaveHeightBuffer(ObjectFile);
  Exit_Graphics();
}

/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                    BkGraph.C = the graphics module                      บ
บ                                                                         บ
บ       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     บ
บ                                                                         บ
บ       ALL RIGHTS RESERVED.   This software is published, but is NOT     บ
บ         Public Domain and remains the propery of ALGORITHM, Inc.,       บ
บ   Christopher D. Watkins and Stephen B. Coy.  This software may not be  บ
บ  reproduced or integrated into other packages without the prior written บ
บ          consent of Christopher D. Watkins and Stephen B. Coy.          บ
บ                                                                         บ
บ               Requires: BkDefs.H, BkMath.H and BkGraph.H                บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

/*
    Plot      - place pixel to screen
    Set_Palette    - set palette register
    Init_Palette - 64 levels of grey, red, green and blue
    Init_Palette2 - 7 colors with 35 intensities each - use with Pixel
    Cycle_Palette - cycle through palette
    Circle    - circle draw routine
    Draw      - line draw routine
    Init_Graphics - initialize graphics
    Wait_For_Key   - wait for key press
    Exit_Graphics - sound and wait for keypress before exiting graphics
    Title     - set up text screen colors
*/


#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>
#include <math.h>
#include <malloc.h>
#include <mem.h>
#include <graphics.h>

#include "BkDefs.H"
#include "BkMath.H"
#include "BkGraph.H"

Word  X_Off, Y_Off;
static Byte Res;

#define Low_Res   1
#define Medium_Res 2
#define High_Res  3


void Plot(Word x, Word y, Byte color)
{
 if((x<XRes) && (y<YRes))
     putpixel(x, y, color);
}


static Palette_Register Color;

void Set_Palette(Palette_Register Hue) {
    int subi;

    for (subi=0; subi<256; subi++) {
         setrgbpalette (subi, Hue[subi].Red, Hue[subi].Grn, Hue[subi].Blu);
    }
}



void Init_Palette(Palette_Register Color)
{
 Word i;

 for(i=0; i<64; i++)
 {
     Color[i].Red=i << 2;
     Color[i].Grn=i << 2;
     Color[i].Blu=i << 2;
 }
 for(i=64; i<128; i++)
 {
     Color[i].Red=(i-64) << 2;
     Color[i].Grn=0;
     Color[i].Blu=0;
 }
 for(i=128; i<192; i++)
 {
     Color[i].Red=0;
     Color[i].Grn=(i-128) << 2;
     Color[i].Blu=0;
 }
 for(i=192; i<=255; i++)
 {
     Color[i].Red=0;
     Color[i].Grn=0;
     Color[i].Blu=(i-192) << 2;
 }
}

void Init_Palette_2(Palette_Register Color)
{
 Word i;

 for(i=0; i<36; i++)
 {
     Color[i].Red=0;
     Color[i].Grn=0;
     Color[i].Blu=(Round(1.8*i)) << 2;
 }
 for(i=36; i<72; i++)
 {
     Color[i].Red=0;
     Color[i].Grn=(Round(1.8*(i-36))) << 2;
     Color[i].Blu=0;
 }
 for(i=72; i<108; i++)
 {
     Color[i].Red=0;
     Color[i].Grn=(Round(1.8*(i-72))) << 2;
     Color[i].Blu=(Round(1.8*(i-72))) << 2;
 }
 for(i=108; i<144; i++)
 {
     Color[i].Red=(Round(1.8*(i-108))) << 2;
     Color[i].Grn=0;
     Color[i].Blu=0;
 }
 for(i=144; i<180; i++)
 {
     Color[i].Red=(Round(1.8*(i-144))) << 2;
     Color[i].Grn=0;
     Color[i].Blu=(Round(1.8*(i-144))) << 2;
 }
 for(i=180; i<216; i++)
 {
     Color[i].Red=(Round(1.8*(i-180))) << 2;
     Color[i].Grn=(Round(1.8*(i-180))) << 2;
     Color[i].Blu=0;
 }
 for(i=216; i<252; i++)
 {
     Color[i].Red=(Round(1.8*(i-216))) << 2;
     Color[i].Grn=(Round(1.8*(i-216))) << 2;
     Color[i].Blu=(Round(1.8*(i-216))) << 2;
 }
}

void Cycle_Palette(Palette_Register Hue)
{
 Word i;
 RGB tmp;

 tmp=Hue[0];
 memcpy(&Hue[0], &Hue[1], 765);
 Hue[255]=tmp;
 Set_Palette(Hue);
}

void Swap(int *first, int *second)
{
 int temp;

 temp=*first;
 *first=*second;
 *second=temp;
}

void Circle(Word x, Word y, Word radius, Byte color)
{
 int a, af, b, bf, target, r2, asp;

 if(Res==High_Res)
   asp=100;
 else
     asp=120;
 target=0;
 a=radius;
 b=0;
 r2=Sqr(radius);
 while(a>=b)
 {
   b=Round(sqrt(r2-Sqr(a)));
   Swap(&target,&b);
     while(b<target)
   {
     af=(asp*a)/100;
     bf=(asp*b)/100;
         Plot(x+af, y+b, color);
         Plot(x+bf, y+a, color);
         Plot(x-af, y+b, color);
	 Plot(x-bf, y+a, color);
         Plot(x-af, y-b, color);
	 Plot(x-bf, y-a, color);
         Plot(x+af, y-b, color);
     Plot(x+bf, y-a, color);
     ++b;
     }
   --a;
 }
}


int MinI (int A, int B) {

    if (A < B) return (A);
    else return (B);
}


int MaxI (int A, int B) {

    if (A > B) return (A);
    else return (B);
}

int Trivial_Reject (int *x1, int *y1, int *x2, int *y2) {

    if (MaxI (*x1, *x2) < 0) return (true);
    if (MinI (*x1, *x2) >= XRes) return (true);
    if (MaxI (*y1, *y2) <0) return (true);
    if (MinI (*y1, *y2) >= YRes) return (true);

    return (false);

}


int Clip_To_Screen (int *x1, int *y1, int *x2, int *y2) {

    int SegmentVisible = true;

    if (Trivial_Reject( x1, y1, x2, y2)) {
         SegmentVisible = false;
    }

    return (SegmentVisible);
}


void Line(int x1, int y1, int x2, int y2, Byte Color)
{
    if ( (x1 == x2) && (y1 == y2)) {
         /* Do Nothing. Line is actually a point */
    }

    else {
         if (Clip_To_Screen(&x1, &y1, &x2, &y2)) {
	      setcolor(Color);
              line(x1, y1, x2, y2);
	 }
    }
}

int  CentreX, CentreY;


Byte G_Mode;

int huge Detect()
{
 return(G_Mode);
}

int gdriver=DETECT, gmode, errorcode;

void Init_Graphics(Byte mode)
{
 G_Mode=mode;
 printf("\nEntering Graphics Mode: %d", mode);
 installuserdriver("SVGA256", Detect);
 initgraph(&gdriver, &gmode, "");
 errorcode=graphresult();
 if(errorcode!=grOk)
 {
     printf("Graphics error: %s\n", grapherrormsg(errorcode));
     getch();
     exit(1);
 }
 XRes=getmaxx();
 YRes=getmaxy();
 CentreX = XRes / 2;
 CentreY = YRes / 2;
}


void Set_Graphics_Mode(Word xRes, Word yRes)
{
 XRes=xRes;
 YRes=yRes;
 if((XRes<321) && (YRes<201))
     Init_Graphics(0);
 else if((XRes<641) && (YRes<401))
     Init_Graphics(1);
 else if((XRes<641) && (YRes<481))
     Init_Graphics(2);
 else if((XRes<801) && (YRes<601))
     Init_Graphics(3);
 else
     Init_Graphics(4);
}

void Wait_For_Key()
{
 char k;

 while(!(k=getch()));
}

void Exit_Graphics()
{
 sound(1000);
 delay(500);
 nosound();
 Wait_For_Key();
 closegraph();
}

void Title()
{
 textcolor(YELLOW);
 textbackground(BLUE);
 clrscr();
}


/* **********************************************************************
    *                                                                   *
  *            Three Dimensional Plotting Routines                   *
    *                                              *
  **********************************************************************

    InitPlotting      - rotation and tilt angles
    InitPerspective   - observer location and distances
  MapCoordinates    - maps 3D space onto the 2D screen
    CartesianPlot     - plot a cartesian system point
  CylindricalPlot3D - plot a cylindrical system point
  SphericalPlot3D   - plot a spherical system point
    DrawLine3D          - plots a line from 3D coordinates
*/

float CosA, SinA;
float CosB, SinB;
float CosACosB, SinASinB;
float CosASinB, SinACosB;

void Init_Plotting(float Ang, float Tlt)
{
 CentreX=XRes/2;
 CentreY=YRes/2;
 Angl= (int) Ang;
 Tilt= (int) Tlt;
 CosA=CosD(Ang);
 SinA=SinD(Ang);
 CosB=CosD(Tlt);
 SinB=SinD(Tlt);
 CosACosB=CosA*CosB;
 SinASinB=SinA*SinB;
 CosASinB=CosA*SinB;
 SinACosB=SinA*CosB;
}

void Init_Perspective(Boolean Perspective, float x, float y, float z, float m)
{
 PerspectivePlot=Perspective;
 Mx=x;
 My=y;
 Mz=z;
 ds=m;
}

void Map_Coordinates(float X, float Y, float Z, int *Xp, int *Yp)
{
 float Xt, Yt, Zt;
 float OneOverZt;

 Xt=(Mx+X*CosA-Y*SinA);
 Yt=(My+X*SinASinB+Y*CosASinB+Z*CosB);
 if(PerspectivePlot)
 {
     Zt=Mz+X*SinACosB+Y*CosACosB-Z*SinB;
     OneOverZt=1.0/Zt;
     *Xp=CentreX+Round(ds*Xt*OneOverZt);
     if(Res!=Low_Res)
         *Yp=CentreY-Round(ds*Yt*OneOverZt);
     else
         *Yp=CentreY-Round(ds*Yt*OneOverZt*Asp);
 }
 else
 {
     *Xp=CentreX+Round(Xt);
     if(Res!=Low_Res)
         *Yp=CentreY-Round(Yt);
   else
     *Yp=CentreY-Round(Yt*Asp);
 }
}

void Cartesian_Plot_3D(float X, float Y, float Z, Byte Color)
{
 int Xp, Yp;

 Map_Coordinates(X, Y, Z, &Xp, &Yp);
 Plot(Xp, Yp, Color);
}

void Cylindrical_Plot_3D(float Rho, float Theta, float Z, Byte Color)
{
 float X, Y;

 Theta=Radians(Theta);
 X=Rho*cos(Theta);
 Y=Rho*sin(Theta);
 Cartesian_Plot_3D(X, Y, Z, Color);
}
void Spherical_Plot_3D(float R, float Theta, float Phi, Byte Color)
{
 float X, Y, Z;

 Theta=Radians(Theta);
 Phi=Radians(Phi);
 X=R*sin(Theta)*cos(Phi);
 Y=R*sin(Theta)*sin(Phi);
 Z=R*cos(Theta);
 Cartesian_Plot_3D(X, Y, Z, Color);
}

void Draw_Line_3D(TDA Pnt1, TDA Pnt2, Byte Color)
{
 int  Xp1, Yp1;
 int  Xp2, Yp2;
 float x1, y1, z1;
 float x2, y2, z2;

 UnVec(Pnt1, &x1, &y1, &z1);
 UnVec(Pnt2, &x2, &y2, &z2);
 Map_Coordinates(x1, y1, z1, &Xp1, &Yp1);
 Map_Coordinates(x2, y2, z2, &Xp2, &Yp2);
 Line(Xp1, Yp1, Xp2, Yp2, Color);
}

/* ***********************************************************************
    *                                            *
  *                             Pixel                     *
  *                                         *
  ***********************************************************************

  PutPixel - plots pixel
    GetPixel - gets pixel

  Color 1 - Blue
     2 - Green
     3 - Cyan
     4 - Red
     5 - Magenta
     6 - Brown/Yellow
     7 - Gray Scale

    Intensity levels (0..35) for each color
*/

void Put_Pixel(int x, int y, Byte Color, Byte Intensity)
{
 Byte Col;

 if(Intensity>MaxInten)
     exit(1);
 Col=((MaxInten+1)*(Color-1)+Intensity) & 255;
 Plot(x, y, Col);
}


Byte Get_Pixel(Word x, Word y)
{
 if((x<XRes) && (y<YRes))
     return(getpixel(x, y));
 else
   return(0);
}

/* ***********************************************************************
  *                                         *
    *            Setup of Coordinate Axes and Color Palette         *
  *                                         *
    ***********************************************************************

  PutAxisAndPalette - toggle for Axis and Palette
    AxisAndPalette   - places Axis and Color Palette on screen
*/

void Put_Axis_And_Palette(Boolean PlaceOnScreen)
{
 if(PlaceOnScreen)
   Draw_Axis_And_Palette=true;
 else
   Draw_Axis_And_Palette=false;
}

void Display_Axis()
{
 int x, y, z, sx, sy;

 sx=XRes/320;
 sy=YRes/200;
 for(x=-100*sx; x<101*sx; x++)
 {
     Cartesian_Plot_3D(x, 0, 0, 35);
   Cartesian_Plot_3D(100*sx, 0, 0, 251);
 }
 for(y=-100*sy; y<101*sy; y++)
 {
   Cartesian_Plot_3D(0, y, 0, 71);
   Cartesian_Plot_3D(0, 100*sy, 0,251);
 }
 for(z=-100*sx; z<101*sx; z++)
 {
   Cartesian_Plot_3D(0, 0, z, 107);
   Cartesian_Plot_3D(0, 0, 100*sx, 251);
 }
}

void Display_Palette()
{
 int X, Y, sx, sy;
 Byte Color;
 Byte Intensity;

 sx=XRes/320;
 sy=YRes/200;
 for(Color=1; Color<=MaxCol; Color++)
 {
   for(Intensity=0; Intensity<=MaxInten; Intensity++)
   {
     for(X=0; X<4*sx; X++)
     {
    for(Y=0; Y<4*sy; Y++)
      Put_Pixel(X+(5*sx)*Color,
            (190*sy)-Y-(5*sy)*Intensity,
              Color,
            Intensity);
     }
   }
 }
}

void Axis_And_Palette()
{
 if(Draw_Axis_And_Palette)
 {
   Display_Axis();
     Display_Palette();
 }
}

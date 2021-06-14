/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                       Quaternion Fractal Generator			  บ
บ                                                                         บ
บ         Quater.C = generates rough approximations of Quaternions 	  บ
บ	      	       through the use of the Forward Equation.  	  บ
บ                    The program is set up to generate a Quaternion       บ
บ		       Dragon and a Solid of Revolution                   บ
บ                                                                         บ
บ       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     บ
บ                                                                         บ
บ      NOTE : this program performs iteration tests from back to front    บ
บ                                                                         บ
บ       ALL RIGHTS RESERVED.   This software is published, but is NOT     บ
บ         Public Domain and remains the propery of ALGORITHM, Inc.,       บ
บ   Christopher D. Watkins and Stephen B. Coy.  This software may not be  บ
บ  reproduced or integrated into other packages without the prior written บ
บ          consent of Christopher D. Watkins and Stephen B. Coy.          บ
บ                                                                         บ
บ            Requires = BkDefs.H, BkGlobs.H, BkMath.H, BkGraph.H,         บ
บ			BkZBuf.H, BkQDefs.H, BkQuat.H                     บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
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

#include "BkQDefs.H"
#include "BkQuat.H"


/*
 ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
 บ                                                                         บ
 บ                  Constants for the Quaternion Generator        	   บ
 บ                                                                         บ
 ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

   QSetCons - set up constants for quaternion generator
*/

QA     q, l;
double DivergenceBoundary;
int    MaximumIterations;
double XLeft, XRight;
double YBottom, YTop;
double ZBack, ZFront;


char ObjF1[]="Dragon";

void QSetCons1()
{
  q[0]=0.0;
  q[1]=0.0;
  q[2]=0.0;
  q[3]=0.0;
  l[0]=0.2809;
  l[1]=0.53;
  l[2]=0.0;
  l[3]=0.0;
  DivergenceBoundary=4.0;
  MaximumIterations=100;
  XLeft  =-1.3;
  XRight = 1.3;
  YBottom=-1.3;
  YTop   = 1.3;
  ZBack  = 0.0;
  ZFront = 1.3;
}


char ObjF2[]="RevSolid";

void QSetCons2()
{
  q[0]= 0.0;
  q[1]= 0.0;
  q[2]= 0.0;
  q[3]= 0.0;
  l[0]=-1.0;
  l[1]= 0.0;
  l[2]= 0.0;
  l[3]= 0.0;
  DivergenceBoundary=4.0;
  MaximumIterations=100;
  XLeft  =-1.8;
  XRight = 1.8;
  YBottom=-1.8;
  YTop   = 1.8;
  ZBack  = 0.0;
  ZFront = 1.8;
}


double XScale, YScale, ZScale;
double XOffset, YOffset;

void Parameters()
{
  int tmp;

  textcolor(YELLOW);
  textbackground(BLUE);
  clrscr();
  printf("Quaternion Generator\n\n");
  printf("Equation : f(q)=lambda+qý\n\n");
  printf("lambda   : quaternion constant for a particular fractal curve\n\n");
  printf("q        : quaternion variable seeded as 0+0i+0j+0k\n\n");
  printf("The fractal surface is realized by examination of the\n");
  printf("  divergence properties of the equation in quaternion space\n\n");

  XScale=(double) Res/(XRight-XLeft);
  XOffset=-XScale*XLeft;
  YScale=(double) Res/(YBottom-YTop);
  YOffset=-YScale*YTop;
  tmp=Res*Scaling;
  ZScale=(double) tmp/(ZFront-ZBack);
}

/*
 ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
 บ                                                                         บ
 บ                           Function Iteration        	   		   บ
 บ                                                                         บ
 ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

void Func(QA Q, QA NQ)
{
  QSquare(Q, Q);
  QAdd(Q, l, NQ);
}


void Iterate(QA p, int *NumberOfIterations)
{
  QA nq;
  double magnitude;

  Func(p, q);
  *NumberOfIterations=0;
  do
  {
    Func(q, nq);
    ++*NumberOfIterations;
    q[0]=nq[0];
    q[1]=nq[1];
    q[2]=nq[2];
    q[3]=nq[3];

    magnitude=SqrFPQ(nq[0])+SqrFPQ(nq[1]);
  }
  while( (magnitude<DivergenceBoundary) && (*NumberOfIterations!=MaximumIterations) );
}

/*
 ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
 บ                                                                         บ
 บ                            Scanning Space 			           บ
 บ                                                                         บ
 ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/


void UpdateHeight(double x, double y, double z)
{
  int PlotX, PlotY, PlotZ;

  PlotX=Round(XScale*x+XOffset);
  PlotY=Round(YScale*y+YOffset);
  PlotZ=Round(ZScale*z);
  Height[PlotX][PlotY]=PlotZ;
}


QA p;
double X, Y, Z;
int NumOfIters;

void ScanSpace()
{
  double dx, dy, dz;
  int Slice;
  int Done;

  dx=(XRight-XLeft)/(double) Res;
  dy=(YBottom-YTop)/(double) Res;
  dz=(ZFront-ZBack)/(double) Res;

  Slice=0;

  X=XLeft;
  do
  {
    Y=YTop;
    do
    {
      Z=ZBack;
      do
      {
	p[0]=X;
	p[1]=Y;
	p[2]=Z;
	p[3]=0.0;
	Iterate(p, &NumOfIters);

	if(!(NumOfIters<MaximumIterations))
	  goto Done;

	Z+=dz;
      }
      while(!(Z>=ZFront));
Done:
      UpdateHeight(X, Y, Z);
      Y+=dy;
    }
    while(!(Y<=YBottom));
    gotoxy(1, 20);
    printf("Slice # %d\n", Slice);
    ++Slice;
    X+=dx;
  }
  while(!(X>=XRight));
}

/*
 ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
 บ                                                                         บ
 บ                        M a i n  P r o g r a m 			   บ
 บ                                                                         บ
 ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

void main()
{
  Res=64;
  MaxHeight=Res;
  HeightBufferScalingFactor();

  Write_RAW_Data = 1;

  /* Create Dragon. */

  ClearHeightBuffer();
  QSetCons1();
  Parameters();
  ScanSpace();
  strcpy(ObjectFile, ObjF1);
  SaveHeightBuffer(ObjectFile);

  /* Solid of Revolution */

  ClearHeightBuffer();
  QSetCons2();
  Parameters();
  ScanSpace();
  strcpy(ObjectFile, ObjF2);
  SaveHeightBuffer(ObjectFile);
}

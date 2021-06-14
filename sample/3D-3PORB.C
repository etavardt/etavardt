/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                    3-D Three Particle Orbit Simulation                  บ
บ                                                                         บ
บ           3D-3POrb.C = 3-D simulation of three orbiting particles       บ
บ                                                                         บ
บ       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     บ
บ                                                                         บ
บ       ALL RIGHTS RESERVED.   This software is published, but is NOT     บ
บ         Public Domain and remains the propery of ALGORITHM, Inc.,       บ
บ   Christopher D. Watkins and Stephen B. Coy.  This software may not be  บ
บ  reproduced or integrated into other packages without the prior written บ
บ          consent of Christopher D. Watkins and Stephen B. Coy.          บ
บ                                                                         บ
บ Requires: BkDefs.H, BkGlobs.H, BkMath.H, BkMath.C, BkGraph.H, BkGraph.C บ
บ                                                                         บ
บ                       C conversion by Larry Sharp                       บ
บ	       2-D concept by C.D. Watkins and Martin Jaspan              บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ


   increase time step "dt" to greater than zero to increase speed
*/

#include "stdio.h"
#include "conio.h"
#include "math.h"

#include "BkDefs.H"
#include "BkGlobs.H"
#include "BkMath.H"
#include "BkGraph.H"


float X1, Y1, Z1, Vx1, Vy1, Vz1, Ax1, Ay1, Az1;
float X2, Y2, Z2, Vx2, Vy2, Vz2, Ax2, Ay2, Az2;
float X3, Y3, Z3, Vx3, Vy3, Vz3, Ax3, Ay3, Az3;
float D12, D23, D31, dt;
float Dx12, Dx23, Dx31;
float Dy12, Dy23, Dy31;
float Dz12, Dz23, Dz31;
float Tx12, Tx23, Tx31;
float Ty12, Ty23, Ty31;
float Tz12, Tz23, Tz31;
int   Xp1, Yp1, Zp1, Xp2, Yp2, Zp2, Xp3, Yp3, Zp3;
int   M1, M2, M3;
float s;
Palette_Register PalArray;


void main()
{
  Init_Graphics(4);

  Init_Palette_2(PalArray);
  Set_Palette(PalArray);

  Init_Perspective(false, 0, 0, 500, 500);
  Init_Plotting(190, 25);

  Put_Axis_And_Palette(true);
  Axis_And_Palette();


/* stable orbit */
/*
  M1=12;
  M2=1;
  M3=1;
  X1=0.0;	Y1=0.0;		Z1=  0.0;
  X2=0.0;	Y2=0.0;         Z2=-40.0;
  X3=0.0;	Y3=0.0;         Z3= 40.0;
  Vx1= 0.0000;	Vy1= 0.0000;    Vz1= 0.0000;
  Vx2=-0.1000;	Vy2=-0.1000;    Vz2=-0.1000;
  Vx3= 0.1000;	Vy3= 0.1000;    Vz3= 0.1000;
  dt=0.2;
  s=1.0;
*/

/*
  M1=1;
  M2=10;
  M3=3;
  X1=-40.0;	Y1=0.0;		Z1=0.0;
  X2= 00.0;	Y2=0.0;         Z2=0.0;
  X3= 90.0;	Y3=0.0;         Z3=0.0;
  Vx1= 0.1010;	Vy1= 0.2500;    Vz1=-0.0240;
  Vx2= 0.0010;	Vy2= 0.0010;    Vz2= 0.0240;
  Vx3=-0.0200;	Vy3=-0.1010;    Vz3= 0.1540;
  dt=0.4;
  s=1.0;
*/

/*
  M1=5;
  M2=1;
  M3=10;
  X1=-10.0;	Y1=-90.0;	Z1= 10.0;
  X2=  0.0;	Y2=-70.0;       Z2=-40.0;
  X3= 15.0;	Y3=-90.0;       Z3= 40.0;
  Vx1= 0.2000;	Vy1= 0.0300;    Vz1= 0.0070;
  Vx2=-0.1000;	Vy2=-0.1020;    Vz2=-0.1000;
  Vx3= 0.1000;	Vy3= 0.0991;    Vz3= 0.1000;
  dt=0.3;
  s=1.0;
*/


  M1=1;
  M2=6;
  M3=4;
  X1=-40.0;	Y1=  0.0;	Z1=  0.0;
  X2=  0.0;	Y2=  0.0;       Z2=  0.0;
  X3= 90.0;	Y3=  0.0;       Z3=  0.0;
  Vx1= 0.1010;	Vy1= 0.2500;    Vz1=-0.0240;
  Vx2= 0.0010;	Vy2= 0.0010;    Vz2=-0.0440;
  Vx3=-0.0200;	Vy3=-0.1010;    Vz3= 0.1240;
  dt=0.1;
  s=1.0;


/*
  M1=5;
  M2=7;
  M3=12;
  X1=-40.0;	Y1= 16.0;	Z1=-20.0;
  X2=  0.0;	Y2= 64.0;       Z2=  2.0;
  X3=-10.0;	Y3=-20.0;       Z3= 52.0;
  Vx1= 0.2010;	Vy1= 0.2600;    Vz1=-0.1240;
  Vx2= 0.2600;	Vy2= 0.2010;    Vz2=-0.1623;
  Vx3=-0.0200;	Vy3=-0.1010;    Vz3= 0.3140;
  dt=0.15;
  s=0.5;
*/

/*
  M1=6;
  M2=1;
  M3=16;
  X1=-20.0;	Y1=-16.0;	Z1=-25.0;
  X2= 20.0;	Y2= 14.0;       Z2= -2.0;
  X3= 10.0;	Y3= 20.0;       Z3= 12.0;
  Vx1= 0.2010;	Vy1= 0.2200;    Vz1=-0.1240;
  Vx2= 0.1600;	Vy2= 0.3010;    Vz2=-0.2623;
  Vx3=-0.1200;	Vy3=-0.3910;    Vz3=-0.1140;
  dt=0.30;
  s=0.25;
*/

/*
  M1=35;
  M2=2;
  M3=4;
  X1=  0.0;	Y1=  0.0;	Z1=  0.0;
  X2=  0.0;	Y2= 40.0;       Z2=  0.0;
  X3=  0.0;	Y3= 80.0;       Z3=  0.0;
  Vx1= 0.0000;	Vy1= 0.0000;    Vz1= 0.0000;
  Vx2= 0.0000;	Vy2= 0.0000;    Vz2= 1.0000;
  Vx3= 0.0000;	Vy3= 0.0000;    Vz3=-0.6500;
  dt=0.10;
  s=1.0;
*/

/*
  M1=35;
  M2=2;
  M3=4;
  X1=  0.0;	Y1=  0.0;	Z1=  0.0;
  X2=  0.0;	Y2= 40.0;       Z2=  0.0;
  X3=  0.0;	Y3=100.0;       Z3=  0.0;
  Vx1= 0.0000;	Vy1= 0.0000;    Vz1= 0.0000;
  Vx2= 0.0010;	Vy2= 0.0000;    Vz2= 1.0000;
  Vx3=-0.0010;	Vy3= 0.0000;    Vz3=-0.5000;
  dt=0.10;
  s=1.0;
*/

/*
  M1=35;
  M2=2;
  M3=4;
  X1=  0.0;	Y1=  0.0;	Z1=  0.0;
  X2=  0.0;	Y2= 40.0;       Z2=  0.0;
  X3=  0.0;	Y3=100.0;       Z3=  0.0;
  Vx1= 0.0000;	Vy1= 0.0000;    Vz1= 0.0000;
  Vx2= 0.0000;	Vy2= 0.0000;    Vz2= 1.0000;
  Vx3= 0.6500;	Vy3= 0.4000;    Vz3=-0.0400;
  dt=0.10;
  s=1.0;
*/

  do
  {
//  the next three lines of code remove the trails so that you see
//    only the particles, comment these lines out to see paticle trails

//    Cartesian_Plot_3D(X1*s, Y1*s, Z1*s, 0);
//    Cartesian_Plot_3D(X2*s, Y2*s, Z2*s, 0);
//    Cartesian_Plot_3D(X3*s, Y3*s, Z3*s, 0);

    X1+=Vx1*dt;
    Y1+=Vy1*dt;
    Z1+=Vz1*dt;

    X2+=Vx2*dt;
    Y2+=Vy2*dt;
    Z2+=Vz2*dt;

    X3+=Vx3*dt;
    Y3+=Vy3*dt;
    Z3+=Vz3*dt;

    Cartesian_Plot_3D(X1*s, Y1*s, Z1*s, 143);
    Cartesian_Plot_3D(X2*s, Y2*s, Z2*s, 169);
    Cartesian_Plot_3D(X3*s, Y3*s, Z3*s, 205);

    Dx12=X1-X2;
    Dy12=Y1-Y2;
    Dz12=Z1-Z2;

    Dx23=X2-X3;
    Dy23=Y2-Y3;
    Dz23=Z2-Z3;

    Dx31=X3-X1;
    Dy31=Y3-Y1;
    Dz31=Z3-Z1;

    D12=sqrt(SqrFP(Dx12)+SqrFP(Dy12)+SqrFP(Dz12));
    D12=1.0/(D12*D12*D12);

    D23=sqrt(SqrFP(Dx23)+SqrFP(Dy23)+SqrFP(Dz23));
    D23=1.0/(D23*D23*D23);

    D31=sqrt(SqrFP(Dx31)+SqrFP(Dy31)+SqrFP(Dz31));
    D31=1.0/(D31*D31*D31);

    Tx31=Dx31*D31;
    Ty31=Dy31*D31;
    Tz31=Dz31*D31;

    Tx12=Dx12*D12;
    Ty12=Dy12*D12;
    Tz12=Dz12*D12;

    Tx23=Dx23*D23;
    Ty23=Dy23*D23;
    Tz23=Dz23*D23;

    Ax1=(M3*Tx31-M2*Tx12);
    Ay1=(M3*Ty31-M2*Ty12);
    Az1=(M3*Tz31-M2*Tz12);

    Ax2=(M1*Tx12-M3*Tx23);
    Ay2=(M1*Ty12-M3*Ty23);
    Az2=(M1*Tz12-M3*Tz23);

    Ax3=(M2*Tx23-M1*Tx31);
    Ay3=(M2*Ty23-M1*Ty31);
    Az3=(M2*Tz23-M1*Tz31);

    Vx1+=Ax1*dt;
    Vy1+=Ay1*dt;
    Vz1+=Az1*dt;

    Vx2+=Ax2*dt;
    Vy2+=Ay2*dt;
    Vz2+=Az2*dt;

    Vx3+=Ax3*dt;
    Vy3+=Ay3*dt;
    Vz3+=Az3*dt;
  }
  while(!(kbhit()));

  Exit_Graphics();
}


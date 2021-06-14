/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                          3-D Starfield Simulation                       บ
บ                                                                         บ
บ         Stars-3D.C = 3-D simulation of flying through a starfield       บ
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
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/


#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>
#include <math.h>
#include <string.h>

#include "BkDefs.H"
#include "BkGlobs.H"
#include "BkMath.H"
#include "BkGraph.H"


#define cblk 0
#define laststar 250
#define infinity -1000
#define windshield 400
#define minbright 24
#define maxbright 63
#define bitshifts 10
#define intscaling (1<<bitshifts)
#define brightness Trunc((float)(maxbright-minbright)/(float)(windshield-infinity)*(float)intscaling)
#define shiprange 35

typedef struct {
  int xpos;
  int ypos;
  int zpos;
  int zn;
  Byte rate;
  Byte hue;
} startype;

startype star[laststar+1];
Byte color;
Word i;
Word starnum;
Palette_Register Pal_Array;

void ship(int x, int y, int z, Byte c)
{
  Cartesian_Plot_3D(x, y, z, c);
  Cartesian_Plot_3D(x+1, y, z, c);
  Cartesian_Plot_3D(x-1, y, z, c);
  Cartesian_Plot_3D(x, y+1, z, c);
  Cartesian_Plot_3D(x, y-1, z, c);
  Cartesian_Plot_3D(x, y, z+1, c);
  Cartesian_Plot_3D(x, y, z-1, c);
}

void main()
{
  Init_Graphics(1);

  Init_Palette(Pal_Array);
  Set_Palette(Pal_Array);

  Init_Perspective(true, 0, 0, 450, 500);
  Init_Plotting(0, 90);

  Put_Axis_And_Palette(false);

  randomize();

  for(i=0; i<=laststar; i++)
  {
    star[i].xpos=random(windshield<<1)-windshield;
    star[i].ypos=random(windshield<<1)-windshield;
    star[i].zpos=infinity;
    star[i].zn=star[i].zpos;
    star[i].rate=random(8)+5;
    star[i].hue=random(4)*64;
  }
  starnum=0;
  do
  {
    /*  Clear Old Star Position  */

    /*  note that if the star falls in the box
	(-shiprange,-shiprange,shiprange,shiprange) then its a ship */

    if((star[starnum].xpos<-shiprange)||
       (star[starnum].ypos<-shiprange)||
       (star[starnum].xpos>shiprange) ||
       (star[starnum].ypos>shiprange))
      Cartesian_Plot_3D(star[starnum].xpos,
			star[starnum].ypos,
			star[starnum].zn, cblk);
    else
      ship(star[starnum].xpos, star[starnum].ypos, star[starnum].zn, cblk);

    if(star[starnum].zpos>windshield)
    {
      /*  If Star is Past WindShield Then  */
      /*   Remove Star and Create Another  */

      star[starnum].xpos=random(windshield<<1)-windshield;
      star[starnum].ypos=random(windshield<<1)-windshield;
      star[starnum].zpos=infinity;
      star[starnum].rate=random(8)+5;
    }
    else
    {
      /*    Else Update the Star Position  */
      /*                    and Intensity  */

      color=star[starnum].hue+minbright+(brightness*(star[starnum].zpos-infinity))>>bitshifts;

      if((star[starnum].xpos<-shiprange)||
	 (star[starnum].ypos<-shiprange)||
	 (star[starnum].xpos>shiprange)||
	 (star[starnum].ypos>shiprange))
	Cartesian_Plot_3D(star[starnum].xpos,
			  star[starnum].ypos,
			  star[starnum].zpos,color);
      else
	ship(star[starnum].xpos, star[starnum].ypos, star[starnum].zpos, color);

      star[starnum].zn=star[starnum].zpos;
      star[starnum].zpos=star[starnum].zpos + star[starnum].rate;
    }
    starnum=(starnum+1)%(laststar+1);
  }
  while(!kbhit());
  Exit_Graphics();
}
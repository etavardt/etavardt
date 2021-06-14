/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                          Plant Growth Simulation                        บ
บ                                                                         บ
บ       Plants.C = simulation of plant growth with light source above     บ
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
บ                       C conversion by Addison Rose                      บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include "stdio.h"
#include "dos.h"
#include "string.h"


#include "BkDefs.H"
#include "BkGlobs.H"
#include "BkMath.H"
#include "BkGraph.H"


int Polyps = 2;
int EqNo = 1;
int Color = 191;
int Delta1 = 1;
int Delta2 = 1;
int Delta3 = 1;

int MaxColumn   = 1023;
int StartColumn =  511;
int StartRow    =  750;

int Height[1024];
int Column;
int Row, MaxRow;
int Growing = 1;

Palette_Register PalArray;


main()
{
    InitRand(12.149);

    Init_Graphics(4);

    Init_Palette(PalArray);
    Set_Palette(PalArray);


    // initialize Height array
    for (Column = 0; Column <= MaxColumn; Column++)
      Height[Column] = -1;
    Height[StartColumn] = 0;

    switch (EqNo) {
      case 1 :
	for (Column = 2; Column <= Polyps; Column++)
	  Height[RandInt(MaxColumn - 100) + 50] = 0;
	break;
      case 2 :
	for (Column = 35; Column <= (MaxColumn - 35); Column++)
	  Height[Column] = 0;
	break;
      case 3 :
	for (Column = 35; Column <= (MaxColumn - 35); Column++) {
	  Row = StartRow - Round(30 * SinD(Column / 100) ) + 30;
	  Plot(Column, Row, Color);
	  Height[Column] = Row;
	}
	break;
      case 4 :
	for (Column = 15; Column <= (MaxColumn - 15); Column++)
	  Height[Column] = Round(Column / 4);
	break;
    }

    while (Growing) {

      Column = RandInt(MaxColumn-2)+1;
      if ( (Height[Column-1] > -1) || (Height[Column+1] > -1) ) {

	// grow
	MaxRow = 0;

	if (Height[Column-1] > MaxRow)
	  MaxRow = Height[Column-1] - RandInt(Delta1);

	if (Height[Column]   > MaxRow)
	  MaxRow = Height[Column  ] - RandInt(Delta2);

	if (Height[Column+1] > MaxRow)
	  MaxRow = Height[Column+1] - RandInt(Delta1);

	Height[Column] = MaxRow + Delta3;
	Row = StartRow - Height[Column];

	Plot(Column, Row, Color);
							// halt if:
	if ( (Height[Column] == StartRow) ||            // top of screen or
	     (Column < 4) || (Column > MaxColumn-4) ||  // sides of screen or
	     kbhit() )                                  // key pressed
	  Growing = 0;
      }
    }

    Exit_Graphics();
    return 0;
}

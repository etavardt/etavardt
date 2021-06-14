/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                        BkZBuf.C = z-Buffer Module			  บ
บ                                                                         บ
บ       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     บ
บ                                                                         บ
บ       ALL RIGHTS RESERVED.   This software is published, but is NOT     บ
บ         Public Domain and remains the propery of ALGORITHM, Inc.,       บ
บ   Christopher D. Watkins and Stephen B. Coy.  This software may not be  บ
บ  reproduced or integrated into other packages without the prior written บ
บ          consent of Christopher D. Watkins and Stephen B. Coy.          บ
บ                                                                         บ
บ          Requires: BkDefs.H, BkMath.H, BkGraph.H and BkZBuf.H           บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include "stdlib.h"
#include "stdio.h"
#include "dos.h"
#include "conio.h"
#include "ctype.h"
#include "math.h"
#include "malloc.h"
#include "string.h"

#include "BkDefs.H"
#include "BkMath.H"
#include "BkGraph.H"
#include "BkZBuf.H"

/* ***********************************************************************
   *									 *
   *			Declare Constants and Variables			 *
   *									 *
   ***********************************************************************

   HeightBufferScalingFactor - scales height for integer manipulation
*/

int Height[MaxRes+1][MaxRes+1];
int Scaling;
int MaxHeight;
int Res;

void HeightBufferScalingFactor()
{
  Scaling=32767/MaxHeight;
}

/* ***********************************************************************
   *									 *
   *		  Clear, Load and Save Height Buffer Data		 *
   *									 *
   ***********************************************************************

   ClearHeightBuffer - clears all heights to zero
   SaveHeightBuffer  - saves height buffer
   LoadHeightBuffer  - loads height buffer
   GetObjectFile     - get filename
   GetObjectColor    - text representation of color
*/

FILE *TextDiskFile;
Name ObjectFile;

void ClearHeightBuffer()
{
  int i, j;

  for(i=0; i<=MaxRes; i++)
  {
    for(j=0; j<=MaxRes; j++)
      Height[i][j]=0;
  }
}

void SaveHeightBuffer(Name FileName)
{
  int i, j;

  if (Write_RAW_Data == 1)
    strcat(FileName, ".RAW");
  else
    strcat(FileName, ".BO");
  TextDiskFile=fopen(FileName, "w+b");
  if(ferror(TextDiskFile))
  {
    Exit_Graphics();
    printf("Can't open file!\n");
    getch();
    exit(1);
  }
  for(i=0; i<Res; i++)
  {
    for(j=0; j<Res; j++)
    {
      if (Write_RAW_Data == 1)
      {
	fprintf(TextDiskFile, "%5.3f %5.3f %5.3f\n\r", (float)i, (float)j, (float)Height[i][j] / (float)Scaling);
	fprintf(TextDiskFile, "%5.3f %5.3f %5.3f\n\r", (float)i+1, (float)j, (float)Height[i+1][j] / (float)Scaling);
	fprintf(TextDiskFile, "%5.3f %5.3f %5.3f\n\r", (float)i, (float)j+1, (float)Height[i][j+1] / (float)Scaling);

	fprintf(TextDiskFile, "%5.3f %5.3f %5.3f\n\r", (float)i+1, (float)j, (float)Height[i+1][j] / (float)Scaling);
	fprintf(TextDiskFile, "%5.3f %5.3f %5.3f\n\r", (float)i+1, (float)j+1, (float)Height[i+1][j+1] / (float)Scaling);
	fprintf(TextDiskFile, "%5.3f %5.3f %5.3f\n\r", (float)i, (float)j+1, (float)Height[i][j+1] / (float)Scaling);
      }
      else
      {
	fprintf(TextDiskFile, "polygon {\npoints 3\n\r\t vertex %5.3f %5.3f %5.3f\n\r", (float)i, (float)j, (float)Height[i][j] / (float)Scaling);
	fprintf(TextDiskFile, "  \t vertex %5.3f %5.3f %5.3f\n\r", (float)i+1, (float)j, (float)Height[i+1][j] / (float)Scaling);
	fprintf(TextDiskFile, "\t vertex %5.3f %5.3f %5.3f\n\r}\n\r", (float)i, (float)j+1, (float)Height[i][j+1] / (float)Scaling);

	fprintf(TextDiskFile, "polygon {\npoints 3\n\r\t vertex %5.3f %5.3f %5.3f\n\r", (float)i+1, (float)j, (float)Height[i+1][j] / (float)Scaling);
	fprintf(TextDiskFile, " \t vertex %5.3f %5.3f %5.3f\n\r", (float)i+1, (float)j+1, (float)Height[i+1][j+1] / (float)Scaling);
	fprintf(TextDiskFile, "\t vertex %5.3f %5.3f %5.3f\n\r}\n\r", (float)i, (float)j+1, (float)Height[i][j+1] / (float)Scaling);
      }
    }
  }
  fclose(TextDiskFile);
}

void GetObjectFile()
{
  int i;
  Byte x, y;

  printf("\nEnter File Name -> ");
  x=wherex();
  y=wherey();
  gets(ObjectFile);
  if(!(strcmp(ObjectFile, "")))
  {
    strcpy(ObjectFile, "MOUNTAIN");
    gotoxy(x, y);
    puts(ObjectFile);
  }
  puts("");
  for(i=0; i<strlen(ObjectFile); i++)
    ObjectFile[i]=toupper(ObjectFile[i]);
}

void GetObjectColor(int C)
{
  switch(C)
  {
    case 0 : puts("Black"); break;
    case 1 : puts("Blue"); break;
    case 2 : puts("Green"); break;
    case 3 : puts("Cyan"); break;
    case 4 : puts("Red"); break;
    case 5 : puts("Magenta"); break;
    case 6 : puts("Brown/Yellow"); break;
    case 7 : puts("Grey"); break;
  }
}

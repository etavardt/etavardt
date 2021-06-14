/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ    	        BkDefs.H = Defines Header File for Modules		  บ
บ                                                                         บ
บ       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     บ
บ                                                                         บ
บ       ALL RIGHTS RESERVED.   This software is published, but is NOT     บ
บ         Public Domain and remains the propery of ALGORITHM, Inc.,       บ
บ   Christopher D. Watkins and Stephen B. Coy.  This software may not be  บ
บ  reproduced or integrated into other packages without the prior written บ
บ          consent of Christopher D. Watkins and Stephen B. Coy.          บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/


typedef unsigned char     Byte;

typedef unsigned int      Word;

typedef unsigned long     DWord;

typedef enum {false, true} Boolean;

typedef char Name[80];

typedef struct
{
 Byte Red;
 Byte Grn;
 Byte Blu;
} RGB;

typedef RGB  Palette_Register[256];

typedef float TDA[3];

typedef int  TDIA[3];
typedef float FDA[4];

typedef float Matx4x4[4][4];

#define MaxCol   7
#define MaxInten 35

#define Ln10       2.30258509299405E+000
#define OneOverLn10 0.43429448190325E+000
#define Pi         3.1415927
#define PiOver180  1.74532925199433E-002
#define PiUnder180 5.72957795130823E+001

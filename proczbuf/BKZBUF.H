/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                  BkZBuf.H = Header for the z-Buffer Module		  บ
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

#define MaxRes 160

extern int Height[MaxRes+1][MaxRes+1];
extern int Scaling;
extern int MaxHeight;
extern int Res;

extern void HeightBufferScalingFactor();

extern FILE *TextDiskFile;
extern Name ObjectFile;

extern void ClearHeightBuffer();

extern void SaveHeightBuffer(Name FileName);

extern void GetObjectFile();

extern void GetObjectColor(int C);

extern int Write_RAW_Data;

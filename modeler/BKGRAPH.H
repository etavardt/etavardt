/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                  BkGraph.H = proto-types for BkGraph.C 	          บ
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

extern int XRes, YRes;
extern int CentreX, CentreY;
extern Word MaxXRes, MaxYRes;
extern Word MaxX, MaxY;
extern float Asp;
extern Boolean PerspectivePlot;
extern float  Mx, My, Mz, ds;
extern int  Angl, Tilt;
extern Boolean Draw_Axis_And_Palette;
extern void Set_Mode(int Mode);
extern void Pre_Calc();
extern void Init_Palette(Palette_Register Color);
extern void Init_Palette_2(Palette_Register Color);
extern void Cycle_Palette(Palette_Register Hue);

extern int MinI (int A, int B);
extern int MaxI (int A, int B);

extern void Swap(int *first, int *second);
extern void Circle(Word x, Word y, Word radius, Byte color);
extern void Set_Graphics_Mode(Word xRes, Word yRes);
extern void Wait_For_Key();
extern void Exit_Graphics();
extern void Title();
extern void Init_Plotting(float Ang, float Tlt);
extern void Init_Perspective(Boolean Perspective,
			    float x, float y, float z, float m);
extern void Map_Coordinates(float X, float Y, float Z, int *Xp, int *Yp);
extern void Cartesian_Plot_3D(float X, float Y, float Z, Byte Color);
extern void Cylindrical_Plot_3D(float Rho, float Theta, float Z, Byte Color);
extern void Spherical_Plot_3D(float R, float Theta, float Phi, Byte Color);
extern void Draw_Line_3D(TDA Pnt1, TDA Pnt2, Byte Color);
extern void Put_Pixel(int x, int y, Byte Color, Byte Intensity);
extern void Put_Axis_And_Palette(Boolean PlaceOnScreen);
extern void Display_Axis();
extern void Display_Palette();
extern void Axis_And_Palette();

extern void Set_Palette(Palette_Register Hue);
extern void Line(int x1, int y1, int x2, int y2, Byte Color);
extern void Plot(Word x, Word y, Byte color);
extern Byte Get_Pixel(Word x, Word y);
extern void Init_Graphics(Byte mode);

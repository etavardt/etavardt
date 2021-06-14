/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                BkMath.H = Header File for BkMath.C Module 		  บ
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


extern int  Round(double x);
extern float Frac(double x);
extern int  Trunc(double x);
extern float SqrFP(float x);
extern int  Sqr(int x);
extern float Radians(float Angle);
extern float Degrees(float Angle);
extern float CosD(float Angle);
extern float SinD(float Angle);
extern float Power(float Base, int Exponent);
extern float Log(float x);
extern float Exp10(float x);
extern float Sign(float x);
extern int  IntSign(int x);
extern int  IntSqrt(int x);
extern int  IntPower(int Base, int Exponent);
extern float MIN(float a, float b);
extern float MAX(float a, float b);
extern float MIN3(float a, float b, float c);
extern float MAX3(float a, float b, float c);
extern float MIN4(float a, float b, float c, float d);
extern float MAX4(float a, float b, float c, float d);
extern void Vec(float r, float s, float t, TDA A);
extern void VecInt(int r, int s, int t, TDIA A);
extern void UnVec(TDA A, float *r, float *s, float *t);
extern void UnVecInt(TDIA A, int *r, int *s, int *t);
extern float VecDot(TDA A, TDA B);
extern void VecCross(TDA A, TDA B, TDA C);
extern float VecLen(TDA A);
extern void VecNormalize(TDA A);
extern void VecMatxMult(FDA A, Matx4x4 Matrix, FDA B);
extern void VecSub(TDA A, TDA B, TDA C);
extern void VecSubInt(TDIA A, TDIA B, TDA C);
extern void VecAdd(TDA A, TDA B, TDA C);
extern void VecAdd3(TDA A, TDA B, TDA C, TDA D);
extern void VecCopy(TDA A, TDA B);
extern void VecCopyInt(TDIA A, TDIA B);
extern void VecLinComb(float r, TDA A, float s, TDA B, TDA C); 
extern void VecScalMult(float r, TDA A, TDA B);
extern void VecScalMultI(float r, TDIA A, TDA B);
extern void VecScalMultInt(float r, TDA A, TDIA B);
extern void VecAddScalMult(float r, TDA A, TDA B, TDA C);
extern void VecNull(TDA A);
extern void VecNullInt(TDIA A);
extern void VecElemMult(float r, TDA A, TDA B, TDA C);
extern void VecNegate(TDA A);
extern void VecMin(TDA a, TDA b, TDA c);
extern void VecMax(TDA a, TDA b, TDA c);
extern void VecNegate(TDA A);
extern void ZeroMatrix(Matx4x4 A);
extern void Translate3D(float tx, float ty, float tz, 
               Matx4x4 A); 
extern void Scale3D(float  sx, float sy,  float sz, Matx4x4  A); extern void  
Rotate3D(int m, float Theta, Matx4x4 A);
extern void Multiply3DMatricies(Matx4x4 A, Matx4x4 B,
               Matx4x4 C); 
extern void MatCopy(Matx4x4 a, Matx4x4 b);
extern void PrepareMatrix(float Tx, float Ty, float Tz,
			  float Sx, float Sy, float Sz,
                          float Rx, float Ry, float Rz,
                          Matx4x4 XForm);
extern   void    PrepareInvMatrix(float    Tx,   float    Ty,    float   Tz,    
float          Sx,           float          Sy,           float          Sz,           
float          Rx,           float          Ry,           float          Rz,           
Matx4x4 XForm);
extern void Transform(TDA A, Matx4x4 M, TDA B);
extern void InitRand(float Seed);
extern int  RandInt(Word Range);
extern float Rand();

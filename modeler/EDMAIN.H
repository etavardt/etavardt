/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ              Program  ED - A DATABASE MODELING TOOL                     บ
บ                                                                         บ
บ              EDMAIN.H   The Grand-Daddy header file.  All data          บ
บ                         structure definitions and global                บ
บ                         constants are found here.                       บ
บ                                                                         บ																								 บ																								 บ
บ              written by William A. Tolhurst                             บ
บ                                                                         บ
บ       This software is NOT Copyrighted.  Please feel free to use it     บ
บ       in any way you see fit, either in its entirety or in whatever     บ
บ       bits and pieces you need.  I, however, will not assume any re-    บ
บ       sponsibility for any aspect of its use, and do not claim suit-    บ
บ       ability for a particular purpose.  Enjoy!                         บ
บ                                                                         บ
บ       NOTE:  This statement does not affect the copyright status of     บ
บ              other software that accompanies it.  Please respect the    บ
บ              Copyright claims exercised by the other software authors.  บ
บ                                                                         บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

typedef unsigned char      Byte;

typedef unsigned int       Word;

typedef unsigned long      DWord;

typedef enum {false, true} Boolean;

typedef struct
{
  Byte Red;
  Byte Grn;
  Byte Blu;
}
RGB;

typedef RGB Palette_Register[256];

typedef float TDA[3];

typedef int   TDIA[3];

typedef float FDA[4];

typedef float Matx4x4[4][4];

#define MaxCol   7
#define MaxInten 35

#define Ln10 2.30258509299405E+000
#define OneOverLn10 0.43429448190325E+000
#define Pi 3.1415927
#define PiOver180 1.74532925199433E-002
#define PiUnder180 5.72957795130823E+001

// System Modes
#define GC_SYSMODE_SCENE 1
#define GC_SYSMODE_OBJECT 2
#define GC_SYSMODE_VERTEX 3
#define GC_SYSMODE_STUDIO 4

#define GC_CRT_XRES 1024		// Define Screen Resolution
#define GC_CRT_YRES 768

#define GC_VIEW_XRES 800
#define GC_VIEW_YRES 600
#define GC_VIEW_LEFT 0			// Define Viewer Viewport
#define GC_VIEW_TOP 0
#define GC_VIEW_RIGHT 799
#define GC_VIEW_BOTTOM 599

#define GC_STAT_LEFT 806		// Define Status Viewport
#define GC_STAT_TOP 0
#define GC_STAT_RIGHT 1023
#define GC_STAT_BOTTOM 599

#define GC_MSG_LEFT 0			// Define Messsage Viewport
#define GC_MSG_TOP 606
#define GC_MSG_RIGHT 1023
#define GC_MSG_BOTTOM 767


#define KEY_ESC 0x1B

struct ViewType {
	TDA Position;
	float Roll, Pitch, Yaw;
	float Zoom;
	float FOV;
	float ViewPlaneDist;
	Matx4x4 RotMatrix;
};

struct SystemParmType {
	int SystemMode;
	float ScaleDelta;
	float LinDelta;
	float AngDelta;
};

struct ColorType {
	float R;
	float G;
	float B;
};

struct VertexType {
	TDA Vertex;
	struct VertexType *Prev;
	struct VertexType *Next;
};

struct PolyType {
	int NumberVertices;
	struct VertexType *VertexPtr;
};

struct ConeType {
	TDA Center1;
	float Radius1;
	TDA Center2;
	float Radius2;
};

struct SphereType {
	TDA Center;
	float Radius;
};

struct PatchType {
	TDA Vertex1;
	TDA Normal1;
	TDA Vertex2;
	TDA Normal2;
	TDA Vertex3;
	TDA Normal3;
};

struct RingType {
	TDA Center;
	float Radius1;
	float Radius2;
	TDA Normal;
};

union PrimUnion {
	struct PolyType Poly;
	struct ConeType Cone;
	struct SphereType Sphere;
	struct PatchType Patch;
	struct RingType Ring;
};

struct TransformType {
	float Scale;
	float DX;
	float DY;
	float DZ;
	float Roll;
	float Pitch;
	float Yaw;
};

struct SurfaceType {
	struct ColorType Diffuse;
	struct ColorType Ambient;
	struct ColorType Specular;
	int ShinePower;
	struct ColorType Transparent;
	float IORNumber;
	float FuzzMag;
	unsigned int AntiAlias;
};

struct PrimType {
	int Type;
	union PrimUnion Prim;
	int ColorIndex;
	struct PrimType *Prev;
	struct PrimType *Next;
};

#define GC_ANTIALIAS_ENABLE 1
#define GC_ANTIALIAS_DISABLE 0

struct ObjectType {
	int ID;
	unsigned int Visible;
	unsigned int DirtyFlag;
	char FileName[81];
	struct TransformType Transform;
	struct SurfaceType Surface;
	struct PrimType *PrimListPtr;
	struct ObjectType *Prev;
	struct ObjectType *Next;
};

struct StudioType {
	char FileName[81];
	TDA From;
	TDA At;
	TDA Up;
	float Angle;
	int I;
	int J;
	int Start;
	int Stop;
	float Aspect;
	char Projection[32];
	struct ColorType Ambient;
	struct ColorType Background;
	float Haze;
	char AntiAlias[32];
	int Threshold;
	Boolean Jitter;
	float Aperture;
	float FocalLength;
	int Samples;
	Boolean NoShadows;
	Boolean NoExpTrans;
	Boolean Caustics;
	int Depth;
};


// Primitive Types
#define GC_PRIM_POLYGON 1
#define GC_PRIM_PATCH 2
#define GC_PRIM_SPHERE 3
#define GC_PRIM_CONE 4
#define GC_PRIM_RING 5

// Object Attributes
#define GC_OBJECT_VISIBLE 1
#define GC_OBJECT_INVISIBLE 0

#define GC_STD_COLOR_OBJ 183
#define GC_HIGH_COLOR_OBJ 191
#define GC_STD_COLOR_PRIM 119
#define GC_HIGH_COLOR_PRIM 127
#define GC_TEXT_COLOR_MSG 60
#define GC_TEXT_COLOR_STAT 55
#define GC_BAR_COLOR 40


/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ              Program  ED - A DATABASE MODELING TOOL                     บ
บ                                                                         บ
บ              EDSYS.C    System initialization functions are coded       บ
บ                         here to setup initial conditions for ED.        บ
บ                                                                         บ
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

#include <stdio.h>
#include <string.h>
#include <graphics.h>
#include <dos.h>
#include "edmain.h"
#include "edprotos.h"
#include "bkgraph.h"
#include "bkmath.h"

extern struct SystemParmType gvSystemParms;
extern struct ViewType gvViewer;

extern struct StudioType gvStudio;
extern struct ObjectType *gvObjectListPtr;
extern struct ObjectType *gvCurrentObjectPtr;
extern struct PrimType *gvCurrentPrimPtr;
extern struct VertexType *gvCurrentVertexPtr;
extern struct TransformType gvCurrentTransform;
extern struct SurfaceType gvCurrentSurface;

void Error_Beep () {
	sound(1000);
	delay(250);
	nosound();
}



void Initialize_Viewer() {

	gvViewer.Zoom = 1.0;
	gvViewer.FOV = gvStudio.Angle;
	gvViewer.Yaw = 0.0;
	gvViewer.Pitch = 0.0;
	gvViewer.Position[0] = gvStudio.From[0];
	gvViewer.Position[1] = gvStudio.From[1];
	gvViewer.Position[2] = gvStudio.From[2];
}

void Initialize_Studio () {
	strcpy (gvStudio.FileName, "studio.b");
	Vec (1000.0, 0.0, 0.0, gvStudio.From);
	Vec (-5000.0, 0.0, 0.0, gvStudio.At);
	Vec (0.0, 0.0, 1.0, gvStudio.Up);
	gvStudio.Angle = 35;
	gvStudio.I = GC_CRT_XRES;
	gvStudio.J = GC_CRT_YRES;
	gvStudio.Start = 0;
	gvStudio.Stop = GC_CRT_YRES;
	gvStudio.Aspect = ((float) GC_CRT_XRES) / ((float) GC_CRT_YRES);
	strcpy (gvStudio.Projection, "flat");
	gvStudio.Ambient.R = 0.2;
	gvStudio.Ambient.G = 0.2;
	gvStudio.Ambient.B = 0.2;
	gvStudio.Background.R = 0.6;
	gvStudio.Background.G = 0.6;
	gvStudio.Background.B = 0.6;
	gvStudio.Haze = 0.0;
	strcpy (gvStudio.AntiAlias, "none");
	gvStudio.Threshold = 16;
	gvStudio.Jitter = false;
	gvStudio.Aperture = 0.0;
	gvStudio.FocalLength = 0.0;
	gvStudio.Samples = 8;
	gvStudio.NoShadows = false;
	gvStudio.NoExpTrans = false;
	gvStudio.Caustics = false;
	gvStudio.Depth = 20;
}

void Initialize_Object_Data () {

	gvObjectListPtr = NULL;
	gvCurrentObjectPtr = NULL;
	gvCurrentPrimPtr = NULL;
	gvCurrentVertexPtr = NULL;

	gvCurrentTransform.Scale = 1.0;

	gvCurrentTransform.DX = 0.0;
	gvCurrentTransform.DY = 0.0;
	gvCurrentTransform.DZ = 0.0;

	gvCurrentTransform.Roll = 0.0;
	gvCurrentTransform.Pitch = 0.0;
	gvCurrentTransform.Yaw = 0.0;


	gvCurrentSurface.Diffuse.R = 0.0;
	gvCurrentSurface.Diffuse.G = 0.0;
	gvCurrentSurface.Diffuse.B = 0.0;

	gvCurrentSurface.Ambient.R = 0.0;
	gvCurrentSurface.Ambient.G = 0.0;
	gvCurrentSurface.Ambient.B = 0.0;

	gvCurrentSurface.Specular.R = 0.0;
	gvCurrentSurface.Specular.G = 0.0;
	gvCurrentSurface.Specular.B = 0.0;

	gvCurrentSurface.ShinePower = 1;

	gvCurrentSurface.Transparent.R = 0.0;
	gvCurrentSurface.Transparent.G = 0.0;
	gvCurrentSurface.Transparent.B = 0.0;

	gvCurrentSurface.IORNumber = 1.0;
	gvCurrentSurface.FuzzMag = 0.0;
	gvCurrentSurface.AntiAlias = GC_ANTIALIAS_ENABLE;
}

void Initialize_System_Parms () {
	gvSystemParms.LinDelta = 50.0;
	gvSystemParms.AngDelta = 5.0;
	gvSystemParms.ScaleDelta = 1.25;
}

void Delete_All_Objects() {
	struct ObjectType *ObjPtr, *NextObjPtr;

	ObjPtr = gvObjectListPtr;

	while (ObjPtr != NULL) {
		NextObjPtr = ObjPtr->Next;
		Delete_Object (ObjPtr);
		ObjPtr = NextObjPtr;
	}
}

void Initialize_System() {

	Initialize_System_Parms ();
	Initialize_Object_Data ();
	Initialize_Studio ();
	Initialize_Viewer ();
	Initialize_Graphics ();
	SubDivide_Screen ();
}


void Shutdown_System() {

	Delete_All_Objects();
	Shutdown_Graphics();
}
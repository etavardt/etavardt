/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ              Program  ED - A DATABASE MODELING TOOL                     บ
บ                                                                         บ
บ              EDEDIT.C   Provides functions for editing data             บ
บ                         for objects, surfaces, transforms,              บ
บ                         system and studio parameters. Prompts           บ
บ                         and responses are placed in the dialog          บ
บ                         area of the screen.                             บ
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
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "edmain.h"
#include "edprotos.h"


extern struct SystemParmType gvSystemParms;
extern struct PrimType *gvCurrentPrimPtr;
extern struct ViewType gvViewer;
extern struct StudioType gvStudio;

void Edit_Vertex (TDA Vertex, int VertexNumber) {

	char InBuffer[80];
	char OutBuffer[80];

	sprintf (OutBuffer, " Vertex %d X [%f]: ", VertexNumber, Vertex[0]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		Vertex[0] = atof (InBuffer);
	}

	sprintf (OutBuffer, " Vertex %d Y [%f]: ", VertexNumber, Vertex[1]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		Vertex[1] = atof (InBuffer);
	}

	sprintf (OutBuffer, " Vertex %d Z [%f]: ", VertexNumber, Vertex[2]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		Vertex[2] = atof (InBuffer);
	}
}

void Edit_Polygon (struct PrimType *PrimPtr) {
	struct VertexType *VertexPtr;
	int Subi;

	Subi = 0;

	VertexPtr = PrimPtr->Prim.Poly.VertexPtr;

	while (VertexPtr != NULL) {
		Edit_Vertex (VertexPtr->Vertex, Subi);
		Subi++;
		VertexPtr = VertexPtr->Next;
	}
}

void Edit_Sphere (struct PrimType *PrimPtr) {

	char InBuffer[80];
	char OutBuffer[80];

	sprintf (OutBuffer, "Center X [%f] : ",
		PrimPtr->Prim.Sphere.Center[0]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Sphere.Center[0] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Center Y [%f] : ",
		PrimPtr->Prim.Sphere.Center[1]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Sphere.Center[1] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Center Z [%f] : ",
		PrimPtr->Prim.Sphere.Center[2]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Sphere.Center[2] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Radius [%f] : ",
		PrimPtr->Prim.Sphere.Radius);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Sphere.Radius = atof (InBuffer);
	}

}

void Edit_Cone (struct PrimType *PrimPtr) {

	char InBuffer[80];
	char OutBuffer[80];

	sprintf (OutBuffer, "Base Center X [%f] : ",
		PrimPtr->Prim.Cone.Center1[0]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Cone.Center1[0] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Base Center Y [%f] : ",
		PrimPtr->Prim.Cone.Center1[1]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Cone.Center1[1] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Base Center Z [%f] : ",
		PrimPtr->Prim.Cone.Center1[2]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Cone.Center1[2] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Base Radius [%f] : ",
		PrimPtr->Prim.Cone.Radius1);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Cone.Radius1 = atof (InBuffer);
	}

	sprintf (OutBuffer, "Apex Center X [%f] : ",
		PrimPtr->Prim.Cone.Center2[0]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Cone.Center2[0] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Apex Center Y [%f] : ",
		PrimPtr->Prim.Cone.Center2[1]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Cone.Center2[1] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Apex Center Z [%f] : ",
		PrimPtr->Prim.Cone.Center2[2]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Cone.Center2[2] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Apex Radius [%f] : ",
		PrimPtr->Prim.Cone.Radius2);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Cone.Radius2 = atof (InBuffer);
	}

}

void Edit_Patch (struct PrimType *PrimPtr) {

	char InBuffer[80];
	char OutBuffer[80];

	sprintf (OutBuffer, "Vertex 1 X [%f] : ",
		PrimPtr->Prim.Patch.Vertex1[0]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Patch.Vertex1[0] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Vertex 1 Y [%f] : ",
		PrimPtr->Prim.Patch.Vertex1[1]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Patch.Vertex1[1] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Vertex 1 Z [%f] : ",
		PrimPtr->Prim.Patch.Vertex1[2]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Patch.Vertex1[2] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Normal 1 X [%f] : ",
		PrimPtr->Prim.Patch.Normal1[0]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Patch.Normal1[0] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Normal 1 Y [%f] : ",
		PrimPtr->Prim.Patch.Normal1[1]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Patch.Normal1[1] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Normal 1 Z [%f] : ",
		PrimPtr->Prim.Patch.Normal1[2]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Patch.Normal1[2] = atof (InBuffer);
	}
	sprintf (OutBuffer, "Vertex 2 X [%f] : ",
		PrimPtr->Prim.Patch.Vertex2[0]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Patch.Vertex2[0] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Vertex 2 Y [%f] : ",
		PrimPtr->Prim.Patch.Vertex2[1]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Patch.Vertex2[1] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Vertex 2 Z [%f] : ",
		PrimPtr->Prim.Patch.Vertex2[2]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Patch.Vertex2[2] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Normal 2 X [%f] : ",
		PrimPtr->Prim.Patch.Normal2[0]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Patch.Normal2[0] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Normal 2 Y [%f] : ",
		PrimPtr->Prim.Patch.Normal2[1]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Patch.Normal2[1] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Normal 2 Z [%f] : ",
		PrimPtr->Prim.Patch.Normal2[2]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Patch.Normal2[2] = atof (InBuffer);
	}
	sprintf (OutBuffer, "Vertex 3 X [%f] : ",
		PrimPtr->Prim.Patch.Vertex3[0]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Patch.Vertex3[0] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Vertex 3 Y [%f] : ",
		PrimPtr->Prim.Patch.Vertex3[1]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Patch.Vertex3[1] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Vertex 3 Z [%f] : ",
		PrimPtr->Prim.Patch.Vertex3[2]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Patch.Vertex3[2] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Normal 3 X [%f] : ",
		PrimPtr->Prim.Patch.Normal3[0]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Patch.Normal3[0] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Normal 3 Y [%f] : ",
		PrimPtr->Prim.Patch.Normal3[1]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Patch.Normal3[1] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Normal 3 Z [%f] : ",
		PrimPtr->Prim.Patch.Normal3[2]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Patch.Normal3[2] = atof (InBuffer);
	}
}

void Edit_Ring (struct PrimType *PrimPtr) {

	char InBuffer[80];
	char OutBuffer[80];

	sprintf (OutBuffer, "Center X [%f] : ",
		PrimPtr->Prim.Ring.Center[0]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Ring.Center[0] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Center Y [%f] : ",
		PrimPtr->Prim.Ring.Center[1]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Ring.Center[1] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Center Z [%f] : ",
		PrimPtr->Prim.Ring.Center[2]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Ring.Center[2] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Inner Radius [%f] : ",
		PrimPtr->Prim.Ring.Radius1);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Ring.Radius1 = atof (InBuffer);
	}

	sprintf (OutBuffer, "Outer Radius [%f] : ",
		PrimPtr->Prim.Ring.Radius2);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Ring.Radius2 = atof (InBuffer);
	}

	sprintf (OutBuffer, "Normal X [%f] : ",
		PrimPtr->Prim.Ring.Normal[0]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Ring.Normal[0] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Normal Y [%f] : ",
		PrimPtr->Prim.Ring.Normal[1]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Ring.Normal[1] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Normal Z [%f] : ",
		PrimPtr->Prim.Ring.Normal[2]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		PrimPtr->Prim.Ring.Normal[2] = atof (InBuffer);
	}
}

void Edit_Primitive (struct PrimType *PrimPtr) {

	switch (PrimPtr->Type) {

		case GC_PRIM_POLYGON:
			Edit_Polygon (PrimPtr);
			break;

		case GC_PRIM_CONE:
			Edit_Cone (PrimPtr);
			break;

		case GC_PRIM_SPHERE:
			Edit_Sphere (PrimPtr);
			break;

		case GC_PRIM_PATCH:
			Edit_Patch (PrimPtr);
			break;

		case GC_PRIM_RING:
			Edit_Ring (PrimPtr);
			break;
	}

	Draw_View ();
}

void Edit_Object (struct ObjectType *ObjPtr) {
	char OutBuffer[80];
	char InBuffer[80];

	sprintf (OutBuffer, "Object File Name [%s] : ", ObjPtr->FileName);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		strcpy (ObjPtr->FileName, InBuffer);
	}

	sprintf (OutBuffer, "Diffuse Red [%f] : ", ObjPtr->Surface.Diffuse.R);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		ObjPtr->Surface.Diffuse.R = atof (InBuffer);
	}

	sprintf (OutBuffer, "Diffuse Green [%f] : ", ObjPtr->Surface.Diffuse.G);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		ObjPtr->Surface.Diffuse.G = atof (InBuffer);
	}

	sprintf (OutBuffer, "Diffuse Blue [%f] : ", ObjPtr->Surface.Diffuse.B);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		ObjPtr->Surface.Diffuse.B = atof (InBuffer);
	}

	sprintf (OutBuffer, "Ambient Red [%f] : ", ObjPtr->Surface.Ambient.R);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		ObjPtr->Surface.Ambient.R = atof (InBuffer);
	}

	sprintf (OutBuffer, "Diffuse Green [%f] : ", ObjPtr->Surface.Ambient.G);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		ObjPtr->Surface.Ambient.G = atof (InBuffer);
	}

	sprintf (OutBuffer, "Diffuse Blue [%f] : ", ObjPtr->Surface.Ambient.B);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		ObjPtr->Surface.Ambient.B = atof (InBuffer);
	}

	sprintf (OutBuffer, "Specular Red [%f] : ", ObjPtr->Surface.Specular.R);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		ObjPtr->Surface.Specular.R = atof (InBuffer);
	}

	sprintf (OutBuffer, "Specular Green [%f] : ", ObjPtr->Surface.Specular.G);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		ObjPtr->Surface.Specular.G = atof (InBuffer);
	}

	sprintf (OutBuffer, "Specular Blue [%f] : ", ObjPtr->Surface.Specular.B);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		ObjPtr->Surface.Specular.B = atof (InBuffer);
	}

	sprintf (OutBuffer, "Shine Power [%d] : ", ObjPtr->Surface.ShinePower);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		ObjPtr->Surface.ShinePower = atoi (InBuffer);
	}

	sprintf (OutBuffer, "Transparency Red [%f] : ", ObjPtr->Surface.Transparent.R);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		ObjPtr->Surface.Transparent.R = atof (InBuffer);
	}

	sprintf (OutBuffer, "Transparency Green [%f] : ", ObjPtr->Surface.Transparent.G);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		ObjPtr->Surface.Transparent.G = atof (InBuffer);
	}

	sprintf (OutBuffer, "Transparency Blue [%f] : ", ObjPtr->Surface.Transparent.B);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		ObjPtr->Surface.Transparent.B = atof (InBuffer);
	}

	sprintf (OutBuffer, "IOR Value [%f] : ", ObjPtr->Surface.IORNumber);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		ObjPtr->Surface.IORNumber = atof (InBuffer);
	}

	sprintf (OutBuffer, "Fuzz Mag [%f] : ", ObjPtr->Surface.FuzzMag);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		ObjPtr->Surface.FuzzMag = atof (InBuffer);
	}

	sprintf (OutBuffer, "AnitAlias Disable [%i] : ", ObjPtr->Surface.AntiAlias);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		ObjPtr->Surface.AntiAlias = atoi (InBuffer);
	}

	sprintf (OutBuffer, "Translate X [%f] : ", ObjPtr->Transform.DX);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		ObjPtr->Transform.DX = atof (InBuffer);
	}

	sprintf (OutBuffer, "Translate Y [%f] : ", ObjPtr->Transform.DY);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		ObjPtr->Transform.DY = atof (InBuffer);
	}

	sprintf (OutBuffer, "Translate Z [%f] : ", ObjPtr->Transform.DZ);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		ObjPtr->Transform.DZ = atof (InBuffer);
	}

	sprintf (OutBuffer, "Rotate Roll [%f] : ", ObjPtr->Transform.Roll);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		ObjPtr->Transform.Roll = atof (InBuffer);
	}

	sprintf (OutBuffer, "Rotate Pitch [%f] : ", ObjPtr->Transform.Pitch);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		ObjPtr->Transform.Pitch = atof (InBuffer);
	}

	sprintf (OutBuffer, "Rotate Yaw [%f] : ", ObjPtr->Transform.Yaw);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		ObjPtr->Transform.Yaw = atof (InBuffer);
	}

	sprintf (OutBuffer, "Scale Factor [%f] : ", ObjPtr->Transform.Scale);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		ObjPtr->Transform.Scale = atof (InBuffer);
	}
}


void Edit_Studio () {


	char OutBuffer[80];
	char InBuffer[80];

	sprintf (OutBuffer, "From X [%f] : ", gvStudio.From[0]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvStudio.From[0] = atof (InBuffer);
	}

	sprintf (OutBuffer, "From Y [%f] : ", gvStudio.From[1]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvStudio.From[1] = atof (InBuffer);
	}

	sprintf (OutBuffer, "From Z [%f] : ", gvStudio.From[2]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvStudio.From[2] = atof (InBuffer);
	}

	sprintf (OutBuffer, "At X [%f] : ", gvStudio.At[0]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvStudio.At[0] = atof (InBuffer);
	}

	sprintf (OutBuffer, "At Y [%f] : ", gvStudio.At[1]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvStudio.At[1] = atof (InBuffer);
	}

	sprintf (OutBuffer, "At Z [%f] : ", gvStudio.At[2]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvStudio.At[2] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Up X [%f] : ", gvStudio.Up[0]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvStudio.Up[0] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Up Y [%f] : ", gvStudio.Up[1]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvStudio.Up[1] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Up Z [%f] : ", gvStudio.Up[2]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvStudio.Up[2] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Angle [%f] : ", gvStudio.Angle);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvStudio.Angle = atof (InBuffer);
	}

	sprintf (OutBuffer, "Resolution i [%d] : ", gvStudio.I);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvStudio.I = atoi (InBuffer);
	}

	sprintf (OutBuffer, "Resolution j [%d] : ", gvStudio.J);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvStudio.J = atoi (InBuffer);
	}

	sprintf (OutBuffer, "Start [%d] : ", gvStudio.Start);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvStudio.Start = atoi (InBuffer);
	}

	sprintf (OutBuffer, "Stop [%d] : ", gvStudio.Stop);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvStudio.Stop = atoi (InBuffer);
	}

	sprintf (OutBuffer, "Aspect Ratio [%f] : ", gvStudio.Aspect);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvStudio.Aspect = atof (InBuffer);
	}

	sprintf (OutBuffer, "Projection [%s] : ", gvStudio.Projection);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		strcpy (gvStudio.Projection, InBuffer);
	}

	sprintf (OutBuffer, "Ambient Red [%f] : ", gvStudio.Ambient.R);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvStudio.Ambient.R = atof (InBuffer);
	}

	sprintf (OutBuffer, "Ambient Green [%f] : ", gvStudio.Ambient.G);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvStudio.Ambient.G = atof (InBuffer);
	}

	sprintf (OutBuffer, "Ambient Blue [%f] : ", gvStudio.Ambient.B);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvStudio.Ambient.B = atof (InBuffer);
	}

	sprintf (OutBuffer, "Background Red [%f] : ", gvStudio.Background.R);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvStudio.Background.R = atof (InBuffer);
	}

	sprintf (OutBuffer, "Background Green [%f] : ", gvStudio.Background.G);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvStudio.Background.G = atof (InBuffer);
	}

	sprintf (OutBuffer, "Background Blue [%f] : ", gvStudio.Background.B);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvStudio.Background.B = atof (InBuffer);
	}

	sprintf (OutBuffer, "Haze Density [%f] : ", gvStudio.Haze);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvStudio.Haze = atof (InBuffer);
	}

	sprintf (OutBuffer, "AntiAlias Mode [%s] : ", gvStudio.AntiAlias);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		strcpy (gvStudio.AntiAlias, InBuffer);
	}

	sprintf (OutBuffer, "Threshold [%d] : ", gvStudio.Threshold);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvStudio.Threshold = atoi (InBuffer);
	}

	sprintf (OutBuffer, "Jitter [%d] : ", gvStudio.Jitter);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvStudio.Jitter = atoi (InBuffer);
	}

	sprintf (OutBuffer, "Aperture [%f] : ", gvStudio.Aperture);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvStudio.Aperture = atof (InBuffer);
	}

	sprintf (OutBuffer, "Focal Length [%f] : ", gvStudio.FocalLength);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvStudio.FocalLength = atof (InBuffer);
	}

	sprintf (OutBuffer, "Samples [%d] : ", gvStudio.Samples);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvStudio.Samples = atoi (InBuffer);
	}

	sprintf (OutBuffer, "Shadow Disable [%d] : ", gvStudio.NoShadows);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvStudio.NoShadows = atoi (InBuffer);
	}

	sprintf (OutBuffer, "Exponential Atten Disable [%d] : ", gvStudio.NoExpTrans);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvStudio.NoExpTrans = atoi (InBuffer);
	}

	sprintf (OutBuffer, "Caustics Enable [%d] : ", gvStudio.Caustics);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvStudio.Caustics = atoi (InBuffer);
	}

	sprintf (OutBuffer, "Max Recursion Depth [%d] : ", gvStudio.Depth);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvStudio.Depth = atoi (InBuffer);
	}

}

void Edit_SysParms () {

	char OutBuffer[80];
	char InBuffer[80];



	sprintf (OutBuffer, "Eyepoint X [%f] : ", gvViewer.Position[0]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvViewer.Position[0] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Eyepoint Y [%f] : ", gvViewer.Position[1]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvViewer.Position[1] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Eyepoint Z [%f] : ", gvViewer.Position[2]);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvViewer.Position[2] = atof (InBuffer);
	}

	sprintf (OutBuffer, "Eyepoint Roll [%f] : ", gvViewer.Roll);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvViewer.Roll = atof (InBuffer);
	}

	sprintf (OutBuffer, "Eyepoint Pitch [%f] : ", gvViewer.Pitch);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvViewer.Pitch = atof (InBuffer);
	}

	sprintf (OutBuffer, "Eyepoint Yaw [%f] : ", gvViewer.Yaw);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvViewer.Yaw = atof (InBuffer);
	}

	sprintf (OutBuffer, "Field of View [%f] : ", gvViewer.FOV);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvViewer.FOV = atof (InBuffer);
	}

	sprintf (OutBuffer, "Zoom Factor [%f] : ", gvViewer.Zoom);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvViewer.Zoom = atof (InBuffer);
	}

	sprintf (OutBuffer, "Linear Delta [%f] : ", gvSystemParms.LinDelta);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvSystemParms.LinDelta = atof (InBuffer);
	}

	sprintf (OutBuffer, "Angular Delta [%f] : ", gvSystemParms.AngDelta);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvSystemParms.AngDelta = atof (InBuffer);
	}

	sprintf (OutBuffer, "Scale Delta [%f] : ", gvSystemParms.ScaleDelta);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		gvSystemParms.ScaleDelta = atof (InBuffer);
	}
}
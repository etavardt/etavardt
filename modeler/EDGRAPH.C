/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ              Program  ED - A DATABASE MODELING TOOL                     บ
บ                                                                         บ
บ              EDGRAPH.C  Provides functions which perform all the        บ
บ                         graphics and related mathematical processing    บ
บ                         necessary to create the 3D wireframe            บ
บ                         representation of objects.                      บ
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
#include <graphics.h>
#include <math.h>
#include <values.h>
#include "edmain.h"
#include "edprotos.h"
#include "bkgraph.h"
#include "bkmath.h"

#define CLIP_TO_VIEWPORT 1
#define NUMBER_CONE_FACETS 18
#define NUMBER_SPHERE_FACETS 18
#define NUMBER_RING_FACETS 18

extern struct ObjectType *gvObjectListPtr;
extern struct ObjectType *gvCurrentObjectPtr;
extern struct PrimType *gvCurrentPrimPtr;
extern struct ViewType gvViewer;


// Various pre-computed values for conversions

float ScreenSpace_X_Limit;
float ScreenSpace_Y_Limit;
float ScreenSpace_CenterX;
float ScreenSpace_CenterY;
float ScreenSpace_AspectRatio;


void Update_ESR_Matrix () {

	Matx4x4 Rx, Ry, Rz, Rxy;

	Rotate3D (1, gvViewer.Roll, Rx);
	Rotate3D (2, gvViewer.Pitch, Ry);
	Rotate3D (3, gvViewer.Yaw, Rz);
	Multiply3DMatricies (Rx, Ry, Rxy);
	Multiply3DMatricies (Rxy, Rz, gvViewer.RotMatrix);
}

void WorldSpace_To_EyeSpace ( TDA InPoint, TDA OutPoint) {

	TDA XlatPoint;

	VecSub (InPoint, gvViewer.Position, XlatPoint);
	Transform (XlatPoint, gvViewer.RotMatrix, OutPoint);
}

void Update_ViewPlane_Dist () {
	float VPDist;
	float CosFOV, SinFOV;

	CosFOV = CosD (gvViewer.FOV);
	SinFOV = SinD (gvViewer.FOV);

	if (SinFOV != 0.0) {
		VPDist = (((float) GC_VIEW_XRES) * 0.5) * CosFOV / SinFOV;
		VPDist = VPDist * gvViewer.Zoom;
	}

	else {
		VPDist = MAXFLOAT;
	}

	gvViewer.ViewPlaneDist = VPDist;
}

void Initialize_ScreenSpace_Values () {

	ScreenSpace_X_Limit = ((float) (GC_VIEW_XRES));
	ScreenSpace_Y_Limit = ((float) (GC_VIEW_YRES));
	ScreenSpace_CenterX = ((float) (GC_VIEW_XRES)) * 0.5;
//	ScreenSpace_CenterY = ((float) (GC_VIEW_YRES)) * 0.5;

	//NOTE:	The Screen Space "Center" is chosen to be somewhat higher
	//			than true center in order to make the perspective
	//			effect appear correct.  You might want to play with this
	//			a little to see the results.  In particular, set it to
	//			the true pixel center of the screen, and notice how
	//			objects above your eyepoint seem to be pulled toward
	//			the horizon excessively.  Empirically, a ratio of
	//			horizon to grouns plane of about 3/8 yields
	//			natural looking perspective.
	ScreenSpace_CenterY = ((float) (GC_VIEW_YRES)) * 0.375;
	ScreenSpace_AspectRatio = ((float) GC_VIEW_XRES) / ((float) GC_VIEW_YRES);
}

int Hither_Clip_Line (TDA InPoint1, TDA InPoint2) {

	int ReturnStatus;

	ReturnStatus = true;

	if ((InPoint1[0] > -1.0) && (InPoint2[0] > -1.0)) {
		ReturnStatus = false;
	}

	return (ReturnStatus);
}

void EyeSpace_To_ScreenSpace (TDA InPoint, float *OutX, float *OutY) {

	TDA TempV;

	// Transform from world coordinate axis to screen axis

	TempV[0] = InPoint[1];
	TempV[1] = - InPoint[2];
	TempV[2] = - InPoint[0];

	// Perform Perspective Divide and Scale

	TempV[0] = (TempV[0] / (MAX (1.0, TempV[2]))) * gvViewer.ViewPlaneDist;
	TempV[1] = (TempV[1] / (MAX (1.0, TempV[2]))) * gvViewer.ViewPlaneDist;

	// You now have 2D coordinates.
	// Now Correct for Aspect Ratio of Display

	TempV[0] *= ScreenSpace_AspectRatio;

	// Now adjust so that upper left is 0,0

	TempV[0] += ScreenSpace_CenterX;
	TempV[1] += ScreenSpace_CenterY;

	*OutX = TempV[0];
	*OutY = TempV[1];
}

int Clip_Point (float x, float y) {

	int ReturnStatus;

	ReturnStatus = false;

	if ((x > 0.0) && (x < ScreenSpace_X_Limit)) {

		if ((y > 0.0) && (y < ScreenSpace_Y_Limit)) {
			ReturnStatus = true;
		}
	}

	return (ReturnStatus);
}

int Clip_To_Edge ( float Denom, float Num, float *te, float *tl) {
	int ReturnStatus;
	float t;

	ReturnStatus = true;

	if (Denom > 0.0) {
		t = Num / Denom;

		if (t > *tl) {
			ReturnStatus = false;
		}

		else {

			if (t > *te) {
				*te = t;
			}
		}
	}

	else {

		if (Denom < 0.0) {
			t = Num / Denom;

			if (t < *te) {
				ReturnStatus = false;
			}

			else {

				if (t < *tl) {
					*tl = t;
				}
			}
		}

		else {

			if (Num > 0.0) {
				ReturnStatus = false;
			}
		}
	}

	return (ReturnStatus);
}

// This is Liang-Barsky line clipping algorithm.
// See Section 3.12 of "Computer Graphics", 2ed
// by Foley vanDam, Feiner, Hughes for algorithm details.
int Screen_Clip_Line (float *x0, float *y0, float *x1, float *y1) {
	float dx, dy;
	float te, tl;
	float tempx, tempy;

	if ((Clip_Point (*x0, *y0)) && (Clip_Point (*x1, *y1))) {
		// Both points are within the view volume
		// No clipping is needed
		return (true);
	}

	dx = *x1 - *x0;
	dy = *y1 - *y0;

	if ((fabs(dx) < 1.0) && (fabs(dy) < 1.0)) {
		// Line endpoints are less than 1 pixel apart.
		// Extinguish line by returning "false"
		return (false);
	}

	te = 0.0;
	tl = 1.0;

	if (Clip_To_Edge (dx, -*x0, &te, &tl)) {

		if (Clip_To_Edge (-dx, (*x0 - ScreenSpace_X_Limit), &te, &tl)) {

			if (Clip_To_Edge (dy, -*y0, &te, &tl)) {

				if (Clip_To_Edge (-dy, (*y0 - ScreenSpace_Y_Limit), &te, &tl)) {

					if (tl < 1.0) {
						*x1 = *x0 + (tl * dx);
						*y1 = *y0 + (tl * dy);
					}

					if (te > 0.0) {
						*x0 += (te * dx);
						*y0 += (te * dy);
					}
				}
			}
		}
	}

	return (true);
}

void Render_Line (TDA WorldPoint1, TDA WorldPoint2, int Color) {
	float ScreenX1, ScreenY1, ScreenX2, ScreenY2;
	TDA EyePoint1, EyePoint2;

	WorldSpace_To_EyeSpace (WorldPoint1, EyePoint1);
	WorldSpace_To_EyeSpace (WorldPoint2, EyePoint2);

	if (Hither_Clip_Line (EyePoint1, EyePoint2)) {
		EyeSpace_To_ScreenSpace (EyePoint1, &ScreenX1, &ScreenY1);
		EyeSpace_To_ScreenSpace (EyePoint2, &ScreenX2, &ScreenY2);

		if (Screen_Clip_Line (&ScreenX1, &ScreenY1, &ScreenX2, &ScreenY2)) {
			Line (((int) ScreenX1), ((int) ScreenY1),
				((int) ScreenX2), ((int) ScreenY2), Color);
		}

		else {
			// Do Nothing, Line is entirely outside view volume or is
			// too small to bother drawing.
		}
	}

	else {
		// Do Nothing, Line is entirely behind the viewer
	}
}

void Transform_Point (struct TransformType *XfrmPtr, TDA InputVector, TDA OutputVector) {

	Matx4x4 XfrmMatrix;

		// NOTE: The delta values passed here are SUBTRACTED
		// 		from the current position in the math function
		//			Translate3D(), which is called by PrepareMatrix().
		//			This means that to move an object
		//			UP requires a NEGATIVE DZ, etc.  Hence the
		//			need for negating the deltas.

	PrepareMatrix (-(XfrmPtr->DX), -(XfrmPtr->DY), -(XfrmPtr->DZ),
						XfrmPtr->Scale, XfrmPtr->Scale, XfrmPtr->Scale,
						XfrmPtr->Roll, XfrmPtr->Pitch, XfrmPtr->Yaw,
						XfrmMatrix);
	Transform (InputVector, XfrmMatrix, OutputVector);
}

void Draw_Polygon(struct PrimType *PrimPtr, struct TransformType *XfrmPtr) {

	struct VertexType *VertPtr, *NextVertPtr;
	TDA Vertex1, Vertex2;

	VertPtr = PrimPtr->Prim.Poly.VertexPtr;
	NextVertPtr = VertPtr->Next;

	while (NextVertPtr != NULL) {
		Transform_Point (XfrmPtr, VertPtr->Vertex, Vertex1);
		Transform_Point (XfrmPtr, NextVertPtr->Vertex, Vertex2);
		Render_Line (Vertex1, Vertex2, PrimPtr->ColorIndex);
		VertPtr = NextVertPtr;
		NextVertPtr = VertPtr->Next;
	}

	NextVertPtr = PrimPtr->Prim.Poly.VertexPtr;
	Transform_Point (XfrmPtr, VertPtr->Vertex, Vertex1);
	Transform_Point (XfrmPtr, NextVertPtr->Vertex, Vertex2);
	Render_Line (Vertex1, Vertex2, PrimPtr->ColorIndex);
}

void Draw_Cone (struct PrimType *PrimPtr, struct TransformType *XfrmPtr) {

	int Subi;
	float Theta, DTheta;
	TDA Axis, Normal1, Normal2, BaseRadius1, BaseRadius2;
	TDA ApexRadius1, ApexRadius2, Temp1, Temp2, Temp3;
	TDA P1, P2, P3, P4;
	TDA Vertex1, Vertex2, Vertex3, Vertex4;

	Theta = 0.0;
	DTheta = 360.0 / NUMBER_CONE_FACETS;
	VecSub (PrimPtr->Prim.Cone.Center2, PrimPtr->Prim.Cone.Center1, Axis);
	Vec (0.0, 0.0, 1.0, Temp1);

	if ( fabs (VecDot (Axis, Temp1)) ==
			(VecLen(Axis) * VecLen(Temp1))) {
		Vec (0.0, 1.0, 0.0, Temp1);
	}

	VecCross (Axis, Temp1, Normal1);
	VecNormalize (Normal1);
	VecCross (Normal1, Axis, Normal2);
	VecNormalize (Normal2);
	VecScalMult (PrimPtr->Prim.Cone.Radius1, Normal1, BaseRadius1);
	VecScalMult (PrimPtr->Prim.Cone.Radius1, Normal2, BaseRadius2);
	VecScalMult (PrimPtr->Prim.Cone.Radius2, Normal1, ApexRadius1);
	VecScalMult (PrimPtr->Prim.Cone.Radius2, Normal2, ApexRadius2);

	VecAdd (PrimPtr->Prim.Cone.Center1, BaseRadius1, P1);
	VecAdd (PrimPtr->Prim.Cone.Center2, ApexRadius1, P2);

	for (Subi = 1; Subi <= NUMBER_CONE_FACETS; Subi++) {
		Theta += DTheta;

		if (Theta >= 360.0) {
			Theta -= 360.0;
		}

		VecScalMult (CosD (Theta), ApexRadius1, Temp1);
		VecScalMult (SinD (Theta), ApexRadius2, Temp2);
		VecAdd (Temp1, Temp2, Temp3);
		VecAdd (PrimPtr->Prim.Cone.Center2, Temp3, P3);

		VecScalMult (CosD (Theta), BaseRadius1, Temp1);
		VecScalMult (SinD (Theta), BaseRadius2, Temp2);
		VecAdd (Temp1, Temp2, Temp3);
		VecAdd (PrimPtr->Prim.Cone.Center1, Temp3, P4);

		Transform_Point (XfrmPtr, P1, Vertex1);
		Transform_Point (XfrmPtr, P2, Vertex2);
		Transform_Point (XfrmPtr, P3, Vertex3);
		Transform_Point (XfrmPtr, P4, Vertex4);

		Render_Line (Vertex1, Vertex2, PrimPtr->ColorIndex);
		Render_Line (Vertex2, Vertex3, PrimPtr->ColorIndex);
		Render_Line (Vertex3, Vertex4, PrimPtr->ColorIndex);
		Render_Line (Vertex4, Vertex1, PrimPtr->ColorIndex);

		VecCopy (P3, P2);
		VecCopy (P4, P1);
	}
}

void Draw_Sphere (struct PrimType *PrimPtr, struct TransformType *XfrmPtr) {

	float Theta, Rho, DTheta, DRho;
	int Subi, Subj;
	TDA Center, Vertex1, Vertex2;
	float Radius;
	TDA Ring1[NUMBER_SPHERE_FACETS];
	TDA Ring2[NUMBER_SPHERE_FACETS];

	Theta = 0.0;
	Rho = 0.0;
	DTheta = 360.0 / NUMBER_SPHERE_FACETS;
	DRho = 360.0 / NUMBER_SPHERE_FACETS;
	VecCopy (PrimPtr->Prim.Sphere.Center, Center);
	Radius = PrimPtr->Prim.Sphere.Radius;

	for (Subj = 0; Subj < NUMBER_SPHERE_FACETS; Subj++) {
		Ring1[Subj][0] = Center[0] + (Radius * SinD (Theta) * CosD (Rho));
		Ring1[Subj][1] = Center[1] + (Radius * SinD (Theta) * SinD (Rho));
		Ring1[Subj][2] = Center[2] + (Radius * CosD (Theta));
		Rho += DRho;
	}

	for (Subj = 0; Subj < NUMBER_SPHERE_FACETS; Subj++) {
		Transform_Point (XfrmPtr, Ring1[Subj], Vertex1);
		Transform_Point (XfrmPtr, Ring1[((Subj + 1) % NUMBER_SPHERE_FACETS)], Vertex2);
		Render_Line (Vertex1, Vertex2, PrimPtr->ColorIndex);

	}

	// Only need to Compute Rho from 0 to 180, as what is
	// drawn from 180 to 360 is just a repeat.
	for (Subi = 0; Subi < NUMBER_SPHERE_FACETS; Subi += 2) {

		Theta += DTheta;
		Rho = 0.0;

		for (Subj = 0; Subj < NUMBER_SPHERE_FACETS; Subj++) {
			Ring2[Subj][0] = Center[0] + (Radius * SinD (Theta) * CosD (Rho));
			Ring2[Subj][1] = Center[1] + (Radius * SinD (Theta) * SinD (Rho));
			Ring2[Subj][2] = Center[2] + (Radius * CosD (Theta));
			Rho += DRho;
		}

		for (Subj = 0; Subj < NUMBER_SPHERE_FACETS; Subj++) {
			Transform_Point (XfrmPtr, Ring2[Subj], Vertex1);
			Transform_Point (XfrmPtr, Ring2[((Subj + 1) % NUMBER_SPHERE_FACETS)], Vertex2);
			Render_Line (Vertex1, Vertex2, PrimPtr->ColorIndex);
			Transform_Point (XfrmPtr, Ring1[Subj], Vertex2);
			Render_Line (Vertex1, Vertex2, PrimPtr->ColorIndex);
			VecCopy (Ring2[Subj], Ring1[Subj]);
		}
	}
}

void Draw_Patch (struct PrimType *PrimPtr, struct TransformType *XfrmPtr) {
	TDA Vertex1, Vertex2, Vertex3;

	Transform_Point (XfrmPtr, PrimPtr->Prim.Patch.Vertex1, Vertex1);
	Transform_Point (XfrmPtr, PrimPtr->Prim.Patch.Vertex2, Vertex2);
	Transform_Point (XfrmPtr, PrimPtr->Prim.Patch.Vertex3, Vertex3);

	Render_Line (Vertex1, Vertex2, PrimPtr->ColorIndex);
	Render_Line (Vertex2, Vertex3, PrimPtr->ColorIndex);
	Render_Line (Vertex3, Vertex1, PrimPtr->ColorIndex);
}

void Draw_Ring (struct PrimType *PrimPtr, struct TransformType *XfrmPtr) {

	int Subi;
	float Theta, DTheta;
	float R1, R2;
	TDA P1, P2, P3, P4;
	TDA RV11, RV12, RV21, RV22;
	TDA Temp1, Temp2, Temp3;
	TDA Vertex1, Vertex2, Vertex3, Vertex4;

	Theta = 0.0;
	DTheta = 360.0 / NUMBER_RING_FACETS;
	R1 = PrimPtr->Prim.Ring.Radius1;
	R2 = PrimPtr->Prim.Ring.Radius2;
	VecNormalize (PrimPtr->Prim.Ring.Normal);
	Vec (0.0, 0.0, 1.0, Temp1);

	if ( fabs (VecDot(PrimPtr->Prim.Ring.Normal, Temp1)) ==
			(VecLen(PrimPtr->Prim.Ring.Normal) * VecLen(Temp1))) {
		Vec (0.0, 1.0, 0.0, Temp1);
	}

	VecCross (PrimPtr->Prim.Ring.Normal, Temp1, Temp2);
	VecNormalize (Temp2);
	VecCross (Temp2, PrimPtr->Prim.Ring.Normal, Temp3);
	VecNormalize (Temp3);
	VecScalMult (R1, Temp2, RV11);
	VecScalMult (R1, Temp3, RV12);
	VecScalMult (R2, Temp2, RV21);
	VecScalMult (R2, Temp3, RV22);

	VecAdd (PrimPtr->Prim.Ring.Center, RV11, P1);
	VecAdd (PrimPtr->Prim.Ring.Center, RV21, P3);

	for (Subi = 1; Subi <= NUMBER_RING_FACETS; Subi++) {
		Theta += DTheta;

		if (Theta >= 360.0) {
			Theta = 0.0;
		}

		VecScalMult (CosD (Theta), RV11, Temp1);
		VecScalMult (SinD (Theta), RV12, Temp2);
		VecAdd (Temp1, Temp2, Temp3);
		VecAdd (PrimPtr->Prim.Ring.Center, Temp3, P2);

		VecScalMult (CosD (Theta), RV21, Temp1);
		VecScalMult (SinD (Theta), RV22, Temp2);
		VecAdd (Temp1, Temp2, Temp3);
		VecAdd (PrimPtr->Prim.Ring.Center, Temp3, P4);

		Transform_Point (XfrmPtr, P1, Vertex1);
		Transform_Point (XfrmPtr, P2, Vertex2);
		Transform_Point (XfrmPtr, P3, Vertex3);
		Transform_Point (XfrmPtr, P4, Vertex4);


		Render_Line (Vertex1, Vertex2, PrimPtr->ColorIndex);
		Render_Line (Vertex2, Vertex4, PrimPtr->ColorIndex);
		Render_Line (Vertex4, Vertex3, PrimPtr->ColorIndex);
		Render_Line (Vertex3, Vertex1, PrimPtr->ColorIndex);

		VecCopy (P2, P1);
		VecCopy (P4, P3);
	}
}

void Draw_Primitive (struct PrimType *PrimPtr, struct TransformType *XfrmPtr) {

	switch (PrimPtr->Type) {

		case GC_PRIM_POLYGON:
			Draw_Polygon (PrimPtr, XfrmPtr);
			break;

		case GC_PRIM_CONE:
			Draw_Cone (PrimPtr, XfrmPtr);
			break;

		case GC_PRIM_SPHERE:
			Draw_Sphere (PrimPtr, XfrmPtr);
			break;

		case GC_PRIM_PATCH:
			Draw_Patch (PrimPtr, XfrmPtr);
			break;

		case GC_PRIM_RING:
			Draw_Ring (PrimPtr, XfrmPtr);
			break;
	}
}

void Draw_Object(struct ObjectType *ObjPtr) {

	struct PrimType *PrimPtr;

	PrimPtr = ObjPtr->PrimListPtr;

	while (PrimPtr != NULL) {
		Draw_Primitive (PrimPtr, &(ObjPtr->Transform));
		PrimPtr = PrimPtr->Next;
	}
}

void HighLight_Primitive() {
	gvCurrentPrimPtr->ColorIndex = GC_HIGH_COLOR_PRIM;
//	setviewport (GC_VIEW_LEFT, GC_VIEW_TOP, GC_VIEW_RIGHT, GC_VIEW_BOTTOM, CLIP_TO_VIEWPORT);
//	Draw_Primitive (gvCurrentPrimPtr, &(gvCurrentObjectPtr->Transform));
}

void UnHighLight_Primitive() {
	gvCurrentPrimPtr->ColorIndex = GC_STD_COLOR_PRIM;
//	setviewport (GC_VIEW_LEFT, GC_VIEW_TOP, GC_VIEW_RIGHT, GC_VIEW_BOTTOM, CLIP_TO_VIEWPORT);
//	Draw_Primitive (gvCurrentPrimPtr, &(gvCurrentObjectPtr->Transform));
}

void HighLight_Object () {

	struct PrimType *PrimPtr;

	PrimPtr = gvCurrentObjectPtr->PrimListPtr;

	while (PrimPtr != NULL) {
		PrimPtr->ColorIndex = GC_HIGH_COLOR_OBJ;
		PrimPtr = PrimPtr->Next;
	}
}

void UnHighLight_Object () {

	struct PrimType *PrimPtr;

	PrimPtr = gvCurrentObjectPtr->PrimListPtr;

	while (PrimPtr != NULL) {
		PrimPtr->ColorIndex = GC_STD_COLOR_OBJ;
		PrimPtr = PrimPtr->Next;
	}
}


void Draw_View() {

	struct ObjectType *ObjPtr;

	setviewport (GC_VIEW_LEFT, GC_VIEW_TOP, GC_VIEW_RIGHT, GC_VIEW_BOTTOM, CLIP_TO_VIEWPORT);
	clearviewport ();
	ObjPtr = gvObjectListPtr;
	Update_ESR_Matrix ();
	Update_ViewPlane_Dist ();
	Init_Plotting (0, 0);
	Init_Perspective (true, 0.0, 0.0, 0.0, gvViewer.ViewPlaneDist);

	while (ObjPtr != NULL) {

		if (ObjPtr->Visible == GC_OBJECT_VISIBLE) {
			Draw_Object (ObjPtr);
		}

		ObjPtr = ObjPtr->Next;
	}
}

void Initialize_Graphics() {

	Palette_Register InitialPalette;

	Set_Graphics_Mode (GC_CRT_XRES,GC_CRT_YRES);
	XRes = GC_VIEW_XRES;
	YRes = GC_VIEW_YRES;
	Init_Palette (InitialPalette);
	Set_Palette (InitialPalette);
	Initialize_ScreenSpace_Values ();
}

void SubDivide_Screen() {

	int subi;

	setviewport (0, 0, GC_CRT_XRES, GC_CRT_YRES, 1);
	setcolor(GC_BAR_COLOR);
	for (subi = (GC_VIEW_BOTTOM + 1); subi < (GC_MSG_TOP - 1); subi++) {
		line (0, subi, (GC_CRT_XRES - 1), subi);
	}

	for (subi = (GC_VIEW_RIGHT + 1); subi < (GC_STAT_LEFT - 1); subi++) {
		line (subi, 0, subi, (GC_MSG_TOP - 1));
   }

}

void Shutdown_Graphics() {

	closegraph();
}

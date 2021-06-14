/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ              Program  ED - A DATABASE MODELING TOOL                     บ
บ                                                                         บ
บ              EDOBJECT.C Functions in this file handle adding,           บ
บ                         deleting, and selecting Object data             บ
บ                         structures.                                     บ
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
#include <alloc.h>
#include <mem.h>
#include <string.h>
#include "edmain.h"
#include "edprotos.h"
#include "edscmode.h"
#include "edomode.h"

static int ObjectCount;

extern struct SystemParmType gvSystemParms;
extern struct ObjectType *gvObjectListPtr;
extern struct ObjectType *gvCurrentObjectPtr;
extern struct PrimType *gvCurrentPrimPtr;
extern struct VertexType *gvCurrentVertexPtr;
extern struct SurfaceType gvCurrentSurface;
extern struct TransformType gvCurrentTransform;


struct ObjectType * Insert_Object (char *FileNamePtr) {
	struct ObjectType *LastPtr, *NewPtr;

	LastPtr = gvObjectListPtr;

	if (gvObjectListPtr == NULL) {
		gvObjectListPtr = malloc (sizeof (struct ObjectType));
		setmem (gvObjectListPtr, (sizeof (struct ObjectType)), 0);
		NewPtr = gvObjectListPtr;
		NewPtr->Prev = NULL;
		NewPtr->Next = NULL;
	}

	else {

		while (LastPtr->Next != NULL) {
			LastPtr = LastPtr->Next;
		}

		NewPtr = malloc (sizeof (struct ObjectType));
		setmem (NewPtr, (sizeof (struct ObjectType)), 0);
		NewPtr->Prev = LastPtr;
		NewPtr->Next = NULL;
		LastPtr->Next = NewPtr;
	}

	strcpy (NewPtr->FileName, FileNamePtr);
	NewPtr->ID = ObjectCount;
	NewPtr->Visible = GC_OBJECT_VISIBLE;
	NewPtr->DirtyFlag = false;
	ObjectCount++;
	NewPtr->Surface = gvCurrentSurface;
	NewPtr->Transform = gvCurrentTransform;
	NewPtr->PrimListPtr = NULL;
	gvCurrentObjectPtr = NewPtr;
	gvCurrentPrimPtr = NULL;
	gvCurrentVertexPtr = NULL;
	UnHighLight_Object ();

	return (NewPtr);
}

void Add_Object () {

	struct ObjectType *ObjPtr;
	FILE *NewFilePtr;
	char FileName[80];
	char InBuffer[80];
	char OutBuffer[80];

	sprintf (FileName, "obj%d.ed",ObjectCount);
   sprintf (OutBuffer, "New object filename [%s]: ", FileName);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		strcpy (FileName, InBuffer);
	}

	ObjPtr = Insert_Object (FileName);
	ObjPtr->DirtyFlag = true;
	Edit_Object (ObjPtr);
	Add_Primitive (ObjPtr);
	UnHighLight_Object ();
}

void Select_Object () {

	if (gvCurrentObjectPtr != NULL) {
		UnHighLight_Object ();
		gvCurrentObjectPtr = gvCurrentObjectPtr->Next;
	}

	else {
		gvCurrentObjectPtr = gvObjectListPtr;
	}

	if (gvCurrentObjectPtr != NULL) {
		HighLight_Object ();
	}

	gvCurrentPrimPtr = gvCurrentObjectPtr->PrimListPtr;
	gvCurrentVertexPtr = NULL;
}

void Deallocate_Object (struct ObjectType *ObjPtr) {
	struct PrimType *PrimPtr, *NextPrimPtr;

	if (ObjPtr != NULL) {
		PrimPtr = ObjPtr->PrimListPtr;

		while (PrimPtr != NULL) {
			NextPrimPtr = PrimPtr->Next;
			Deallocate_Primitive (PrimPtr);
			PrimPtr = NextPrimPtr;
		}

		free (ObjPtr);
	}
}

void Delete_Object (struct ObjectType *ObjPtr) {

	if (ObjPtr->Prev != NULL) {

		if (ObjPtr->Next != NULL) {
			(ObjPtr->Prev)->Next = (ObjPtr->Next);
			(ObjPtr->Next)->Prev = (ObjPtr->Prev);
		}

		else {
			(ObjPtr->Prev)->Next = NULL;
		}
	}

	else {

		if (ObjPtr->Next != NULL) {
			(ObjPtr->Next)->Prev = NULL;
			gvObjectListPtr = ObjPtr->Next;
		}

		else {
			gvObjectListPtr = NULL;
		}
	}

	Deallocate_Object (ObjPtr);
	ObjPtr = NULL;
}

void Scale_Object (int FunctionIndex) {

	if (FunctionIndex == FUN_SCALE_UP) {
		gvCurrentObjectPtr->Transform.Scale *= gvSystemParms.ScaleDelta;

	}

	else {
		gvCurrentObjectPtr ->Transform.Scale /= gvSystemParms.ScaleDelta;
	}
}

void Move_Object (int FunctionIndex) {

	switch (FunctionIndex) {

		case FUN_MOVE_UP:
			gvCurrentObjectPtr->Transform.DZ += gvSystemParms.LinDelta;
			break;

		case FUN_MOVE_DOWN:
			gvCurrentObjectPtr->Transform.DZ -= gvSystemParms.LinDelta;
			break;

		case FUN_MOVE_LEFT:
			gvCurrentObjectPtr->Transform.DY -= gvSystemParms.LinDelta;
			break;

		case FUN_MOVE_RIGHT:
			gvCurrentObjectPtr->Transform.DY += gvSystemParms.LinDelta;
			break;

		case FUN_MOVE_IN:
			gvCurrentObjectPtr->Transform.DX += gvSystemParms.LinDelta;
			break;

		case FUN_MOVE_OUT:
			gvCurrentObjectPtr->Transform.DX -= gvSystemParms.LinDelta;
			break;

		case FUN_MOVE_PITCHDOWN:
			gvCurrentObjectPtr->Transform.Pitch -= gvSystemParms.AngDelta;

			if (gvCurrentObjectPtr->Transform.Pitch < 0.0) {
				gvCurrentObjectPtr->Transform.Pitch += 360.0;
			}
			break;

		case FUN_MOVE_PITCHUP:
			gvCurrentObjectPtr->Transform.Pitch += gvSystemParms.AngDelta;

			if (gvCurrentObjectPtr->Transform.Pitch >= 360.0) {
				gvCurrentObjectPtr->Transform.Pitch -= 360.0;
			}
			break;

		  case FUN_MOVE_YAWLEFT:
			gvCurrentObjectPtr->Transform.Yaw -= gvSystemParms.AngDelta;

			if (gvCurrentObjectPtr->Transform.Yaw < 0.0) {
				gvCurrentObjectPtr->Transform.Yaw += 360.0;
			}
			break;

		case FUN_MOVE_YAWRIGHT:
			gvCurrentObjectPtr->Transform.Yaw += gvSystemParms.AngDelta;

			if (gvCurrentObjectPtr->Transform.Yaw >= 360.0) {
				gvCurrentObjectPtr->Transform.Yaw -= 360.0;
			}
			break;

		case FUN_MOVE_ROLLLEFT:
			gvCurrentObjectPtr->Transform.Roll -= gvSystemParms.AngDelta;

			if (gvCurrentObjectPtr->Transform.Roll < 0.0) {
				gvCurrentObjectPtr->Transform.Roll += 360.0;
			}
			break;

		case FUN_MOVE_ROLLRIGHT:
			gvCurrentObjectPtr->Transform.Roll += gvSystemParms.AngDelta;

			if (gvCurrentObjectPtr->Transform.Roll >= 360.0) {
				gvCurrentObjectPtr->Transform.Roll -= 360.0;
			}
			break;

	}
}

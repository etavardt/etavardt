/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ              Program  ED - A DATABASE MODELING TOOL                     บ
บ                                                                         บ
บ              EDPRIM.C   Functions used to manipulate graphics           บ
บ	                       primtives, including add/delete from            บ
บ                         an object description.                          บ
บ                                                                         บ
บ		         written by William A. Tolhurst                             บ
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
#include <stdlib.h>
#include <string.h>
#include <alloc.h>
#include <mem.h>
#include "edmain.h"
#include "edprotos.h"

extern struct ObjectType *gvCurrentObjectPtr;
extern struct PrimType *gvCurrentPrimPtr;
extern struct VertexType *gvCurrentVertexPtr;


struct PrimType * Insert_Primitive (struct ObjectType *ObjPtr) {

	struct PrimType *LastPrimPtr, *NewPrimPtr;

	LastPrimPtr = ObjPtr->PrimListPtr;

	if (LastPrimPtr != NULL) {

		while (LastPrimPtr->Next != NULL) {
			LastPrimPtr = LastPrimPtr->Next;
		}
	}

	if (ObjPtr->PrimListPtr == NULL) {
		ObjPtr->PrimListPtr = malloc (sizeof (struct PrimType));
		setmem (ObjPtr->PrimListPtr, (sizeof (struct PrimType)), 0);
		LastPrimPtr = ObjPtr->PrimListPtr;
		NewPrimPtr = LastPrimPtr;
		NewPrimPtr->Prev = NULL;
	}

	else {
		NewPrimPtr = malloc (sizeof (struct PrimType));
		setmem (NewPrimPtr, (sizeof (struct PrimType)), 0);
		NewPrimPtr->Prev = LastPrimPtr;
		LastPrimPtr->Next = NewPrimPtr;
	}

	NewPrimPtr->Next = NULL;
	gvCurrentPrimPtr = NewPrimPtr;
	UnHighLight_Primitive ();

	return (NewPrimPtr);
}

int Determine_Prim_Index (char *TextString) {

	if (strcmpi ("polygon", TextString) == 0) {
		return (GC_PRIM_POLYGON);
	}

	if (strcmpi ("cone", TextString) == 0) {
		return (GC_PRIM_CONE);
	}

	if (strcmpi ("sphere", TextString) == 0) {
		return (GC_PRIM_SPHERE);
	}

	if (strcmpi ("patch", TextString) == 0) {
		return (GC_PRIM_PATCH);
	}

	if (strcmpi ("ring", TextString) == 0) {
		return (GC_PRIM_RING);
	}

	return (0);
}

void Add_Primitive (struct ObjectType *ObjPtr) {

	char InBuffer[80];
	int PrimIndex = GC_PRIM_POLYGON;
	struct PrimType *NewPrimPtr;
	int Subi;
	int NumPolyVertices = 4;
	// 4 is the default number of vertices for polygons

	strcpy (InBuffer, Get_Dialog (5, 1, "Primitive Type To Add [polygon]: "));

	if (strcmpi (InBuffer, "") != 0) {
		PrimIndex = Determine_Prim_Index (InBuffer);
	}

	NewPrimPtr = Insert_Primitive (ObjPtr);

	if (NewPrimPtr) {
		NewPrimPtr->Type = PrimIndex;

		if (PrimIndex == GC_PRIM_POLYGON) {
			NewPrimPtr->Prim.Poly.VertexPtr = NULL;
			strcpy (InBuffer, Get_Dialog (5,1,
						"How Many Vertices for Polygon [4]: "));

			if (strcmpi (InBuffer, "") != 0) {
				NumPolyVertices = atoi (InBuffer);
			}

			NewPrimPtr->Prim.Poly.NumberVertices = NumPolyVertices;

			for (Subi = 0; Subi < NumPolyVertices; Subi++) {
				Insert_Vertex (NewPrimPtr);
			}
		}

		Edit_Primitive (NewPrimPtr);
	}
}

void Deallocate_Primitive (struct PrimType *PrimPtr) {
	struct VertexType *VertexPtr, *NextVertexPtr;

	if (PrimPtr != NULL) {

		if (PrimPtr->Type == GC_PRIM_POLYGON) {
			VertexPtr = PrimPtr->Prim.Poly.VertexPtr;

			while (VertexPtr != NULL) {
				NextVertexPtr = VertexPtr->Next;
				free (VertexPtr);
				VertexPtr = NextVertexPtr;
			}
		}

		free (PrimPtr);
	}
}

void Delete_Primitive () {

	int NoPrimsInObject = false;

	if (gvCurrentPrimPtr->Prev != NULL) {

		if (gvCurrentPrimPtr->Next != NULL) {
			(gvCurrentPrimPtr->Prev)->Next = (gvCurrentPrimPtr->Next);
			(gvCurrentPrimPtr->Next)->Prev = (gvCurrentPrimPtr->Prev);
		}

		else {
			(gvCurrentPrimPtr->Prev)->Next = NULL;
		}
	}

	else {

		gvCurrentObjectPtr->PrimListPtr = gvCurrentPrimPtr->Next;

		if (gvCurrentPrimPtr->Next != NULL) {
			(gvCurrentPrimPtr->Next)->Prev = NULL;
		}

		else {
			NoPrimsInObject = true;
		}
	}

	Deallocate_Primitive (gvCurrentPrimPtr);
	gvCurrentPrimPtr = NULL;

	if (NoPrimsInObject) {
		Delete_Object (gvCurrentObjectPtr);
	}
}

void Select_Primitive () {

	gvCurrentVertexPtr = NULL;

	if (gvCurrentPrimPtr != NULL) {
		UnHighLight_Primitive ();
		gvCurrentPrimPtr = gvCurrentPrimPtr->Next;
	}

	else {
		gvCurrentPrimPtr = gvCurrentObjectPtr->PrimListPtr;
	}

	if (gvCurrentPrimPtr != NULL) {
		HighLight_Primitive ();
	}
}

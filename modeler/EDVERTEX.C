/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ              Program  ED - A DATABASE MODELING TOOL                     บ
บ                                                                         บ
บ              EDVERTEX.C Functions that add, delete, select, and         บ
บ                         otherwise manipulate the vertex data            บ
บ                         structure are contained in this file.           บ
บ                                                                         บ
บ             written by William A. Tolhurst                              บ
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
#include <math.h>
#include "edmain.h"
#include "edprotos.h"

extern struct PrimType *gvCurrentPrimPtr;
extern struct VertexType *gvCurrentVertexPtr;

struct VertexType * Insert_Vertex (struct PrimType *PrimPtr) {

	struct VertexType *LastVertexPtr, *NewVertexPtr;

	LastVertexPtr = PrimPtr->Prim.Poly.VertexPtr;

	if (LastVertexPtr == NULL) {
		PrimPtr->Prim.Poly.VertexPtr = malloc (sizeof (struct VertexType));
		setmem (PrimPtr->Prim.Poly.VertexPtr, (sizeof (struct VertexType)), 0);
		LastVertexPtr = PrimPtr->Prim.Poly.VertexPtr;
		NewVertexPtr = LastVertexPtr;
		NewVertexPtr->Prev = NULL;
	}

	else {

		while (LastVertexPtr->Next != NULL) {
			LastVertexPtr = LastVertexPtr->Next;
		}

		NewVertexPtr = malloc (sizeof (struct VertexType));
		setmem (NewVertexPtr, (sizeof (struct VertexType)), 0);
		NewVertexPtr->Prev = LastVertexPtr;
		LastVertexPtr->Next = NewVertexPtr;
	}

	NewVertexPtr->Next = NULL;
	return (NewVertexPtr);
}

void Add_Vertex () {

	char InBuffer[80];
	struct VertexType *NewVertexPtr;

	if (gvCurrentPrimPtr != NULL) {
		NewVertexPtr = Insert_Vertex (gvCurrentPrimPtr);

		if (NewVertexPtr) {
			strcpy (InBuffer, Get_Dialog (5, 1, "New Vextex X: "));
			NewVertexPtr->Vertex[0] = atof (InBuffer);
			strcpy (InBuffer, Get_Dialog (5, 1, "New Vextex Y: "));
			NewVertexPtr->Vertex[1] = atof (InBuffer);
			strcpy (InBuffer, Get_Dialog (5, 1, "New Vextex Z: "));
			NewVertexPtr->Vertex[2] = atof (InBuffer);
		}
	}
}

void Select_Vertex () {

	char OutBuffer[80];

	if (gvCurrentVertexPtr != NULL) {
		gvCurrentVertexPtr = gvCurrentVertexPtr->Next;
	}

	else {
		gvCurrentVertexPtr = gvCurrentPrimPtr->Prim.Poly.VertexPtr;
	}

	if (gvCurrentVertexPtr != NULL) {
		sprintf(OutBuffer, "Vertex [%f %f %f] Selected",
			gvCurrentVertexPtr->Vertex[0],
			gvCurrentVertexPtr->Vertex[1],
			gvCurrentVertexPtr->Vertex[2]);
		Clear_Dialog ();
		Put_Dialog (5, 2, OutBuffer);
	}

	else {
		Clear_Dialog ();
		HighLight_Primitive ();
	}
}

void Delete_Vertex (struct PrimType *PrimPtr, struct VertexType *VertexPtr) {

	if (VertexPtr->Prev != NULL) {

		if (VertexPtr->Next != NULL) {
			(VertexPtr->Prev)->Next = (VertexPtr->Next);
			(VertexPtr->Next)->Prev = (VertexPtr->Prev);
		}

		else {
			(VertexPtr->Prev)->Next = NULL;
		}
	}

	else {

		PrimPtr->Prim.Poly.VertexPtr = VertexPtr->Next;

		if (VertexPtr->Next != NULL) {
			(VertexPtr->Next)->Prev = NULL;
		}

		else {
			// Should Never be here since a polygon
			// should always have at least 3 vertices
		}
	}

	free (VertexPtr);
	VertexPtr = NULL;
	Clear_Dialog ();
}


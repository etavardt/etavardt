/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ              Program  ED - A DATABASE MODELING TOOL                     บ
บ                                                                         บ
บ              EDFILE.C   Formats and transfers object and studio         บ
บ                         data to/from BOB compatible files.              บ
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
#include <dos.h>
#include "edmain.h"
#include "edprotos.h"

extern struct ObjectType *gvObjectListPtr;
extern struct StudioType gvStudio;
extern struct SurfaceType gvCurrentSurface;
extern struct TransformType gvCurrentTransform;
extern struct ObjectType *gvCurrentObjectPtr;
extern struct PrimType *gvCurrentPrimPtr;
extern struct VertexType *gvCurrentVertexPtr;

void Write_Vector (FILE *FilePtr, char *PreString, TDA Vector) {

	fprintf (FilePtr, "%s  %f %f %f\n", PreString,
				Vector[0], Vector[1], Vector[2]);
}

void Write_Color (FILE *FilePtr, char *PreString, struct ColorType Color) {

	fprintf (FilePtr, "%s  %f %f %f\n", PreString,
				Color.R, Color.G, Color.B);
}

void Write_Polygon (struct PrimType *PrimPtr, FILE *FilePtr) {

	int Subi;
	struct VertexType *VertexPtr;

	fprintf (FilePtr, "polygon {\n");
	fprintf (FilePtr, "   points %d\n", PrimPtr->Prim.Poly.NumberVertices);
	VertexPtr = PrimPtr->Prim.Poly.VertexPtr;

	for (Subi = 0; Subi < PrimPtr->Prim.Poly.NumberVertices; Subi++) {

		if (VertexPtr) {
			Write_Vector (FilePtr, "      vertex", VertexPtr->Vertex);
			VertexPtr = VertexPtr->Next;
		}
	}

	fprintf (FilePtr, "}\n\n");
}

void Write_Cone (struct PrimType *PrimPtr, FILE *FilePtr) {

	fprintf (FilePtr, "cone {\n");
	Write_Vector (FilePtr, "   base", PrimPtr->Prim.Cone.Center1);
	fprintf (FilePtr, "   base_radius %f\n", PrimPtr->Prim.Cone.Radius1);
	Write_Vector (FilePtr, "   apex", PrimPtr->Prim.Cone.Center2);
	fprintf (FilePtr, "   apex_radius %f\n", PrimPtr->Prim.Cone.Radius2);
	fprintf (FilePtr, "}\n\n");
}

void Write_Sphere (struct PrimType *PrimPtr, FILE *FilePtr) {

	fprintf (FilePtr, "sphere {\n");
	Write_Vector (FilePtr, "   center", PrimPtr->Prim.Sphere.Center);
	fprintf (FilePtr, "   radius %f\n", PrimPtr->Prim.Sphere.Radius);
	fprintf (FilePtr, "}\n\n");
}

void Write_Patch (struct PrimType *PrimPtr, FILE *FilePtr) {

	fprintf (FilePtr, "patch {\n");
	Write_Vector (FilePtr, "   vertex", PrimPtr->Prim.Patch.Vertex1);
	Write_Vector (FilePtr, "      normal", PrimPtr->Prim.Patch.Normal1);
	Write_Vector (FilePtr, "   vertex", PrimPtr->Prim.Patch.Vertex2);
	Write_Vector (FilePtr, "      normal", PrimPtr->Prim.Patch.Normal2);
	Write_Vector (FilePtr, "   vertex", PrimPtr->Prim.Patch.Vertex3);
	Write_Vector (FilePtr, "      normal", PrimPtr->Prim.Patch.Normal3);
	fprintf (FilePtr, "}\n\n");

}

void Write_Ring (struct PrimType *PrimPtr, FILE *FilePtr) {

	fprintf (FilePtr, "ring {\n");
	Write_Vector (FilePtr, "   center", PrimPtr->Prim.Ring.Center);
	Write_Vector (FilePtr, "   normal", PrimPtr->Prim.Ring.Normal);
	fprintf (FilePtr, "   min_radius %f\n", PrimPtr->Prim.Ring.Radius1);
	fprintf (FilePtr, "   max_radius %f\n", PrimPtr->Prim.Ring.Radius2);
	fprintf (FilePtr, "}\n\n");
}

void Write_Surface (struct ObjectType *ObjPtr, FILE *FilePtr) {

	fprintf (FilePtr, "surface {\n");
	Write_Color (FilePtr, "   diffuse", ObjPtr->Surface.Diffuse);
	Write_Color (FilePtr, "   ambient", ObjPtr->Surface.Ambient);
	Write_Color (FilePtr, "   specular", ObjPtr->Surface.Specular);
	fprintf (FilePtr, "   shine %d\n", ObjPtr->Surface.ShinePower);
	Write_Color (FilePtr, "   transparent", ObjPtr->Surface.Transparent);
	fprintf (FilePtr, "   ior %f\n", ObjPtr->Surface.IORNumber);
	fprintf (FilePtr, "   fuzz %f\n", ObjPtr->Surface.FuzzMag );

	if (!ObjPtr->Surface.AntiAlias) {
		fprintf (FilePtr, "   no_antialias\n");
	}

	fprintf (FilePtr, "}\n\n");
}

void Write_Transform (struct ObjectType *ObjPtr, FILE *FilePtr) {

	fprintf (FilePtr, "transform {\n");
	fprintf (FilePtr, "   scale %f\n", ObjPtr->Transform.Scale);
	fprintf (FilePtr, "   rotate %f %f %f\n", ObjPtr->Transform.Roll,
					ObjPtr->Transform.Pitch, ObjPtr->Transform.Yaw);
	fprintf (FilePtr, "   translate %f %f %f\n", ObjPtr->Transform.DX,
					ObjPtr->Transform.DY, ObjPtr->Transform.DZ);
	fprintf (FilePtr, "}\n\n");
}

void Write_Object (struct ObjectType *ObjPtr, FILE *StudioFilePtr) {

	struct PrimType *PrimPtr;
	FILE *ObjFilePtr;
	char InBuffer[80];
	char OutBuffer[80];

	PrimPtr = ObjPtr->PrimListPtr;

		// Any Objects without primitives are discarded.
	if (PrimPtr) {

		if (ObjPtr->DirtyFlag) {
			sprintf (OutBuffer, "Write Object Data to [%s]: ", ObjPtr->FileName);
			strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

			if (strcmpi (InBuffer, "") != 0) {
				strcpy (ObjPtr->FileName, InBuffer);
			}

			ObjFilePtr = fopen (ObjPtr->FileName, "w");
		}
		Write_Surface (ObjPtr, StudioFilePtr);
		Write_Transform (ObjPtr, StudioFilePtr);
		fprintf (StudioFilePtr, "#include %s\n", ObjPtr->FileName);
		fprintf (StudioFilePtr, "transform_pop\n\n");
	}

	while ((PrimPtr != NULL) && (ObjPtr->DirtyFlag)) {

		switch (PrimPtr->Type) {

			case GC_PRIM_POLYGON:
				Write_Polygon (PrimPtr, ObjFilePtr);
				break;

			case GC_PRIM_CONE:
				Write_Cone (PrimPtr, ObjFilePtr);
				break;

			case GC_PRIM_SPHERE:
				Write_Sphere (PrimPtr, ObjFilePtr);
				break;

			case GC_PRIM_PATCH:
				Write_Patch (PrimPtr, ObjFilePtr);
				break;

			case GC_PRIM_RING:
				Write_Ring (PrimPtr, ObjFilePtr);
				break;
		}

		PrimPtr = PrimPtr->Next;
	}

	if (ObjPtr->DirtyFlag) {
		fclose (ObjFilePtr);
	}
}

void Write_Studio (FILE *FilePtr) {

	fprintf (FilePtr, "studio {\n");
	Write_Vector (FilePtr, "   from", gvStudio.From);
	Write_Vector (FilePtr, "   at", gvStudio.At);
	Write_Vector (FilePtr, "   up", gvStudio.Up);
	fprintf (FilePtr, "   angle %f\n", gvStudio.Angle);
	fprintf (FilePtr, "   resolution %d %d\n", gvStudio.I, gvStudio.J);
	fprintf (FilePtr, "   start %d\n", gvStudio.Start);
	fprintf (FilePtr, "   stop %d\n", gvStudio.Stop);
	fprintf (FilePtr, "   aspect %f\n", gvStudio.Aspect);
	fprintf (FilePtr, "   projection %s\n", gvStudio.Projection);
	Write_Color (FilePtr, "   ambient", gvStudio.Ambient);
	Write_Color (FilePtr, "   background", gvStudio.Background);
	fprintf (FilePtr, "   haze %f\n", gvStudio.Haze);
	fprintf (FilePtr, "   antialias %s\n", gvStudio.AntiAlias);

	if (gvStudio.Jitter) {
		fprintf (FilePtr, "   jitter\n");
	}

	fprintf (FilePtr, "   aperture %f\n", gvStudio.Aperture);
	fprintf (FilePtr, "   focal_length %f\n", gvStudio.FocalLength);

	if (gvStudio.NoShadows) {
		fprintf (FilePtr, "   no_shadows\n");
	}

	fprintf (FilePtr, "   samples %d\n", gvStudio.Samples);

	if (gvStudio.NoExpTrans) {
		fprintf (FilePtr, "   no_exp_trans\n");
	}

	fprintf (FilePtr, "   threshold %d\n", gvStudio.Threshold);

	if (gvStudio.Caustics) {
		fprintf (FilePtr, "   caustics\n");
	}

	fprintf (FilePtr, "   depth %d\n", gvStudio.Depth);
	fprintf (FilePtr, "}\n\n");
}

void Write_Session () {

	struct ObjectType *ObjPtr;
	FILE *StudioFilePtr;
	char InBuffer[80];
	char OutBuffer[80];

	sprintf (OutBuffer, "Write Studio Data to [%s]: ", gvStudio.FileName);
	strcpy (InBuffer, Get_Dialog (5, 1, OutBuffer));

	if (strcmpi (InBuffer, "") != 0) {
		strcpy (gvStudio.FileName, InBuffer);
	}

	StudioFilePtr = fopen (gvStudio.FileName, "w");

	Write_Studio (StudioFilePtr);
	ObjPtr = gvObjectListPtr;

	if (ObjPtr != NULL) {

		while (ObjPtr != NULL) {
			Write_Object (ObjPtr, StudioFilePtr);
			ObjPtr = ObjPtr->Next;
		}
	}

	fclose (StudioFilePtr);
}

void Read_Vector (FILE *FilePtr, TDA *Vector) {

	char Tag[80];

	fscanf (FilePtr, "%s %f %f %f", Tag, ((float *) Vector),
				(((float *) Vector)+1), (((float *) Vector)+2));
}

void Read_Color (FILE *FilePtr, struct ColorType *Color) {

	char Tag[80];

	fscanf (FilePtr, "%s %f %f %f", Tag, ((float *) Color),
				(((float *) Color)+1), (((float *) Color)+2));
}

void Read_Polygon (struct PrimType *PrimPtr, FILE *FilePtr) {

	int NumberVertices, subi;
	struct VertexType *VertexPtr;
	char Tag[80];

	fscanf (FilePtr, " %s %d", Tag, &NumberVertices);
	PrimPtr->Prim.Poly.NumberVertices = NumberVertices;

	for (subi = 0; subi < NumberVertices; subi++) {
		VertexPtr = Insert_Vertex (PrimPtr);

		if (VertexPtr) {
      	Read_Vector (FilePtr, &(VertexPtr->Vertex));
		}
	}

	fscanf (FilePtr, "%s", Tag);	// Closing Right Brace
}

void Read_Cone (struct PrimType *PrimPtr, FILE *FilePtr) {

	char Tag[80];

	Read_Vector (FilePtr, &(PrimPtr->Prim.Cone.Center1));
	fscanf (FilePtr, "%s %f", Tag, &(PrimPtr->Prim.Cone.Radius1));
	Read_Vector (FilePtr, &(PrimPtr->Prim.Cone.Center2));
	fscanf (FilePtr, "%s %f", Tag, &(PrimPtr->Prim.Cone.Radius2));
	fscanf (FilePtr, "%s", Tag);	// Closing Right Brace
}

void Read_Sphere (struct PrimType *PrimPtr, FILE *FilePtr) {

	char Tag[80];

	Read_Vector (FilePtr, &(PrimPtr->Prim.Sphere.Center));
	fscanf (FilePtr, "%s %f", Tag, &(PrimPtr->Prim.Sphere.Radius));
	fscanf (FilePtr, "%s", Tag);	// Closing Right Brace
}

void Read_Patch (struct PrimType *PrimPtr, FILE *FilePtr) {

	char Tag[80];

	Read_Vector (FilePtr, &(PrimPtr->Prim.Patch.Vertex1));
	Read_Vector (FilePtr, &(PrimPtr->Prim.Patch.Normal1));
	Read_Vector (FilePtr, &(PrimPtr->Prim.Patch.Vertex2));
	Read_Vector (FilePtr, &(PrimPtr->Prim.Patch.Normal2));
	Read_Vector (FilePtr, &(PrimPtr->Prim.Patch.Vertex3));
	Read_Vector (FilePtr, &(PrimPtr->Prim.Patch.Normal3));
	fscanf (FilePtr, "%s", Tag);	// Closing Right Brace

}

void Read_Ring (struct PrimType *PrimPtr, FILE *FilePtr) {

	char Tag[80];

	Read_Vector (FilePtr, &(PrimPtr->Prim.Ring.Center));
	Read_Vector (FilePtr, &(PrimPtr->Prim.Ring.Normal));
	fscanf (FilePtr, "%s %f", Tag, &(PrimPtr->Prim.Ring.Radius1));
	fscanf (FilePtr, "%s %f", Tag, &(PrimPtr->Prim.Ring.Radius2));
	fscanf (FilePtr, "%s", Tag);	// Closing Right Brace
}

int Read_Primitives (struct ObjectType *ObjPtr, FILE *FilePtr) {

	struct PrimType *PrimPtr;
	int ScanStatus;
	char PrimName[80];
	int PrimIndex;
	char Tag[80];
	char OutBuffer [80];

	ScanStatus = fscanf (FilePtr, "%s %s", PrimName, Tag);

	if (strcmpi (Tag, "{") != 0) {
		Error_Beep ();
		Clear_Dialog ();
		sprintf (OutBuffer, "File %s is not in proper format for ED",
					PrimName);
		Put_Dialog (5, 1, OutBuffer);
		delay (5000);
		Clear_Dialog ();
		return (false);
	}

	else {

		while ((ScanStatus != 0) && (ScanStatus != EOF)) {

			PrimPtr = Insert_Primitive (ObjPtr);
			PrimIndex = Determine_Prim_Index (PrimName);
			PrimPtr->Type = PrimIndex;

			switch (PrimIndex) {

				case GC_PRIM_POLYGON:
					PrimPtr->Prim.Poly.VertexPtr = NULL;
					Read_Polygon (PrimPtr, FilePtr);
					break;

				case GC_PRIM_CONE:
					Read_Cone (PrimPtr, FilePtr);
					break;

				case GC_PRIM_SPHERE:
					Read_Sphere (PrimPtr, FilePtr);
					break;

				case GC_PRIM_PATCH:
					Read_Patch (PrimPtr, FilePtr);
					break;

				case GC_PRIM_RING:
					Read_Ring (PrimPtr, FilePtr);
					break;
			}

			ScanStatus = fscanf (FilePtr, "%s %s", PrimName, Tag);
		}
	}

	return (true);
}

void Read_Object () {

	FILE *FilePtr;
	char FileName[80];
	char OutBuffer[80];
	struct ObjectType *ObjPtr;
	int ValidFileName = false;
	int ForgetIt = false;

	while (!ValidFileName && !ForgetIt) {
		strcpy (FileName, Get_Dialog (5, 1, "Object filename: "));

		if (strcmpi (FileName, "") != 0) {
			FilePtr = fopen (FileName, "r");

			if (FilePtr) {
				ValidFileName = true;
			}
		}

		else {
			ForgetIt = true;
		}

		if (!ValidFileName && !ForgetIt) {
			Error_Beep ();
			sprintf (OutBuffer, "Couldn't find file: %s - Try Again",
						FileName);
			Put_Dialog (5, 1, OutBuffer);
			delay (5000);
			Clear_Dialog ();
		}
	}

	if (!ForgetIt) {
		ObjPtr = Insert_Object (FileName);

		if (!(Read_Primitives (ObjPtr, FilePtr))) {
			Delete_Object (ObjPtr);
		}

		else {
			HighLight_Object ();
			gvCurrentObjectPtr = ObjPtr;
			gvCurrentPrimPtr = ObjPtr->PrimListPtr;
			gvCurrentVertexPtr = NULL;
		}

		fclose (FilePtr);
	}
}

void Read_Surface (FILE *FilePtr) {

	char Tag[80];

	Read_Color (FilePtr, &(gvCurrentSurface.Diffuse));
	Read_Color (FilePtr, &(gvCurrentSurface.Ambient));
	Read_Color (FilePtr, &(gvCurrentSurface.Specular));
	fscanf (FilePtr, "%s %d", Tag, &(gvCurrentSurface.ShinePower));
	Read_Color (FilePtr, &(gvCurrentSurface.Transparent));
	fscanf (FilePtr, "%s %f", Tag, &(gvCurrentSurface.IORNumber));
	fscanf (FilePtr, "%s %f", Tag, &(gvCurrentSurface.FuzzMag));
	fscanf (FilePtr, "%s", Tag);

	if (strcmpi (Tag, "no_antialias") == 0) {
		gvCurrentSurface.AntiAlias = false;
		fscanf (FilePtr, "%s", Tag); //Discard Left Brace
	}

	else {
		gvCurrentSurface.AntiAlias = true;
	}
}

void Read_Transform (FILE *FilePtr) {

	char Tag[80];

	fscanf (FilePtr, "%s %f",Tag, &(gvCurrentTransform.Scale));
	fscanf (FilePtr, "%s %f %f %f", Tag, &(gvCurrentTransform.Roll),
				&(gvCurrentTransform.Pitch), &(gvCurrentTransform.Yaw));
	fscanf (FilePtr, "%s %f %f %f", Tag, &(gvCurrentTransform.DX),
				&(gvCurrentTransform.DY), &(gvCurrentTransform.DZ));
	fscanf (FilePtr, "%s", Tag);	//Discard Right Brace
}

void Read_Studio (FILE *FilePtr) {

	char Tag[80];
	char Temp[80];

	Read_Vector (FilePtr, &(gvStudio.From));
	Read_Vector (FilePtr, &(gvStudio.At));
	Read_Vector (FilePtr, &(gvStudio.Up));
	fscanf (FilePtr, "%s %f", Tag, &(gvStudio.Angle));
	fscanf (FilePtr, "%s %d %d", Tag, &(gvStudio.I), &(gvStudio.J));
	fscanf (FilePtr, "%s %d", Tag, &(gvStudio.Start));
	fscanf (FilePtr, "%s %d", Tag, &(gvStudio.Stop));
	fscanf (FilePtr, "%s %f", Tag, &(gvStudio.Aspect));
	fscanf (FilePtr, "%s %s", Tag, &(gvStudio.Projection));
	Read_Color (FilePtr, &(gvStudio.Ambient));
	Read_Color (FilePtr, &(gvStudio.Background));
	fscanf (FilePtr, "%s %f", Tag, &(gvStudio.Haze));
	fscanf (FilePtr, "%s %s", Tag, &(gvStudio.AntiAlias));
	fscanf (FilePtr, "%s", Tag);

	if (strcmpi (Tag, "jitter") == 0) {
		gvStudio.Jitter = true;
		fscanf (FilePtr, "%s %f", Tag, &(gvStudio.Aperture));
	}

	else {
		gvStudio.Jitter = false;
		fscanf (FilePtr, "%f", &(gvStudio.Aperture));
	}

	fscanf (FilePtr, "%s %f", Tag, &(gvStudio.FocalLength));
	fscanf (FilePtr, "%s", Tag);

	if (strcmpi (Tag, "no_shadows") == 0) {
		gvStudio.NoShadows = true;
		fscanf (FilePtr, "%s %d", Tag, &(gvStudio.Samples));
	}

	else {
		gvStudio.NoShadows = false;
		fscanf (FilePtr, "%d", &(gvStudio.Samples));
	}

	fscanf (FilePtr, "%s", Tag);

	if (strcmpi (Tag, "no_exp_trans") == 0) {
		gvStudio.NoExpTrans = true;
		fscanf (FilePtr, "%s %d", Tag, &(gvStudio.Threshold));
	}

	else {
		gvStudio.NoExpTrans = false;
		fscanf (FilePtr, "%d", &(gvStudio.Threshold));
	}

	fscanf (FilePtr, "%s", Tag);

	if (strcmpi (Tag, "caustics") == 0) {
		gvStudio.Caustics = true;
		fscanf (FilePtr, "%s %d", Tag, &(gvStudio.Depth));
	}

	else {
		gvStudio.Caustics = false;
		fscanf (FilePtr, "%d", &(gvStudio.Depth));
	}

	fscanf (FilePtr, "%s", Tag);
}

void Read_Session () {

	FILE *FilePtr, *ObjFilePtr;
	char FileName[80], ObjFileName[80];
	struct ObjectType *ObjPtr;
	int ValidFileName = false;
	int ForgetIt = false;
	int ScanStatus;
	char Tag[80], OutBuffer[80];

	while (!ValidFileName && !ForgetIt) {
		strcpy (FileName, Get_Dialog (5, 1, "Studio filename: "));

		if (strcmpi (FileName, "") != 0) {
			FilePtr = fopen (FileName, "r");

			if (FilePtr) {
				ValidFileName = true;
				strcpy (gvStudio.FileName, FileName);
			}
		}

		else {
			ForgetIt = true;
		}

		if (!ValidFileName && !ForgetIt) {
			Error_Beep ();
			sprintf (OutBuffer, "Couldn't find file: %s - Try Again",
						FileName);
			Put_Dialog (5, 1, OutBuffer);
			delay (5000);
			Clear_Dialog ();
		}
	}

	if (!ForgetIt) {

		fscanf (FilePtr, "%s", Tag);

		// A quick sanity check to see if there is even the vagest
		// possibility that we're actually reading a .ED file
		if (strcmpi (Tag, "studio") == 0) {
			fscanf (FilePtr, "%s", Tag); // Discard Left Brace
			Read_Studio (FilePtr);
		}

		else {
			Error_Beep ();
			sprintf (OutBuffer, "File %s is not in .ED format - Try Again",
						FileName);
			Put_Dialog (5, 1, OutBuffer);
			delay (5000);
			Clear_Dialog ();
			return;
		}

		ScanStatus = fscanf (FilePtr, "%s", Tag);

		while ((ScanStatus != 0) && (ScanStatus != EOF)) {

			if (strcmpi (Tag, "surface") == 0) {
				fscanf (FilePtr, "%s", Tag); // Discard Left Brace
				Read_Surface (FilePtr);
			}

			if (strcmpi (Tag, "transform") == 0) {
				fscanf (FilePtr, "%s", Tag); // Discard Left Brace
				Read_Transform (FilePtr);
			}

			if (strcmpi (Tag, "#include") == 0) {
				fscanf (FilePtr, "%s", ObjFileName);
				ObjFilePtr = fopen (ObjFileName, "r");

				if (ObjFilePtr) {
					ObjPtr = Insert_Object (ObjFileName);

					if (!(Read_Primitives (ObjPtr, ObjFilePtr))) {
						Delete_Object (ObjPtr);
					}

					else {
						gvCurrentObjectPtr = ObjPtr;
						UnHighLight_Object ();
					}

					fclose (ObjFilePtr);
				}
			}

			ScanStatus = fscanf (FilePtr, "%s", Tag);
		}

		fclose (FilePtr);
	}
}
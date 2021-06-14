/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ              Program    ED - A DATABASE MODELING TOOL                   บ
บ                                                                         บ
บ              EDOEXEC.C  Command Executive which controls operation      บ
บ                         when in Object Mode.                            บ
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
#include "edmain.h"
#include "edprotos.h"
#include "edomode.h"

extern struct SystemParmType gvSystemParms;
extern struct ObjectType *gvCurrentObjectPtr;
extern struct PrimType *gvCurrentPrimPtr;

void Object_Exec () {
	int CmdIndex, FunctionIndex;
	struct PrimType *TempPtr;

	CmdIndex = CMD_INIT;

	while (CmdIndex != CMD_EXIT) {

		switch (CmdIndex) {

			case CMD_INIT:
				gvSystemParms.SystemMode = GC_SYSMODE_OBJECT;
				gvCurrentPrimPtr = gvCurrentObjectPtr->PrimListPtr;
				TempPtr = gvCurrentPrimPtr;

				while (gvCurrentPrimPtr != NULL) {
					UnHighLight_Primitive ();
					gvCurrentPrimPtr = gvCurrentPrimPtr->Next;
				}

				gvCurrentPrimPtr = TempPtr;
				Draw_View ();
				Display_Status ();
				break;

			case CMD_MOVE_OBJECT:
				Move_Object (FunctionIndex);
				Draw_View ();
				Display_Status ();
				break;

			case CMD_SCALE_OBJECT:
				Scale_Object (FunctionIndex);
				Draw_View ();
				break;

			case CMD_ADD_PRIMITIVE:

				if (gvCurrentPrimPtr == NULL) {
					Add_Primitive (gvCurrentObjectPtr);
               Draw_View ();
					gvCurrentObjectPtr->DirtyFlag = true;
				}

				else {
					Error_Beep ();
				}
				break;

			case CMD_DELETE_PRIMITIVE:

				if (gvCurrentPrimPtr != NULL) {
					Delete_Primitive ();

					// If last primitive is deleted, the object is
					// also deleted, so...
					if (gvCurrentObjectPtr != NULL) {
						gvCurrentObjectPtr->DirtyFlag = true;
					}
				}

				else {
					Error_Beep ();
				}

				Draw_View ();
				break;

			case CMD_SELECT_PRIMITIVE:
				Select_Primitive ();
				Draw_View ();
				break;

			case CMD_EDIT_PRIMITIVE:

				if (gvCurrentPrimPtr != NULL) {
					Edit_Primitive (gvCurrentPrimPtr);
					gvCurrentObjectPtr->DirtyFlag = true;
				}

				else {
					Error_Beep ();
				}

				Draw_View ();
				break;

			case CMD_VERTEX_MODE:

				if ((gvCurrentPrimPtr != NULL) && (gvCurrentPrimPtr->Type == GC_PRIM_POLYGON)) {
					Vertex_Exec ();
					gvSystemParms.SystemMode = GC_SYSMODE_OBJECT;
					Draw_View ();
					Display_Status ();
				}

				else {
					Error_Beep ();
				}

				break;

			case CMD_IDLE:
				/* Do Nothing */
				break;

		}

		CmdIndex = Get_Object_Cmd(&FunctionIndex);

	}

	if (gvCurrentObjectPtr != NULL) {
		HighLight_Object ();
	}
}
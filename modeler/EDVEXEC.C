/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ              Program  ED - A DATABASE MODELING TOOL                     บ
บ                                                                         บ
บ              EDVEXEC.C  Yet another execution controller, this time     บ
บ                         for vertex mode.                                บ
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
#include "edvmode.h"

extern struct SystemParmType gvSystemParms;
extern struct ObjectType *gvCurrentObjectPtr;
extern struct PrimType *gvCurrentPrimPtr;
extern struct VertexType *gvCurrentVertexPtr;

void Vertex_Exec () {
	int CmdIndex, FunctionIndex;

	CmdIndex = CMD_INIT;

	while (CmdIndex != CMD_EXIT) {

		switch (CmdIndex) {

			case CMD_INIT:
				gvSystemParms.SystemMode = GC_SYSMODE_VERTEX;
				Display_Status ();
				break;


			case CMD_ADD_VERTEX:
				Add_Vertex ();
				gvCurrentObjectPtr->DirtyFlag = true;
				Draw_View ();
				break;

			case CMD_DELETE_VERTEX:

				if ((gvCurrentVertexPtr != NULL) &&
						(gvCurrentPrimPtr->Prim.Poly.NumberVertices > 3)) {
					Delete_Vertex (gvCurrentPrimPtr, gvCurrentVertexPtr);
					gvCurrentPrimPtr->Prim.Poly.NumberVertices--;
					gvCurrentObjectPtr->DirtyFlag = true;
					Draw_View ();
				}

				else {
					Error_Beep ();
				}
				break;

			case CMD_SELECT_VERTEX:
				Select_Vertex ();
				break;

			case CMD_IDLE:
				/* Do Nothing */
				break;

		}

		CmdIndex = Get_Vertex_Cmd ();

	}

	HighLight_Primitive ();
}
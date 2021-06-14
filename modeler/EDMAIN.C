/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ              Program  ED - A DATABASE MODELING TOOL                     บ
บ                                                                         บ
บ              EDMAIN.C   The main function fo ED.  All system globals    บ
บ                         are declared at the start of this file.         บ
บ                         The main() function is a just a command         บ
บ                         interpreter, directing the rest of the          บ
บ                         program while in Scene mode.                    บ
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
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <dos.h>
#include "edmain.h"
#include "bkglobs.h"
#include "edscmode.h"
#include "edprotos.h"

/*
int				XRes, YRes;
unsigned int   MaxXRes, MaxYRes;
unsigned int   MaxX, MaxY;
float   			Asp;
Boolean 			PerspectivePlot;
float   			Mx, My, Mz, ds;
Boolean 			Draw_Axis_And_Palette;
int     			Angl, Tilt;
*/

// Ed's System Global Variables

struct 	SystemParmType		gvSystemParms;
struct 	ViewType	 			gvViewer;
struct 	StudioType			gvStudio;
struct 	ObjectType	 		*gvObjectListPtr;
struct 	ObjectType			*gvCurrentObjectPtr;
struct 	PrimType				*gvCurrentPrimPtr;
struct	VertexType			*gvCurrentVertexPtr;
struct 	SurfaceType 		gvCurrentSurface;
struct 	TransformType		gvCurrentTransform;


int main () {
	int CmdIndex, FunctionIndex;
	int ExitFlag = false;
	char KeyChar;
	int SessionDirty = false;
	int SessionLoaded = false;

	CmdIndex = CMD_INITSYS;
	gvSystemParms.SystemMode = GC_SYSMODE_SCENE;

	while (!ExitFlag) {

		switch (CmdIndex) {

			case CMD_INITSYS:
				Initialize_System ();
				Draw_View ();
				Display_Status ();
				break;

			case CMD_MOVE_VIEW:
				Move_Viewer (FunctionIndex);
				Draw_View ();
				Display_Status ();
				break;

			case CMD_LOAD_OBJ_FILE:
				Read_Object ();
				Draw_View ();
				SessionDirty = true;
				break;

			case CMD_SELECT_OBJECT:
				Select_Object ();
				Draw_View ();
				break;

			case CMD_ADD_OBJECT:

				if (gvCurrentObjectPtr == NULL) {
					Add_Object ();         // <-\
					Draw_View ();          // <- >--- Pure C Poetry!
					SessionDirty = true;   // <-/
				}

				else {
					Error_Beep ();
				}
				break;

			case CMD_DELETE_OBJECT:

				if (gvCurrentObjectPtr != NULL) {
					Delete_Object (gvCurrentObjectPtr);
					SessionDirty = true;
					Draw_View ();
				}

				else {
					Error_Beep ();
				}

				break;

			case CMD_EDIT_OBJECT:
				if (gvCurrentObjectPtr != NULL) {
					Edit_Object (gvCurrentObjectPtr);
					SessionDirty = true;
					Draw_View ();
				}

				else {
					Error_Beep ();
				}

				break;

			case CMD_OBJECT_MODE:

				if (gvCurrentObjectPtr != NULL) {
					Object_Exec ();
					SessionDirty = true;
					gvSystemParms.SystemMode = GC_SYSMODE_SCENE;
					Draw_View ();
					Display_Status ();
				}

				else {
					Error_Beep ();
					/* Error, No object selected*/
				}

				break;

			case CMD_STUDIO_MODE:
				Studio_Exec ();
				SessionDirty = true;
				gvSystemParms.SystemMode = GC_SYSMODE_SCENE;
				Display_Status ();
				break;

			case CMD_EDIT_SYSPARMS:
				Edit_SysParms ();
				Draw_View ();
				Display_Status ();
				break;

			case CMD_READ_SESSION:
				Read_Session ();
				Initialize_Viewer ();
				Draw_View ();
				Display_Status ();

				if (SessionLoaded == true) {
					SessionDirty = true;
				}

				SessionLoaded = true;
				break;

			case CMD_WRITE_SESSION:
				Write_Session ();
				SessionDirty = false;
				break;

			case CMD_EXIT:

				if (SessionDirty) {
					Error_Beep ();
					Clear_Dialog ();
					Put_Dialog (5, 1, "(Potential) Unsaved Changes in");
					Put_Dialog (5, 2, "studio/object files");
					Put_Dialog (5, 3, "Press Escape Again to leave Anyway,");
					Put_Dialog (5, 4, "Any Other Key to Resume");

					while (!kbhit ()); //Wait for a key
					KeyChar = getch ();

					if (KeyChar == KEY_ESC) {
						ExitFlag = true;
					}

					if (KeyChar == 0) {
						getch ();			// Discard 2nd byte of extended key
					}

					Clear_Dialog ();
				}

				else {
					ExitFlag = true;
				}
				break;

			case CMD_IDLE:
				/* Do Nothing */
				break;

		}

		if (!ExitFlag) {
			CmdIndex = Get_Cmd (&FunctionIndex);
		}
	}

	Shutdown_System ();
	return (0);
}
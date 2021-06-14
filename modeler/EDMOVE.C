/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ              Program  ED - A DATABASE MODELING TOOL                     บ
บ                                                                         บ
บ              EDMOVE.C   Contains a function that adjusts the            บ
บ                         view (eye) point parameters based on            บ
บ                         user input.                                     บ
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
#include <conio.h>
#include "edmain.h"
#include "bkmath.h"
#include "edprotos.h"
#include "edscmode.h"

extern struct ViewType gvViewer;
extern struct SystemParmType gvSystemParms;
extern struct StudioType gvStudio;

void Move_Viewer (int FunctionIndex) {

	switch (FunctionIndex) {

		case FUN_UP:
			gvViewer.Position[2] += gvSystemParms.LinDelta;
			break;

		case FUN_DOWN:
			gvViewer.Position[2] -= gvSystemParms.LinDelta;
			break;

		case FUN_LEFT:
			gvViewer.Position[1] -= gvSystemParms.LinDelta;
			break;

		case FUN_RIGHT:
			gvViewer.Position[1] += gvSystemParms.LinDelta;
			break;

		case FUN_IN:
			gvViewer.Position[0] -= gvSystemParms.LinDelta;
			break;

		case FUN_OUT:
			gvViewer.Position[0] += gvSystemParms.LinDelta;
			break;

		case FUN_PITCHDOWN:
			gvViewer.Pitch -= gvSystemParms.AngDelta;

			if (gvViewer.Pitch < 0.0) {
				gvViewer.Pitch += 360.0;
			}
			break;

		case FUN_PITCHUP:
			gvViewer.Pitch += gvSystemParms.AngDelta;

			if (gvViewer.Pitch >= 360.0) {
				gvViewer.Pitch -= 360.0;
			}
			break;

		  case FUN_YAWLEFT:
			gvViewer.Yaw -= gvSystemParms.AngDelta;

			if (gvViewer.Yaw < 0.0) {
				gvViewer.Yaw += 360.0;
			}
			break;

		case FUN_YAWRIGHT:
			gvViewer.Yaw += gvSystemParms.AngDelta;

			if (gvViewer.Yaw >= 360.0) {
				gvViewer.Yaw -= 360.0;
			}
			break;

		case FUN_ROLLLEFT:
			gvViewer.Roll -= gvSystemParms.AngDelta;

			if (gvViewer.Roll < 0.0) {
				gvViewer.Roll += 360.0;
			}
			break;

		case FUN_ROLLRIGHT:
			gvViewer.Roll += gvSystemParms.AngDelta;

			if (gvViewer.Roll >= 360.0) {
				gvViewer.Roll -= 360.0;
			}
			break;

	}
}
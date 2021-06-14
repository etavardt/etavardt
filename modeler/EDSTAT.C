/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ              Program  ED - A DATABASE MODELING TOOL                     บ
บ                                                                         บ
บ              EDSTAT.C   The functions in this module handle             บ
บ                         writing system status info to the               บ
บ                         Status area on the right of the screen.         บ
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
#include "edmain.h"
#include "edprotos.h"

extern struct ViewType gvViewer;
extern struct SystemParmType gvSystemParms;
extern struct StudioType gvStudio;

void Put_FStatus (int X, int Y, char *StringPtr, float value) {

	char CharBuffer[64];

	sprintf (CharBuffer, "%s %07.3f", StringPtr, value);
	outtextxy ((X * textwidth ("X")), (Y * textheight (CharBuffer)), CharBuffer);

}

void Put_IStatus (int X, int Y, char *StringPtr, int value) {

	char CharBuffer[64];

	sprintf (CharBuffer, "%s %d", StringPtr, value);
	outtextxy ((X * textwidth ("X")), (Y * textheight (CharBuffer)), CharBuffer);

}

void Put_SStatus (int X, int Y, char *StringPtr) {
	outtextxy ((X * textwidth ("X")), (Y * textheight (StringPtr)), StringPtr);
}

void Display_Studio_Parms () {

	Put_FStatus (2, 15, "From X: ", gvStudio.From[0]);
	Put_FStatus (2, 16, "     Y: ", gvStudio.From[1]);
	Put_FStatus (2, 17, "     Z: ", gvStudio.From[2]);

	Put_FStatus (2, 18, "At X:   ", gvStudio.At[0]);
	Put_FStatus (2, 19, "   Y:   ", gvStudio.At[1]);
	Put_FStatus (2, 20, "   Z:   ", gvStudio.At[2]);

	Put_FStatus (2, 21, "Up X:   ", gvStudio.Up[0]);
	Put_FStatus (2, 22, "   Y:   ", gvStudio.Up[1]);
	Put_FStatus (2, 23, "   Z:   ", gvStudio.Up[2]);

	Put_FStatus (2, 24, "FOV:    ", gvStudio.Angle);
	Put_IStatus (2, 25, "Res X:  ", gvStudio.I);
	Put_IStatus (2, 26, "    Y:  ", gvStudio.J);
	Put_IStatus (2, 27, "Start:  ", gvStudio.Start);
	Put_IStatus (2, 28, "End:    ", gvStudio.Stop);
	Put_FStatus (2, 29, "Aspect: ", gvStudio.Aspect);
	Put_SStatus (2, 30, "Proj:   ");
	Put_SStatus (10, 30, gvStudio.Projection);

	Put_FStatus (2, 31, "Amb R:  ", gvStudio.Ambient.R);
	Put_FStatus (2, 32, "    G:  ", gvStudio.Ambient.G);
	Put_FStatus (2, 33, "    B:  ", gvStudio.Ambient.B);

	Put_FStatus (2, 34, "Back R: ", gvStudio.Background.R);
	Put_FStatus (2, 35, "    G:  ", gvStudio.Background.G);
	Put_FStatus (2, 36, "    B:  ", gvStudio.Background.B);

	Put_FStatus (2, 37, "Haze:   ", gvStudio.Haze);
	Put_SStatus (2, 38, "AAlias: ");
	Put_SStatus (10, 38, gvStudio.AntiAlias);
	Put_IStatus (2, 39, "Thresh: ", gvStudio.Threshold);
	Put_IStatus (2, 40, "Jitter: ", gvStudio.Jitter);
	Put_FStatus (2, 41, "Aper:   ", gvStudio.Aperture);
	Put_FStatus (2, 42, "Focal:  ", gvStudio.FocalLength);
	Put_IStatus (2, 43, "Sample: ", gvStudio.Samples);
	Put_IStatus (2, 44, "No Sdw: ", gvStudio.NoShadows);
	Put_IStatus (2, 45, "NoExp:  ", gvStudio.NoExpTrans);
	Put_IStatus (2, 46, "Caust:  ", gvStudio.Caustics);
	Put_IStatus (2, 47, "Depth:  ", gvStudio.Depth);
}

void Display_Status () {

	setviewport (GC_STAT_LEFT, GC_STAT_TOP, GC_STAT_RIGHT, GC_STAT_BOTTOM, 1);
	clearviewport ();
	setcolor (GC_TEXT_COLOR_STAT);
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);

	Put_FStatus (2, 2, "X: ", gvViewer.Position[0]);
	Put_FStatus (2, 3, "Y: ", gvViewer.Position[1]);
	Put_FStatus (2, 4, "Z: ", gvViewer.Position[2]);
	Put_FStatus (2, 6, "ROLL : ", gvViewer.Roll);
	Put_FStatus (2, 7, "PITCH: ", gvViewer.Pitch);
	Put_FStatus (2, 8, "YAW  : ", gvViewer.Yaw);

	switch (gvSystemParms.SystemMode) {

		case GC_SYSMODE_SCENE:
			Put_SStatus (2, 13, "MODE: SCENE  ");
			break;

		case GC_SYSMODE_OBJECT:
			Put_SStatus (2, 13, "MODE: OBJECT ");
			break;

		case GC_SYSMODE_STUDIO:
			Put_SStatus (2, 13, "MODE: STUDIO ");
			Display_Studio_Parms ();
			break;

		case GC_SYSMODE_VERTEX:
			Put_SStatus (2, 13, "MODE: VERTEX ");
         break;
	}

	Put_FStatus (2, 10, "Lin  : ", gvSystemParms.LinDelta);
	Put_FStatus (2, 11, "Ang  : ", gvSystemParms.AngDelta);
	Put_FStatus (2, 12, "Scale: ", gvSystemParms.ScaleDelta);

}
/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ              Program  ED - A DATABASE MODELING TOOL                     บ
บ                                                                         บ
บ              EDDIA.C    Provides functions to interact with the         บ
บ                         Dialog area of the screen.  If the system       บ
บ                         requires more information or parameters         บ
บ                         as the result of an action, it uses the         บ
บ                         Dialog Area                                     บ
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
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/
//eddia.c - Dialog Handling functions

#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include "edmain.h"
#include "edprotos.h"
#include "edkey.h"


#define BUFFER_LENGTH 81


static char InputBuffer[BUFFER_LENGTH];

void Clear_Dialog () {
	setviewport (GC_MSG_LEFT, GC_MSG_TOP, GC_MSG_RIGHT, GC_MSG_BOTTOM, 1);
	clearviewport();
}

void Put_Dialog (int X, int Y, char *OutStringPtr) {
	setviewport (GC_MSG_LEFT, GC_MSG_TOP, GC_MSG_RIGHT, GC_MSG_BOTTOM, 1);
	setcolor(GC_TEXT_COLOR_MSG);
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
	outtextxy((X * textwidth ("X")), (Y * textheight (OutStringPtr)),
		OutStringPtr);

}

char * Get_Dialog (int X, int Y, char *PromptPtr) {

	int BufferIndex;
	char InputChar;
	int ExitFlag = false;
	int subi;

	BufferIndex = 0;

	for (subi = 0; subi < BUFFER_LENGTH; subi++) {
		InputBuffer[subi] = ' ';
	}

	InputBuffer[(BUFFER_LENGTH - 1)] = 0;
	setviewport (GC_MSG_LEFT, GC_MSG_TOP, GC_MSG_RIGHT, GC_MSG_BOTTOM, 1);
	clearviewport();
	setcolor(GC_TEXT_COLOR_MSG);
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
	outtextxy((X * textwidth ("X")), (Y * textheight (PromptPtr)), PromptPtr);

	while (!ExitFlag) {
		InputChar = getch();

		setcolor(0);
		outtextxy((X * textwidth ("X")), (Y * textheight (PromptPtr)), PromptPtr);
		outtextxy(((X * textwidth ("X")) + textwidth (PromptPtr)), (Y * textheight (PromptPtr)), InputBuffer);

		switch (InputChar) {

			case ENHANCED_KEY:
				getch();
				break;

			case KEY_ENTER:
				ExitFlag = true;
				InputBuffer[BufferIndex] = 0;
				break;

			case KEY_BACKSPACE:
				BufferIndex--;

				if (BufferIndex <= 0)	{
					BufferIndex = 0;
					InputBuffer[BufferIndex] = 0;
				}

				else {
					InputBuffer[BufferIndex] = ' ';
				}

				break;

			default:
				InputBuffer[BufferIndex] = InputChar;
				BufferIndex++;
				if (BufferIndex >= BUFFER_LENGTH) {
					BufferIndex = (BUFFER_LENGTH - 1);
					InputBuffer[BufferIndex] = 0;
				}
		}

		setcolor(GC_TEXT_COLOR_MSG);
		outtextxy((X * textwidth ("X")), (Y * textheight (PromptPtr)), PromptPtr);
		outtextxy(((X * textwidth ("X")) + textwidth (PromptPtr)), (Y * textheight (PromptPtr)), InputBuffer);

	}
	clearviewport ();
	return (InputBuffer);
}


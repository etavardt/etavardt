/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ              Program  ED - A DATABASE MODELING TOOL                     บ
บ                                                                         บ
บ              EDVCMD.C   This module provides the keyboard               บ
บ                         interpretation when ED is in Vertex Mode.       บ
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
#include "edprotos.h"
#include "edkey.h"
#include "edvmode.h"

int Get_Vertex_Cmd () {

	int CmdIndex = CMD_IDLE;

	if (kbhit ()) {
		switch (getch ()) {

			case ENHANCED_KEY:
				Error_Beep ();
				getch ();
				break;

			case 'a':
			case 'A':
				CmdIndex = CMD_ADD_VERTEX;
				break;

			case 'd':
			case 'D':
				CmdIndex = CMD_DELETE_VERTEX;
				break;

			case KEY_TAB:
				CmdIndex = CMD_SELECT_VERTEX;
            break;

			case KEY_ENTER:
				CmdIndex = CMD_EXIT;
				break;

			default:
				Error_Beep ();
		}
	}

	else {
		CmdIndex = CMD_IDLE;
	}

	return (CmdIndex);
}

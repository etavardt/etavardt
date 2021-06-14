/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ              Program  ED - A DATABASE MODELING TOOL                     บ
บ                                                                         บ
บ              EDSTCMD.C  The command interpreter used when the           บ
บ                         system is in Studio Mode.                       บ
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
#include "edstmode.h"

int Get_Studio_Cmd () {

	int CmdIndex = CMD_IDLE;

	if (kbhit ()) {
		switch (getch ()) {

			case ENHANCED_KEY:
				Error_Beep ();
				getch ();
				break;

			case 's':
			case 'S':
				CmdIndex = CMD_EDIT_STUDIO;
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

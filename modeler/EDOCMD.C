/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ              Program  ED - A DATABASE MODELING TOOL                     บ
บ                                                                         บ
บ              EDOCMD.C   This function translates keyboard entry         บ
บ                         into appropriate commands for Object Mode       บ
บ                         operation.                                      บ
บ                                                                         บ
บ              writtem by William A. Tolhurst                             บ
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
#include "edomode.h"

int Get_Object_Cmd (int *FunctionIndex) {

	int CmdIndex = CMD_IDLE;

	if (kbhit()) {
		switch(getch()) {

			case ENHANCED_KEY:

				switch(getch()) {
					case EKEY_LEFT:
						CmdIndex = CMD_MOVE_OBJECT;
						*FunctionIndex = FUN_MOVE_LEFT;
						break;

					case EKEY_RIGHT:
						CmdIndex = CMD_MOVE_OBJECT;
						*FunctionIndex = FUN_MOVE_RIGHT;
						break;

					case EKEY_DOWN:
						CmdIndex = CMD_MOVE_OBJECT;
						*FunctionIndex = FUN_MOVE_DOWN;
						break;

					case EKEY_UP:
						CmdIndex = CMD_MOVE_OBJECT;
						*FunctionIndex = FUN_MOVE_UP;
						break;

					case EKEY_CTRL_LEFT:
						CmdIndex = CMD_MOVE_OBJECT;
						*FunctionIndex = FUN_MOVE_YAWLEFT;
						break;

					case EKEY_CTRL_RIGHT:
						CmdIndex = CMD_MOVE_OBJECT;
						*FunctionIndex = FUN_MOVE_YAWRIGHT;
						break;

					case EKEY_CTRL_UP:
						CmdIndex = CMD_MOVE_OBJECT;
						*FunctionIndex = FUN_MOVE_PITCHUP;
						break;

					case EKEY_CTRL_DOWN:
						CmdIndex = CMD_MOVE_OBJECT;
						*FunctionIndex = FUN_MOVE_PITCHDOWN;
						break;

					default:
						Error_Beep ();
				}
				break;

			case KEY_PLUS:
				CmdIndex = CMD_MOVE_OBJECT;
				*FunctionIndex = FUN_MOVE_IN;
				break;

			case KEY_MINUS:
				CmdIndex = CMD_MOVE_OBJECT;
				*FunctionIndex = FUN_MOVE_OUT;
				break;

			case '\\':
				CmdIndex = CMD_MOVE_OBJECT;
				*FunctionIndex = FUN_MOVE_ROLLLEFT;
				break;

			case '/':
				CmdIndex = CMD_MOVE_OBJECT;
				*FunctionIndex = FUN_MOVE_ROLLRIGHT;
				break;

			case '<':
				CmdIndex = CMD_SCALE_OBJECT;
				*FunctionIndex = FUN_SCALE_UP;
				break;


			case '>':
				CmdIndex = CMD_SCALE_OBJECT;
				*FunctionIndex = FUN_SCALE_DOWN;
				break;


			case KEY_TAB:
				CmdIndex = CMD_SELECT_PRIMITIVE;
				*FunctionIndex = 0;
				break;

			case 'm':
			case 'M':
				CmdIndex = CMD_EDIT_PRIMITIVE;
				*FunctionIndex = 0;
				break;

			case 'a':
			case 'A':
				CmdIndex = CMD_ADD_PRIMITIVE;
				*FunctionIndex = 0;
				break;

			case 'd':
			case 'D':
				CmdIndex = CMD_DELETE_PRIMITIVE;
				*FunctionIndex = 0;
				break;

			case 'v':
			case 'V':
				CmdIndex = CMD_VERTEX_MODE;
				*FunctionIndex = 0;
				break;

			case KEY_ENTER:
				CmdIndex = CMD_EXIT;
				*FunctionIndex = 0;
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

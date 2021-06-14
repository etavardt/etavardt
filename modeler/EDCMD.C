/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ              Program  ED - A DATABASE MODELING TOOL                     บ
บ                                                                         บ
บ              EDCMD.C    This module contains a function which           บ
บ                         Interprets keystrokes into commands             บ
บ                         while the system is in Scene Mode               บ
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
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include <stdio.h>
#include <conio.h>
#include "edmain.h"
#include "edprotos.h"
#include "edkey.h"
#include "edscmode.h"

#define BUFFER_LENGTH 81


int Get_Cmd (int *FunctionIndex) {

	int CmdIndex = CMD_IDLE;

	if (kbhit()) {
		switch(getch()) {

			case ENHANCED_KEY:

				switch(getch()) {
					case EKEY_LEFT:
						CmdIndex = CMD_MOVE_VIEW;
						*FunctionIndex = FUN_LEFT;
						break;

					case EKEY_RIGHT:
						CmdIndex = CMD_MOVE_VIEW;
						*FunctionIndex = FUN_RIGHT;
						break;

					case EKEY_DOWN:
						CmdIndex = CMD_MOVE_VIEW;
						*FunctionIndex = FUN_DOWN;
						break;

					case EKEY_UP:
						CmdIndex = CMD_MOVE_VIEW;
						*FunctionIndex = FUN_UP;
						break;

					case EKEY_CTRL_LEFT:
						CmdIndex = CMD_MOVE_VIEW;
						*FunctionIndex = FUN_YAWLEFT;
						break;

					case EKEY_CTRL_RIGHT:
						CmdIndex = CMD_MOVE_VIEW;
						*FunctionIndex = FUN_YAWRIGHT;
						break;

					case EKEY_CTRL_UP:
						CmdIndex = CMD_MOVE_VIEW;
						*FunctionIndex = FUN_PITCHUP;
						break;

					case EKEY_CTRL_DOWN:
						CmdIndex = CMD_MOVE_VIEW;
						*FunctionIndex = FUN_PITCHDOWN;
						break;

					default:
						Error_Beep ();
				}
				break;

			case KEY_PLUS:
				CmdIndex = CMD_MOVE_VIEW;
				*FunctionIndex = FUN_IN;
				break;

			case KEY_MINUS:
				CmdIndex = CMD_MOVE_VIEW;
				*FunctionIndex = FUN_OUT;
				break;

			case '/':
				CmdIndex = CMD_MOVE_VIEW;
				*FunctionIndex = FUN_ROLLRIGHT;
				break;

			case '\\':
				CmdIndex = CMD_MOVE_VIEW;
				*FunctionIndex = FUN_ROLLLEFT;
				break;

			case 'i':
			case 'I':
				CmdIndex = CMD_LOAD_OBJ_FILE;
				*FunctionIndex = 0;
				break;

			case KEY_TAB:
				CmdIndex = CMD_SELECT_OBJECT;
				*FunctionIndex = 0;
				break;

			case 'a':
			case 'A':
				CmdIndex = CMD_ADD_OBJECT;
				*FunctionIndex = 0;
				break;

			case 'd':
			case 'D':
				CmdIndex = CMD_DELETE_OBJECT;
				*FunctionIndex = 0;
				break;

			case 'c':
			case 'C':
				CmdIndex = CMD_EDIT_OBJECT;
				*FunctionIndex = 0;
				break;

			case 'o':
			case 'O':
				CmdIndex = CMD_OBJECT_MODE;
				*FunctionIndex = 0;
				break;

			case 's':
			case 'S':
				CmdIndex = CMD_STUDIO_MODE;
				*FunctionIndex = 0;
				break;

			case 'p':
			case 'P':
				CmdIndex = CMD_EDIT_SYSPARMS;
				*FunctionIndex = 0;
				break;

			case 'r':
			case 'R':
				CmdIndex = CMD_READ_SESSION;
				*FunctionIndex = 0;
				break;

			case 'w':
			case 'W':
				CmdIndex = CMD_WRITE_SESSION;
				*FunctionIndex = 0;
				break;

			case KEY_ESCAPE:
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


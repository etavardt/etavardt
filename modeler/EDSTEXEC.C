/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ              Program  ED - A DATABASE MODELING TOOL                     บ
บ                                                                         บ
บ              EDSTEXEC.C Provides the executive functions (lots          บ
บ                         of them, aren't there) that control ED          บ
บ                         and direct operation within Studio Mode.        บ
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
#include "edstmode.h"

extern struct SystemParmType gvSystemParms;
extern struct PrimType *gvCurrentPrimPtr;

void Studio_Exec () {
	int CmdIndex, FunctionIndex;

	CmdIndex = CMD_INIT;

	while (CmdIndex != CMD_EXIT) {

		switch (CmdIndex) {

			case CMD_INIT:
				gvSystemParms.SystemMode = GC_SYSMODE_STUDIO;
				Display_Status ();
				break;

			case CMD_EDIT_STUDIO:
				Edit_Studio ();
				Display_Status ();

			case CMD_IDLE:
				/* Do Nothing */
				break;

		}

		CmdIndex = Get_Studio_Cmd ();

	}
}
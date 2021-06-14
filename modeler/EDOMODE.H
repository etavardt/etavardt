/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ              Program  ED - A DATABASE MODELING TOOL                     บ
บ                                                                         บ
บ              EDOMODE.H  Definitions for commands and command            บ
บ                         functions valid in Object Mode                  บ
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


// Commands

#define CMD_INIT 1
#define CMD_MOVE_OBJECT 2
#define CMD_SCALE_OBJECT 3
#define CMD_ADD_PRIMITIVE 4
#define CMD_DELETE_PRIMITIVE 5
#define CMD_SELECT_PRIMITIVE 6
#define CMD_EDIT_PRIMITIVE 7
#define CMD_VERTEX_MODE 8
#define CMD_IDLE 0
#define CMD_EXIT -1

// Object Move & Scale Functions
#define FUN_MOVE_LEFT 1
#define FUN_MOVE_RIGHT 2
#define FUN_MOVE_UP 3
#define FUN_MOVE_DOWN 4
#define FUN_MOVE_YAWLEFT 5
#define FUN_MOVE_YAWRIGHT 6
#define FUN_MOVE_PITCHUP 7
#define FUN_MOVE_PITCHDOWN 8
#define FUN_MOVE_ROLLLEFT 9
#define FUN_MOVE_ROLLRIGHT 10
#define FUN_MOVE_IN 11
#define FUN_MOVE_OUT 12

#define FUN_SCALE_UP 0
#define FUN_SCALE_DOWN 1

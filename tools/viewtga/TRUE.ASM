;
;
;***********************************************************************
;
;  File:	true.asm
;
;  Purpose:	Basic utilities for using the true-color modes.  The
;		true color modes supported are:
;
;			19-1  -	 320x200, 32K colors
;			19-2  -	 320x200, 65K colors
;			19-3  -	 320x200, 16M colors
;			45-1  -	 640x350, 32K colors
;			45-2  -	 640x350, 65K colors
;			45-3  -	 640x350, 16M colors
;			47-1  -	 640x400, 32K colors
;			47-2  -	 640x400, 65K colors
;			47-3  -	 640x400, 16M colors
;			46-1  -	 640x480, 32K colors
;			46-2  -	 640x480, 65K colors
;			46-3  -	 640x480, 16M colors
;			48-1  -	 800x600, 32K colors
;			48-2  -	 800x600, 65K colors
;
;		In addition, all routines are applicable to standard
;		16 and 256 color modes.
;
;  Warning:	The byte parameter in the write_vga_scanline() function
;		must be an even number!!!!!
;
;************************************************************************
;
FALSE	  equ	0		; Standard C values.
TRUE	  equ	1
;
CALLSIZE  equ	4		; Use 2 for small, 4 for medium model.
;				;  (also change NEAR/FAR)
;
in_arg0	  equ	[BP+2+CALLSIZE]
in_arg1	  equ	[BP+4+CALLSIZE]
in_arg2	  equ	[BP+6+CALLSIZE]
in_arg3	  equ	[BP+8+CALLSIZE]
in_arg4	  equ	[BP+10+CALLSIZE]
in_arg5	  equ	[BP+12+CALLSIZE]
;
PUBLIC	_get_vga_type
PUBLIC	_set_vga_mode
PUBLIC	_get_vga_mode
PUBLIC	_set_vga_seg
PUBLIC	_write_vga_scanline
;
;
_TEXT	SEGMENT BYTE PUBLIC 'CODE'	; Microsoft C segments.
_TEXT	ENDS
_DATA	SEGMENT WORD PUBLIC 'DATA'
_DATA	ENDS
CONST	SEGMENT WORD PUBLIC 'CONST'
CONST	ENDS
_BSS	SEGMENT WORD PUBLIC 'BSS'
_BSS	ENDS
;
DGROUP	GROUP CONST, _BSS, _DATA
;
ASSUME CS:_TEXT, DS:DGROUP, SS:DGROUP, ES:DGROUP
;
_TEXT	SEGMENT BYTE PUBLIC 'CODE'
;
GDC_SEG_SELECT  DW   03CDh	; ET4000 registers.
;
VGA_MEM_SEG	DW  0A000h	; Starting address (segment) of video memory.
;
;
;************************************************************************
;
;  Function:	get_vga_type()  
;
;  Purpose:	Returns the type of dac which is installed (a non-zero
;		value indicates that the card is true-color capable):
;
;			0  -  normal vga dac
;			1  -  dac capable of 32K colors
;			2  -  dac capable of 32K/65K colors
;			3  -  dac capable of 32K/65K/16M colors
;
;***********************************************************************
;
_get_vga_type	PROC FAR
;
		MOV  AH, 010h		; Get dac type using bios call.
		MOV  AL, 0F1h
		INT  010h
		MOV  AH, 0
		MOV  AL, BL
;
		RET
;
_get_vga_type	ENDP
;
;
;************************************************************************
;
;  Function:	set_vga_mode(mode, color_mode)  
;
;  Purpose:	Sets the hardware to the specified vga graphics mode.
;		The "mode" parameter is a normal (256 color) vga graphics
;		mode:
;
;			19 (0x13)   -	320x200
;			45 (0x2D)   -	640x350
;			47 (0x2F)   -	640x400
;			46 (0x2E)   -	640x480
;			48 (0x30)   -	800x600
;
;		The "color_mode" specifies the color resolution:
;
;			 0   -   256 colors
;			 1   -   32K colors
;			 2   -   65K colors
;			 3   -   16M colors
;
;		Returns TRUE if the mode is set, FALSE if the mode
;		was not sucessfully set.
;
;		Note:  for a color monitor, 0x03 is the default text mode.
;
;***********************************************************************
;
_set_vga_mode	PROC FAR
;
		PUSH BP			; Save registers.
		MOV  BP, SP
;
		MOV  AX, in_arg1
		CMP  AL, 000h		; If not true-color....
		JNE  svm0
;
		MOV  AH, 000h		; .....use normal bios mode set.
		MOV  AL, in_arg0
		INT  010h
		JMP  short svm2
;
svm0:		CMP  AL, 003h		; ....else if 15 or 16 bit mode, use
		JE   svm0a		; F0 then F2 bios calls.
		MOV  AH, 010h
		MOV  AL, 0F0h
		MOV  BL, in_arg0
		INT  010h
		CMP  AH, 000h
		JNE  svm1
;
		MOV  AH, 010h		; (adjust 15/16 bit mode)
		MOV  AL, 0F2h
		MOV  BL, in_arg1
		INT  010h
		CMP  AH, 000h
		JE   svm2
		JNE  svm1
;
svm0a:		MOV  AH, 010h		; ....else if 24 bit mode, use
		MOV  AL, 0F0h		; special F0 bios call.
		MOV  BL, 0FFh
		MOV  BH, in_arg0
		INT  010h
		CMP  AH, 000h
		JE   svm2
;
svm1:		MOV  AX, FALSE
		JMP  short svm4
;
svm2:		MOV  AX, TRUE
;
svm4:		POP  BP			; Restore registers.
		RET
;
_set_vga_mode	ENDP
;
;
;************************************************************************
;
;  Function:	get_vga_mode()  
;
;  Purpose:	Returns the current graphics mode.
;
;***********************************************************************
;
_get_vga_mode	PROC FAR
;
		MOV  AH, 00Fh		; .....use normal bios mode set.
		INT  010h
		MOV  AH, 000h
		RET
;
_get_vga_mode	ENDP
;
;
;************************************************************************
;
;  Function:	set_vga_seg(seg_num)  
;
;  Purpose:	Sets the vga segment register (read and write page
;		registers are set to the same segment).  Segment
;		may range from 0 to 15.
;
;***********************************************************************
;
_set_vga_seg	PROC FAR
;
		PUSH BP		; Save registers.
		MOV  BP, SP
;
		MOV  DX, CS:GDC_SEG_SELECT
		MOV  AX, in_arg0
		MOV  AH, AL
		SHL  AH, 1
		SHL  AH, 1
		SHL  AH, 1
		SHL  AH, 1
		OR   AL, AH
		OUT  DX, AL
;
		POP  BP			; Restore registers.
		RET
;
_set_vga_seg	ENDP
;
;
;************************************************************************
;
;  Function:	write_vga_scanline(x, y, xres, bytes_per_pixel,
;					 num_bytes, bytes)  
;
;  Purpose:	Writes an array of bytes starting at co-ordinate (x,y).
;		This method provides for different byte packing
;		between various true-color modes.
;
;  Warning:	The number of bytes must be an even number!!!!!!!
;
;***********************************************************************
;
_write_vga_scanline	PROC FAR
;
	PUSH BP			; Save registers.
	MOV  BP, SP
	PUSH SI
	PUSH DI
	PUSH ES
	PUSH DS
;
	MOV  SI, in_arg5	; Set source offset (DS is offset).
;
;				; Perform caculation for x, y start address
;				; Address = y(xres * bytes_per_pixel)
;				;  + x(bytes_per_pixel)
;
	MOV  AX, in_arg2	; AX = xres
	MOV  BX, in_arg3	; BX = bytes_per_pixel
	MUL  BX			; AX = (xres * bytes_per_pixel)
	MOV  DX, in_arg1	; DX = y
	MUL  DX			; DX:AX = y(xres * bytes_per_pixel) 
	XCHG AX, CX		; DX:CX = y(xres * bytes_per_pixel)
	XCHG DI, DX		; DI:CX = y(xres * bytes_per_pixel)
	MOV  AX, in_arg0	; AX = x
	MUL  BX			; AX = x(bytes_per_pixel)
	XCHG DX, DI
	ADD  AX, CX
	ADC  DX, 000h		; DX:AX = address
;
	MOV  ES, CS:VGA_MEM_SEG	; Set vga (destination) segment.
	MOV  DI, AX		; Set vga (destination) offset.
	MOV  AL, DL		; Set vga paging register.
	SHL  AL, 1
	SHL  AL, 1
	SHL  AL, 1
	SHL  AL, 1
	OR   AL, DL
	MOV  DX, CS:GDC_SEG_SELECT
	OUT  DX, AL
;
	MOV  CX, in_arg4	; Check if page boundary will be
	MOV  BX, CX		;  crossed
	ADD  BX, DI
	JNC  wsl4
;
	SUB  CX, BX		; If so, break into two bursts.
	SHR  CX, 1
	REP  MOVSW		; Transfer first group of pixels.
;
	MOV  CX, BX		; Cross page.
	MOV  DX, CS:GDC_SEG_SELECT
	IN   AL, DX
	ADD  AL, 011h
	OUT  DX, AL
;
wsl4:	SHR  CX, 1
	JCXZ wsl6		; Transfer final group of pixels.
	REP  MOVSW
;
wsl6:	POP  DS			; Restore registers.
	POP  ES
	POP  DI
	POP  SI
	POP  BP
	RET
;
_write_vga_scanline	ENDP
;
;
;
_TEXT		ENDS
		END

/************************************************************************
 *
 *  File:	true.h
 *
 *  Purpose:	Prototypes for functions implemented in true.asm.
 *
 ************************************************************************/

int get_vga_type(void);
int set_vga_mode(int mode, int true_flag);
int get_vga_mode(void);
void set_vga_seg(int seg);
void write_vga_scanline(int x, int y, int xres, int bytes_per_pixel, int bytes_per_line, unsigned char *pixels);

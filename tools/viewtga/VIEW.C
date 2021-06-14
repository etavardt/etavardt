/***********************************************************************
 *
 *  File:       view.c
 *
 *  Purpose:    Provide a simple demo of the true-color mode.  To
 *              use, type "trueview filename [/I | /mode_num]",
 *              where mode_num is:
 *
 *                      19  -   320x200, 32768 colors
 *                      45  -   640x350, 32768 colors
 *                      47  -   640x400, 32768 colors
 *                      46  -   640x480, 32768 colors*
 *                      48  -   800x600, 32768 colors
 *                      62  -   640x480, 16.8M colors**
 *
 *                       * = Standard default mode 
 *                      ** = 24-Bit default mode 
 *
 *
 *              The filename is assumed to be an uncompressed
 *              Targa file (type 2) and will be displayed in the
 *              selected mode. If no mode selection is made then
 *              the applicable default mode will be used.
 *
 *              If the "/I" switch is used then information
 *              about the file will be displayed.
 *
 *
 ************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <fcntl.h>
#include <dos.h>

#define MAIN_COMPILE 1

#include "true.h"

#define  OFF            0               /* Standard stuff.              */
#define  FALSE          0
#define  ON             1
#define  TRUE           1


typedef struct {
	char *mode_switch;      /* Mode switch argument */
	int mode_num;           /*   equivalent Mode # */
	int dac_req;            /* Minimum Dac Type requirement for mode */
	int x_res;              /* X Resolution of screen */
	int y_res;              /* Y Resolution of screen */
	int bits_per_pixel;     /* Bits per pixel */
	int hicolor;            /* HiColor Mode Bios call required */
	} MODE;


static MODE mode_table [] =

{
	"/19",0x13,2,320,200,16,1,   /* Mode 19 - 320x200, 32768 colors */
	"/45",0x2d,1,640,350,16,1,   /* Mode 45 - 640x350, 32768 colors */
	"/47",0x2f,1,640,400,16,1,   /* Mode 47 - 640x400, 32768 colors */
	"/46",0x2e,1,640,480,16,1,   /* Mode 46 - 640x480, 32768 colors */
	"/48",0x30,2,800,600,16,1,   /* Mode 48 - 800x600, 32768 colors */
	"/62",0x2e,3,640,480,24,3    /* Mode 62 - 640x480, 16.8M colors */
} ;

#define NMODES  (sizeof(mode_table)/sizeof(MODE))


/* Standard default Mode 46 - 640x480, 32768 colors */

static MODE default_mode_table [] =
{
	"/46",0x2e,1,640,480,16,TRUE
} ;

static MODE *mode_ptr;          /* Pointer for Mode parameters */


typedef struct {
	int image_type;         /* Image Type (must be 2 for RGB TrueColor) */
	int image_xorg;         /* Bottom Left X coordinate of image */
	int image_yorg;         /* Bottom Left Y coordinate of image */
	int image_width;        /* Width of image in pixels */
	int image_height;       /* Height of image in pixels */
	int image_bpp;          /* Bits per pixel (must be 16 or 24) */
	int image_descriptor;   /* Image Descriptor */
	} IMAGE;

static IMAGE image_info [1];    /* storage for image information */
static IMAGE *image_ptr;
	
char *info_switch1 = "/i";      /* Command Line switch for Info */
char *info_switch2 = "/I";      /* Command Line switch for Info */
char *default_mode = "/46";     /* Default Mode switch */
char *default24_mode = "/62";   /* Default 24-Bit Mode Switch */

static FILE *fptr;
static unsigned char image_buf[1920];

/* Prototypes */

int main(int argc, char *argv[]);
int set_mode_parms(char *mode_sel);
int open_targa_file(char *filename, int iflag);
void image_to_et4000(void);


/************************************************************************
 *
 *  Function:   main(argc, argv)
 *
 ************************************************************************/


main(int argc, char *argv[])

{
	int dac_type;           /* storage for hardware DAC type */
	int saved_mode;         /* storage for saved VGA mode */
	int mode_switch_flag;   /* Mode switch on command line indicator */
	int file_info_flag;     /* Info switch on command line indicator */
	
	mode_ptr = default_mode_table;
	mode_switch_flag = FALSE;
	file_info_flag = FALSE;
	
	if (argc == 1) {                /* Display Help Menu if no filename */

		printf ("\n\r");
		printf ("\n\r");
		printf ("\n\r");
		printf("TRUEVIEW      Targa File Viewer     Version 1.0   2-3-92");
		printf("\n\r");
		printf("Cardinal Technologies, Inc.");
		printf ("\n\r");
		printf ("\n\r");
		printf("Demo Program for TRUE-COLOR Modes");
		printf ("\n\r");
		printf("Usage: TRUEVIEW [d:path] filename [/I | /Mode].");
		printf ("\n\r");
		printf ("\n\r");
		printf("Where filename is an uncompressed Targa RGB file."); 
		printf ("\n\r");
		printf("Where /I is to display targa file information only.");
		printf ("\n\r");
		printf("Where /Mode is one of the following numbers:");
		printf ("\n\r");
		printf ("\n\r");
		printf("    19  -   320x200, 32768 colors");
		printf ("\n\r");
		printf("    45  -   640x350, 32768 colors");
		printf ("\n\r");
		printf("    47  -   640x400, 32768 colors");
		printf ("\n\r");
		printf("    46  -   640x480, 32768 colors*");
		printf ("\n\r");
		printf("    48  -   800x600, 32768 colors");
		printf ("\n\r");
		printf("    62  -   640x480, 16.8M colors**");
		printf ("\n\r");
		printf ("\n\r");
		printf("     * = Standard default mode ");
		printf ("\n\r");
		printf("    ** = 24-Bit default mode ");
		printf ("\n\r");
		printf ("\n\r");
		printf ("\n\r");
		printf ("\n\r");
		exit(0);

	}

	if (argc > 3) {         /* check if illegal argument count */

		printf("\n\rInvalid Parameters\n\r");
		exit(0);
	}

	set_mode_parms(default_mode);   /* set default mode parameters */

	if (argc == 3) {                /* if possible switch on command line */

		if (!strcmp(argv[2], info_switch1)) {

			file_info_flag = TRUE;

		} else if (!strcmp(argv[2], info_switch2)) {

			file_info_flag = TRUE;

		} else {

			if(!(mode_switch_flag = set_mode_parms(argv[2]))) {

				printf("\n\rInvalid Parameter\n\r");
				exit(0);
			}
		}
	}

	if (!open_targa_file(argv[1],file_info_flag)) {

		exit(0);

	}

	if (mode_ptr->bits_per_pixel != image_ptr->image_bpp) {

		if (mode_switch_flag == TRUE) {

			printf("\n\rRequested Mode does not support File Image\n\r");
			fclose(fptr);
			exit(0);

		} else if (image_ptr->image_bpp == 24) {

			set_mode_parms(default24_mode);

		} else {

			printf("\n\rFile Image not supported by Hardware\n\r");
			fclose(fptr);
			exit(0);

		}
	}

	dac_type = get_vga_type();      /* Check if hi-color DAC present */

	if (dac_type < mode_ptr->dac_req) {

		printf("\n\rHicolor Hardware Support not present:");
		printf("\n\r    Dac Type %d found", dac_type);
		printf("\n\r    Dac Type %d required", mode_ptr->dac_req);
		printf ("\n\r");
		fclose(fptr);
		exit(0);

	}

	saved_mode = get_vga_mode();    /* Save present VGA Mode  */

	if (!set_vga_mode(mode_ptr->mode_num, mode_ptr->hicolor)) {

		printf("\n\rCould not set (%d) mode.", mode_ptr->mode_num);
		printf("\n\r");
		fclose(fptr);
		exit(0);

	}

	image_to_et4000();      /* Output File Image */

	getch();
	
	set_vga_mode(saved_mode, FALSE);

/*      
	printf("\n\rMode Switch: %s", mode_ptr->mode_switch);
	printf("\n\rMode: 0x%x",mode_ptr->mode_num);
	printf("\n\rDac type required: %d", mode_ptr->dac_req);
	printf("\n\rX Resolution: %d", mode_ptr->x_res);
	printf("\n\rY Resolution: %d", mode_ptr->y_res);
	printf("\n\rbbp: %d", mode_ptr->bits_per_pixel);
	printf("\n\r");
*/
	return(0);
	
}

/*
 *      Function: set_mode_parms(char *mode_sel)
 *
 *      Purpose: Sets VGA Mode parameters from /Mode
 *               switch on command line or from
 *               internal default mode switches.
 *
 *      Returns: TRUE for mode found in mode table,
 *               FALSE for mode not found in table.
 *
 */

int set_mode_parms(char *mode_sel)

{
	int i;
	int mode_flag;

	mode_ptr = default_mode_table;
	mode_flag = FALSE;
	
	for (i = 0; i < NMODES; i++) {

		if (!strcmp(mode_sel, mode_table[i].mode_switch)) {

			mode_ptr->mode_switch = mode_table[i].mode_switch;
			mode_ptr->mode_num = mode_table[i].mode_num;
			mode_ptr->dac_req = mode_table[i].dac_req;
			mode_ptr->x_res = mode_table[i].x_res;
			mode_ptr->y_res = mode_table[i].y_res;
			mode_ptr->bits_per_pixel = mode_table[i].bits_per_pixel;
			mode_ptr->hicolor = mode_table[i].hicolor;
			mode_flag = TRUE;
			break;
		}
	}
	return(mode_flag);
}

/*
 *      Function: open_targa_file(filename, iflag)
 *
 *      Purpose: Opens requested targa file and stores
 *               header information. If iflag is set
 *               then header information is displayed
 *               and file is closed.
 *
 *      Note:   The only checking done is to ensure that
 *              the Image Type field = 2 for uncompressed
 *              True-Color image. If wrong image type
 *              then file is closed.
 *
 *      Returns: File open status; TRUE for file is open,
 *               FALSE for file closed or unable to open.
 *
 *
 */

int open_targa_file(filename,iflag)

	char *filename;
	int     iflag;          /* Open File for Information Only Flag */
{
	unsigned int buf[18/2]; /* Targa File Header = 18 Bytes */ 
	char fname[80];

	strcpy (fname, filename);
	
	image_ptr = image_info;
	
	if ((strstr(fname, ".TGA") == NULL) && (strstr(fname, ".tga") == NULL))

		strcat(fname, ".TGA");

	fptr = fopen(fname, "rb"); 

	if (fptr == NULL) {

		printf ("\n\r %s Input file not found.\n\r",fname);
		return(FALSE);

	}

	 /* Read targa header. */
	if (!fread((void *)buf, 18/2, sizeof(int), fptr)) {

		printf ("\n\rError reading Input file.\n\r");
		return(FALSE);

	}

	/* store header information */
	image_ptr->image_type = (buf[1] & 0xff);
	image_ptr->image_xorg = buf[4];
	image_ptr->image_yorg = buf[5];
	image_ptr->image_width = buf[6];
	image_ptr->image_height = buf[7];
	image_ptr->image_bpp = (buf[8] & 0xff);
	image_ptr->image_descriptor = ((buf[8] >> 8) & 0xff);
	

	if (iflag == TRUE) {    /* if iflag set display info and close file */
	
		printf("\n\r\n\rFILE: %s", filename);
		printf("\n\rImage Type: %d", image_ptr->image_type);
		printf("\n\rImage X Origin: %d", image_ptr->image_xorg);
		printf("\n\rImage Y Origin: %d", image_ptr->image_yorg);
		printf("\n\rImage Width: %d", image_ptr->image_width);
		printf("\n\rImage Heigth: %d", image_ptr->image_height);
		printf("\n\rBits per Pixel: %d", image_ptr->image_bpp);
		printf("\n\rImage Descriptor: %d", image_ptr->image_descriptor);
		printf("\n\r");
/*      
		printf("\n\rMode Switch: %s", mode_ptr->mode_switch);
		printf("\n\rMode: 0x%x",mode_ptr->mode_num);
		printf("\n\rDac type required: %d", mode_ptr->dac_req);
		printf("\n\rX Resolution: %d", mode_ptr->x_res);
		printf("\n\rY Resolution: %d", mode_ptr->y_res);
		printf("\n\rbbp: %d", mode_ptr->bits_per_pixel);
		printf("\n\r");
*/
		fclose(fptr);
		return(FALSE);
	}

	if (image_ptr->image_type != 2) {       /* ensure image type 2 */

		printf ("\n\rInput file not RGB Type 2.\n\r");
		fclose(fptr);
		return(FALSE);

	}

	return(TRUE);
}


/*
 *      Function: image_to_et4000()
 *
 *      Purpose: Transfers file Image Data to ET4000
 *               display memory. It is assumed that
 *               that the file is opened and that
 *               fptr is pointing to start of Image
 *               data. The file is closed at the end
 *               of the transfer. No checking is done
 *               during the transfer.
 *
 *      Note:   If the Image Data is wider than the
 *              Display then the image is clipped on
 *              the right.
 *              It is also assumed that the ordering
 *              of the image data starts from the bottom
 *              left. (Image Descriptor not used).
 *
 */

void image_to_et4000(void)

{
	int i;
	int width;              /* width of image data in bytes */
	int height;             /* height of image in scanlines */
	int xres;               /* current x resolution of display */
	int bytes_per_pixel;    /* bytes per pixel */
	int bytes_per_line;     /* #bytes transfered to ET4000 for each scanline */

	xres = mode_ptr->x_res;

	/* calculate bytes per pixel */
	bytes_per_pixel = mode_ptr->bits_per_pixel / 8;

	/* calculate #image bytes to read per scanline */
	width = image_ptr->image_width * bytes_per_pixel;

	height = image_ptr->image_height - 1;


	if (image_ptr->image_width > xres) {    /* clip image if necessary */

		bytes_per_line = xres * bytes_per_pixel;

	} else {

		bytes_per_line = width;

	}
	
	for (i = 0; i <= height; i++)         /* transfer image data */
		{
		if (fread((void *)image_buf, width, sizeof (char), fptr))
		        {
			write_vga_scanline(0, i, xres, bytes_per_pixel,
						 bytes_per_line, image_buf);
			}
		else
			{
			break;
			}
		}
	fclose(fptr);
}

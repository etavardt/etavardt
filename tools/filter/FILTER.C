
/*
	filter -- filter an image using a user defined 3x3 filter.

	Modifid to read stdin and stdout ie
		filter -f ave.flt -o foo.img <bar.img
			-f filter def
			-i input file (stdin default)
			-o output file (defaults to stdout)

	filter files have extension .flt and are of the form
	1 2 3
	4 5 6
	7 8 9
	div

*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

//#define MSDOS

#ifdef MSDOS
	#define READ_MODE       "rb"
	#define WRITE_MODE      "wb"
#else
	#define READ_MODE       "r"
	#define WRITE_MODE      "w"
#endif

unsigned char   *inbuf[3][3];   /* index, rgb */
unsigned char   *outbuf[3];     /* rgb */

int     xres, yres;
int     weights[3][3], divisor;

main(int ac, char **av)
{
	FILE    *infp, *outfp, *filtfp,
		*open_input_file(), *open_output_file();
	char    filter_file[256], input_file[256], output_file[256];
	int     i, j, y;

	/* if no args present, remind user of command line args */
	if(ac == 1) {
		usage(av[0]);
	}

	/* init strings for file names */
	filter_file[0] = '\0';
	input_file[0] = '\0';
	output_file[0] = '\0';

	/* loop through and process command line args */
	for(i=1; i<ac; i++) {
		/* check first character, must be a '-' */
		if(av[i][0] != '-') {
			fprintf(stderr, "Error, unknown command line argument %s\n\n", av[i]);
			usage(av[0]);
		}
		switch(av[i][1]) {
			case 'f' :
				i++;
				strcpy(filter_file, av[i]);
				break;
			case 'i' :
				i++;
				strcpy(input_file, av[i]);
				break;
			case 'o' :
				i++;
				strcpy(output_file, av[i]);
				break;
			default :
				usage(av[0]);
				break;
		}       /* end of command line switch statement */
	}

	filtfp = fopen(filter_file, "r");
	if(!filtfp) {
		fprintf(stderr, "Error opening filter definition file %s.\n", filter_file);
		exit(1);
	}
	fscanf(filtfp, "%d %d %d", &weights[0][0], &weights[1][0], &weights[2][0]);
	fscanf(filtfp, "%d %d %d", &weights[0][1], &weights[1][1], &weights[2][1]);
	fscanf(filtfp, "%d %d %d", &weights[0][2], &weights[1][2], &weights[2][2]);
	fscanf(filtfp, "%d", &divisor);

	/* open files, read/write headers, get image resolution */

	infp = open_input_file(input_file);
	outfp = open_output_file(output_file);

	fprintf(stderr, "Image size %dx%d\n", xres, yres);

	/* allocate space for input/output buffers */
	for(i=0; i<3; i++) {
		for(j=0; j<3; j++) {
			inbuf[i][j] = (unsigned char *)malloc(xres * sizeof(char));
			if(inbuf[i][j] == NULL) {
				fprintf(stderr, "Error allocating input buffer memory.  Aborting.\n");
				exit(1);
			}
		}
		outbuf[i] = (unsigned char *)malloc(xres * sizeof(char));
		if(outbuf[i] == NULL) {
			fprintf(stderr, "Error allocating output buffer memory.  Aborting.\n");
			exit(1);
		}
	}

	read_scan(infp, 0);     /* get first scan line */
	read_scan(infp, 1);     /* get second scan line */

	write_scan(outfp, inbuf[0]);    /* output 1st scan line */

	for(y=1; y<yres-1; y++) {
		fprintf(stderr, "%5d%c", y, 13);
		read_scan(infp, (y+1)%3);       /* get bottom scan line */

		filter(y%3);

		write_scan(outfp, outbuf);
	}
	fputc('\n', stderr);

	write_scan(outfp, inbuf[y%3]);  /* output last scan line */

	fclose(infp);
	fclose(outfp);

}       /* end of main() */

/*
	usage() -- If the user executes the program with no arguments
		or has an error in the arg list then print out
		instructions.  Note that this function exits.
*/

usage(char *prog_name)
{
	fprintf(stderr, "Usage: %s [-f <filt_file>] -i <input_file> -o <output_file>\n\n", prog_name);
	fprintf(stderr, "       -f causes the specified filter input file to be used.\n");
	fprintf(stderr, "          Filter input files should have an extension of .flt\n");
	fprintf(stderr, "          If no input and/or output files are specified, standard\n");
	fprintf(stderr, "          input/output is assumed.\n");

	exit(1);
}       /* end of usage() */

/*
	open_input_file() -- opens the specified file for input and reads
		the header info.  If no file name is given then stdin is
		assumed.  For MSDOS this must be forced into binary mode
		before use.
*/

FILE *open_input_file(char *filename)
{
	FILE    *fp;

	if(filename[0] != '\0') {
		fp = fopen(filename, READ_MODE);
		if(!fp) {
			fprintf(stderr, "Error opening file %s for input.\n", filename);
			exit(1);
		}
	} else {
		fp = stdin;
#ifdef MSDOS
		setmode(fileno(fp), O_BINARY);
#endif
	}

	/* read header */

	xres = fgetc(fp) * 256;
	xres += fgetc(fp);

	yres = fgetc(fp) * 256;
	yres += fgetc(fp);

	fgetc(fp); fgetc(fp);   /* wasted bytes */
	fgetc(fp); fgetc(fp);
	fgetc(fp); fgetc(fp);

	return fp;

}       /* end of open_input_file() */

/*
	open_output_file() -- opens the specified file for output and writes
		the header info.  If no file name is given then stdout is
		assumed.  For MSDOS this must be forced into binary mode
		before use.
*/

FILE *open_output_file(char *filename)
{
	FILE    *fp;

	if(filename[0] != '\0') {
		fp = fopen(filename, WRITE_MODE);
		if(!fp) {
			fprintf(stderr, "Error opening file %s for output.\n", filename);
			exit(1);
		}
	} else {
		fp = stdout;
#ifdef MSDOS
		setmode(fileno(fp), O_BINARY);
#endif
	}

	/* write header */

	fputc(xres/256, fp);
	fputc(xres%256, fp);

	fputc(yres/256, fp);
	fputc(yres%256, fp);

	fputc(0, fp); fputc(0, fp);
	fputc(0, fp); fputc(0, fp);
	fputc(0, fp); fputc(24, fp);

	return fp;

}       /* end of open_input_file() */

/*
	read_scan() -- Reads a single scan line from the input file.
*/

read_scan(FILE *fp, int index)
{
	int     cnt, r, g, b, i;

	i = 0;
	while(i < xres) {
		cnt = fgetc(fp);
		if(cnt == EOF) {
			fprintf(stderr, "Error, unexpected end of file reached.\n");
			exit(1);
		}
		r = fgetc(fp);
		g = fgetc(fp);
		b = fgetc(fp);
		while(cnt--) {
			inbuf[index][0][i] = r;
			inbuf[index][1][i] = g;
			inbuf[index][2][i] = b;
			i++;
		}
	}
}       /* end of read_scan() */

/*
	write_scan() -- Writes a single scan line to the output file.
*/

old_write_scan(FILE *fp, char *buf[3])
{
	int     i;

	for(i=0; i<xres; i++) {
		fputc(1, fp);   /* kludge count to 1 */
		fputc((int)buf[0][i], fp);
		fputc((int)buf[1][i], fp);
		fputc((int)buf[2][i], fp);
	}
}       /* end of write_scan() */

write_scan(FILE *fp, char *buf[3])
{
	int     i,              /* which pixel? */
		total,          /* how many left in scan? */
		count,          /* current run total */
		or, og, ob,     /* current run color */
		r, g, b;        /* next pixel color */

	i = 0;
	total = xres;
	or = buf[0][i];
	og = buf[1][i];
	ob = buf[2][i];
	i++;
	do {
		count = 1;
		total--;
		while(1) {
			r = buf[0][i];
			g = buf[1][i];
			b = buf[2][i];
			i++;
			if(r!=or || g!=og || b!=ob || count>=254 || total<=0) {
				break;
			}
			total--;
			count++;
		}
		if(fputc(count, fp) == EOF) {
			fprintf(stderr, "Error writing to disk.  Must be out of space.\n");
			exit(1);
		}
		fputc(ob, fp);
		fputc(og, fp);
		fputc(or, fp);

		or = r;
		og = g;
		ob = b;

		if(total==1) {          /* if at last pixel */
			fputc(1, fp);
			fputc(buf[0][xres-1], fp);
			fputc(buf[1][xres-1], fp);
			fputc(buf[2][xres-1], fp);
			total--;
		}
	} while(total>0);

}       /* end of write_scan() */

/*
	filter() -- Applies the filter to a scan line.  Uses the index
		to determine which scan line in the buffer is the
		"center" one.
*/

filter(int index)
{
	int     i, j, k, sum, offset;

	/* catch first and last pixels on scan line */
	for(i=0; i<3; i++) {
		outbuf[i][0] = inbuf[index][i][0];
		outbuf[i][xres-1] = inbuf[index][i][xres-1];
	}

	if(index == 0) {
		offset = 1;
	} else if(index == 1) {
		offset = 0;
	} else {
		offset = 2;
	}

	for(j=0; j<3; j++) {                    /* for each color */
		for(i=1; i<xres-1; i++) {       /* for each pixel not on edge */
			sum = 0.0;
			for(k=0; k<3; k++) {
				sum +=  inbuf[k][j][i-1] * weights[0][(k+offset)%3] +
					inbuf[k][j][i] * weights[1][(k+offset)%3] +
					inbuf[k][j][i+1] * weights[2][(k+offset)%3];
			}
			sum = sum/divisor;
			if(sum<0) {
				sum = -sum;
			}
			if(sum>255) {
				sum = 255;
			}
			outbuf[j][i] = sum;
		}
	}
}       /* end of filter() */


/*
	img2gif -- convert .img files to .gif files
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <io.h>
#include <ctype.h>

#define MIN(a,b)        ((a)<(b)?(a):(b))
#define MAX(a,b)        ((a)>(b)?(a):(b))

#define CLIP(x) ((x)>255?255:((x)<0?0:(x)))

#define PRINT_STEP      (16)

#define RES             (64)
#define DEF_NOISE       (8)

/* color palette selection methods */
#define POP             (0)
#define MEDIAN_BOX      (1)
#define FIXED           (2)
#define PAL_FILE        (3)

#define COUNT_LIMIT     (0xffffL)

long    count_limit = COUNT_LIMIT;      /* limit pixel count to this */

typedef struct box {
		int     r0, r1, g0, g1, b0, b1;
		long    rave, gave, bave;
		long    count;
} Box;

typedef struct node {
	unsigned short  color;
	unsigned short  count;  /* doubles as pixel count & palette index */
	struct node     *next;
} Node;

Node    *grid[RES][RES];        /* linked list headers for each red/green pair */

int     red[256], grn[256], blu[256];           /* 6 bit palette */
int     red8[256], grn8[256], blu8[256];        /* 8 bit for gif */

FILE    *infp, *palfp;

unsigned long   total_pixels = 0,
		total_colors = 0;

int     width, height,
	noise = 0,              /* default to no noise */
	method = POP,           /* default to pop */
	ordered = 0,            /* do ordered dither */
	fixed_pal = 0,          /* default fixed palette */
	save_pal = 0,           /* save palette to file? */
	last_pass = 0;          /* to help with out of RAM conditions */

int     ord_dith[4][4] = {{-7,  1, -5,  3},
			  { 5, -3,  7, -1},
			  {-4,  4, -6,  2},
			  { 8,  0,  6, -2}};

main(ac, av)
	int     ac;
	char    **av;
{
	int     i, w, h, c, r, g, b;
	long    total;
	char    file[80],       /* root of file name */
		infile[80],     /* input file name */
		outfile[80],    /* gif file name */
		palfile[80],    /* palette file name */
		name[80];       /* name of file as run from av[0] */
	int     (*grab_a_pix)(), get_pix(), get_pix_fs();

	strcpy(name, strrchr(av[0], '\\')+1);   /* get rid of the path */
	*strchr(name, '.') = 0;                 /* get rid of the .exe */
	strlwr(name);                           /* gimmee lower case */

	palfile[0] = 0;         /* NULL string */

	grab_a_pix = get_pix;   /* default, non dithered function for gif */
	if(ac < 2)
		usage(name);
	infile[0] = '\0';
	for(i=1; i<ac; i++) {   /* loop through command line args */
		strlwr(av[i]);
		if(av[i][0] != '-') {   /* must be file name */
			strcpy(file, av[i]);
			strcpy(infile, av[i]);
			strcpy(outfile, av[i]);
			strcat(infile, ".img");
			strcat(outfile, ".gif");
		} else {
			switch(av[i][1]) {
				case 'd':
					grab_a_pix = get_pix_fs;
					break;
				case 'f':
					method = FIXED;
					if(isdigit(av[i+1][0])) {
						fixed_pal = atoi(av[i+1]);
						i++;
					}
					break;
				case 'm':
					method = MEDIAN_BOX;
					if(isdigit(av[i+1][0])) {
						count_limit = atoi(av[i+1]);
						i++;
					}
					break;
				case 'o':
					ordered = 1;
					break;
				case 'p':
					i++;
					strcpy(palfile, av[i]);
					if(access(palfile, 0) == 0) {   /* if file exists */
						method = PAL_FILE;
					} else {
						save_pal = 1;
					}
					break;
				case 'r':
					srand(1962);
					noise = DEF_NOISE;
					if(isdigit(av[i+1][0])) {
						noise = atoi(av[i+1]);
						i++;
					}
					break;
				default:
					fprintf(stderr, "Error, unknown flag %s\n", av[i]);
					usage(name);
					break;
			}
		}
	}       /* end of i loop through args */

	infp = fopen(infile, "rb");
	if(!infp) {
		fprintf(stderr, "Error opening file %s for input.\n", infile);
		usage(name);
	}

	w = getc(infp) * 256;
	w += getc(infp);
	width = w;
	h = getc(infp) * 256;
	h += getc(infp);
	height = h;
	printf("Image size = %dx%d\n", w, h);
	getc(infp); getc(infp);         /* toss extra bytes */
	getc(infp); getc(infp);
	getc(infp); getc(infp);

	/* read in image and build tree if we need to */

	if(method==POP || method==MEDIAN_BOX) {
		printf("Building tree.\n        total colors = ");
		total_pixels = total_colors;
		total = (long)w * (long)h;
		while(total > 0) {
			c = getc(infp);
			b = getc(infp);
			g = getc(infp);
			r = getc(infp);
			add_color(r>>2, g>>2, b>>2, c);         /* add to tree */
			total -= c;
			if(total_pixels != total_colors) {
				if((total_colors % (PRINT_STEP<<2)) == 0)
					printf("%8ld\b\b\b\b\b\b\b\b", total_colors);
				total_pixels = total_colors;
			}
		}
		printf("%8ld\b\b\b\b\b\b\b\b", total_colors);
		printf("\n");
	}

	switch(method) {
		case POP        : pop();        break;
		case MEDIAN_BOX : m_box();      break;
		case FIXED      : fixed();      break;
		case PAL_FILE   :
			if((palfp = fopen(palfile, "r")) == NULL) {
				fprintf(stderr, "Error opening file %s for reading palette.\n", palfile);
				exit(1);
			}
			printf("Reading palette from %s\n", palfile);
			for(i=0; i<256; i++) {
				fscanf(palfp, "%d %d %d", &red8[i], &grn8[i], &blu8[i]);
			}
			fclose(palfp);
			break;
		default : fprintf(stderr, "No palette selection method chosen.\n");
			break;
	}

	/* fix palette entries for gif encoding */

	if(method != PAL_FILE) {
		for(i=0; i<256; i++) {
			red8[i] = red[i]*255, red8[i] /= 63;
			grn8[i] = grn[i]*255, grn8[i] /= 63;
			blu8[i] = blu[i]*255, blu8[i] /= 63;
		}
		if(method == FIXED && fixed_pal == 0)   /* grey */
			for(i=0; i<256; i++) {
				red8[i] = i;
				grn8[i] = i;
				blu8[i] = i;
			}
		if(save_pal) {
			if((palfp = fopen(palfile, "w")) == NULL) {
				fprintf(stderr, "Error opening file %s for writing palette.\n", palfile);
				exit(1);
			}
			printf("Writing palette to %s\n", palfile);
			for(i=0; i<256; i++) {
				fprintf(palfp, "%d %d %d\n", red8[i], grn8[i], blu8[i]);
			}
			fclose(palfp);
		}
	} else {        /* else if method == PAL_FILE */
		for(i=0; i<256; i++) {
			red[i] = red8[i]>>2;
			grn[i] = grn8[i]>>2;
			blu[i] = blu8[i]>>2;
		}
	}

	if(method==POP || method==MEDIAN_BOX)
		remap_all();    /* remap all the tree elements to the palette */

	/* 2nd pass through image */

	fclose(infp);
	infp = fopen(infile, "rb");
	if(!infp) {
		fprintf(stderr, "Error opening file %s for 2nd pass input.\n", infile);
		exit(1);
	}
	getc(infp); getc(infp);
	getc(infp); getc(infp);

	getc(infp); getc(infp);
	getc(infp); getc(infp);
	getc(infp); getc(infp);

	last_pass = 1;

	printf("GIF encoding in progress.\n");

	GIFEncode(outfile,              /* file name */
		w, h,                   /* image size */
		0,                      /* no interlace */
		0,                      /* bkg color */
		8,                      /* bits per pixel */
		red8, grn8, blu8,       /* palette arrays */
		grab_a_pix);            /* pixel function */

	fclose(infp);

}       /* end of img2gif */

#define SWAP(a, b)    { unsigned int   tmp;            \
			tmp = a; a = b; b = tmp; }

/*
	pop -- choose the best colors by picking the most popular
*/

pop()
{
	int             i, r, g;
	Node            *ptr;
	unsigned int    pal[256];

	printf("Choosing palette by popularity.\n");
	for(i=0; i<256; i++)
		pal[i] = 0;

	/* force corners of rgb color cube out of the running */
	add_color( 0,  0,  0, 0);
	add_color(63,  0,  0, 0);
	add_color( 0, 63,  0, 0);
	add_color( 0,  0, 63, 0);
	add_color(63, 63,  0, 0);
	add_color( 0, 63, 63, 0);
	add_color(63,  0, 63, 0);
	add_color(63, 63, 63, 0);

	/* force feed the corners into the palette */
	red[0] =  0; grn[0] =  0; blu[0] =  0;
	red[1] = 63; grn[1] =  0; blu[1] =  0;
	red[2] =  0; grn[2] = 63; blu[2] =  0;
	red[3] =  0; grn[3] =  0; blu[3] = 63;
	red[4] = 63; grn[4] = 63; blu[4] =  0;
	red[5] =  0; grn[5] = 63; blu[5] = 63;
	red[6] = 63; grn[6] =  0; blu[6] = 63;
	red[7] = 63; grn[7] = 63; blu[7] = 63;

	for(r=0; r<RES; r++)
		for(g=0; g<RES; g++) {
			ptr = grid[r][g];
			while(ptr) {
				if(ptr->count > pal[255]) {
					pal[255] = ptr->count;
					red[255] = r;
					grn[255] = g;
					blu[255] = ptr->color;

					i = 255;        /* bubble up */
					while(pal[i]>pal[i-1] && i>8) {
						SWAP(pal[i], pal[i-1]);
						SWAP(red[i], red[i-1]);
						SWAP(grn[i], grn[i-1]);
						SWAP(blu[i], blu[i-1]);
						i--;
					}
				}
				ptr = ptr->next;        /* move to next color */
			}       /* end of current chain */
		}       /* end of r loop */
}       /* end of pop */


/*
	fixed() -- use a fixed palette
		   0 == grey scale
		   1 == 8x8x4
		   2 == 6x7x6
*/

fixed()
{
	register int    i;
	int             r, g, b;

	printf("Using fixed palette %d, ", fixed_pal);
	switch(fixed_pal) {
		case 0: printf("grey scale.\n");
			for(i=0; i<256; i++) {
				red[i] = i>>2;
				grn[i] = i>>2;
				blu[i] = i>>2;
			}
			break;
		case 1: printf("8 red * 8 green * 4 blue shades.\n");
			for(i=0; i<256; i++) {
				red[i] = (i>>5)*63, red[i] /= 7;
				grn[i] = ((i>>2)&0x07)*63, grn[i] /= 7;
				blu[i] = (i&0x03)*63, blu[i] /= 3;
			}
			break;
		case 2: printf("6 red * 7 green * 6 blue shades.\n");
			for(b=0; b<6; b++)
				for(g=0; g<7; g++)
					for(r=0; r<6; r++) {
						i = r*42 + g*6 + b;
						red[i] = r*63/5;
						grn[i] = g*63/6;
						blu[i] = b*63/5;
					}
			break;
		default:
			printf("Error, palette choice not valid\n");
			printf("Valid choices are 0..2.\n");
			exit(1);
	}       /* end of switch */
}       /* end of fixed */

/*
	m_box() -- choose palette by median cut using boxes
*/

Box     box[256];

m_box()
{
	int     i, j, max, dr, dg, db;

	/* force the counts in the corners to be zero */

	force( 0,  0,  0, 0);
	force(63,  0,  0, 0);
	force( 0, 63,  0, 0);
	force( 0,  0, 63, 0);
	force(63, 63,  0, 0);
	force( 0, 63, 63, 0);
	force(63,  0, 63, 0);
	force(63, 63, 63, 0);

	/* assign the 1st eight boxes to be the corners */
	make_box( 0,  0,  0, 0, 1);
	make_box(63,  0,  0, 1, 1);
	make_box( 0, 63,  0, 2, 1);
	make_box( 0,  0, 63, 3, 1);
	make_box(63, 63,  0, 4, 1);
	make_box( 0, 63, 63, 5, 1);
	make_box(63,  0, 63, 6, 1);
	make_box(63, 63, 63, 7, 1);

	/* set up 9th box to hold the rest of the world */
	box[8].r0 = 0;
	box[8].r1 = 63;
	box[8].g0 = 0;
	box[8].g1 = 63;
	box[8].b0 = 0;
	box[8].b1 = 63;
	squeeze(8);

	/* split the rest of the boxes */

	printf("Choosing palette using median cut with boxes.\n");
	printf("        Counting by pixel, limit = %u.\n", count_limit);
	printf("        palette entry ");
	for(i=9; i<256; i++) {
#ifdef DEBUG
		printf("\n");
		for(j=0; j<i; j++)
			printf("box %3d    %2d %2d %2ld  %2d %2d %2ld  %2d %2d %2ld    %5ld\n", j, box[j].r0, box[j].r1, box[j].rave, box[j].g0, box[j].g1, box[j].gave, box[j].b0, box[j].b1, box[j].bave, box[j].count);
#endif
		printf("%3d\b\b\b", i+1);
		/* find biggest box */
		max = 8;
		for(j=8; j<i; j++)
			if(box[j].count > box[max].count)
				max = j;

		/* decide which side to split the box along, and split it */

		dr = box[max].r1 - box[max].r0;
		dg = box[max].g1 - box[max].g0;
		db = box[max].b1 - box[max].b0;
		box[i] = box[max];              /* copy info over */
		if(dr>=dg && dr>=db) {          /* red! */
			if(dr==2) {             /* tight squeeze */
				box[i].r1 = box[i].r0;
				box[max].r0 = box[max].r1;
			} else {                /* figure out where to split */
				j = box[max].rave;
				if(j==box[max].r1)
					j--;
				box[max].r1 = j;
				box[i].r0 = j+1;
			}
			squeeze(i);
			squeeze(max);
		} else if(dg>=db) {             /* green! */
			if(dg==2) {             /* tight squeeze */
				box[i].g1 = box[i].g0;
				box[max].g0 = box[max].g1;
			} else {                /* figure out where to split */
				j = box[max].gave;
				if(j==box[max].g1)
					j--;
				box[max].g1 = j;
				box[i].g0 = j+1;
			}
			squeeze(i);
			squeeze(max);
		} else {                        /* blue! */
			if(db==2) {             /* tight squeeze */
				box[i].b1 = box[i].b0;
				box[max].b0 = box[max].b1;
			} else {                /* figure out where to split */
				j = box[max].bave;
				if(j==box[max].b1)
					j--;
				box[max].b1 = j;
				box[i].b0 = j+1;
			}
			squeeze(i);
			squeeze(max);
		}

	}       /* end of i loop, all the boxes are found */

	printf("\n");

	/* get palette colors for each box */
	for(i=0; i<256; i++) {
		 red[i] = (box[i].r0+box[i].r1)/2;
		 grn[i] = (box[i].g0+box[i].g1)/2;
		 blu[i] = (box[i].b0+box[i].b1)/2;
	}

	/* fix *8 palette for gif encoding */
	for(i=0; i<256; i++) {
		red8[i] = red[i]*255, red8[i] /= 63;
		grn8[i] = grn[i]*255, grn8[i] /= 63;
		blu8[i] = blu[i]*255, blu8[i] /= 63;
	}

}       /* end of m_box() */

/*
	make_box -- make a 1x1x1 box at index with color rgb count c
*/

make_box(r, g, b, index, c)
	int     r, g, b, index, c;
{
	box[index].r0 = r;
	box[index].r1 = r;
	box[index].g0 = g;
	box[index].g1 = g;
	box[index].b0 = b;
	box[index].b1 = b;
	box[index].count = c;
}       /* end of make_box

/*
	squeeze -- shrink a boxes extremes to fit tightly

		   if a box is 1x1x1 change its count to 1
*/

squeeze(b)
	int     b;      /* the box to put the squeeze on */
{
	int     r0, r1, g0, g1, b0, b1;
	long    i, j, count = 0;
	Node    *ptr;

	r0 = box[b].r0;
	r1 = box[b].r1;
	g0 = box[b].g0;
	g1 = box[b].g1;
	b0 = box[b].b0;
	b1 = box[b].b1;

	box[b].r0 = 63; box[b].r1 = 0;
	box[b].g0 = 63; box[b].g1 = 0;
	box[b].b0 = 63; box[b].b1 = 0;
	box[b].rave = 0;
	box[b].gave = 0;
	box[b].bave = 0;

	for(i=r0; i<=r1; i++)
		for(j=g0; j<=g1; j++) {
			ptr = grid[i][j];
			while(ptr) {
				if(ptr->color > b1) {
					ptr = NULL;
				} else {
					if(ptr->color>=b0 && ptr->count>0L) {
						box[b].r0 = MIN(i, box[b].r0);
						box[b].r1 = MAX(i, box[b].r1);
						box[b].g0 = MIN(j, box[b].g0);
						box[b].g1 = MAX(j, box[b].g1);
						box[b].b0 = MIN(ptr->color, box[b].b0);
						box[b].b1 = MAX(ptr->color, box[b].b1);
						box[b].rave += (long)i * (long)ptr->count;
						box[b].gave += (long)j * (long)ptr->count;
						box[b].bave += (long)ptr->color * (long)ptr->count;
						count += (long)ptr->count;
					}
					ptr = ptr->next;
				}
			}
		}
	/* box is now shrunk */

	if(count) {
		box[b].rave /= count;
		box[b].gave /= count;
		box[b].bave /= count;
	}

	box[b].count = MIN(count, count_limit);

	if(box[b].r0 == box[b].r1 &&
	   box[b].g0 == box[b].g1 &&
	   box[b].b0 == box[b].b1) {    /* box is min size */
		box[b].count = 1;       /* so it won't get split again */
	}

}       /* end of squeeze */


/*
	usage -- tell them how its done
*/

usage(name)
	char    *name;          /* av(0) */
{
	fprintf(stderr, "Usage:  %s [-d] [-d] [-r (#)] [-m (#)] [-f (#)] <file>\n\n", name);
	fprintf(stderr, "where -d    use Floyd Steinberg dithering\n");
	fprintf(stderr, "      -o    use ordered dithering\n");
	fprintf(stderr, "      -r #  add random noise +- #\n");
	fprintf(stderr, "            defaults to +-%d if # not given\n", DEF_NOISE);
	fprintf(stderr, "      -m    choose palette with median cut\n");
	fprintf(stderr, "            defaults to popularity\n");
	fprintf(stderr, "      -m #  limit median cut pixel count to #\n");
	fprintf(stderr, "      -f #  use a fixed palette, fast but (sometimes) ugly\n");
	fprintf(stderr, "            # determines which palette to use\n");
	fprintf(stderr, "            0 == grey scale\n");
	fprintf(stderr, "            1 == 8 red * 8 green * 4 blue shades\n");
	fprintf(stderr, "            2 == 6 red * 7 green * 6 blue shades\n");
	fprintf(stderr, "      -p <pal_file>  if pal_file exists use it\n");
	fprintf(stderr, "            as the source of the palette, if it\n");
	fprintf(stderr, "            doesn't exist, create it with the new palette\n");

	exit(1);
}

/*
	my_calloc
*/

#define CHUNK           (32000)
#define MIN_SIZE        (1024)

void    *my_calloc(n)
	int     n;      /* how many bytes */
{
	static char             *ptr = NULL;
	char                    *ret;
	static unsigned int     bytes_left = 0;

	if(n > bytes_left) {            /* we need more RAM */
		ptr = calloc(CHUNK, 1);
		if(ptr==NULL) {         /* squeeze every last bit */
			ret = calloc(n, 1);
			if(ret) {
				return ret;
			}
			if(last_pass) {
				return NULL;
			}
			fprintf(stderr, "\nError, not enough RAM.\n");
			exit(1);
		}
		bytes_left = CHUNK;
	}
	ret = ptr;
	ptr += n;
	bytes_left -= n;

	return ret;
}       /* my_calloc() */


/*
	add_color -- add a color to the tree (or at least up its count)
*/

add_color(r, g, b, c)
	int     r, g, b,        /* color to add to tree */
		c;              /* number of pixels */
{
	Node    *ptr, *prev;
	void    *my_calloc();
	long    ltmp;

	c = MIN(c, count_limit);

	ptr = grid[r][g];
	prev = ptr;
	if(!ptr) {              /* new list */
		ptr = (Node *)my_calloc(sizeof(Node));
		grid[r][g] = ptr;
		ptr->color = b;
		ptr->count = (unsigned int)c;
		ptr->next = NULL;
		total_colors++;

		return;
	}
	if(ptr->color > b) {    /* need a new node at the head */
		prev = (Node *)my_calloc(sizeof(Node));
		prev->color = b;
		prev->count = (unsigned int)c;
		prev->next = ptr;
		grid[r][g] = prev;
		total_colors++;

		return;
	}

	for(;;) {       /* walk down the list looking for the right place */
		if(ptr->color == b) {
			ltmp = ptr->count;
			ltmp += (unsigned int)c;
			ltmp = MIN(ltmp, count_limit);
			ptr->count = (unsigned int)ltmp;

			return;
		}
		if(ptr->next == (Node *)NULL) {     /* end of the rope */
			ptr->next = (Node *)my_calloc(sizeof(Node));
			ptr = ptr->next;
			ptr->color = b;
			ptr->count = (unsigned int)c;
			ptr->next = NULL;
			total_colors++;

			return;
		}

		prev = ptr;             /* step down to next node */
		ptr = ptr->next;
	}       /* end of forever loop */
}       /* end of add_color()*/

/*
	get_index -- get the palette index of an rgb color
*/

get_index(r, g, b)
	int     r, g, b;        /* color to find*/
{
	Node            *ptr, *prev;
	void            *my_calloc();
	unsigned short  color;

	if(method==FIXED) {
		switch(fixed_pal) {
			case 0: {
					long  i, j, k;
					i = r; j = g; k = b;
					r = (i*77+j*150+k*29)/64;
					return r;
				}
				break;
			case 1: return ((r<<2)&0xe0) + ((g>>1)&0x1c) + ((b>>4)&0x03);
				break;
			case 2: return ((6*r)/64)*42 + ((7*g)/64)*6 + ((6*b)/64);
				break;
		}
	}

	ptr = grid[r][g];
	prev = ptr;
	if(!ptr) {      /* no color so we must create one */
		ptr = (Node *)my_calloc(sizeof(Node));
		if(ptr) {
			grid[r][g] = ptr;
			color = map(r, g, b);
			ptr->count = color;
			ptr->color = b;
			ptr->next = NULL;
			total_colors++;
		} else {
			color = map(r, g, b);
		}

		return color;
	}
	if(ptr->color > b) {    /* need a new node at the head */
		prev = (Node *)my_calloc(sizeof(Node));
		if(prev) {
			color = map(r, g, b);
			prev->count = color;
			prev->color = b;
			prev->next = ptr;
			grid[r][g] = prev;
			total_colors++;
		} else {
			color = map(r, g, b);
		}

		return color;
	}

	for(;;) {       /* walk down the list looking for the right color */
		if(ptr->color == b) {
			return ptr->count;
		}
		if(ptr->next == (Node *)NULL) {     /* end of the rope */
			ptr->next = (Node *)my_calloc(sizeof(Node));
			if(ptr->next) {
				ptr = ptr->next;
				ptr->color = b;
				color = map(r, g, b);
				ptr->count = color;
				ptr->next = NULL;
				total_colors++;
			} else {
				color = map(r, g, b);
			}

			return color;
		}

		prev = ptr;             /* step down to next node */
		ptr = ptr->next;
	}       /* end of forever loop */
}       /* end of get_index()*/

/*
	force -- force a specific count on a color (if it exists)
*/

force(r, g, b, c)
	int     r, g, b,        /* color to add to tree */
		c;              /* count to force */
{
	Node    *ptr, *prev;
	void    *my_calloc();
	long    ltmp;

	ptr = grid[r][g];
	prev = ptr;
	if(!ptr) {              /* new list */
		ptr = (Node *)my_calloc(sizeof(Node));
		grid[r][g] = ptr;
		ptr->color = b;
		ptr->count = (unsigned int)c;
		ptr->next = NULL;
		total_colors++;

		return;
	}
	if(ptr->color > b) {    /* need a new node at the head */
		prev = (Node *)my_calloc(sizeof(Node));
		prev->color = b;
		prev->count = (unsigned int)c;
		prev->next = ptr;
		grid[r][g] = prev;
		total_colors++;

		return;
	}

	for(;;) {       /* walk down the list looking for the right place */
		if(ptr->color == b) {
			ptr->count = (unsigned int)c;

			return;
		}
		if(ptr->next == (Node *)NULL) {     /* end of the rope */
			ptr->next = (Node *)my_calloc(sizeof(Node));
			ptr = ptr->next;
			ptr->color = b;
			ptr->count = (unsigned int)c;
			ptr->next = NULL;
			total_colors++;

			return;
		}

		prev = ptr;             /* step down to next node */
		ptr = ptr->next;
	}       /* end of forever loop */
}       /* end of force()*/


/*
	get_pix(x, y) -- return the palette index of x,y
*/

get_pix(x, y)
	int     x, y;
{
	static int      pal, count=0, line = -1;
	static int      c, r, g, b;
	int             r2, g2, b2;

	if(line == -1) {
		printf("        line ");
	}
	if(line != y) {
		line = y;
		if((line % PRINT_STEP) == 0)
			printf("%4d  colors %6lu\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b", line, total_colors);
	}
	if(count == 0) {
		count = getc(infp) - 1;
		b = getc(infp);
		g = getc(infp);
		r = getc(infp);
		pal = get_index(r>>2, g>>2, b>>2);
	} else {
		count--;
	}

	if(noise) {
		r2 = r + rand()%noise - rand()%noise; r2 = CLIP(r2);
		g2 = g + rand()%noise - rand()%noise; g2 = CLIP(g2);
		b2 = b + rand()%noise - rand()%noise; b2 = CLIP(b2);
		pal = get_index(r2>>2, g2>>2, b2>>2);
	}
	if(ordered) {
		register int    scale;

		scale = fixed==0 ? 1 : 2;
		if(noise == 0) {
			r2 = r;
			g2 = g;
			b2 = b;
		}
		r2 += scale * ord_dith[x%4][y%4]; r2 = CLIP(r2);
		g2 += scale * ord_dith[x%4][y%4]; g2 = CLIP(g2);
		b2 += scale * ord_dith[x%4][y%4]; b2 = CLIP(b2);
		pal = get_index(r2>>2, g2>>2, b2>>2);
	}

	return pal;
}       /* end of get_pix() */



/*
	get_pix_fs -- get_pix that does fs dithering
*/

get_pix_fs(x, y)
	int     x, y;
{
	int             i, j, r, g, b, c;
	int             rerr, gerr, berr;
	static int      *buf[2][3];             /* our buffer */
	static int      line = -1;

	if(line == -1) {
		/* allocate space for the buffer */
		for(i=0; i<2; i++)
			for(j=0; j<3; j++)
				buf[i][j] = my_calloc((width+1)*sizeof(int));
		i = 0;                  /* read in scan line 0 */
		while(i<width) {
			c = getc(infp);
			b = getc(infp);
			g = getc(infp);
			r = getc(infp);
			rerr = r;
			gerr = g;
			berr = b;
			while(c) {
				if(noise) {
					rerr = r + rand()%noise - rand()%noise; rerr = CLIP(rerr);
					gerr = g + rand()%noise - rand()%noise; gerr = CLIP(gerr);
					berr = b + rand()%noise - rand()%noise; berr = CLIP(berr);
				}
				buf[line&0x01][0][i] = rerr;
				buf[line&0x01][1][i] = gerr;
				buf[line&0x01][2][i] = berr;
				c--; i++;
			}
		}       /* we've got line 0 */
		printf("        dithering line ");
	}       /* end of initial setup */

	if(line != y) {         /* we're starting a new line */
		line = y;
		if((line % PRINT_STEP) == 0)
			printf("%4d  colors %6lu\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b", line, total_colors);
		if(y < height-1) {     /* if not at last scan line */
			i = 0;                  /* read in scan line y */
			while(i<width) {
				c = getc(infp);
				b = getc(infp);
				g = getc(infp);
				r = getc(infp);
				rerr = r;
				gerr = g;
				berr = b;
				while(c) {
					if(noise) {
						rerr = r + rand()%noise - rand()%noise; rerr = CLIP(rerr);
						gerr = g + rand()%noise - rand()%noise; gerr = CLIP(gerr);
						berr = b + rand()%noise - rand()%noise; berr = CLIP(berr);
					}
					buf[line&0x01][0][i] = rerr;
					buf[line&0x01][1][i] = gerr;
					buf[line&0x01][2][i] = berr;
					c--; i++;
				}
			}       /* we've got line y in buf y%2 */

			j = (line+1)%2;

			/* dither buf (line+1)%2  ie j */

			if(j) {     /* alternate for serpentine */
				for(i=1; i<width-1; i++) {
					r = CLIP(buf[j][0][i]);
					g = CLIP(buf[j][1][i]);
					b = CLIP(buf[j][2][i]);
					c = get_index(r>>2, g>>2, b>>2);
					buf[j][0][i] = c;   /* put palette index back for output */
					rerr = r - red8[c];
					gerr = g - grn8[c];
					berr = b - blu8[c];
					if(rerr) {
						buf[j][0][i+1] += rerr*7/16;
						buf[y%2][0][i] += rerr*5/16;
						buf[y%2][0][i+1] += rerr/16;
						buf[y%2][0][i-1] += rerr*3/16;
					}
					if(gerr) {
						buf[j][1][i+1] += gerr*7/16;
						buf[y%2][1][i] += gerr*5/16;
						buf[y%2][1][i+1] += gerr/16;
						buf[y%2][1][i-1] += gerr*3/16;
					}
					if(berr) {
						buf[j][2][i+1] += berr*7/16;
						buf[y%2][2][i] += berr*5/16;
						buf[y%2][2][i+1] += berr/16;
						buf[y%2][2][i-1] += berr*3/16;
					}
				}   /* end of i loop */
				/* fix first and last pixels in scan line */
				i = 0;
				r = CLIP(buf[j][0][i]);
				g = CLIP(buf[j][1][i]);
				b = CLIP(buf[j][2][i]);
				c = get_index(r>>2, g>>2, b>>2);
				buf[j][0][i] = c;
				i = width-1;
				r = CLIP(buf[j][0][i]);
				g = CLIP(buf[j][1][i]);
				b = CLIP(buf[j][2][i]);
				c = get_index(r>>2, g>>2, b>>2);
				buf[j][0][i] = c;
			} else {    /* else scan left */
				for(i=width-2; i>0; i--) {
					r = CLIP(buf[j][0][i]);
					g = CLIP(buf[j][1][i]);
					b = CLIP(buf[j][2][i]);
					c = get_index(r>>2, g>>2, b>>2);
					buf[j][0][i] = c;   /* put palette index back for output */
					rerr = r - red8[c];
					gerr = g - grn8[c];
					berr = b - blu8[c];
					if(rerr) {
						buf[j][0][i-1] += rerr*7/16;
						buf[y%2][0][i] += rerr*5/16;
						buf[y%2][0][i-1] += rerr/16;
						buf[y%2][0][i+1] += rerr*3/16;
					}
					if(gerr) {
						buf[j][1][i-1] += gerr*7/16;
						buf[y%2][1][i] += gerr*5/16;
						buf[y%2][1][i-1] += gerr/16;
						buf[y%2][1][i+1] += gerr*3/16;
					}
					if(berr) {
						buf[j][2][i-1] += berr*7/16;
						buf[y%2][2][i] += berr*5/16;
						buf[y%2][2][i-1] += berr/16;
						buf[y%2][2][i+1] += berr*3/16;
					}
				}   /* end of i loop */
				/* fix first and last pixels in scan line */
				i = 0;
				r = CLIP(buf[j][0][i]);
				g = CLIP(buf[j][1][i]);
				b = CLIP(buf[j][2][i]);
				c = get_index(r>>2, g>>2, b>>2);
				buf[j][0][i] = c;
				i = width-1;
				r = CLIP(buf[j][0][i]);
				g = CLIP(buf[j][1][i]);
				b = CLIP(buf[j][2][i]);
				c = get_index(r>>2, g>>2, b>>2);
				buf[j][0][i] = c;
			}   /* end of right to left scan */
		} else {    /* we're at the last scan line */
			j = (line+1) & 0x01;
			/* just convert last line, no dithering */
			for(i=0; i<width; i++) {
				r = CLIP(buf[j][0][i]);
				g = CLIP(buf[j][1][i]);
				b = CLIP(buf[j][2][i]);
				c = get_index(r>>2, g>>2, b>>2);
				buf[j][0][i] = c;
			}
		}
	}       /* end of if at beginning of new line */

	/* output appropriate pixel value from buf */

	c = buf[(line+1)&0x01][0][x];
	if(c>255 || c<0) {
		fprintf(stderr, "Oops, trying to pass of %d as a valid color @ %d %d.\n", c, x, y);
		c = CLIP(c);
	}
	return c;

}       /* end of get_pix_fs() */


/*
	map -- find the "best" mapping of an rgb value in the palette
*/

map(r, g, b)
	int     r, g, b;
{
	int     i, j, k;
	int     dr, dg, db, c;
	int     best;
	long    min_dist, dist;

	best = 0;
	i = red[best];
	j = grn[best];
	k = blu[best];
	dr = i - r;
	dg = j - g;
	db = k - b;
	min_dist = (long)dr*(long)dr + (long)dg*(long)dg + (long)db*(long)db;
	for(c=1; c<256; c++) {
		i = red[c];
		j = grn[c];
		k = blu[c];
		dr = i - r;
		dg = j - g;
		db = k - b;
		dist = (long)dr*(long)dr + (long)dg*(long)dg + (long)db*(long)db;
		if(dist<min_dist) {
			min_dist = dist;
			best = c;
		}
	}
	return best;
}

/*
	remap_all -- find the palette mapping for the whole tree
*/

remap_all()
{
	register int    r, g;
	unsigned long   colors;
	Node            *ptr;

	colors = total_colors;
	printf("Remapping tree to palette.\n        colors to go = ");
	for(r=0; r<RES; r++)
		for(g=0; g<RES; g++) {
			ptr = grid[r][g];
			while(ptr) {
				ptr->count = map(r, g, ptr->color);
				ptr = ptr->next;
				--colors;
				if((colors % (PRINT_STEP<<1)) == 0)
					printf("%8lu\b\b\b\b\b\b\b\b", colors);
			}
		}
	printf("%8lu\b\b\b\b\b\b\b\b", colors);
	printf("\n");
}       /* end of remap_all */

/*
	here and beyond there be dragons...
*/

/*****************************************************************************
 *
 * GIFENCODE.C    - GIF Image compression interface
 *
 * GIFEncode( FName, GHeight, GWidth, GInterlace, Background, 
 *	      BitsPerPixel, Red, Green, Blue, GetPixel )
 *
 *****************************************************************************/
 

/*
 * Pointer to function returning an int
 */
typedef int (* ifunptr)();

#define TRUE 1
#define FALSE 0

static int Width, Height;
static int curx, cury;
static long CountDown;
static int Pass = 0;
static int Interlace;

/*
 * Bump the 'curx' and 'cury' to point to the next pixel
 */
static
BumpPixel()
{
	/*
	 * Bump the current X position
	 */
	curx++;

	/*
	 * If we are at the end of a scan line, set curx back to the beginning
	 * If we are interlaced, bump the cury to the appropriate spot,
	 * otherwise, just increment it.
	 */
	if( curx == Width ) {
		curx = 0;

	        if( !Interlace ) 
			cury++;
		else {
		     switch( Pass ) {
	     
	               case 0:
        	          cury += 8;
                	  if( cury >= Height ) {
		  		Pass++;
				cury = 4;
		  	  }
                          break;
		  
	               case 1:
        	          cury += 8;
                	  if( cury >= Height ) {
		  		Pass++;
				cury = 2;
		  	  }
			  break;
		  
	               case 2:
	                  cury += 4;
	                  if( cury >= Height ) {
	                     Pass++;
	                     cury = 1;
	                  }
	                  break;
			  
	               case 3:
	                  cury += 2;
	                  break;
			}
		}
	}
}

/*
 * Return the next pixel from the image
 */
GIFNextPixel( getpixel )
ifunptr getpixel;
{
	int r;

	if( CountDown == 0 )
		return EOF;

	CountDown--;

	r = ( * getpixel )( curx, cury );

	BumpPixel();

	return r;
}

/* public */

GIFEncode( FName, GWidth, GHeight, GInterlace, Background, 
	   BitsPerPixel, Red, Green, Blue, GetPixel )
	 
char *FName;
int GWidth, GHeight;
int GInterlace;
int Background;
int BitsPerPixel;
int Red[], Green[], Blue[];
ifunptr GetPixel;

{
	FILE *fp;
	int B;
	int RWidth, RHeight;
	int LeftOfs, TopOfs;
	int Resolution;
	int ColorMapSize;
	int InitCodeSize;
	int i;

	Interlace = GInterlace;
	
	ColorMapSize = 1 << BitsPerPixel;
	
	RWidth = Width = GWidth;
	RHeight = Height = GHeight;
	LeftOfs = TopOfs = 0;
	
	Resolution = BitsPerPixel;

	/*
	 * Calculate number of bits we are expecting
	 */
	CountDown = (long)Width * (long)Height;

	/*
	 * Indicate which pass we are on (if interlace)
	 */
	Pass = 0;

	/*
	 * The initial code size
	 */
	if( BitsPerPixel <= 1 )
		InitCodeSize = 2;
	else
		InitCodeSize = BitsPerPixel;

	/*
	 * Set up the current x and y position
	 */
	curx = cury = 0;

	/*
	 * Open the GIF file for binary write
	 */
	fp = fopen( FName, "wb" );

	if( fp == (FILE *)0 ) {
		printf( "error: could not open output file\n" );
		exit(1);
	}

	/*
	 * Write the Magic header
	 */
	fwrite( "GIF87a", 1, 6, fp );

	/*
	 * Write out the screen width and height
	 */
	Putword( RWidth, fp );
	Putword( RHeight, fp );

	/*
	 * Indicate that there is a global colour map
	 */
	B = 0x80;	/* Yes, there is a color map */

	/*
	 * OR in the resolution
	 */
	B |= (Resolution - 1) << 5;

	/*
	 * OR in the Bits per Pixel
	 */
	B |= (BitsPerPixel - 1);

	/*
	 * Write it out
	 */
	fputc( B, fp );

	/*
	 * Write out the Background colour
	 */
	fputc( Background, fp );

	/*
	 * Byte of 0's (future expansion)
	 */
	fputc( 0, fp );

	/*
	 * Write out the Global Colour Map
	 */
     	for( i=0; i<ColorMapSize; i++ ) {
		fputc( Red[i], fp );
		fputc( Green[i], fp );
		fputc( Blue[i], fp );
	}

	/*
	 * Write an Image separator
	 */
	fputc( ',', fp );

	/*
	 * Write the Image header
	 */

	Putword( LeftOfs, fp );
	Putword( TopOfs, fp );
	Putword( Width, fp );
	Putword( Height, fp );

	/*
	 * Write out whether or not the image is interlaced
	 */
	if( Interlace )
		fputc( 0x40, fp );
	else
		fputc( 0x00, fp );

	/*
	 * Write out the initial code size
	 */
	fputc( InitCodeSize, fp );

	/*
	 * Go and actually compress the data
	 */
	compress( InitCodeSize+1, fp, GetPixel );

	/*
	 * Write out a Zero-length packet (to end the series)
	 */
	fputc( 0, fp );

	/*
	 * Write the GIF file terminator
	 */
	fputc( ';', fp );

	/*
	 * And close the file
	 */
	fclose(fp);
	
}

/*
 * Write out a word to the GIF file
 */
static
Putword( w, fp )
int w;
FILE *fp;
{
	fputc( w & 0xff, fp );
	fputc( (w / 256) & 0xff, fp );
}

/***************************************************************************
 *
 *  GIFENCOD.C       - GIF Image compression routines
 *
 *  Lempel-Ziv compression based on 'compress'.  GIF modifications by
 *  David Rowley (mgardi@watdcsu.waterloo.edu)
 *
 ***************************************************************************/

/*
 * General DEFINEs
 */

// #define min(a,b)        ((a>b) ? b : a)

#define BITS    12

// #define MSDOS   1

#define HSIZE  5003            /* 80% occupancy */

/*
 * Pointer to function returning an int
 */

// typedef int (* ifunptr)();

/*
 * a code_int must be able to hold 2**BITS values of type int, and also -1
 */
typedef int             code_int;

#ifdef SIGNED_COMPARE_SLOW
typedef unsigned long int count_int;
typedef unsigned short int count_short;
#else
typedef long int          count_int;
#endif

#ifdef NO_UCHAR
 typedef char   char_type;
#else
 typedef        unsigned char   char_type;
#endif /* UCHAR */

/*
 *
 * GIF Image compression - modified 'compress'
 *
 * Based on: compress.c - File compression ala IEEE Computer, June 1984.
 *
 * By Authors:  Spencer W. Thomas       (decvax!harpo!utah-cs!utah-gr!thomas)
 *              Jim McKie               (decvax!mcvax!jim)
 *              Steve Davies            (decvax!vax135!petsd!peora!srd)
 *              Ken Turkowski           (decvax!decwrl!turtlevax!ken)
 *              James A. Woods          (decvax!ihnp4!ames!jaw)
 *              Joe Orost               (decvax!vax135!petsd!joe)
 *
 */
#include <stdio.h>
#include <ctype.h>
#include <signal.h>

#define ARGVAL() (*++(*argv) || (--argc && *++argv))

static int n_bits;                        /* number of bits/code */
static int maxbits = BITS;                /* user settable max # bits/code */
static code_int maxcode;                  /* maximum code, given n_bits */
static code_int maxmaxcode = (code_int)1 << BITS; /* should NEVER generate this code */
#ifdef COMPATIBLE               /* But wrong! */
# define MAXCODE(n_bits)        ((code_int) 1 << (n_bits) - 1)
#else
# define MAXCODE(n_bits)        (((code_int) 1 << (n_bits)) - 1)
#endif /* COMPATIBLE */

static count_int htab [HSIZE];
static unsigned short codetab [HSIZE];
#define HashTabOf(i)       htab[i]
#define CodeTabOf(i)    codetab[i]

static code_int hsize = HSIZE;                 /* for dynamic table sizing */
static count_int fsize;

/*
 * To save much memory, we overlay the table used by compress() with those
 * used by decompress().  The tab_prefix table is the same size and type
 * as the codetab.  The tab_suffix table needs 2**BITS characters.  We
 * get this from the beginning of htab.  The output stack uses the rest
 * of htab, and contains characters.  There is plenty of room for any
 * possible stack (stack used to be 8000 characters).
 */

#define tab_prefixof(i) CodeTabOf(i)
#define tab_suffixof(i)        ((char_type *)(htab))[i]
#define de_stack               ((char_type *)&tab_suffixof((code_int)1<<BITS))

static code_int free_ent = 0;                  /* first unused entry */
static int exit_stat = 0;

/*
 * block compression parameters -- after all codes are used up,
 * and compression rate changes, start over.
 */
static int clear_flg = 0;

static int offset;
static long int in_count = 1;            /* length of input */
static long int out_count = 0;           /* # of codes output (for debugging) */

/*
 * compress stdin to stdout
 *
 * Algorithm:  use open addressing double hashing (no chaining) on the
 * prefix code / next character combination.  We do a variant of Knuth's
 * algorithm D (vol. 3, sec. 6.4) along with G. Knott's relatively-prime
 * secondary probe.  Here, the modular division first probe is gives way
 * to a faster exclusive-or manipulation.  Also do block compression with
 * an adaptive reset, whereby the code table is cleared when the compression
 * ratio decreases, but after the table fills.  The variable-length output
 * codes are re-sized at this point, and a special CLEAR code is generated
 * for the decompressor.  Late addition:  construct the table according to
 * file size for noticeable speed improvement on small files.  Please direct
 * questions about this implementation to ames!jaw.
 */

static int g_init_bits;
static FILE *g_outfile;

static int ClearCode;
static int EOFCode;

compress( init_bits, outfile, ReadValue )
int init_bits;
FILE *outfile;
ifunptr ReadValue;
{
    register long fcode;
    register code_int i = 0;
    register int c;
    register code_int ent;
    register code_int disp;
    register code_int hsize_reg;
    register int hshift;

    /*
     * Set up the globals:  g_init_bits - initial number of bits
     *                      g_outfile   - pointer to output file
     */
    g_init_bits = init_bits;
    g_outfile = outfile;

    /*
     * Set up the necessary values
     */
    offset = 0;
    out_count = 0;
    clear_flg = 0;
    in_count = 1;
    maxcode = MAXCODE(n_bits = g_init_bits);

    ClearCode = (1 << (init_bits - 1));
    EOFCode = ClearCode + 1;
    free_ent = ClearCode + 2;

    char_init();

    ent = GIFNextPixel( ReadValue );

    hshift = 0;
    for ( fcode = (long) hsize;  fcode < 65536L; fcode *= 2L )
	hshift++;
    hshift = 8 - hshift;                /* set hash code range bound */

    hsize_reg = hsize;
    cl_hash( (count_int) hsize_reg);            /* clear hash table */

    output( (code_int)ClearCode );

#ifdef SIGNED_COMPARE_SLOW
    while ( (c = GIFNextPixel( ReadValue )) != (unsigned) EOF ) {
#else
    while ( (c = GIFNextPixel( ReadValue )) != EOF ) {
#endif

	in_count++;

	fcode = (long) (((long) c << maxbits) + ent);
	i = (((code_int)c << hshift) ^ ent);    /* xor hashing */

	if ( HashTabOf (i) == fcode ) {
	    ent = CodeTabOf (i);
	    continue;
	} else if ( (long)HashTabOf (i) < 0 )      /* empty slot */
	    goto nomatch;
	disp = hsize_reg - i;           /* secondary hash (after G. Knott) */
	if ( i == 0 )
	    disp = 1;
probe:
	if ( (i -= disp) < 0 )
	    i += hsize_reg;

	if ( HashTabOf (i) == fcode ) {
	    ent = CodeTabOf (i);
	    continue;
	}
	if ( (long)HashTabOf (i) > 0 )
	    goto probe;
nomatch:
	output ( (code_int) ent );
	out_count++;
	ent = c;
#ifdef SIGNED_COMPARE_SLOW
	if ( (unsigned) free_ent < (unsigned) maxmaxcode) {
#else
	if ( free_ent < maxmaxcode ) {
#endif
	    CodeTabOf (i) = free_ent++; /* code -> hashtable */
	    HashTabOf (i) = fcode;
	} else
		cl_block();
    }
    /*
     * Put out the final code.
     */
    output( (code_int)ent );
    out_count++;
    output( (code_int) EOFCode );

    return;
}

/*****************************************************************
 * TAG( output )
 *
 * Output the given code.
 * Inputs:
 *      code:   A n_bits-bit integer.  If == -1, then EOF.  This assumes
 *              that n_bits =< (long)wordsize - 1.
 * Outputs:
 *      Outputs code to the file.
 * Assumptions:
 *      Chars are 8 bits long.
 * Algorithm:
 *      Maintain a BITS character long buffer (so that 8 codes will
 * fit in it exactly).  Use the VAX insv instruction to insert each
 * code in turn.  When the buffer fills up empty it and start over.
 */

static unsigned long cur_accum = 0;
static int  cur_bits = 0;

static
unsigned long masks[] = { 0x0000, 0x0001, 0x0003, 0x0007, 0x000F,
				  0x001F, 0x003F, 0x007F, 0x00FF,
				  0x01FF, 0x03FF, 0x07FF, 0x0FFF,
				  0x1FFF, 0x3FFF, 0x7FFF, 0xFFFF };

static
output( code )
code_int  code;
{
    cur_accum &= masks[ cur_bits ];

    if( cur_bits > 0 )
	cur_accum |= ((long)code << cur_bits);
    else
	cur_accum = code;

    cur_bits += n_bits;

    while( cur_bits >= 8 ) {
	char_out( (unsigned int)(cur_accum & 0xff) );
	cur_accum >>= 8;
	cur_bits -= 8;
    }

    /*
     * If the next entry is going to be too big for the code size,
     * then increase it, if possible.
     */
   if ( free_ent > maxcode || clear_flg ) {

	    if( clear_flg ) {

		maxcode = MAXCODE (n_bits = g_init_bits);
		clear_flg = 0;

	    } else {

		n_bits++;
		if ( n_bits == maxbits )
		    maxcode = maxmaxcode;
		else
		    maxcode = MAXCODE(n_bits);
	    }
	}

    if( code == EOFCode ) {
	/*
	 * At EOF, write the rest of the buffer.
	 */
	while( cur_bits > 0 ) {
		char_out( (unsigned int)(cur_accum & 0xff) );
		cur_accum >>= 8;
		cur_bits -= 8;
	}

	flush_char();

	fflush( g_outfile );

	if( ferror( g_outfile ) )
		writeerr();
    }
}

/*
 * Clear out the hash table
 */
static
cl_block ()             /* table clear for block compress */
{

	cl_hash ( (count_int) hsize );
	free_ent = ClearCode + 2;
	clear_flg = 1;

	output( (code_int)ClearCode );
}

static
cl_hash(hsize)          /* reset code table */
register count_int hsize;
{

	register count_int *htab_p = htab+hsize;

	register long i;
	register long m1 = -1;

	i = hsize - 16;
	do {                            /* might use Sys V memset(3) here */
		*(htab_p-16) = m1;
		*(htab_p-15) = m1;
		*(htab_p-14) = m1;
		*(htab_p-13) = m1;
		*(htab_p-12) = m1;
		*(htab_p-11) = m1;
		*(htab_p-10) = m1;
		*(htab_p-9) = m1;
		*(htab_p-8) = m1;
		*(htab_p-7) = m1;
		*(htab_p-6) = m1;
		*(htab_p-5) = m1;
		*(htab_p-4) = m1;
		*(htab_p-3) = m1;
		*(htab_p-2) = m1;
		*(htab_p-1) = m1;
		htab_p -= 16;
	} while ((i -= 16) >= 0);

	for ( i += 16; i > 0; i-- )
		*--htab_p = m1;
}

static
writeerr()
{
	printf( "error writing output file\n" );
	exit(1);
}

/******************************************************************************
 *
 * GIF Specific routines
 *
 ******************************************************************************/

/*
 * Number of characters so far in this 'packet'
 */
static int a_count;

/*
 * Set up the 'byte output' routine
 */
static
char_init()
{
	a_count = 0;
}

/*
 * Define the storage for the packet accumulator
 */
static char accum[ 256 ];

/*
 * Add a character to the end of the current packet, and if it is 254
 * characters, flush the packet to disk.
 */
static
char_out( c )
int c;
{
	accum[ a_count++ ] = c;
	if( a_count >= 254 )
		flush_char();
}

/*
 * Flush the packet to disk, and reset the accumulator
 */
static
flush_char()
{
	if( a_count > 0 ) {
		fputc( a_count, g_outfile );
		fwrite( accum, 1, a_count, g_outfile );
		a_count = 0;
	}
}

/* The End */

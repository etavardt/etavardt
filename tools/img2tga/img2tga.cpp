/*
�������������������������������������������
�                                                                         �
�                          IMG to TGA conversion                          �
�                                                                         �
�              IMG2TGA.C = IMG to TGA 24-bit color conversion             �
�       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     �
�                                                                         �
�������������������������������������������

	img2tga -- converts .img format files to 24 bit
		uncompressed Targa files.
*/
//#define MSDOS
//#ifdef MSDOS
	#define READ_MODE       "rb"
	#define WRITE_MODE      "wb"
//#else
//	#define READ_MODE       "r"
//	#define WRITE_MODE      "w"
//#endif

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
//#include <istream>
//#include <ostream>
#include <fstream>
#include <string>

using namespace std;

int main(int ac, char **av) {
	int     xres, yres;             /* image size */
	ifstream ifs;
	ofstream ofs;
	string infile;
	string outfile;
	int     i, x, y, total, count, red, grn, blu;

	if(ac != 2) {
		cout << "Usage: img2tga <file_root>" << endl;
		exit(0);
	}

	infile.append(av[1]);
	infile.append(".img");

	ifs.open(infile, ios::binary);
	if(!ifs.is_open()) {
		cerr << "Error opening file " << infile << " for input." << endl;
		exit(1);
	}

	outfile.append(av[1]);
	outfile.append(".tga");

	ofs.open(outfile, ios::binary);
	if(!ofs.is_open()) {
		cerr << "Error opening file " << outfile << " for output." << endl;
		exit(1);
	}

	/* Read .img header.  Get resolution and toss the rest. (TCE: img file is Big Endian)*/

	xres = ifs.get()<<8;
	xres += ifs.get();
	yres = ifs.get()<<8;
	yres += ifs.get();

	ifs.get();
	ifs.get();
	ifs.get();
	ifs.get();
	ifs.get();
	ifs.get();

	/* write .tga header */

	ofs.put(0);
	ofs.put(0);
	ofs.put(2);          /* type 2 targa file */
	for(i=3; i<12; i++) {
		ofs.put(0);
	}
	ofs.put(xres&0xff); /* TCE: Little Endian? */
	ofs.put(xres/256);
	ofs.put(yres&0xff);
	ofs.put(yres/256);
	ofs.put(24);         /* bits per pixel TCE: RGB 8 bits each for total of 24 */
	ofs.put(32);         /* image descriptor TCE:? */

	cout << "image size : " << xres << " x " << yres << endl;

	for(y=0; y<yres; y++) {         /* for each scan line */
		/* let user know we're awake */
		if(y%10 == 0) {
			cout << "\rLine: " << y << flush;
			//printf("%cLine: %4d", 13, y); /* TCE 13 ascii is carrage return */
		}
		total = xres;
		while(total>0) {   /* TCE: Weird it looks like the img file is bigger than the tga even though it appears to be run length encoded. */
			count = ifs.get();  /* TCE: img count starts at 10(0A) */
			total -= count;
			blu = ifs.get();  /* TCE: img color starts at 11(0B) */
			grn = ifs.get();
			red = ifs.get();

			//cout << "\n" << (xres - (total+count)) << ": RGB(" << red << ", " << grn << ", " << blu << ") : Run Count(" << count << ") : Total(" << total << ")";
			if (!ifs.good()) {
				ifs.close();
				ofs.close();
				cout << "exiting prematurly due to a bad char read" << endl;
				exit(1);
			}
			for(i=0; i<count; i++) {
				ofs.put(blu); /* TCE: tga color starts at 19 */
				ofs.put(grn);
				ofs.put(red);
			}
		}
	}

	/* wave goodbye */

	ifs.close();
	ofs.close();

	exit(0);

}       /* end of main() */

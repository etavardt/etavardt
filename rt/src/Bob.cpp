#include <cstring> // strtok
#include <iostream>
#include "Bob.hpp"
#include "Exception.hpp"
#include "screen.hpp"
#include "defs.hpp"
#include "extern.hpp"
#include "proto.hpp"

using std::cout;
using std::cerr;
using std::endl;

String Bob::_Program   = "Bob";
String Bob::_Version   = "Version 1.0";
String Bob::_Date      = "August 3, 1992";
String Bob::_Copyright = "Copyright 1989-1992 Christopher D. Watkins & Stephen B. Coy: Cpp upgrade by Thomas C. Etavard circa 2021";

Bob &Bob::getApp() {
    if (app == nullptr) {
        app = new Bob();
    }
    return static_cast<Bob&>( *app );
}

int  Bob::runApp() {
    stop_line = (-1);       /* helps to catch no studio error */
    if(preprocess) {
        preproc(infilename.data(), "yyz.b");
        ReadSceneFile(infilename.c_str(), String("yyz.b").data());
        /* remove("yyz.b"); */
    } else {
        ReadSceneFile(infilename.c_str(), infilename.data());
    }
	
    BuildBoundingSlabs();
    init_noise();
    Screen(&Eye, outfilename, Xresolution, Yresolution);

    return 1;
}


/*
    init_env() -- reads the environment and sets up the
        paths[] array.
*/

ArrayOfStrings Bob::paths;
//String Bob::paths[MAX_PATHS];
//int Bob::num_paths;

void Bob::init_env() {
    cerr <<"cerr: " << "In Bob::init_env" << endl;

    String path;
	String sub;

    //num_paths = 1;
    //paths[0] = "";          /* set first path to nothing */
    //paths.push_back("");          /* set first path to nothing */
	
	cerr << "cerr: " << "In Bob::init_env : Pre getenv" << endl;
    sub = (getenv(BOB_ENV) == NULL) ? "" : getenv(BOB_ENV);
    cerr << "cerr: " << "In Bob::init_env : Post getenv: Pre strdup: sub=" << sub << endl;
	if (sub == "") { throw Exception("Bob Environment variable was not set"); }
    cerr << "cerr: " << "In Bob::init_env : Post getenv: Pre strdup" << endl;
//    path = strdup(sub);             /* make a local copy to work with */
	path = eraseAllMatching(sub, "\"");
    cerr << "cerr: " << "In Bob::init_env : Post strdup: Pre strtok" << endl;

//TODO: TCE replace this segment utilizing strtok with a function that returns a std::array of strings
	paths = toArrayOfStrings(path, PATH_DELIM);
		cout << "Bob::paths.size(): " << Bob::paths.size() << endl;
	paths.insert(paths.begin(),""); /* current dir */
		cout << "Bob::paths.size(): " << Bob::paths.size() << endl;
	/*
    sub = strtok(path.data(), DELIMS);

    cerr << "cerr: " << "In Bob::init_env : Post strtok: Pre while: sub=" << sub << endl;
    while((sub != "") && num_paths < MAX_PATHS) {
        cerr << "cerr: " << "In Bob::init_env while : Pre strdup" << endl;
//        paths[num_paths] = strdup(sub.c_str());
        paths[num_paths] = sub;
        cerr << "cerr: " << "In Bob::init_env while : Post strdup : Pre strtok" << endl;
        num_paths++;
        sub = (strtok(NULL, DELIMS) == NULL) ? "" : strtok(NULL, DELIMS);
        cerr << "cerr: " << "In Bob::init_env while : Post strtok" << endl;
    }
    cerr << "cerr: " << "In Bob::init_env : Post while" << endl;
	*/
}       /* end of init_env() */


int Bob::processCmdLine(int argCnt, char **argList) {
    cerr << "cerr: " << "In Bob::processCmdLine : Pre App::processCmdLine" << endl;
    App::processCmdLine(argCnt, argList);
    cerr << "cerr: " << "In Bob::processCmdLine : Post App::processCmdLine" << endl;

    int    i;
    int     xres = (-1), yres = (-1), depth = (-1), amode = (-1),
        start = (-1), stop = (-1), bunch = (-1);
//    char    infilename[80];
//    char    outfilename[80];

    cerr << "cerr: " << "In Bob::processCmdLine : Pre init_env" << endl;
    init_env();     /* init environment paths */
    cerr << "cerr: " << "In Bob::processCmdLine : Post init_env" << endl;

    /* clip path off of program name */
    String arg0 = argList[0];
    size_t pos = arg0.rfind('\\');
    if (pos == String::npos) {
//        Progname = arg0.data();
        Progname = arg0;
    } else {
//        Progname = (arg0.substr(0,pos+1)).data();
        Progname = arg0.substr(0,pos+1);
    }
    /* clip path off of program name * /
#define rindex     strrchr
    if((Progname = rindex(argList[0], '\\')) == NULL)
        Progname = argList[0];
    else 
        Progname++;
*/
//TODO: TCE implement To Lowercase?    strlwr(Progname);

    /* init global clips before parser is called */
    GlobalClipTop = (GlobalClip *)vmalloc(sizeof(GlobalClip));
    ptrchk(GlobalClipTop, "global clip structure");
    GlobalClipTop->next = NULL;
    GlobalClipTop->clip = NULL;

    infilename[0] = 0;        /* to be safe */

    tickflag = 1;                   /* default to full stats */
    resume = 0;
    for(i=1; i<argCnt; i++) {        /* loop through command line args */
        if(argList[i][0] == '-') {
            switch(argList[i][1]) {
                case 'i':       /* set resolution */
                    i++;
                    xres = atoi(argList[i]);
                    i++;
                    yres = atoi(argList[i]);
                    break;
                case 'l':       /* set start & stop line */
                    i++;
                    start = atoi(argList[i]);
                    i++;
                    stop = atoi(argList[i]);
                    break;
                case 'd':       /* set max recursion depth */
                    i++;
                    depth = atoi(argList[i]);
                    break;
                case 'b':       /* set bunching factor */
                    i++;
                    bunch = atoi(argList[i]);
                    break;
                case 'a':
                    i++;
                    switch(argList[i][0]) {
                        case 'n' : amode = A_NONE; break;
                        case 'c' : amode = A_CORNERS; break;
                        case 'q' : amode = A_QUICK; break;
                        case 'a' : amode = A_ADAPTIVE; break;
                        default :
                            fprintf(stderr, "Error, unkown antialiasing mode %s.\n", argList[i]);
                            usage();
                            break;
                    }
                    break;
                case 'n':
                    no_shadows = 1;
                    break;
                case 'r':
                    resume = 1;
                    break;
                case 's':
                    tickflag = 0;
                    break;
                case 'p':
                    preprocess = 0;
                    break;
                case '?':
                    usage();    /* no return */
                    break;
                default:
                    fprintf(stderr, "Error, unkown command line flag %s.\n", argList[i]);
                    usage();
                    break;
            }    /* end of switch */
        } else {
            infilename = argList[i];
//            strcpy(infilename, argList[i]);
        }
    }
    if(infilename[0] == 0)        /* no file name given */
        usage();    /* no return */

    outfilename = infilename;
    infilename.append(".b");
    outfilename.append(".img");
//    strcpy(outfilename, infilename);
//    strcat(infilename, ".b");
//    strcat(outfilename, ".img");

    camera.aperture = -1.0;
    camera.focal_length = 0.0;
    Eye.view_aspect = -1.0;

    if(tickflag) {
//        fprintf(stderr, "%s    %s\n%s\n", _Program, _Version, _Copyright);
        cerr << _Program << "    " << _Version << endl << _Copyright << endl;
    }


    if(Eye.view_aspect != -1.0) {
        Eye.view_angle_y = Eye.view_angle_x / Eye.view_aspect;
    }
    if(xres > 0) {          /* if command line override */
        Xresolution = xres;
        if(stop_line == Yresolution)    /* only change if not default */
            stop_line = yres;
        Yresolution = yres;
    }
    if(start != -1) {       /* if command line override */
        start_line = start;
        stop_line = stop;
    }
    if(depth > 0) {         /* if command line override */
        maxlevel = depth;
    }
    if(bunch > 0) {         /* if command line override */
        bunching = bunch;
    }
    if(amode >= 0) {        /* if command line override */
        antialias = amode;
    }


    return 1;
}

void Bob::usage() {
    cerr << _Program << "        " << _Version << "        " << _Date << endl << _Copyright << endl << endl;
    cerr << "Usage:  " << Progname << " [flags] <file>" << endl;
    cerr << "       -s set silent mode" << endl;
    cerr << "       -r resume generating an interrupted image." << endl;
    cerr << "       -i change image size to xres by yres." << endl;
    cerr << "       -l change start and stop line." << endl;
    cerr << "       -n no shadows" << endl;
    cerr << "       -d limit recursion level to depth." << endl;
    cerr << "       -a change antialiasing to mode.  Valid modes are:" << endl;
    cerr << "          none, corners, quick, and adaptive." << endl;
    cerr << "       -p run without the preprocessor." << endl;
    cerr << "       -b set bunching factor." << endl;
    cerr << "Assumes file.b for input file and file.img for output file." << endl;

    // fprintf(stderr, "%s        %s        %s\n%s\n\n", _Program, _Version, _Date, _Copyright);
    // fprintf(stderr, "Usage:  %s [flags] <file>\n", Progname);
    // fprintf(stderr, "       -s set silent mode\n");
    // fprintf(stderr, "       -r resume generating an interrupted image.\n");
    // fprintf(stderr, "       -i change image size to xres by yres.\n");
    // fprintf(stderr, "       -l change start and stop line.\n");
    // fprintf(stderr, "       -n no shadows\n");
    // fprintf(stderr, "       -d limit recursion level to depth.\n");
    // fprintf(stderr, "       -a change antialiasing to mode.  Valid modes are:\n");
    // fprintf(stderr, "          none, corners, quick, and adaptive.\n");
    // fprintf(stderr, "       -p run without the preprocessor.\n");
    // fprintf(stderr, "       -b set bunching factor.\n");
    // fprintf(stderr, "Assumes file.b for input file and file.img for output file.\n");

    exit(0);
}

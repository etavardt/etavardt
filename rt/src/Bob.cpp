#include "Bob.hpp"
#include "Exception.hpp"
#include "Stats.hpp"
#include "defs.hpp"
#include "extern.hpp"
#include "proto.hpp"
#include "Screen_3D.hpp"
#include <filesystem>
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;

String Bob::_Program = "Bob";
String Bob::_Version = "Version 1.0";
String Bob::_Date = "August 3, 1992";
String Bob::_Copyright = "Copyright 1989-1992 Christopher D. Watkins & Stephen B. Coy: Cpp upgrade by Thomas C. Etavard circa 2021";

Bob &Bob::getApp() {
    if (app == nullptr) {
        app = new Bob();
    }
    return static_cast<Bob &>(*app);
}

int Bob::runApp() {
    stop_line = (-1); /* helps to catch no studio error */
                      //    cout << "cout: In Bob::runApp preprocess=" << preprocess << endl;
    if (preprocess) {
        preproc(infilename, "yyz.b");
        parser.ReadSceneFile(infilename, String("yyz.b"));
        /* remove("yyz.b"); */
    } else {
        parser.ReadSceneFile(infilename, infilename);
    }
    //    cout << "cout: In Bob::runApp Pre BuildBoundingSlabs" << endl;
    BuildBoundingSlabs();
    //    cout << "cout: In Bob::runApp Post BuildBoundingSlabs Pre init_noise" << endl;
    init_noise();
    //    cout << "cout: In Bob::runApp Post init_noise Pre Screen" << endl;
    Screen_3D scr;
    scr.screen(&Eye, outfilename, Xresolution, Yresolution);
    //    cout << "cout: In Bob::runApp Post Screen return to caller" << endl;

    return 1;
}

/*
    init_env() -- reads the environment and sets up the
        paths[] array.
*/

ArrayOfStrings Bob::paths;

void Bob::init_env() {
    //    cout <<"cout: " << "In Bob::init_env" << endl;

    String path;
    String sub;

    //	cout << "cout: " << "In Bob::init_env : Pre getenv" << endl;
    sub = (getenv(BOB_ENV) == NULL) ? "" : getenv(BOB_ENV);
    //    cout << "cout: " << "In Bob::init_env : Post getenv: Pre strdup: sub=" << sub << endl;
    if (sub == "") {
        // setup Bob Env Paths
        std::filesystem::path cur_path = std::filesystem::current_path();
        cout << "cur_path: " << cur_path << endl;

        // if (std::filesystem::exists(path))
        paths.push_back("");
        paths.push_back(".");
        paths.push_back(".\\scns");
        paths.push_back(".\\objs");
        paths.push_back(".\\maps");
        paths.push_back(".\\bkgs");
        paths.push_back(".\\cols");
        paths.push_back(".\\srfs");
        // throw ExceptionBob Environment variable was not set");
    } else {
        //    cout << "cout: " << "In Bob::init_env : Post getenv: Pre eraseAllMatching" << endl;
        /* make a local copy to work with */
        path = eraseAllMatching(sub, "\"");
        //    cout << "cout: " << "In Bob::init_env : Post eraseAllMatching: Pre toArrayOfStrings" << endl;

        paths = toArrayOfStrings(path, PATH_DELIM);
        // cout << "Bob::paths.size(): " << Bob::paths.size() << endl;
        paths.insert(paths.begin(), ""); /* current dir */
    }
    // cout << "Bob::paths.size(): " << Bob::paths.size() << endl;

} /* end of init_env() */

int Bob::processCmdLine(int argCnt, char **argList) {
    // cout << "cout: " << "In Bob::processCmdLine : Pre App::processCmdLine" << endl;
    App::processCmdLine(argCnt, argList);
    // cout << "cout: " << "In Bob::processCmdLine : Post App::processCmdLine" << endl;

    int i;
    int xres = (-1), yres = (-1), depth = (-1), amode = (-1), start = (-1), stop = (-1), bunch = (-1);

    // cout << "cout: " << "In Bob::processCmdLine : Pre init_env" << endl;
    init_env(); /* init environment paths */
    // cout << "cout: " << "In Bob::processCmdLine : Post init_env" << endl;

    /* clip path off of program name */
    String arg0 = argList[0];
    size_t pos = arg0.rfind('\\');
    if (pos == String::npos) {
        Progname = arg0;
    } else {
        Progname = arg0.substr(0, pos + 1);
    }

    // TODO: TCE implement To Lowercase?    strlwr(Progname);

    /* init global clips before parser is called */
    GlobalClipTop = new GlobalClip();
    Stats::trackMemoryUsage(sizeof(GlobalClip));
    parser.ptrchk(GlobalClipTop, "global clip structure");
    GlobalClipTop->next = NULL;
    GlobalClipTop->clip = NULL;

    //    infilename[0] = 0; /* to be safe */
    infilename = ""; /* to be safe */

    tickflag = 1; /* default to full stats */
    resume = 0;
    for (i = 1; i < argCnt; i++) { /* loop through command line args */
        if (argList[i][0] == '-') {
            switch (argList[i][1]) {
            case 'i': /* set resolution */
                i++;
                xres = atoi(argList[i]);
                i++;
                yres = atoi(argList[i]);
                break;
            case 'l': /* set start & stop line */
                i++;
                start = atoi(argList[i]);
                i++;
                stop = atoi(argList[i]);
                break;
            case 'd': /* set max recursion depth */
                i++;
                depth = atoi(argList[i]);
                break;
            case 'b': /* set bunching factor */
                i++;
                bunch = atoi(argList[i]);
                break;
            case 'a':
                i++;
                switch (argList[i][0]) {
                case 'n':
                    amode = A_NONE;
                    break;
                case 'c':
                    amode = A_CORNERS;
                    break;
                case 'q':
                    amode = A_QUICK;
                    break;
                case 'a':
                    amode = A_ADAPTIVE;
                    break;
                default:
                    cerr << "Error, unkown antialiasing mode " << argList[i] << "." << endl;
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
                preprocess = false;
                break;
            case '?':
                usage(); /* no return */
                break;
            default:
                cerr << "Error, unkown command line flag " << argList[i] << "." << endl;
                usage();
                break;
            } /* end of switch */
        } else {
            infilename = argList[i];
        }
    }
    if (infilename.empty()) { /* no file name given */
        infilename = "venus3";
        amode = A_QUICK;
        xres = 80;
        yres = 50;
        //usage();            /* no return */
    }
    outfilename = infilename;
    infilename.append(".b");
    outfilename.append(".img");

    camera.aperture = -1.0;
    camera.focal_length = 0.0;
    Eye.view_aspect = -1.0;

    if (tickflag) {
        cout << _Program << "    " << _Version << endl << _Copyright << endl;
    }

    if (Eye.view_aspect != -1.0) {
        Eye.view_angle_y = Eye.view_angle_x / Eye.view_aspect;
    }
    if (xres > 0) { /* if command line override */
        Xresolution = xres;
        if (stop_line == Yresolution) /* only change if not default */
            stop_line = yres;
        Yresolution = yres;
    }
    if (start != -1) { /* if command line override */
        start_line = start;
        stop_line = stop;
    }
    if (depth > 0) { /* if command line override */
        maxlevel = depth;
    }
    if (bunch > 0) { /* if command line override */
        bunching = bunch;
    }
    if (amode >= 0) { /* if command line override */
        antialias = amode;
    }

    return 1;
}

void Bob::usage() {
    cout << _Program << "        " << _Version << "        " << _Date << endl << _Copyright << endl << endl;
    cout << "Usage:  " << Progname << " [flags] <file>" << endl;
    cout << "       -s set silent mode" << endl;
    cout << "       -r resume generating an interrupted image." << endl;
    cout << "       -i change image size to xres by yres." << endl;
    cout << "       -l change start and stop line." << endl;
    cout << "       -n no shadows" << endl;
    cout << "       -d limit recursion level to depth." << endl;
    cout << "       -a change antialiasing to mode.  Valid modes are:" << endl;
    cout << "          none, corners, quick, and adaptive." << endl;
    cout << "       -p run without the preprocessor." << endl;
    cout << "       -b set bunching factor." << endl;
    cout << "Assumes file.b for input file and file.img for output file." << endl;

    throw Exception("");
}

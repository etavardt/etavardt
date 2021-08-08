#include "Bob.hpp"

#include <filesystem>
#include <iostream>
#include <memory>

#include "Preprocessor.hpp"
#include "Parser.hpp"
#include "Bound_3D.hpp"
#include "Clip_3D.hpp"
#include "Noise.hpp"
#include "RayTrace_3D.hpp"
#include "Screen_3D.hpp"
#include "Stats.hpp"
#include "Exception.hpp"

#include "extern.hpp"

using std::cerr;
using std::cout;
using std::endl;

double Bob::rayeps = 1e-6; // the smallest allowable ray size, color value, or double

String Bob::_Program = "Bob";
String Bob::_Version = "Version 1.0";
String Bob::_Date = "August 3, 1992";
String Bob::_Copyright = "Copyright 1989-1992 Christopher D. Watkins & Stephen B. Coy: Cpp upgrade by Thomas C. Etavard circa 2021";

extern int tickflag;

const Bob &bobApp = Bob::getApp();

Bob &Bob::getApp() {
    static Bob instance;
    app = &instance;
    return instance;
}

ArrayOfStrings Bob::paths;
/*
    init_env() -- reads the environment and sets up the
        paths[] array.
*/
void Bob::init_env() const {
    //    cout <<"cout: " << "In Bob::init_env" << endl;

    String path;
    String sub;

    //	cout << "cout: " << "In Bob::init_env : Pre getenv" << endl;
    sub = (getenv(BOB_ENV) == nullptr) ? "" : getenv(BOB_ENV);
    //    cout << "cout: " << "In Bob::init_env : Post getenv: Pre strdup: sub=" << sub << endl;
    if (sub == "") {
        // setup Bob Env Paths
        std::filesystem::path cur_path = std::filesystem::current_path();
        cout << "cur_path: " << cur_path << endl;

        // if (std::filesystem::exists(path))
        paths.emplace_back("");
        paths.emplace_back(".");
        paths.emplace_back(".\\scns");
        paths.emplace_back(".\\objs");
        paths.emplace_back(".\\maps");
        paths.emplace_back(".\\bkgs");
        paths.emplace_back(".\\cols");
        paths.emplace_back(".\\srfs");
        // throw ExceptionBob Environment variable was not set");
    } else {
        //    cout << "cout: " << "In Bob::init_env : Post getenv: Pre eraseAllMatching" << endl;
        /* make a local copy to work with */
        path = eraseAllMatching(sub, "\"");
        //    cout << "cout: " << "In Bob::init_env : Post eraseAllMatching: Pre toArrayOfStrings" << endl;

        paths = toArrayOfStrings(path, PATH_DELIM);
        // cout << "Bob::paths.size(): " << Bob::paths.size() << endl;
        paths.emplace(paths.begin(), ""); /* current dir */
    }
    // cout << "Bob::paths.size(): " << Bob::paths.size() << endl;

} /* end of init_env() */

int Bob::processCmdLine(int argCnt, char **argList) {
    // cout << "cout: " << "In Bob::processCmdLine : Pre App::processCmdLine" << endl;
    App::processCmdLine(argCnt, argList);
    // cout << "cout: " << "In Bob::processCmdLine : Post App::processCmdLine" << endl;

    // cout << "cout: " << "In Bob::processCmdLine : Pre init_env" << endl;
    init_env(); /* init environment paths */
    // cout << "cout: " << "In Bob::processCmdLine : Post init_env" << endl;

    /* clip path off of program name */
    String arg0 = argList[0];
    size_t pos = arg0.rfind('\\');
    if (pos == String::npos) {
        progname = arg0;
    } else {
        progname = arg0.substr(0, pos + 1);
    }

    /* init global clips before parser is called */
    GlobalClip::GlobalClipTop = std::make_shared<GlobalClip>();
    Stats::trackMemoryUsage(sizeof(GlobalClip));
    //parser.ptrchk(GlobalClip::GlobalClipTop, "global clip structure");
    GlobalClip::GlobalClipTop->next = nullptr;
    GlobalClip::GlobalClipTop->clip = nullptr;

    infilename = ""; /* to be safe */

    tickflag = 1; /* default to full stats */
    for (int i = 1; i < argCnt; i++) { /* loop through command line args */
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
                resume = true;
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
        infilename = "venus3"; //"crypt1";//"venus3";
        //usage();            /* no return */
    }
    outfilename = infilename;
    infilename.append(".b");
    outfilename.append(".img");

    if (tickflag) {
        cout << _Program << "    " << _Version << endl << _Copyright << endl;
    }

    return 1;
}

void Bob::usage() const {
    cout << _Program << "        " << _Version << "        " << _Date << endl << _Copyright << endl << endl;
    cout << "Usage:  " << progname << " [flags] <file>" << endl;
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

void Bob::clearScreen() {
#ifdef __APPLE__
    system("clear");
#else
    // This is reported to work in Windows and Linix
    cout << "\033[2J\033[1;1H";
    cout.flush();
#endif
}

int Bob::runApp() {
    Parser parser;
    stop_line = (-1); /* helps to catch no studio error */
                      //    cout << "cout: In Bob::runApp preprocess=" << preprocess << endl;
    if (preprocess) {
        Preprocessor::preproc(infilename, "yyz.b");
        parser.ReadSceneFile(infilename, String("yyz.b"));
        /* remove("yyz.b"); */
    } else {
        parser.ReadSceneFile(infilename, infilename);
    }
    if (camera.eye.view_aspect != -1.0) {
        camera.eye.view_angle_y = camera.eye.view_angle_x / camera.eye.view_aspect;
    }
    if (xres > 0) { /* if command line override */
        Parser::xResolution = xres;
        if (stop_line == Parser::yResolution) /* only change if not default */
            stop_line = yres;
        Parser::yResolution = yres;
    }
    if (start != -1) { /* if command line override */
        start_line = start;
        stop_line = stop;
    }
    if (depth > 0) { /* if command line override */
        RayTrace_3D::maxlevel = depth;
    }
    if (bunch > 0) { /* if command line override */
        bunching = bunch;
    }
    if (amode >= 0) { /* if command line override */
        antialias = amode;
    }

    //    cout << "cout: In Bob::runApp Pre BuildBoundingSlabs" << endl;
    Bound_3D::BuildBoundingSlabs();
    //    cout << "cout: In Bob::runApp Post BuildBoundingSlabs Pre init_noise" << endl;
    Noise::init_noise();
    //    cout << "cout: In Bob::runApp Post init_noise Pre Screen" << endl;
    camera.screen.render(outfilename, Parser::xResolution, Parser::Parser::yResolution);
    //    cout << "cout: In Bob::runApp Post Screen return to caller" << endl;

    return 1;
}

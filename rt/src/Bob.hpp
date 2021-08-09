#pragma once

#include "App.hpp"
#include "String.hpp"
#include "Camera_3D.hpp"
#include "PicFile_3D.hpp"

const String BOB_ENV = "BOB";
const char PATH_DELIM = ';';
#define MAX_PATHS (32)

class App;

class Bob : public App {
    using App::App;

  public:
    static String _Program;
    static String _Version;
    static String _Date;
    static String _Copyright;
    static ArrayOfStrings paths;

    String progname;
    String infilename;
    String outfilename;

    bool resume     = false;
    bool preprocess = true;
    int  start_line = 0;
    int  stop_line  = 0;

    Camera_3D camera; // Bob has a camera
    PicFile_3D picFile; // Bob has a Pic File

    static double rayeps; // the smallest allowable ray size, color value, or double

    Bob(Bob const&) = delete;
    void operator=(Bob const&) = delete;

    int runApp() override;
    int processCmdLine(int argCnt, char **argList) override;

    static void clearScreen();
    static Bob &getApp();

    void usage() const;

  private:
    int xres  = -1;
    int yres  = -1;
    int depth = -1;
    int amode = -1;
    int start = -1;
    int stop  = -1;
    int bunch = -1;

    void init_env() const;
};

#pragma once

#include "App.hpp"
#include "String.hpp"
#include "Camera_3D.hpp"

//class Parser;

#define BOB_ENV "BOB"
#define PATH_DELIM ';'
//#define DELIMS          ";"
#define MAX_PATHS (32)

class App;

class Bob : public App {
  private:
    int xres = (-1), yres = (-1), depth = (-1), amode = (-1), start = (-1), stop = (-1), bunch = (-1);

  protected:
    Bob() : App() {}
    ~Bob() { App::app = nullptr; }

    void init_env();

  public:
    static Bob &getApp();
    static void clearScreen();
    int runApp();
    int processCmdLine(int argCnt, char **argList);

    void usage();

    String Progname;
    static String _Program, _Version, _Date, _Copyright;

    static ArrayOfStrings paths;

    String infilename = "";
    String outfilename = "";

    bool preprocess = true;

    Camera_3D camera; // Bob has a camera

    static double rayeps; // the smallest allowable ray size, color value, or double
};

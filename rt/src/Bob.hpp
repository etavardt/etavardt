#pragma once

#include "App.hpp"
#include "String.hpp"
#include "Camera_3D.hpp"
#include "PicFile_3D.hpp"

#define BOB_ENV "BOB"
#define PATH_DELIM ';'
#define MAX_PATHS (32)

class App;

class Bob : public App {
  private:
    int xres = (-1), yres = (-1), depth = (-1), amode = (-1), start = (-1), stop = (-1), bunch = (-1);

  protected:
    Bob() : App() {}
    Bob(Bob const&) = delete;
    void operator=(Bob const&) = delete;
    // ~Bob() { App::app = nullptr; }

    void init_env() const;

  public:
    int runApp() override;
    int processCmdLine(int argCnt, char **argList) override;

    static void clearScreen();
    static Bob &getApp();

    void usage() const;

    static String _Program, _Version, _Date, _Copyright;
    static ArrayOfStrings paths;

    String progname = "";
    String infilename = "";
    String outfilename = "";

    bool resume = false;
    bool preprocess = true;
    int  start_line = 0;
    int  stop_line  = 0;

    Camera_3D camera; // Bob has a camera
    PicFile_3D picFile; // Bob has a Pic File

    static double rayeps; // the smallest allowable ray size, color value, or double
};

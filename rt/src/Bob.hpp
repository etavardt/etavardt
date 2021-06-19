#pragma once

#include "App.hpp"
#include "String.hpp"
#include "Parser.hpp"

#define BOB_ENV         "BOB"
#define PATH_DELIM      ';'
//#define DELIMS          ";"
#define MAX_PATHS       (32)

class Bob:public App {

    protected:
    Bob():App() {}
    ~Bob() { if (App::app != nullptr) { delete App::app; } }

    void init_env();

    public:
    static Bob &getApp();
    int runApp();
    int processCmdLine(int argCnt, char **argList);

    void usage();

    String Progname;
    static String _Program, _Version, _Date, _Copyright;

    static ArrayOfStrings paths;

    String infilename  = "";
    String outfilename = "";

    int preprocess = 1;

    Parser parser;
};

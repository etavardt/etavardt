#pragma once

class App {
    protected:
    App() : argCnt(0), argList(nullptr) {} //Default constructor hidden to make it a singleton

    private:
    int argCnt;
    char **argList;

    public:
    static App *app;

    static App &getApp();
    virtual int runApp() = 0;
    virtual int processCmdLine(int argCnt, char **argList) {this->argCnt = argCnt; this->argList = argList; return 1;}
};

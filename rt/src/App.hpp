#pragma once

//template <class T>
class App {
    protected:
    App(){} //Default constructor hidden to make it a singleton

    private:
    int argCnt;
    char **argList;

    public:
    static App *app;
/*
    static T &app;
    template <class T>
    static T& getApp() 
    {
       return T::getApp();
    }
*/
    static App &getApp();
    virtual int runApp() = 0;
    virtual int processCmdLine(int argCnt, char **argList) {this->argCnt = argCnt; this->argList = argList; return 1;}
};



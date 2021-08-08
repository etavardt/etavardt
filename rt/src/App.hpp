#pragma once

class App {
    protected:
    App() = default; //Default constructor hidden to make it a singleton
    virtual ~App() = default;

    private:
    int argCnt = 0;
    char **argList = nullptr;

    public:
    static App *app;

    static App &getApp();
    virtual int runApp() = 0;
    virtual int processCmdLine(int _argCnt, char **_argList) { argCnt = _argCnt; argList = _argList; return 1; }
};

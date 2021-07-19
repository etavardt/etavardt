/***************************************************************************
�������������������������������������������
�                                                                         �
�                              Bob Ray Tracer                             �
�                                                                         �
�                        Main.C = in the beginning...                     �
�                                                                         �
�       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     �
�                                                                         �
�       ALL RIGHTS RESERVED.   This software is published, but is NOT     �
�         Public Domain and remains the propery of ALGORITHM, Inc.,       �
�   Christopher D. Watkins and Stephen B. Coy.  This software may not be  �
�  reproduced or integrated into other packages without the prior written �
�          consent of Christopher D. Watkins and Stephen B. Coy.          �
�                                                                         �
�                     Requires : defs.h, extern.h                         �
�                                                                         �
�������������������������������������������
*/

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include "Bob.hpp"
#include "Exception.hpp"

using std::cout;
using std::cerr;
using std::endl;

int main(int ac, char **av) {
    try {
        // cout << "cout: In main: Pre getApp : " << App::app << endl;
        Bob &bobApp = Bob::getApp();
        // cout << "cout: In main: Post getApp Pre processCmdLine : " << &bobApp << ":" << App::app << endl;
        bobApp.processCmdLine(ac, av);
        // cout << "cout: In main: Post processCmdLine Pre runApp" << endl;
        bobApp.runApp();
        // cout << "cout: In main: Post runApp" << endl;
    } catch (Exception e) {
        cerr << "cerr: Exception caught in main: " << e.what() << endl;
        exit(1);
    } catch (std::exception e) {
        cerr << "cerr: Exception caught in main: " << e.what() << endl;
        exit(1);
    } catch (...) {
        cerr << "cerr: Unknown Exception caught in main" << endl;
        exit(1);
    }
    // exit(0);
    cout << "Returning 0 from main()" << endl;
    return 0;
} /* end of main() */

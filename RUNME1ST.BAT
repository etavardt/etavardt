echo off
echo .   Setup file for BOB Ray Tracer and Tools
echo .   Usage is :   RUNME1ST C:
echo .   where C: is the drive on which you installed the software
echo .
echo .   Use CNTR-C to stop and enter installation drive
echo .   or ENTER to continue with initialization of BOB
pause
cls
echo . Quiet - Rational Systems Credit
set DOS4G=quiet
echo .
echo . Set BOB Environment Parameters
set bob=%1\bob\rt;%1\bob\rt\scns;%1\bob\rt\objs;%1\bob\rt\maps;%1\bob\rt\bkgs;%1\bob\rt\cols;%1\bob\rt\srfs;
echo .
echo . Set BOB Path Parameters
path=%1\bob;%1\bob\sample;%1\bob\proc;%1\bob\proczbuf;%1\bob\modeler;%1\bob\rt;\bob\tools;

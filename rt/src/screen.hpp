#pragma once
#include "String.hpp"
#include "defs.hpp"
#include "struct_defs.hpp"
extern void Screen (Viewpoint *view , String &picfile , int xres , int yres);
extern void ScrInit (Viewpoint *view , int xres , int yres , String &picfile);
extern void Scan0 (void);
extern void Scan1 (void);
extern void Scan2 (void);
extern void Scan3 (void);
extern void Scan4 (void);
extern void Adapt (int i , int j , Flt x , Flt y , Color color , int step);
extern void Shoot (Flt x , Flt y , Color color);

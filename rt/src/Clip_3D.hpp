#pragma once

#include <memory>
#include "defs.hpp"
#include "Vector_3D.hpp"
//#include "Object_3D.hpp"
class Object_3D;

class Clip_3D {
    public:
    Vec center,
        normal,
        apex, base;
    double radius1,
        radius2,
        length;
    int type;
    std::shared_ptr<Clip_3D> next;

    int clip_check(Vec &P);
    static void bound_opt(Object_3D *obj);

    static std::shared_ptr<Clip_3D> ClipTop; /* current clipping list */
};

typedef Clip_3D Clip;

struct GlobalClip {
    std::shared_ptr<Clip> clip;
    GlobalClip    *next;
    static GlobalClip *GlobalClipTop; /* current global clip list */
};

extern void init_noise (void);
extern double  noise1 (const Vec &p);
extern void noise3 (const Vec &p , double *v);
extern double  turb1 (Vec &p , int lvl);
extern void turb3 (Vec &p , Vec &v , int lvl);

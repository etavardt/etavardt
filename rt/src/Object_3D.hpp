#pragma once

#include <memory>

#include "defs.hpp"
#include "Surface_3D.hpp"
//#include "Clip_3D.hpp"
#include "Ray_3D.hpp"
#include "Isect_3D.hpp"

class Clip_3D; // Use this to avoid circluar processing which causes error: 'Object_3D' has not been declared because Clip_3D.hpp includes Object_3D.hpp
//class Surface_3D;
//class Isect;
class Object_3D {
    public:
    Object_3D() : o_type(0), o_surf(nullptr), o_data(nullptr), next(nullptr) {
        for (int i = 0; i < NSLABS; i++) {
            o_dmin[i] = 0.0;
            o_dmax[i] = 0.0;
        }
    } //default Constructor
    ~Object_3D();

    unsigned short o_type;

    double o_dmin[NSLABS];
    double o_dmax[NSLABS];

    Surface_3D *o_surf;
    void       *o_data;
    std::shared_ptr<Clip_3D> clips;

    Object_3D *next;          /* next object in original list, sibling */

    virtual int  intersect(Ray *b, Isect &c);
    virtual void normal(Isect &hit, Point &P, Vec &N);
    static Object_3D *Root;
    static Vec   Slab[];
};

typedef Object_3D Object;

typedef struct t_compositedata {
    unsigned long   size;
    Object_3D          *children;
} CompositeData;


//#endif //Object_3D_HPP

#pragma once

#include "defs.hpp"
#include "Surface_3D.hpp"
#include "Clip_3D.hpp"
#include "Ray_3D.hpp"
#include "Isect_3D.hpp"
//#include "struct_defs.hpp"
//#include "extern.hpp"
//class Surface_3D;
//class Isect;
class Object_3D {
    public:
    Object_3D() {} //default Constructor
    ~Object_3D();

    unsigned short o_type;

    double o_dmin[NSLABS];
    double o_dmax[NSLABS];

    Surface_3D *o_surf;
    void       *o_data;
    Clip       *clips;

    Object_3D *next;          /* next object in original list, sibling */

    // class Isect {
    //     public:
    //     double         isect_t;       /* distance to intersection */
    //     int         isect_enter;   /* entering? ie hit front? */
    //     Object_3D  *isect_prim;    /* object we hit */
    //     Surface_3D *isect_surf;    /* surface def of hit object */
    //     Object_3D  *isect_self;    /* pointer to self for queue elimination */

    //     Isect &operator=(const Isect isect);
    // };
    virtual int  intersect(Object_3D *a, Ray *b, Isect &c);
    virtual void normal(Object_3D *obj, Isect &hit, Point &P, Vec &N);

};
/*
typedef struct t_object {
    unsigned short o_type;

    double o_dmin[NSLABS];
    double o_dmax[NSLABS];

    struct t_isect {
        double      isect_t;       //* distance to intersection * /
        int      isect_enter;   //* entering? ie hit front? * /
        t_object *isect_prim;    //* object we hit * /
        Surface  *isect_surf;    //* surface def of hit object * /
        t_object *isect_self;    //* pointer to self for queue elimination * /
    };
    struct t_objectprocs {
        int     (*intersect) (t_object*, Ray*, t_isect*);
        void    (*normal) (t_object*, t_isect*, double*, double*);
    } * o_procs;

    Surface *o_surf;
    void    *o_data;
    Clip    *clips;

    struct t_object *next;          //* next object in original list, sibling * /
} Object;
*/
typedef Object_3D Object;

//typedef struct Object::t_isect Isect;
//typedef struct Object_3D::Isect Isect;

/*
typedef struct t_compositedata {
    unsigned long   size;
    Object          *children;
} CompositeData;
*/
//TODO: TCE Remove
//typedef struct t_object::t_objectprocs ObjectProcs;

//#endif //Object_3D_HPP

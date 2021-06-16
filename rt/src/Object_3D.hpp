#pragma once
//#ifndef Object_3D_HPP
//#define Object_3D_HPP

#include "defs.hpp"
#include "Surface_3D.hpp"
#include "Clip.hpp"
#include "Ray_3D.hpp"
#include "Isect_3D.hpp"
//#include "struct_defs.hpp"
//#include "extern.hpp"

class Object_3D {
    public:
    Object_3D(); //default Constructor
    
    unsigned short o_type;

    Flt o_dmin[NSLABS];
    Flt o_dmax[NSLABS];

    Surface *o_surf;
    void    *o_data;
    Clip    *clips;

    Object_3D *next;          /* next object in original list, sibling */
/*
    struct t_objectprocs {
        int     (*intersect) (t_object*, Ray*, Isect*);
        void    (*normal) (t_object*, Isect*, Flt*, Flt*);
    } * o_procs;
*/
	typedef struct t_isect {
		Flt      isect_t;       /* distance to intersection */
		int      isect_enter;   /* entering? ie hit front? */
		Object_3D *isect_prim;    /* object we hit */
		Surface  *isect_surf;    /* surface def of hit object */
		Object_3D *isect_self;    /* pointer to self for queue elimination */
	} Isect;
    int  Intersect(Object_3D *a, Ray *b, Object_3D::Isect *c);
    void Normal(Object_3D *a, Object_3D::Isect *b, Flt *c, Flt *d);

};
typedef struct t_object {
    unsigned short o_type;

    Flt o_dmin[NSLABS];
    Flt o_dmax[NSLABS];

	struct t_isect {
		Flt      isect_t;       /* distance to intersection */
		int      isect_enter;   /* entering? ie hit front? */
		t_object *isect_prim;    /* object we hit */
		Surface  *isect_surf;    /* surface def of hit object */
		t_object *isect_self;    /* pointer to self for queue elimination */
	};
    struct t_objectprocs {
        int     (*intersect) (t_object*, Ray*, t_isect*);
        void    (*normal) (t_object*, t_isect*, Flt*, Flt*);
    } * o_procs;

    Surface *o_surf;
    void    *o_data;
    Clip    *clips;

    struct t_object *next;          /* next object in original list, sibling */
} Object;

typedef struct Object::t_isect Isect;

typedef struct t_compositedata {
    unsigned long   size;
    Object          *children;
} CompositeData;

//TODO: TCE Remove
typedef struct t_object::t_objectprocs ObjectProcs;

//#endif //Object_3D_HPP

//typedef Object_3D Object;

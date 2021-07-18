#pragma once
//#include "Surface_3D.hpp"
#include "Turbulence_3D.hpp"
#include "Texmap_3D.hpp"
#include "Wave_3D.hpp"
#include "Point_3D.hpp"
#include "Vector_3D.hpp"
#include "Color.hpp"
#include "String.hpp"
#include "defs.hpp"

class Surface_3D;
//class Turbulence;

enum TexPattern {
    UNKNOWN_PAT = 0,
    CHECKER_PAT = 1,
    SPHERICAL_PAT = 2,
    NOISE_PAT = 3
};

class Texture_3D {
    private:
    //TODO: TCE: Create a TexPattern base and create 3 children each with one of these functions as an override to a virtual function as TomC suggested.
    double tex_checker(const Point &P, const Texture_3D &tex);
    double tex_spherical(const Point &P, const Texture_3D &tex);
    double tex_noise(const Point &P, const Texture_3D &tex);

    void get_map_entry(Texmap &tm, double x, double y, Color &color);
    void tex_project(Texmap &_tm, Point P, double *x, double *y);
    void tile(const Texmap &tm, double *x, double *y);

    public:
    Texture_3D(){}
    ~Texture_3D(){}

    // double (*func)(const Point&, const Texture_3D&);    /* returns 0.0..1.0 */
    double apply_pattern(const Point&, const Texture_3D&);
    void tex_fix(Surface_3D &surf, Point &P, Point &OP);
    void map_fix(Surface_3D &surf, const Point &P);
    // void tex_read_img(const String &filename, Texmap &tm);

    TexPattern pat_type = UNKNOWN_PAT;
    double blur;        /* % blending between layers */
    double fuzz;        /* white noise blending */
    double r1, r2;         /* just because */
    int terms;          /* for tex_noise() */
    Vec trans,
        scale;
    Surface_3D *surf[2];
    Turbulence *turbulence;
    Wave *waves;
};
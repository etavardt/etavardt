#pragma once
/*
*******************************************
*                                                                         *
*                             Bob Ray Tracer                              *
*                                                                         *
*                  Proto.H = prototypes for all Bob functions             *
*                                                                         *
*       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     *
*                                                                         *
*       ALL RIGHTS RESERVED.   This software is published, but is NOT     *
*         Public Domain and remains the propery of ALGORITHM, Inc.,       *
*   Christopher D. Watkins and Stephen B. Coy.  This software may not be  *
*  reproduced or integrated into other packages without the prior written *
*          consent of Christopher D. Watkins and Stephen B. Coy.          *
*                                                                         *
*******************************************
*/
#include "Object_3D.hpp"
#include "Light_3D.hpp"
//#include "Texture_3D.hpp"
#include "String.hpp"
#include "defs.hpp"
#include "struct_defs.hpp"

/* bound.c */
extern void BuildBoundingSlabs (void);
//extern void FindAxis (Object *top, long count);
//extern int  SortAndSplit (Object **handle, long count);

/* cone.c */
//extern int     ConeIntersect (Object *obj , Ray *ray , Isect *hit);
//extern void    ConeNormal (Object *obj , Isect *hit , Point P , Point N);
//extern Object *MakeCone (Vec basepoint , double baseradius , Vec apexpoint , double apexradius);

/* clip.c */
//extern void bound_opt(Object *obj);
//extern int  clip_check(Clip *head, Vec &P);

/* data.c */

/* error.c */
// TODO: TCE Remove: extern int    NullIntersect (void);
// TODO: TCE Remove: extern void    NullNormal (void);

/* file.c */
// TODO: TCE Remove: extern void  init_env(void);
extern FILE *env_fopen(String name, const String mode);
//extern FILE *env_fopen(char *name, const char *mode);

/* inter.c */
extern void CheckAndEnqueue (Object *obj , double maxdist);
//extern int  Intersect (Ray *ray , Isect *hit , double maxdist , Object *self);

/* main.c */
//extern int main (int argc , char **argv);

/* memory.c */
//extern void *vmalloc(int size);

/* noise.c */
// extern void init_noise (void);
// extern double  noise1 (const Vec &p);
// extern void noise3 (const Vec &p , double *v);
// extern double  turb1 (Vec &p , int lvl);
// extern void turb3 (Vec &p , Vec &v , int lvl);

/* parse.c */
extern int yyparse (void);

/* pic.c */
//#include "pic.hpp"
//extern Pic  *PicOpen (char *filename , int x , int y);
//extern void  PicWriteLine (Pic *pic , Pixel *buf);
//extern void  PicClose (Pic *pic);

/* poly.c */
//extern int     PolyIntersect (Object *obj , Ray *ray , Isect *hit);
//extern void    PolyNormal (Object *obj , Isect *hit , Point P , Point N);
//extern Object *MakePoly (int npoints , Vec *points);

/* pqueue.c */
extern void PriorityQueueNull (void);
extern int  PriorityQueueEmpty (void);
extern void PriorityQueueInsert (double key , Object *obj);
extern void PriorityQueueDelete (double *key , Object **obj);

/* preproc.c */
// extern int preproc(const String &infile, const String &outfile);
// extern void  expand (char *src);
// extern void  sub_macro (Macro *mptr , char *loc);
// extern void  add_macro (char *txt);
// extern void  remove_macro (char *str);
// extern char *brute (char *text , const char *pat , int tlen , int plen);
// extern void  clean_up (void);
// extern int   vfgets (char *dst , int max_count , FILE *fp);
// extern char *get_next_token (char *text);
// extern int   cpy_tok (char *dst , char *src);
// extern int   is_tok (int c);

/* ring.c */
//extern int     RingIntersect (Object *obj , Ray *ray , Isect *hit);
//extern void    RingNormal (Object *obj , Isect *hit , Point P , Point N);
//extern Object *MakeRing (Vec pos , Vec norm , double min_rad , double max_rad);

/* screen.c * /
extern void Screen (Viewpoint *view , char *picfile , int xres , int yres);
extern void ScrInit (Viewpoint *view , int xres , int yres , char *picfile);
extern void Scan0 (void);
extern void Scan1 (void);
extern void Scan2 (void);
extern void Scan3 (void);
extern void Scan4 (void);
extern void Adapt (int i , int j , double x , double y , Color &color , int step);
extern void Shoot (double x , double y , Color &color);
*/
/* shade.c */
//extern void Shade (int level , double weight , Vec P , Vec N , Vec I , Isect *hit , Color &col , double ior);
//extern void reflect (Vec I , Vec N , Vec R, double dot);
//extern int  refract (double eta, Vec I, Vec N, Vec T, double dot);

/* shadow.c */
extern int Shadow (Ray *ray , Isect &hit , double tmax , Color &color , int level , Light &lptr, int inside);
extern int sShadow (Ray *ray , Isect &hit , double tmax , Color &color , int level , Light &lptr, int inside);

/* sphere.c */
//extern int     SphereIntersect (Object *obj , Ray *ray , Isect *hit);
//extern void    SphereNormal (Object *obj , Isect *hit , Point P , Point N);
//extern Object *MakeSphere (Vec pos , double radius , double fuzzy);

/* stats.c */
//extern void statistics (int line);
extern void stat_tic(void);
extern void init_tic(void);

/* texture.c */
/* TCE: sudo Overloaded in Parser on cur_tex->func */
// extern double tex_checker (const Point &P , const Texture_3D &tex);
// extern double tex_spherical (const Point &P , const Texture_3D &tex);
// extern double tex_noise (const Point &P , const Texture_3D &tex);


extern void tex_fix (Surface_3D &surf , Point P, Point OP);
extern void map_fix (Surface_3D &surf, Point P);
//extern void tex_project (Texmap &tm, Point OP, double *i, double *j);
//extern void tile (Texmap &tm, double *i, double *j);
//extern void get_map_entry (Texmap *tm, double i, double j, Color &color);
extern void tex_read_img (const String &file, Texmap &tm);

/* tokens.c */
extern int get_token();
extern int push_token();

/* trace.c */
extern double  Trace (int level , double weight , Ray *ray , Color &color , double ior , Object *self);
extern void bkg (Vec &dir, Color &col);

/* tri.c */
//extern int     TriIntersect (Object *obj , Ray *ray , Isect *hit);
//extern void    TriNormal (Object *obj , Isect *hit , Point P , Point N);
//extern Object *MakeTri (Vec *point);
//extern void    InvertMatrix (Vec in [3 ], Vec out [3 ]);
//extern void    CheckTri (Vec *point);

/* vector.c */
//extern double  VecNormalize (Vec &vec);
// extern void identity (Matrix mat);
// extern void matrix_cat (Matrix m1 , Matrix m2 , Matrix dest);
// extern void trans_vector (Matrix mat , Vec in , Vec out);
// extern void trans_normal (Matrix mat , Vec in , Vec out);
// extern void matrix_inverse (Matrix in , Matrix out);

/* wave.c */
//extern void make_waves (Vec in, Vec out, Wave *head);

/* yystuff.c */
//extern void yyerror (const char *str);
//extern void ReadSceneFile (const char *real, char *temp);
//extern void Bob::getApp().parser.ptrchk (void *ptr , const char *str);
//extern void yyerror (const String str);
//extern void ReadSceneFile (const String real_name, String tmp_name);
//extern void ptrchk (void *ptr , const String str);
//extern void yystats(void);
//extern void yy_popfile(void);
//extern void yy_newfile(char *file);
//extern void yy_newfile(String file);
//extern void trans_pop(void);

//#undef P

#pragma once
/*
�������������������������������������������
�                                                                         �
�                             Bob Ray Tracer                              �
�                                                                         �
�                  Proto.H = prototypes for all Bob functions             �
�                                                                         �
�       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     �
�                                                                         �
�       ALL RIGHTS RESERVED.   This software is published, but is NOT     �
�         Public Domain and remains the propery of ALGORITHM, Inc.,       �
�   Christopher D. Watkins and Stephen B. Coy.  This software may not be  �
�  reproduced or integrated into other packages without the prior written �
�          consent of Christopher D. Watkins and Stephen B. Coy.          �
�                                                                         �
�������������������������������������������
*/
#include "Object_3D.hpp"
#include "Texture_3D.hpp"
#include "defs.hpp"
#include "struct_defs.hpp"

/* bound.c */
extern void BuildBoundingSlabs (void);
//extern void FindAxis (Object *top, long count);
//extern int  SortAndSplit (Object **handle, long count);

/* cone.c */
extern int     ConeIntersect (Object *obj , Ray *ray , Isect *hit);
extern void    ConeNormal (Object *obj , Isect *hit , Point P , Point N);
extern Object *MakeCone (Vec basepoint , Flt baseradius , Vec apexpoint , Flt apexradius);

/* clip.c */
extern void bound_opt(Object *obj);
extern int  clip_check(Clip *head, Vec P);

/* data.c */

/* error.c */
// TODO: TCE Remove: extern int    NullIntersect (void);
// TODO: TCE Remove: extern void    NullNormal (void);

/* file.c */
// TODO: TCE Remove: extern void  init_env(void);
extern FILE *env_fopen(char *name, const char *mode);

/* inter.c */
extern void CheckAndEnqueue (Object *obj , Flt maxdist);
extern int  Intersect (Ray *ray , Isect *hit , Flt maxdist , Object *self);

/* main.c */
extern int main (int argc , char **argv);

/* memory.c */
extern void *vmalloc(int size);

/* noise.c */
extern void init_noise (void);
extern Flt  noise1 (Vec p);
extern void noise3 (Vec p , Flt *v);
extern Flt  turb1 (Vec p , int lvl);
extern void turb3 (Vec p , Vec v , int lvl);

/* parse.c */
extern int yyparse (void);

/* pic.c */
//#include "pic.hpp"
//extern Pic  *PicOpen (char *filename , int x , int y);
//extern void  PicWriteLine (Pic *pic , Pixel *buf);
//extern void  PicClose (Pic *pic);

/* poly.c */
extern int     PolyIntersect (Object *obj , Ray *ray , Isect *hit);
extern void    PolyNormal (Object *obj , Isect *hit , Point P , Point N);
extern Object *MakePoly (int npoints , Vec *points);

/* pqueue.c */
extern void PriorityQueueNull (void);
extern int  PriorityQueueEmpty (void);
extern void PriorityQueueInsert (Flt key , Object *obj);
extern void PriorityQueueDelete (Flt *key , Object **obj);

/* preproc.c */
extern int   preproc (char *infile , const char *outfile);
extern void  expand (char *src);
extern void  sub_macro (Macro *mptr , char *loc);
extern void  add_macro (char *txt);
extern void  remove_macro (char *str);
extern char *brute (char *text , const char *pat , int tlen , int plen);
extern void  clean_up (void);
extern int   vfgets (char *dst , int max_count , FILE *fp);
extern char *get_next_token (char *text);
extern int   cpy_tok (char *dst , char *src);
extern int   is_tok (int c);

/* ring.c */
extern int     RingIntersect (Object *obj , Ray *ray , Isect *hit);
extern void    RingNormal (Object *obj , Isect *hit , Point P , Point N);
extern Object *MakeRing (Vec pos , Vec norm , Flt min_rad , Flt max_rad);

/* screen.c * /
extern void Screen (Viewpoint *view , char *picfile , int xres , int yres);
extern void ScrInit (Viewpoint *view , int xres , int yres , char *picfile);
extern void Scan0 (void);
extern void Scan1 (void);
extern void Scan2 (void);
extern void Scan3 (void);
extern void Scan4 (void);
extern void Adapt (int i , int j , Flt x , Flt y , Color color , int step);
extern void Shoot (Flt x , Flt y , Color color);
*/
/* shade.c */
extern void Shade (int level , Flt weight , Vec P , Vec N , Vec I , Isect *hit , Color col , Flt ior);
extern void reflect (Vec I , Vec N , Vec R, Flt dot);
extern int  refract (Flt eta, Vec I, Vec N, Vec T, Flt dot);

/* shadow.c */
extern int Shadow (Ray *ray , Isect *hit , Flt tmax , Color color , int level , Light *lptr, int inside);
extern int sShadow (Ray *ray , Isect *hit , Flt tmax , Color color , int level , Light *lptr, int inside);

/* sphere.c */
extern int     SphereIntersect (Object *obj , Ray *ray , Isect *hit);
extern void    SphereNormal (Object *obj , Isect *hit , Point P , Point N);
extern Object *MakeSphere (Vec pos , Flt radius , Flt fuzzy);

/* stats.c */
extern void statistics (int line);
extern void stat_tic(void);
extern void init_tic(void);

/* texture.c */
extern Flt  tex_checker (Point P , Texture *tex);
extern Flt  tex_spherical (Point P , Texture *tex);
extern Flt  tex_noise (Point P , Texture *tex);
extern void tex_fix (Surface *surf , Point P, Point OP);
extern void map_fix (Surface *surf, Point P);
extern void tex_project (Texmap *tm, Point OP, Flt *i, Flt *j);
extern void tile (Texmap *tm, Flt *i, Flt *j);
extern void get_map_entry (Texmap *tm, Flt i, Flt j, Color color);
extern void tex_read_img (char *file, Texmap *tm);

/* tokens.c */
extern int get_token();
extern int push_token();

/* trace.c */
extern Flt  Trace (int level , Flt weight , Ray *ray , Color color , Flt ior , Object *self);
extern void bkg (Vec dir, Color col);

/* tri.c */
extern int     TriIntersect (Object *obj , Ray *ray , Isect *hit);
extern void    TriNormal (Object *obj , Isect *hit , Point P , Point N);
extern Object *MakeTri (Vec *point);
extern void    InvertMatrix (Vec in [3 ], Vec out [3 ]);
extern void    CheckTri (Vec *point);

/* vector.c */
extern Flt  VecNormalize (Vec vec);
extern void identity (Matrix mat);
extern void matrix_cat (Matrix m1 , Matrix m2 , Matrix dest);
extern void trans_vector (Matrix mat , Vec in , Vec out);
extern void trans_normal (Matrix mat , Vec in , Vec out);
extern void matrix_inverse (Matrix in , Matrix out);

/* wave.c */
extern void make_waves (Vec in, Vec out, Wave *head);

/* yystuff.c */
extern void yyerror (const char *str);
extern void ReadSceneFile (const char *real, char *temp);
extern void ptrchk (void *ptr , const char *str);
extern void yystats(void);
extern void yy_popfile(void);
extern void yy_newfile(char *file);
extern void trans_pop(void);

//#undef P


#pragma once

#include <memory>

//#include "Bob.hpp"
#include "Token.hpp"
#include "Camera_3D.hpp"
#include "Surface_3D.hpp"
#include "Texture_3D.hpp"
#include "Bump_3D.hpp"
#include "Texmap_3D.hpp"
#include "Turbulence_3D.hpp"
#include "Wave_3D.hpp"
#include "Clip_3D.hpp"
#include "String.hpp"

class Bob;
//extern Bob &bobApp;

class Parser {
    private:
    Bob &bob;
//    Parser() : bob(Bob &b)) {}
    typedef struct t_infile {
        String file_name;
        FILE *fp;
        long line;
        struct t_infile *next;
    } Infile;
    typedef struct t_stack {
        Infile *what;
        struct t_stack *prev;
    } Stack;

    String  Infilename;
    Stack  *InfileTop = nullptr;      /* points to top of input file stack */
    Vec     tmp_vec;
    Camera_3D &camera;
    public:
    Parser();// : camera(Bob::getApp().camera) {}
    ~Parser() {}

    void get_vec();
    void get_num();
    void yy_background();
    void yy_studio();
    void yy_light();
    Surface_3D *yy_surface();
    Texture_3D *yy_texture();
    Bump *yy_bump();
    Texmap *yy_texmap();
    Turbulence *yy_turbulence();
    Wave *yy_wave();
    void yy_transform();
    void yy_transform_pop();
    void yy_global_clip();
    void yy_clip_pop();
    std::shared_ptr<Clip> yy_clip();
    void yy_sphere();
    void yy_cone();
    void yy_ring();
    void yy_polygon();
    void yy_patch();
    int yyparse();
    void yyerror(const String &str);
    void ReadSceneFile(const String &real_name, const String &tmp_name);
    void ptrchk(const void *ptr, const String &str);
    void yy_newfile(const String &new_file);
    void yy_popfile();
    void trans_pop();

    int yylinecount = 1;
    FILE *yyin;          /* pointer to input file */

    static int nLights; /* it's a dark world out there */
    static int xResolution;
    static int yResolution;


    //Token funcs and vars
    TokenType match_token();
    TokenType get_token();
    TokenType push_token();

    TokenType cur_token;     /* parser token stuff */
    String    cur_text;
    double    cur_value;
    int       token_pushed = 0;
};

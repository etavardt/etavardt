#pragma once

#include <memory>

#include "Surface_3D.hpp"
#include "Texture_3D.hpp"
#include "Bump_3D.hpp"
#include "Texmap_3D.hpp"
#include "Turbulence_3D.hpp"
#include "Wave_3D.hpp"
#include "Clip_3D.hpp"
#include "String.hpp"

class Parser {
    public:
    Parser() {}
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
    void ReadSceneFile(const String &real_name, String tmp_name);
    void ptrchk(void *ptr, const String str);
    void yy_newfile(String new_file);
    void yy_popfile();
    void trans_pop();

};

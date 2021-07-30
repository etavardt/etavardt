/*
***************************************************************************
*                                                                         *
*                             Bob Ray Tracer                              *
*                                                                         *
*          Parse.C = Where we figure out what the user really wants       *
*                                                                         *
*       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     *
*                                                                         *
*       ALL RIGHTS RESERVED.   This software is published, but is NOT     *
*         Public Domain and remains the propery of ALGORITHM, Inc.,       *
*   Christopher D. Watkins and Stephen B. Coy.  This software may not be  *
*  reproduced or integrated into other packages without the prior written *
*          consent of Christopher D. Watkins and Stephen B. Coy.          *
*                                                                         *
*                  Requires: defs.h, extern.h, tokens.h                   *
*                                                                         *
***************************************************************************
*/
#include "Parser.hpp"
#include "Token.hpp"

#include <cmath>
#include <iostream>
#include <memory>

#include "Bob.hpp"
#include "RayTrace_3D.hpp"
#include "BobMath.hpp"
#include "Vector_3D.hpp"
#include "Cone_3D.hpp"
#include "Exception.hpp"
#include "Light_3D.hpp"
#include "Poly_3D.hpp"
#include "Ring_3D.hpp"
#include "Sphere_3D.hpp"
#include "Stats.hpp"
#include "String.hpp"
#include "Tri_3D.hpp"
#include "Bound_3D.hpp"
#include "TransformMatrix_3D.hpp"

#include "extern.hpp"

extern int tickflag;

extern FILE *env_fopen(const String &name, const String &mode); // in file.cpp

int Parser::nLights     = 0; /* it's a dark world out there */
int Parser::xResolution = 320;
int Parser::yResolution = 200;

Parser::Parser() : bob(Bob::getApp()), camera(bob.camera), yyin(nullptr), cur_token(UNKNOWN), cur_text(""), cur_value(0.0) {}

/*
    get_vec() -- get a vector.
*/
void Parser::get_vec() {
    //    cout << "cout: In Parser::get_vec Pre get_token" << endl;
    if (get_token() != NUMBER) {
        yyerror(String("Number expected but not found."));
    }
    tmp_vec[0] = cur_value;
    if (get_token() != NUMBER) {
        yyerror(String("Number expected but not found."));
    }
    tmp_vec[1] = cur_value;
    if (get_token() != NUMBER) {
        yyerror(String("Number expected but not found."));
    }
    tmp_vec[2] = cur_value;
}

/*
    get_num() -- get a number.
*/
void Parser::get_num() {
    //    cout << "cout: In Parser::get_num Pre get_token" << endl;
    if (get_token() != NUMBER) {
        yyerror(String("Number expected but not found."));
    }
}

/*
    yy_background() -- Parse a background structure.
*/
void Parser::yy_background() {
    FILE *fp;
    int i, r, g, b;
    char str[256];

    //    cout << "cout: In Parser::yy_background Pre get_token" << endl;
    get_token(); /* peek at next token */
                 //    cout << "cout: In Parser::yy_background Post get_token cur_token=" << cur_token << endl;
    if (cur_token == NUMBER) {
        push_token();
        get_vec();
        //        VecCopy(tmp_vec, background.color);
        RayTrace_3D::background.color = tmp_vec;
    } else if (cur_token == LEFT_BRACE) {
        while (get_token() != RIGHT_BRACE) {
            //            cout << "cout: In Parser::yy_background Post while get_token cur_token=" << cur_token << endl;
            switch (cur_token) {
            case COLOR:
                get_vec();
                // VecCopy(tmp_vec, background.color);
                RayTrace_3D::background.color = tmp_vec;
                break;
            case UP:
                get_vec();
                VecCopy(tmp_vec, RayTrace_3D::background.up);
                break;
            case UNKNOWN: /* must be a palette */
                          // cout << "cout: In Parser::yy_background cur_token= UNKOWN(" << cur_token << ")" << endl;
                          // cout << "cout: In Parser::yy_background Pre env_fopen cur_text:" << cur_text << endl;
                fp = env_fopen(cur_text, "r");
                // cout << "cout: In Parser::yy_background Post env_fopen fp:" << fp << endl;
                if (!fp) {
                    cerr << "Error opening file " << cur_text << " for input as palette file." << endl;
                    throw Exception("Thrown from yy_background");
                }
                for (i = 0; i < 256; i++) {
                    fgets(str, 256, fp);
                    sscanf(str, "%d %d %d", &r, &g, &b);
                    // background.pal[i][0] = r;
                    // background.pal[i][1] = g;
                    // background.pal[i][2] = b;
                    RayTrace_3D::background.pal[i].r = r;
                    RayTrace_3D::background.pal[i].g = g;
                    RayTrace_3D::background.pal[i].b = b;
                }
                // MakeVector(-1, -1, -1, background.color);
                RayTrace_3D::background.color = -1;
                break;
            default:
                yyerror(String("Unexpected token in background structure."));
                break;
            }
        }
    } else {
        yyerror(String("Invalid background definition."));
    }
} /* end of yy_background() */

/*
    yy_studio() -- Parse the studio structure
*/
void Parser::yy_studio() {
    Vec tmp;

    //    cout << "cout: In Parser::yy_studio Pre MakeVectors" << endl;
    /* assign defaults */
    camera.eye.view_aspect = 1.0;
    //    MakeVector(0, 0, 0, Ambient); /* no global illumination */
    //    MakeVector(0, 0, 0, background.color);
    RayTrace_3D::ambient = 0; /* no global illumination */
    RayTrace_3D::background.color = 0;
    MakeVector(0, 0, 0, RayTrace_3D::background.up);
    camera.aperture = 0.0;     /* pinhole camera */
    camera.focal_length = 0.0; /* fix later */
    camera.samples = F_SAMPLES;
    camera.projection = P_FLAT;
    RayTrace_3D::hazeDensity = 0.0; /* on a clear day... */
    bob.start_line = 0;
    bob.stop_line = (-1);

    //    cout << "cout: In Parser::yy_studio Pre get_token" << endl;
    if (get_token() != LEFT_BRACE) {
        yyerror(String("Left brace expected.\n"));
    }

    while (get_token() != RIGHT_BRACE) {
        switch (cur_token) {
        case FROM:
            get_vec();
            VecCopy(tmp_vec, camera.eye.view_from);
            break;
        case AT:
            get_vec();
            VecCopy(tmp_vec, camera.eye.view_at);
            break;
        case UP:
            get_vec();
            VecCopy(tmp_vec, camera.eye.view_up);
            break;
        case ANGLE:
            get_num();
            camera.eye.view_angle_x = bMath::degtorad(cur_value / 2.0);
            break;
        case WIDTH:
            get_num();
            camera.eye.view_angle_x = cur_value;
            break;
        case RESOLUTION:
            get_num();
            Parser::xResolution = cur_value;
            get_num();
            Parser::yResolution = cur_value;
            break;
        case START:
            get_num();
            bob.start_line = cur_value;
            break;
        case STOP:
            get_num();
            bob.stop_line = cur_value;
            break;
        case ASPECT:
            get_num();
            camera.eye.view_aspect = cur_value;
            break;
        case BUNCHING:
            get_num();
            bunching = cur_value;
            break;
        case AMBIENT:
            get_vec();
            // VecCopy(tmp_vec, Ambient);
            RayTrace_3D::ambient = tmp_vec;
            break;
        case BKG:
            yy_background();
            break;
        case APERTURE:
            get_num();
            camera.aperture = cur_value;
            break;
        case FOCAL_LENGTH:
            get_num();
            camera.focal_length = cur_value;
            break;
        case SAMPLES:
            get_num();
            camera.samples = cur_value;
            break;
        case PROJECTION:
            get_token();
            switch (cur_token) {
            case FLAT:
                camera.projection = P_FLAT;
                break;
            case ORTHO:
                camera.projection = P_ORTHOGRAPHIC;
                break;
            case FISHEYE:
                camera.projection = P_FISHEYE;
                break;
            case NO_PARALLAX:
                camera.projection = P_NO_PARALLAX;
                break;
            default:
                yyerror(String("Unknown projection type."));
                break;
            }
            break;
        case ANTIALIAS:
            get_token();
            switch (cur_token) {
            case NONE:
                antialias = A_NONE;
                break;
            case CORNERS:
                antialias = A_CORNERS;
                break;
            case QUICK:
                antialias = A_QUICK;
                break;
            case ADAPTIVE:
                antialias = A_ADAPTIVE;
                break;
            default:
                yyerror(String("Unknown antialias type."));
                break;
            }
            break;
        case JITTER:
            jitter = 1;
            break;
        case THRESHOLD:
            get_num();
            adapt_dist = cur_value;
            break;
        case HAZE:
            get_num();
            RayTrace_3D::hazeDensity = cur_value;
            break;
        case DEPTH:
            get_num();
            RayTrace_3D::maxlevel = cur_value;
            break;
        case NO_SHADOWS:
            no_shadows = 1;
            break;
        case CAUSTICS:
            caustics = 1;
            break;
        case NO_EXP_TRANS:
            exp_trans = 0;
            break;
        default:
            yyerror(String("Unknown studio element."));
            break;
        } /* end of switch */

    } /* end of while loop, must be done */

    /* clean up loose ends in studio structure */

    /* if not pinhole camera and no focal length was
       chosen, assume distance from eye to 'at' point */

    VecNormalize(camera.eye.view_up);

    if (camera.aperture > 0.0 && camera.focal_length == 0.0) {
        VecSub(camera.eye.view_from, camera.eye.view_at, tmp);
        camera.focal_length = VecLen(tmp);
    }

    if (bob.stop_line == (-1)) { /* do whole image */
        bob.stop_line = Parser::yResolution;
    }

    // VecCopy(background.color, RayTrace_3D::hazeColor);
    RayTrace_3D::hazeColor = RayTrace_3D::background.color;
    if (RayTrace_3D::background.up[0] == 0 && RayTrace_3D::background.up[1] == 0 && RayTrace_3D::background.up[2] == 0) {
        VecCopy(camera.eye.view_up, RayTrace_3D::background.up);
    } else {
        VecNormalize(RayTrace_3D::background.up);
    }

} /* end of yy_studio() */

/*
    yy_light() -- Parse a light.
*/
void Parser::yy_light() {
    Light *tmp;
    int i;

    //    cout << "cout: In Parser::yy_light Pre allocs" << endl;
    /* set up light defaults */
    tmp = new Light();
    Stats::trackMemoryUsage(sizeof(Light));
    ptrchk(tmp, String("light structure"));
    tmp->next = Light::light_head; /* add to head of list */
    Light::light_head = tmp;
    Light::light_head->type = L_POINT;
    Light::light_head->illum = L_INFINITE;
    Light::light_head->samples = L_SAMPLES;
    Light::light_head->flag = 0;
    for (i = 0; i < MAXLEVEL; i++) {
        Light::light_head->light_obj_cache[i] = NULL;
    }

    //    cout << "cout: In Parser::yy_light Pre get_token" << endl;
    /* grab and toss left brace */
    if (get_token() != LEFT_BRACE) {
        yyerror(String("Left brace expected."));
    }

    while (get_token() != RIGHT_BRACE) {
        switch (cur_token) {
        case TYPE:
            get_token();
            switch (cur_token) {
            case POINT:
                Light::light_head->type = L_POINT;
                break;
            case SPHERICAL:
                Light::light_head->type = L_SPHERICAL;
                break;
            case SPOT:
                Light::light_head->type = L_SPOT;
                break;
            case DIRECTIONAL:
                Light::light_head->type = L_DIRECTIONAL;
                break;
            default:
                yyerror(String("Unkown light type."));
                break;
            } /* end of type switch */
            break;
        case COLOR:
            get_vec();
            // VecCopy(tmp_vec, light_head->color);
            Light::light_head->color = tmp_vec;
            break;
        case CENTER:
            get_vec();
            if (TransformMatrix_3D::transTop) {
                TransformMatrix_3D::transTop->trans_vector(tmp_vec, tmp_vec);
            }
            VecCopy(tmp_vec, Light::light_head->position);
            break;
        case FALLOFF:
            get_num();
            Light::light_head->illum = cur_value;
            break;
        case NO_SHADOWS:
            get_num();
            Light::light_head->flag |= L_NOSHADOWS;
            break;
        case NOSPEC:
            get_num();
            Light::light_head->flag |= L_NOSPEC;
            break;
        case DIRECTION:
            get_vec();
            if (TransformMatrix_3D::transTop) {
                TransformMatrix_3D::transTop->trans_normal(tmp_vec, tmp_vec);
            }
            VecCopy(tmp_vec, Light::light_head->dir);
            VecS((-1.0), Light::light_head->dir, Light::light_head->dir);
            VecNormalize(Light::light_head->dir);
            break;
        case RADIUS:
            get_num();
            Light::light_head->radius = cur_value;
            break;
        case MIN_ANGLE:
            get_num();
            Light::light_head->min_angle = cos(bMath::degtorad(cur_value / 2.0));
            break;
        case MAX_ANGLE:
            get_num();
            Light::light_head->max_angle = cos(bMath::degtorad(cur_value / 2.0));
            break;
        case SAMPLES:
            get_num();
            Light::light_head->samples = cur_value;
            break;
        case AT:
            get_vec();
            if (TransformMatrix_3D::transTop) {
                TransformMatrix_3D::transTop->trans_vector(tmp_vec, tmp_vec);
            }
            VecCopy(tmp_vec, Light::light_head->dir);
            VecSub(Light::light_head->position, Light::light_head->dir, Light::light_head->dir);
            VecNormalize(Light::light_head->dir);
            break;
        default:
            yyerror(String("Unkown light structure element."));
            break;
        } /* end of light switch() (ha ha) */
    }     /* end of loop looking for right brace */

    nLights++;
    Stats::yystats();
} /* end of yy_light() */

/*
    yy_surface() -- Parse a surface structure.
*/
Surface_3D *Parser::yy_surface() {
    Surface_3D *surf;

    //    cout << "cout: In Parser::yy_surface Pre allocs" << endl;
    /* allocate surface and fill in default values */
    surf = new Surface_3D();
    Stats::trackMemoryUsage(sizeof(Surface_3D));
    ptrchk(surf, "surface");

    /* if a transformation exists, set matrix to inverse
       of current matrix at TransformMatrix_3D::transTop */
    if (TransformMatrix_3D::transTop) {
        surf->flags |= S_TRANSFORM;
        surf->matrix.matrix_copy(TransformMatrix_3D::transTop->matrix_inverse());
    }

    //    cout << "cout: In Parser::yy_surface Pre get_token" << endl;
    /* grab and toss left brace */
    if (get_token() != LEFT_BRACE) {
        yyerror(String("Left brace expected."));
    }

    while (get_token() != RIGHT_BRACE) {
        switch (cur_token) {
        case DIFFUSE:
            get_token(); /* peek ahead */
            if (cur_token == LEFT_BRACE) {
                push_token();
                surf->tm_diff = yy_texmap();
                surf->flags |= S_TM_DIFF;
            } else {
                push_token();
                get_vec();
                // VecCopy(tmp_vec, surf->diff);
                surf->diff = tmp_vec;
            }
            break;
        case SPECULAR:
            get_token(); /* peek ahead */
            if (cur_token == LEFT_BRACE) {
                push_token();
                surf->tm_spec = yy_texmap();
                surf->flags |= S_TM_SPEC;
            } else {
                push_token();
                get_vec();
                // VecCopy(tmp_vec, surf->spec);
                surf->spec = tmp_vec;
                if (surf->shine != 0 && surf->cshine.r != 0 && surf->cshine.g != 0 && surf->cshine.b != 0) {
                    // VecCopy(tmp_vec, surf->cshine);
                    surf->cshine = tmp_vec;
                }
            }
            break;
        case TRANSPARENT:
            get_token(); /* peek ahead */
            if (cur_token == LEFT_BRACE) {
                push_token();
                surf->tm_trans = yy_texmap();
                surf->flags |= S_TM_TRANS;
            } else {
                push_token();
                get_vec();
                // VecCopy(tmp_vec, surf->trans);
                surf->trans = tmp_vec;
            }
            surf->flags &= ~S_CACHE;
            break;
        case AMBIENT:
            get_token(); /* peek ahead */
            if (cur_token == LEFT_BRACE) {
                push_token();
                surf->tm_amb = yy_texmap();
                surf->flags |= S_TM_AMB;
            } else {
                push_token();
                get_vec();
                // VecCopy(tmp_vec, surf->amb);
                surf->amb = tmp_vec;
            }
            break;
        case SHINE:
            get_num();
            surf->shine = cur_value;
            get_token(); /* peek ahead */
            push_token();
            if (cur_token == NUMBER) {
                get_vec();
                // VecCopy(tmp_vec, surf->cshine);
                surf->cshine = tmp_vec;
            } else {
                // VecCopy(surf->spec, surf->cshine);
                surf->cshine = surf->spec;
            }
            break;
        case IOR:
            get_num();
            surf->ior = cur_value;
            break;
        case FUZZ:
            get_num();
            surf->fuzz = cur_value;
            break;
        case NO_ANTIALIAS:
            surf->flags |= S_NO_ANTIALIAS;
            break;
        case TEXTURE:
            surf->tex = yy_texture();
            break;
        case BUMP:
            surf->bump = yy_bump();
            break;
        default:
            yyerror(String("Unknown surface element."));
            break;
        } /* end of bump map switch */
    }     /* end of while loop looking for left brace */

    Surface_3D::currentSurface = surf; /* so primitives can find it */
    return surf;
} /* end of yy_surface() */

/*
    yy_texture() -- Parse procedural texture definition.
*/
Texture_3D *Parser::yy_texture() {
    Texture_3D *cur_tex;
    Wave *tmp_wave;

    //    cout << "cout: In Parser::yy_texture Pre alloc" << endl;
    cur_tex = new Texture_3D();
    Stats::trackMemoryUsage(sizeof(Texture_3D));
    ptrchk(cur_tex, "procedural texture structure");

    /* assign defaults */

    MakeVector(1, 1, 1, cur_tex->scale);
    MakeVector(0, 0, 0, cur_tex->trans);
    cur_tex->turbulence = NULL;
    cur_tex->waves = NULL;
    cur_tex->blur = 0.0;
    cur_tex->fuzz = 0.0;
    cur_tex->r1 = (-1.0);
    cur_tex->r2 = (-1.0);
//    cur_tex->func = NULL;
    cur_tex->surf[0] = NULL;
    cur_tex->surf[1] = NULL;

    //    cout << "cout: In Parser::yy_texture Pre get_token" << endl;
    /* grab and toss left brace */
    if (get_token() != LEFT_BRACE) {
        yyerror(String("Left brace expected."));
    }

    while (get_token() != RIGHT_BRACE) {
        switch (cur_token) {
        case SURFACE:
            if (cur_tex->surf[0] == NULL) {
                cur_tex->surf[0] = yy_surface();
            } else if (cur_tex->surf[1] == NULL) {
                cur_tex->surf[1] = yy_surface();
            } else {
                yyerror(String("Sorry, only two surfaces per texture allowed.\n"));
            }
            break;
        case PATTERN:
            get_token();
            switch (cur_token) {
            case CHECKER:
                // cur_tex->func = tex_checker;
                cur_tex->pat_type = CHECKER_PAT;
                break;
            case SPHERICAL:
                // cur_tex->func = tex_spherical;
                cur_tex->pat_type = SPHERICAL_PAT;
                break;
            case NOISE:
                // cur_tex->func = tex_noise;
                cur_tex->pat_type = NOISE_PAT;
                break;
            default:
                yyerror(String("Unknown texture type."));
                break;
            }
            break;
        case SCALE:
            get_vec();
            VecCopy(tmp_vec, cur_tex->scale);
            break;
        case TRANSLATE:
            get_vec();
            VecCopy(tmp_vec, cur_tex->trans);
            break;
        case BLEND:
            get_num();
            cur_tex->blur = cur_value;
            break;
        case FUZZ:
            get_num();
            cur_tex->fuzz = cur_value;
            break;
        case TERMS:
            get_num();
            cur_tex->terms = cur_value;
            break;
        case RADIUS:
            get_num();
            if (cur_tex->r1 < 0) {
                cur_tex->r1 = cur_value;
            } else if (cur_tex->r2 < 0) {
                cur_tex->r2 = cur_value;
            } else {
                yyerror(String("Thanks anyway but only two radii are needed."));
            }
            break;
        case TURBULENCE:
            cur_tex->turbulence = yy_turbulence();
            break;
        case WAVE:
            /* get new wave structure and add to head of list */
            tmp_wave = yy_wave();
            tmp_wave->next = cur_tex->waves;
            cur_tex->waves = tmp_wave;
            break;
        default:
            yyerror(String("Unknown texture element."));
            break;
        } /* end of texture switch() */
    }     /* end of loop looking for right brace */

    /* clean up loose ends */
//    if (cur_tex->func == NULL) {
    if (cur_tex->pat_type == UNKNOWN_PAT) {
        yyerror(String("No pattern declared for texture."));
    }
    // if (cur_tex->func == tex_spherical) {
    if (cur_tex->pat_type == SPHERICAL_PAT) {
        if (cur_tex->r2 < 0) {
            cur_tex->r2 = cur_tex->r1;
        }
    }

    return cur_tex;

} /* end of yy_texture() */

/*
    yy_bump() -- Parse bump map structure definition.
*/
Bump *Parser::yy_bump() {
    Bump *cur_bump;
    Wave *tmp_wave;

    //    cout << "cout: In Parser::yy_bump Pre allocs" << endl;
    cur_bump = new Bump();
    Stats::trackMemoryUsage(sizeof(Bump));
    ptrchk(cur_bump, "surface normal bump structure");

    /* fill in defaults */
    MakeVector(1, 1, 1, cur_bump->scale);
    MakeVector(0, 0, 0, cur_bump->trans);
    cur_bump->turbulence = NULL;
    cur_bump->waves = NULL;

    //    cout << "cout: In Parser::yy_bump Pre get_token" << endl;
    /* grab and toss left brace */
    if (get_token() != LEFT_BRACE) {
        yyerror(String("Left brace expected."));
    }

    while (get_token() != RIGHT_BRACE) {
        switch (cur_token) {
        case SCALE:
            get_vec();
            VecCopy(tmp_vec, cur_bump->scale);
            break;
        case TRANSLATE:
            get_vec();
            VecCopy(tmp_vec, cur_bump->trans);
            break;
        case TURBULENCE:
            cur_bump->turbulence = yy_turbulence();
            break;
        case WAVE:
            /* get new wave structure and add to head of list */
            tmp_wave = yy_wave();
            tmp_wave->next = cur_bump->waves;
            cur_bump->waves = tmp_wave;
            break;
        default:
            yyerror(String("Unknown bump map element."));
            break;
        } /* end of bump map switch */
    }     /* end of while loop looking for right brace */

    return cur_bump;

} /* end of yy_bump() */

/*
    yy_texmap() -- Parse a texure map structure.
*/
Texmap *Parser::yy_texmap() {
    Texmap *cur_texmap;

    //    cout << "cout: In Parser::yy_texmap Pre allocs" << endl;
    /* allocate and fill in the defaults */
    cur_texmap = new Texmap();
    Stats::trackMemoryUsage(sizeof(Texmap));
    ptrchk(cur_texmap, "texture map structure");
    MakeVector(0.0, 0.0, 0.0, cur_texmap->position);
    MakeVector(0.0, 0.0, 1.0, cur_texmap->normal);
    MakeVector(1.0, 0.0, 0.0, cur_texmap->across);
    MakeVector(0.0, -1.0, 0.0, cur_texmap->down);
    if (TransformMatrix_3D::transTop) {
        cur_texmap->scale = 1.0 / (*TransformMatrix_3D::transTop)[3][3];
    } else {
        cur_texmap->scale = 1.0;
    }
    cur_texmap->red = NULL;
    cur_texmap->grn = NULL;
    cur_texmap->blu = NULL;

    //    cout << "cout: In Parser::yy_texmap Pre get_token" << endl;
    /* grab and toss left brace */
    if (get_token() != LEFT_BRACE) {
        yyerror(String("Left brace expected."));
    }

    while (get_token() != RIGHT_BRACE) {
        switch (cur_token) {
        case IMAGE:
            get_token();
            cur_texmap->tex_read_img(cur_text, *cur_texmap);
            break;
        case CENTER:
            get_vec();
            VecCopy(tmp_vec, cur_texmap->position);
            if (TransformMatrix_3D::transTop) {
                TransformMatrix_3D::transTop->trans_vector(cur_texmap->position, cur_texmap->position);
            }
            break;
        case NORMAL:
            get_vec();
            VecCopy(tmp_vec, cur_texmap->normal);
            if (TransformMatrix_3D::transTop) {
                TransformMatrix_3D::transTop->trans_normal(cur_texmap->normal, cur_texmap->normal);
            }
            break;
        case ACROSS:
            get_vec();
            VecCopy(tmp_vec, cur_texmap->across);
            if (TransformMatrix_3D::transTop) {
                TransformMatrix_3D::transTop->trans_normal(cur_texmap->across, cur_texmap->across);
            }
            break;
        case SCALE:
            get_num();
            cur_texmap->scale *= cur_value;
            break;
        default:
            yyerror(String("Unkown texture map structure element."));
            break;
        } /* end of texture map switch */
    }     /* end of while loop looking for right brace */

    /* make sure everything is normalized and perpendicular */

    VecNormalize(cur_texmap->normal);
    VecNormalize(cur_texmap->across);
    VecCross(cur_texmap->across, cur_texmap->normal, cur_texmap->down);
    VecCross(cur_texmap->normal, cur_texmap->down, cur_texmap->across);

    return cur_texmap;

} /* end of yy_texmap() */

/*
    yy_turbulence() -- Parse a turbulence structure.
*/
Turbulence *Parser::yy_turbulence() {
    Turbulence *cur_turb;

    //    cout << "cout: In Parser::yy_turbulence Pre allocs" << endl;
    /* allocate and fill in defaults */
    cur_turb = new Turbulence();
    Stats::trackMemoryUsage(sizeof(Turbulence));
    ptrchk(cur_turb, "turbulence structure");

    cur_turb->terms = 1;
    cur_turb->amp = 1.0;
    MakeVector(1, 1, 1, cur_turb->scale);
    MakeVector(0, 0, 0, cur_turb->trans);

    //    cout << "cout: In Parser::yy_turbulence Pre get_token" << endl;
    /* grab and toss left brace */
    if (get_token() != LEFT_BRACE) {
        yyerror(String("Left brace expected."));
    }

    while (get_token() != RIGHT_BRACE) {
        switch (cur_token) {
        case TERMS:
            get_num();
            cur_turb->terms = cur_value;
            break;
        case AMP:
            get_num();
            cur_turb->amp = cur_value;
            break;
        case SCALE:
            get_vec();
            VecCopy(tmp_vec, cur_turb->scale);
            break;
        case TRANSLATE:
            get_vec();
            VecCopy(tmp_vec, cur_turb->trans);
            break;
        default:
            yyerror(String("Unknown turbulence structure element."));
            break;
        } /* end of turbulence switch */
    }     /* end of while loop looking for right brace */

    return cur_turb;

} /* end of yy_turbulence() */

/*
    yy_wave() -- Parse a wave structure.
*/
Wave *Parser::yy_wave() {
    Wave *cur_wave;

    //    cout << "cout: In Parser::yy_wave Pre allocs" << endl;
    /* allocate and fill in defaults */
    cur_wave = new Wave();
    Stats::trackMemoryUsage(sizeof(Wave));
    ptrchk(cur_wave, "wave structure");

    MakeVector(0, 0, 0, cur_wave->center);
    cur_wave->wavelength = 1.0;
    cur_wave->amp = 0.1;
    cur_wave->damp = 1.0;
    cur_wave->phase = 0.0;

    //    cout << "cout: In Parser::yy_wave Pre get_token" << endl;
    /* grab and toss left brace */
    if (get_token() != LEFT_BRACE) {
        yyerror(String("Left brace expected."));
    }

    while (get_token() != RIGHT_BRACE) {
        switch (cur_token) {
        case CENTER:
            get_vec();
            VecCopy(tmp_vec, cur_wave->center);
            break;
        case LENGTH:
            get_num();
            cur_wave->wavelength = cur_value;
            break;
        case AMP:
            get_num();
            cur_wave->amp = cur_value;
            break;
        case DAMPING:
            get_num();
            cur_wave->damp = cur_value;
            break;
        case PHASE:
            get_num();
            cur_wave->phase = cur_value;
            break;
        default:
            yyerror(String("Unknown wave structure element."));
            break;
        } /* end of wave switch */
    }     /* end of while loop looking for right brace */

    return cur_wave;

} /* end of yy_wave() */

/*
    yy_transform() -- Parse a transformation.
*/
void Parser::yy_transform() {
    Transform *cur_trans;
    Matrix m;
    double theta, tmp;

    //    cout << "cout: In Parser::yy_transform Pre allocs" << endl;
    cur_trans = new Transform();
    Stats::trackMemoryUsage(sizeof(Transform));
    ptrchk(cur_trans, "transform structure");
    cur_trans->next = TransformMatrix_3D::transTop;
    TransformMatrix_3D::transTop = cur_trans;
    cur_trans->identity();

    //    cout << "cout: In Parser::yy_transform Pre get_token" << endl;
    /* grab and toss left brace */
    if (get_token() != LEFT_BRACE) {
        yyerror(String("Left brace expected."));
    }

    while (get_token() != RIGHT_BRACE) {
        switch (cur_token) {
        case ROTATE:
            get_vec();

            /* do x axis rotation */
            m.identity();
            theta = bMath::degtorad(tmp_vec[0]);
            m[1][1] = cos(theta);
            m[2][1] = -sin(theta);
            m[1][2] = sin(theta);
            m[2][2] = cos(theta);
            TransformMatrix_3D::transTop->matrix_cat(m);

            /* do y axis rotation */
            m.identity();
            theta = bMath::degtorad(tmp_vec[1]);
            m[0][0] = cos(theta);
            m[2][0] = sin(theta);
            m[0][2] = -sin(theta);
            m[2][2] = cos(theta);
            TransformMatrix_3D::transTop->matrix_cat(m);

            /* do z axis rotation */
            m.identity();
            theta = bMath::degtorad(tmp_vec[2]);
            m[0][0] = cos(theta);
            m[1][0] = -sin(theta);
            m[0][1] = sin(theta);
            m[1][1] = cos(theta);
            TransformMatrix_3D::transTop->matrix_cat(m);

            break;
        case TRANSLATE:
            get_vec();
            m.identity();
            m[3][0] = tmp_vec[0];
            m[3][1] = tmp_vec[1];
            m[3][2] = tmp_vec[2];

            TransformMatrix_3D::transTop->matrix_cat(m);

            break;
        case SCALE:
            get_num();
            tmp = cur_value;
            get_token();
            if (cur_token == NUMBER) {
                m.identity();
                m[0][0] = tmp;
                m[1][1] = cur_value;
                get_num();
                m[2][2] = cur_value;

                TransformMatrix_3D::transTop->matrix_cat(m);
            } else {
                push_token();
                (*TransformMatrix_3D::transTop)[3][3] /= tmp;
            }
            break;
        default:
            yyerror(String("Unknown transformation element."));
            break;
        } /* end of transformation switch */
    }     /* end of while loop looking for right brace */

    /* concatenate new transformation onto previous ones */

    if (TransformMatrix_3D::transTop->next) {
        TransformMatrix_3D::transTop->matrix_cat(*TransformMatrix_3D::transTop->next);
    }

} /* end of yy_transform() */

/*
    yy_transform_pop() -- Pop the current transformation.
*/
void Parser::yy_transform_pop() {
    //    cout << "cout: In Parser::yy_transform_pop Pre trans_pop" << endl;
    trans_pop();
} /* end of yy_transform_pop() */

/*
    yy_global_clip() -- Parse a global clip.
*/
void Parser::yy_global_clip() {
    std::shared_ptr<GlobalClip> ptr;
    //Clip *new_clip;

    //    cout << "cout: In Parser::yy_global_clip Pre allocs" << endl;
    ptr = std::shared_ptr<GlobalClip>(new GlobalClip());
    Stats::trackMemoryUsage(sizeof(GlobalClip));
    //ptrchk(ptr, "global clipping linked list structure");
    ptr->next = GlobalClip::GlobalClipTop;
    ptr->clip = NULL;
    GlobalClip::GlobalClipTop = ptr;

    //    cout << "cout: In Parser::yy_global_clip Pre get_token" << endl;
    /* grab and toss left brace */
    if (get_token() != LEFT_BRACE) {
        yyerror(String("Left brace expected."));
    }

    while (get_token() != RIGHT_BRACE) {
        if (cur_token == CLIP) {
            yy_clip();
        } else {
            yyerror(String("Clipping structure expected."));
        }
    } /* end of while loop looking for right brace */

    ptr->clip = Clip_3D::ClipTop;

} /* end of yy_global_clip() */

/*
    yy_clip_pop() -- Pop the current clip.
*/
void Parser::yy_clip_pop() {
    //    cout << "cout: In Parser::yy_clip_pop" << endl;
    GlobalClip::GlobalClipTop = GlobalClip::GlobalClipTop->next;
    Clip_3D::ClipTop = GlobalClip::GlobalClipTop->clip;
} /* end of yy_clip_pop() */

/*
    yy_clip() -- Parse a clipping structure.
*/
std::shared_ptr<Clip> Parser::yy_clip() {
    std::shared_ptr<Clip> cur_clip;

    //    cout << "cout: In Parser::yy_clip Pre allocs" << endl;
    cur_clip = std::shared_ptr<Clip>(new Clip());
    Stats::trackMemoryUsage(sizeof(Clip));
    // ptrchk(cur_clip, "clipping structure");
    MakeVector(0, 0, 0, cur_clip->center);
    MakeVector(0, 0, 1, cur_clip->normal);
    MakeVector(0, 0, 0, cur_clip->apex);
    MakeVector(0, 0, 0, cur_clip->base);
    cur_clip->radius1 = 0.0;
    cur_clip->radius2 = 0.0;
    cur_clip->length = 0.0;
    cur_clip->type = 0;

    //    cout << "cout: In Parser::yy_clip Pre get_token" << endl;
    /* grab and toss left brace */
    if (get_token() != LEFT_BRACE) {
        yyerror(String("Left brace expected."));
    }

    while (get_token() != RIGHT_BRACE) {
        switch (cur_token) {
        case CENTER:
            get_vec();
            VecCopy(tmp_vec, cur_clip->center);
            if (TransformMatrix_3D::transTop) {
                TransformMatrix_3D::transTop->trans_vector(cur_clip->center, cur_clip->center);
            }
            if (!(cur_clip->type & C_PLANE)) {
                cur_clip->type |= C_SPHERE;
            }
            break;
        case NORMAL:
            get_vec();
            VecCopy(tmp_vec, cur_clip->normal);
            if (TransformMatrix_3D::transTop) {
                TransformMatrix_3D::transTop->trans_normal(cur_clip->normal, cur_clip->normal);
            }
            cur_clip->type |= C_PLANE;
            if (cur_clip->type & C_SPHERE) {
                cur_clip->type ^= C_SPHERE;
            }
            break;
        case APEX:
            get_vec();
            VecCopy(tmp_vec, cur_clip->apex);
            if (TransformMatrix_3D::transTop) {
                TransformMatrix_3D::transTop->trans_vector(cur_clip->apex, cur_clip->apex);
            }
            Clip_3D::ClipTop->type |= C_CONE;
            break;
        case BASE:
            get_vec();
            VecCopy(tmp_vec, cur_clip->base);
            if (TransformMatrix_3D::transTop) {
                TransformMatrix_3D::transTop->trans_vector(cur_clip->base, cur_clip->base);
            }
            cur_clip->type |= C_CONE;
            break;
        case RADIUS:
            get_num();
            cur_clip->radius1 = cur_value;
            cur_clip->radius2 = cur_value;
            break;
        case APEX_RADIUS:
            get_num();
            cur_clip->radius1 = cur_value;
            cur_clip->type |= C_CONE;
            break;
        case BASE_RADIUS:
            get_num();
            cur_clip->radius2 = cur_value;
            cur_clip->type |= C_CONE;
            break;
        case INSIDE:
            cur_clip->type |= C_INSIDE;
            break;
        case OUTSIDE: /* this is the default */
            break;
        default:
            yyerror(String("Unknown clipping structure element."));
            break;
        } /* end of clip switch */
    }     /* end of while loop looking for right brace */

    if (cur_clip->type & C_CONE) {
        VecSub(cur_clip->base, cur_clip->apex, cur_clip->normal);
        cur_clip->length = VecNormalize(cur_clip->normal);
    }
    if (cur_clip->type & C_SPHERE) {
        cur_clip->radius1 = cur_clip->radius1 * cur_clip->radius1;
    }

    cur_clip->next = Clip_3D::ClipTop;
    Clip_3D::ClipTop = cur_clip;

    return cur_clip;
} /* end of yy_clip() */

/*
    yy_sphere() -- Parse a sphere
*/
void Parser::yy_sphere() {
    Vec center;
    double radius = 0.0, fuzz = 0.0;
    Object *new_obj = nullptr;

    fuzz = 0.0;

    //    cout << "cout: In Parser::yy_sphere Pre get_token" << endl;
    /* grab and toss left brace */
    if (get_token() != LEFT_BRACE) {
        yyerror(String("Left brace expected."));
    }

    while (get_token() != RIGHT_BRACE) {
        switch (cur_token) {
        case CENTER:
            get_vec();
            VecCopy(tmp_vec, center);
            break;
        case RADIUS:
            get_num();
            radius = cur_value;
            break;
        case FUZZ:
            get_num();
            fuzz = cur_value;
            break;
        case CLIP:
            yy_clip();
            break;
        default:
            yyerror(String("Unknown sphere element."));
            break;
        } /* end of sphere switch */
    }     /* end of while loop looking for right brace */

    if (TransformMatrix_3D::transTop) {
        TransformMatrix_3D::transTop->trans_vector(center, center);
        radius /= (*TransformMatrix_3D::transTop)[3][3];
        fuzz /= (*TransformMatrix_3D::transTop)[3][3];
    }

    new_obj = Sphere_3D::makeSphere(center, radius, fuzz);
    new_obj->next = Object_3D::Root;
    Object_3D::Root = new_obj;

    ++Bound_3D::nPrims;
    Stats::yystats();
} /* end of yy_sphere() */

/*
    yy_cone() -- Parse a cone.
*/
void Parser::yy_cone() {
    Vec apex, base;
    double arad = 0.0, brad = 0.0;
    Object *new_obj = nullptr;

    //    cout << "cout: In Parser::yy_cone Pre get_token" << endl;
    /* grab and toss left brace */
    if (get_token() != LEFT_BRACE) {
        yyerror(String("Left brace expected."));
    }

    while (get_token() != RIGHT_BRACE) {
        switch (cur_token) {
        case APEX:
            get_vec();
            VecCopy(tmp_vec, apex);
            break;
        case BASE:
            get_vec();
            VecCopy(tmp_vec, base);
            break;
        case APEX_RADIUS:
            get_num();
            arad = cur_value;
            break;
        case BASE_RADIUS:
            get_num();
            brad = cur_value;
            break;
        case RADIUS:
            get_num();
            arad = cur_value;
            brad = cur_value;
            break;
        case CLIP:
            yy_clip();
            break;
        default:
            yyerror(String("Unknown cone element."));
            break;
        } /* end of cone switch */
    }     /* end of while loop looking for right brace */

    if (TransformMatrix_3D::transTop) {
        TransformMatrix_3D::transTop->trans_vector(apex, apex);
        TransformMatrix_3D::transTop->trans_vector(base, base);
        arad /= (*TransformMatrix_3D::transTop)[3][3];
        brad /= (*TransformMatrix_3D::transTop)[3][3];
    }

    new_obj = Cone_3D::makeCone(apex, arad, base, brad);
    new_obj->next = Object_3D::Root;
    Object_3D::Root = new_obj;

    ++Bound_3D::nPrims;
    Stats::yystats();
} /* end of yy_cone() */

/*
    yy_ring() -- Parse a ring.
*/
void Parser::yy_ring() {
    Vec center, normal;
    double min_rad = 0.0, max_rad = 0.0;
    Ring_3D *new_obj = nullptr;

    //    cout << "cout: In Parser::yy_ring Pre get_token" << endl;
    /* grab and toss left brace */
    if (get_token() != LEFT_BRACE) {
        yyerror(String("Left brace expected."));
    }

    while (get_token() != RIGHT_BRACE) {
        switch (cur_token) {
        case CENTER:
            get_vec();
            VecCopy(tmp_vec, center);
            break;
        case NORMAL:
            get_vec();
            VecCopy(tmp_vec, normal);
            break;
        case MIN_RADIUS:
            get_num();
            min_rad = cur_value;
            break;
        case MAX_RADIUS:
        case RADIUS:
            get_num();
            max_rad = cur_value;
            break;
        case CLIP:
            yy_clip();
            break;
        default:
            yyerror(String("Unknown ring element."));
            break;
        } /* end of ring switch */
    }     /* end of while loop looking for right brace */

    if (TransformMatrix_3D::transTop) {
        TransformMatrix_3D::transTop->trans_vector(center, center);
        TransformMatrix_3D::transTop->trans_normal(normal, normal);
        min_rad /= (*TransformMatrix_3D::transTop)[3][3];
        max_rad /= (*TransformMatrix_3D::transTop)[3][3];
    }

    new_obj = Ring_3D::makeRing(center, normal, min_rad, max_rad);
    new_obj->next = Object_3D::Root;
    Object_3D::Root = new_obj;

    ++Bound_3D::nPrims;
    Stats::yystats();
} /* end of yy_ring() */

/*
    yy_polygon() -- Parse a polygon.
*/
void Parser::yy_polygon() {
    int num_pnts, i;
    Vec *vlist;
    Object *new_obj;

    //    cout << "cout: In Parser::yy_polygon Pre get_token" << endl;
    /* grab and toss left brace */
    if (get_token() != LEFT_BRACE) {
        yyerror(String("Left brace expected."));
    }

    /* get number of points */

    get_token();
    if (cur_token != POINTS) {
        yyerror(String("Number of points in polygon needed."));
    }
    get_num();
    num_pnts = cur_value;

    vlist = new Vec[num_pnts]();
    Stats::trackMemoryUsage(sizeof(Vec[num_pnts]));
    ptrchk(vlist, "polygon vertex list");
    i = 0;

    while (get_token() != RIGHT_BRACE) {
        switch (cur_token) {
        case VERTEX:
            get_vec();
            VecCopy(tmp_vec, vlist[i]);
            if (TransformMatrix_3D::transTop) {
                TransformMatrix_3D::transTop->trans_vector(vlist[i], vlist[i]);
            }
            i++;
            break;
        case CLIP:
            yy_clip();
            break;
        default:
            yyerror(String("Unknown polygon element.  Vertex expected."));
            break;
        } /* end of ring switch */
    }     /* end of while loop looking for right brace */

    if (i != num_pnts) {
        yyerror(String("Number of vertices expected does not match data."));
    }

    new_obj = Poly_3D::makePoly(num_pnts, vlist);
    new_obj->next = Object_3D::Root;
    Object_3D::Root = new_obj;

    ++Bound_3D::nPrims;
    Stats::yystats();
} /* end of yy_polygon() */

/*
    yy_patch() -- Parse a triangular patch.
*/
void Parser::yy_patch() {
    Vec data[6];
    int v, n;
    Object *new_obj;

    //    cout << "cout: In Parser::yy_patch Pre get_token" << endl;
    /* grab and toss left brace */
    if (get_token() != LEFT_BRACE) {
        yyerror(String("Left brace expected."));
    }

    v = n = 0;
    while (get_token() != RIGHT_BRACE) {
        switch (cur_token) {
        case VERTEX:
            get_vec();
            VecCopy(tmp_vec, data[v * 2]);
            ++v;
            break;
        case NORMAL:
            get_vec();
            VecCopy(tmp_vec, data[n * 2 + 1]);
            ++n;
            break;
        case CLIP:
            yy_clip();
            break;
        default:
            yyerror(String("Unknown triangular patch element."));
            break;
        } /* end of patch switch */
    }     /* end of while loop looking for right brace */

    if (n != 3 || v != 3) {
        yyerror(String("Patches must have 3 vertices and normals each."));
    }

    if (TransformMatrix_3D::transTop) {
        TransformMatrix_3D::transTop->trans_vector(data[0], data[0]);
        TransformMatrix_3D::transTop->trans_vector(data[2], data[2]);
        TransformMatrix_3D::transTop->trans_vector(data[4], data[4]);
        TransformMatrix_3D::transTop->trans_normal(data[1], data[1]);
        TransformMatrix_3D::transTop->trans_normal(data[3], data[3]);
        TransformMatrix_3D::transTop->trans_normal(data[5], data[5]);
    }

    new_obj = Tri_3D::makeTri(data);
    new_obj->next = Object_3D::Root;
    Object_3D::Root = new_obj;

    ++Bound_3D::nPrims;
    Stats::yystats();
} /* end of yy_patch() */

/*
    yyparse() -- The main entry point into the parser.
*/

int Parser::yyparse() {
    //    cout << "cout: In Parser::yyparse Pre get_token" << endl;
    while (get_token() != END_OF_FILE) {
        //        cout << "cout: In Parser::yyparse cur_token=" << cur_token << endl;
        switch (cur_token) {
        case NEWFILE:
            get_token(); /* get file name */
            yy_newfile(cur_text);
            break;
        case POPFILE:
            yy_popfile();
            break;
        case STUDIO:
            yy_studio();
            break;
        case LIGHT:
            yy_light();
            break;
        case SURFACE:
            yy_surface();
            break;
        case TRANSFORM:
            yy_transform();
            break;
        case TRANS_POP:
            yy_transform_pop();
            break;
        case GLOBAL_CLIP:
            yy_global_clip();
            break;
        case CLIP_POP:
            yy_clip_pop();
            break;
        case SPHERE:
            yy_sphere();
            break;
        case CONE:
            yy_cone();
            break;
        case RING:
            yy_ring();
            break;
        case POLYGON:
            yy_polygon();
            break;
        case PATCH:
            yy_patch();
            break;
        case COMMENT:
            // ignore comments
            break;
        default:
            cerr << "\nError parsing, yyparse() found token " << cur_token << " '" << cur_text << "'" << endl;
            yyerror(String("Unknown token.\n"));
            break;
        } /* end of big switch */
    }     /* end of big loop */

    return 0; /* successful */

} /* end of yyparse() */

void Parser::yyerror(const String &str) {
    Infile *iptr;

    iptr = InfileTop->what;
    cerr << "\n\nError at line " << (yylinecount + 1) << endl;
    cerr << "file \"" << iptr->file_name << "\"" << endl;
    cerr << str << endl;
    cerr << "Error at or near = \"" << cur_text << "\"" << endl;
    throw Exception("thrown by yyerror");
}

void Parser::ReadSceneFile(const String &real_name, const String &tmp_name) {
    //    cout << "cout: In Parser::ReadSceneFile real_name=" << real_name << " \ttmp_name=" << tmp_name << endl;

    Infile *iptr;

    if ((yyin = env_fopen(tmp_name, String("r").data())) == NULL) {
        cerr << "Error, cannot open " << tmp_name << endl;
        throw Exception("thrown by ReadSceneFile");
    }
    //    cout << "cout: In Parser::ReadSceneFile Post env_fopen Pre malloc of Stack *InfileTop" << endl;
    Infilename = real_name;

    /* set up input file stack */
    InfileTop = new Stack();
    //    Stats::trackMemoryUsage(sizeof(Stack));
    ptrchk(InfileTop, "input file stack object");
    InfileTop->prev = NULL;

    //    cout << "cout: In Parser::ReadSceneFile Post malloc of Stack InfileTop Pre malloc of Infile *iptr" << endl;

    iptr = new Infile();
    //    Stats::trackMemoryUsage(sizeof(Infile));
    ptrchk(iptr, "input file structure");
    InfileTop->what = iptr;

    iptr->file_name = Infilename;
    //ptrchk(iptr->file_name.data(), "input file name");
    iptr->line = 0;

    //    cout << "cout: In Parser::ReadSceneFile Post ptrchk of input file name Pre Parse the input file" << endl;

    /* parse the input file */
    if (yyparse() == 1) {
        cerr << "Invalid input specification" << endl;
        throw Exception("thrown by ReadSceneFile");
    }

    //    cout << "cout: In Parser::ReadSceneFile Post yyparse Pre clean up transform structures" << endl;
    /* clean up transform structures */
    while (TransformMatrix_3D::transTop)
        trans_pop();

    if (bob.stop_line == -1) {
        cerr << "\n\nError, no studio structure in input file." << endl;
        throw Exception("thrown by ReadSceneFile");
    }

    if (tickflag) {
        cout << "\tinputfile = \"" << Infilename << "\"" << endl;
        cout << "\tlights " << nLights << " prims " << Bound_3D::nPrims << "\n" << endl;
        cout << "\tresolution " << Parser::xResolution << " " << Parser::yResolution << endl;
        for (int i = 0; i < 30; i++)
            cout << endl;
    }
}

void Parser::ptrchk(const void *ptr, const String &str) {
    if (!ptr)
        throw Exception(String("Error allocating memory for a ").append(str).append("\n").append("thrown by ReadSceneFile"));
}

void Parser::yy_newfile(const String &new_file) {
    Infile *iptr;
    Stack *sptr;

    iptr = InfileTop->what; /* save line number for current file */
    iptr->line = yylinecount;

    //    cout << "cout: In Parser::yy_newfile Pre allocs" << endl;
    sptr = new Stack();
    Stats::trackMemoryUsage(sizeof(Stack));
    ptrchk(sptr, "input file stack object");
    sptr->prev = InfileTop;
    InfileTop = sptr;

    iptr = new Infile();
    Stats::trackMemoryUsage(sizeof(Infile));
    ptrchk(iptr, "input file structure");
    InfileTop->what = iptr;

    //    iptr->file_name = strdup(new_file);
    iptr->file_name = new_file;
    iptr->line = 0;
    yylinecount = -1;

} /* end of yy_newfile() */

void Parser::yy_popfile() {
    Infile *iptr;
    // Stack *sptr;

    //    cout << "cout: In Parser::yy_patch Pre InfileTop->prev check" << endl;
    if (InfileTop->prev) {
        // iptr = InfileTop->what;
        // sptr = InfileTop;
        InfileTop = InfileTop->prev;
        iptr = InfileTop->what;
        yylinecount = iptr->line;
    } else {
        cerr << "\nTemp input file corrupted.  Dying" << endl;
        throw Exception("thrown by yy_popfile");
    }
}

void Parser::trans_pop() {
    Transform *tptr;

    //    cout << "cout: In Parser::trans_pop Pre TransformMatrix_3D::transTop ptr check" << endl;
    tptr = TransformMatrix_3D::transTop;
    if (tptr == NULL) {
        yyerror(String("Trying to pop a transformation from an empty stack."));
    }
    TransformMatrix_3D::transTop = TransformMatrix_3D::transTop->next;
}

/*
�������������������������������������������
�                                                                         �
�                             Bob Ray Tracer                              �
�                                                                         �
�          Parse.C = Where we figure out what the user really wants       �
�                                                                         �
�       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     �
�                                                                         �
�       ALL RIGHTS RESERVED.   This software is published, but is NOT     �
�         Public Domain and remains the propery of ALGORITHM, Inc.,       �
�   Christopher D. Watkins and Stephen B. Coy.  This software may not be  �
�  reproduced or integrated into other packages without the prior written �
�          consent of Christopher D. Watkins and Stephen B. Coy.          �
�                                                                         �
�                  Requires: defs.h, extern.h, tokens.h                   �
�                                                                         �
�������������������������������������������
*/

#include <cstdio>
#include <cstdlib>
#include <ctype.h>
#include <math.h>
#include "defs.hpp"
#include "extern.hpp"
#include "tokens.hpp"
#include "proto.hpp"

Vec             tmp_vec;

Surface         *yy_surface();
Wave            *yy_wave();
Texture         *yy_texture();
Bump            *yy_bump();
Texmap          *yy_texmap();
Turbulence      *yy_turbulence();
Clip            *yy_clip();

/*
    get_vec() -- get a vector.
*/
void get_vec()
{
    if(get_token() != NUMBER) {
        yyerror("Number expected but not found.");
    }
    tmp_vec[0] = cur_value;
    if(get_token() != NUMBER) {
        yyerror("Number expected but not found.");
    }
    tmp_vec[1] = cur_value;
    if(get_token() != NUMBER) {
        yyerror("Number expected but not found.");
    }
    tmp_vec[2] = cur_value;
}

/*
    get_num() -- get a number.
*/
void get_num()
{
    if(get_token() != NUMBER) {
        yyerror("Number expected but not found.");
    }
}

/*
    yy_background() -- Parse a background structure.
*/
void yy_background()
{
    FILE    *fp;
    int     i, r, g, b;
    char    str[256];

    get_token();    /* peek at next token */
    if(cur_token == NUMBER) {
        push_token();
        get_vec();
        VecCopy(tmp_vec, background.color);
    } else if(cur_token == LEFT_BRACE) {
        while(get_token() != RIGHT_BRACE) {
            switch(cur_token) {
                case COLOR :
                    get_vec();
                    VecCopy(tmp_vec, background.color);
                    break;
                case UP :
                    get_vec();
                    VecCopy(tmp_vec, background.up);
                    break;
                case UNKNOWN :  /* must be a palette */
                    fp = env_fopen(cur_text, "r");
                    if(!fp) {
                        fprintf(stderr, "Error opening file %s for input as palette file.\n", cur_text);
                        exit(1);
                    }
                    for(i=0; i<256; i++) {
                        fgets(str, 256, fp);
                        sscanf(str, "%d %d %d", &r, &g, &b);
                        background.pal[i][0] = r;
                        background.pal[i][1] = g;
                        background.pal[i][2] = b;
                    }
                    MakeVector(-1, -1, -1, background.color);
                    break;
                default :
                    yyerror("Unexpected token in background structure.");
                    break;
            }
        }
    } else {
        yyerror("Invalid background definition.");
    }
}       /* end of yy_background() */

/*
    yy_studio() -- Parse the studio structure
*/
void yy_studio()
{
    Vec     tmp;

    /* assign defaults */
    Eye.view_aspect = 1.0;
    MakeVector(0, 0, 0, Ambient);   /* no global illumination */
    MakeVector(0, 0, 0, background.color);
    MakeVector(0, 0, 0, background.up);
    camera.aperture = 0.0;          /* pinhole camera */
    camera.focal_length = 0.0;      /* fix later */
    camera.samples = F_SAMPLES;
    camera.projection = P_FLAT;
    HazeDensity = 0.0;              /* on a clear day... */
    start_line = 0;
    stop_line = (-1);

    if(get_token() != LEFT_BRACE) {
        yyerror("Left brace expected.\n");
    }

    while(get_token() != RIGHT_BRACE) {
        switch(cur_token) {
            case FROM :
                get_vec();
                VecCopy(tmp_vec, Eye.view_from);
                break;
            case AT :
                get_vec();
                VecCopy(tmp_vec, Eye.view_at);
                break;
            case UP :
                get_vec();
                VecCopy(tmp_vec, Eye.view_up);
                break;
            case ANGLE :
                get_num();
                Eye.view_angle_x = degtorad(cur_value/2.0);
                break;
            case WIDTH :
                get_num();
                Eye.view_angle_x = cur_value;
                break;
            case RESOLUTION :
                get_num();
                Xresolution = cur_value;
                get_num();
                Yresolution = cur_value;
                break;
            case START :
                get_num();
                start_line = cur_value;
                break;
            case STOP :
                get_num();
                stop_line = cur_value;
                break;
            case ASPECT :
                get_num();
                Eye.view_aspect = cur_value;
                break;
            case BUNCHING :
                get_num();
                bunching = cur_value;
                break;
            case AMBIENT :
                get_vec();
                VecCopy(tmp_vec, Ambient);
                break;
            case BKG :
                yy_background();
                break;
            case APERTURE :
                get_num();
                camera.aperture = cur_value;
                break;
            case FOCAL_LENGTH :
                get_num();
                camera.focal_length = cur_value;
                break;
            case SAMPLES :
                get_num();
                camera.samples = cur_value;
                break;
            case PROJECTION :
                get_token();
                switch(cur_token) {
                    case FLAT :
                        camera.projection = P_FLAT;
                        break;
                    case ORTHO :
                        camera.projection = P_ORTHOGRAPHIC;
                        break;
                    case FISHEYE :
                        camera.projection = P_FISHEYE;
                        break;
                    case NO_PARALLAX :
                        camera.projection = P_NO_PARALLAX;
                        break;
                    default :
                        yyerror("Unknown projection type.");
                        break;
                }
                break;
            case ANTIALIAS :
                get_token();
                switch(cur_token) {
                    case NONE :
                        antialias = A_NONE;
                        break;
                    case CORNERS :
                        antialias = A_CORNERS;
                        break;
                    case QUICK :
                        antialias = A_QUICK;
                        break;
                    case ADAPTIVE :
                        antialias = A_ADAPTIVE;
                        break;
                    default :
                        yyerror("Unknown antialias type.");
                        break;
                }
                break;
            case JITTER :
                jitter = 1;
                break;
            case THRESHOLD :
                get_num();
                adapt_dist = cur_value;
                break;
            case HAZE :
                get_num();
                HazeDensity = cur_value;
                break;
            case DEPTH :
                get_num();
                maxlevel = cur_value;
                break;
            case NO_SHADOWS :
                no_shadows = 1;
                break;
            case CAUSTICS :
                caustics = 1;
                break;
            case NO_EXP_TRANS :
                exp_trans = 0;
                break;
            default :
                yyerror("Unknown studio element.");
                break;
        }       /* end of switch */

    }       /* end of while loop, must be done */

    /* clean up loose ends in studio structure */

    /* if not pinhole camera and no focal length was
       chosen, assume distance from eye to 'at' point */

    VecNormalize(Eye.view_up);

    if(camera.aperture>0.0 && camera.focal_length==0.0) {
        VecSub(Eye.view_from, Eye.view_at, tmp);
        camera.focal_length = VecLen(tmp);
    }

    if(stop_line == (-1)) {         /* do whole image */
        stop_line = Yresolution;
    }

    VecCopy(background.color, HazeColor);
    if(background.up[0]==0 && background.up[1]==0 && background.up[2]==0) {
        VecCopy(Eye.view_up, background.up);
    } else {
        VecNormalize(background.up);
    }

}       /* end of yy_studio() */

/*
    yy_light() -- Parse a light.
*/
void yy_light()
{
    Light   *tmp;
    int     i;

    /* set up light defaults */
    tmp = (Light *)vmalloc(sizeof(Light));
    ptrchk(tmp, "light structure");
    tmp->next = light_head;         /* add to head of list */
    light_head = tmp;
    light_head->type = L_POINT;
    light_head->illum = L_INFINITE;
    light_head->samples = L_SAMPLES;
    light_head->flag = 0;
    for(i=0; i<MAXLEVEL; i++) {
        light_head->light_obj_cache[i] = NULL;
    }

    /* grab and toss left brace */
    if(get_token() != LEFT_BRACE) {
        yyerror("Left brace expected.");
    }

    while(get_token() != RIGHT_BRACE) {
        switch(cur_token) {
            case TYPE :
                get_token();
                switch(cur_token) {
                    case POINT :
                        light_head->type = L_POINT;
                        break;
                    case SPHERICAL :
                        light_head->type = L_SPHERICAL;
                        break;
                    case SPOT :
                        light_head->type = L_SPOT;
                        break;
                    case DIRECTIONAL :
                        light_head->type = L_DIRECTIONAL;
                        break;
                    default :
                        yyerror("Unkown light type.");
                        break;
                }       /* end of type switch */
                break;
            case COLOR :
                get_vec();
                VecCopy(tmp_vec, light_head->color);
                break;
            case CENTER :
                get_vec();
                if(TransTop) {
                    trans_vector(TransTop->mat, tmp_vec, tmp_vec);
                }
                VecCopy(tmp_vec, light_head->position);
                break;
            case FALLOFF :
                get_num();
                light_head->illum = cur_value;
                break;
            case NO_SHADOWS :
                get_num();
                light_head->flag |= L_NOSHADOWS;
                break;
            case NOSPEC :
                get_num();
                light_head->flag |= L_NOSPEC;
                break;
            case DIRECTION :
                get_vec();
                if(TransTop) {
                    trans_normal(TransTop->mat, tmp_vec, tmp_vec);
                }
                VecCopy(tmp_vec, light_head->dir);
                VecS((-1.0), light_head->dir, light_head->dir);
                VecNormalize(light_head->dir);
                break;
            case RADIUS :
                get_num();
                light_head->radius = cur_value;
                break;
            case MIN_ANGLE :
                get_num();
                light_head->min_angle = cos(degtorad(cur_value/2.0));
                break;
            case MAX_ANGLE :
                get_num();
                light_head->max_angle = cos(degtorad(cur_value/2.0));
                break;
            case SAMPLES :
                get_num();
                light_head->samples = cur_value;
                break;
            case AT :
                get_vec();
                if(TransTop) {
                    trans_vector(TransTop->mat, tmp_vec, tmp_vec);
                }
                VecCopy(tmp_vec, light_head->dir);
                VecSub(light_head->position, light_head->dir, light_head->dir);
                VecNormalize(light_head->dir);
                break;
            default:
                yyerror("Unkown light structure element.");
                break;
        }       /* end of light switch() (ha ha) */
    }       /* end of loop looking for right brace */

    nLights++;
    yystats();
}       /* end of yy_light() */

/*
    yy_surface() -- Parse a surface structure.
*/
Surface *yy_surface()
{
    Surface *surf;

    /* allocate surface and fill in default values */

    surf = (Surface *)vmalloc (sizeof(Surface));
    ptrchk(surf, "surface");

    /* assign defaults */

    MakeVector(0, 0, 0, surf->diff);
    MakeVector(0, 0, 0, surf->spec);
    surf->shine = 0.0;
    MakeVector(0, 0, 0, surf->cshine);
    MakeVector(0, 0, 0, surf->trans);
    surf->ior = 1.0;
    MakeVector(0, 0, 0, surf->amb);
    surf->fuzz = 0.0;
    surf->flags = S_CACHE;
    surf->tex = NULL;
    surf->bump = NULL;
    surf->tm_diff = NULL;
    surf->tm_spec = NULL;
    surf->tm_trans = NULL;
    surf->tm_amb = NULL;

    /* if a transformation exists, set matrix to inverse
       of current matrix at TransTop */

    if(TransTop) {
        surf->flags |= S_TRANSFORM;
        matrix_inverse(TransTop->mat, surf->matrix);
    }

    /* grab and toss left brace */
    if(get_token() != LEFT_BRACE) {
        yyerror("Left brace expected.");
    }

    while(get_token() != RIGHT_BRACE) {
        switch(cur_token) {
            case DIFFUSE :
                get_token();    /* peek ahead */
                if(cur_token == LEFT_BRACE) {
                    push_token();
                    surf->tm_diff = yy_texmap();
                    surf->flags |= S_TM_DIFF;
                } else {
                    push_token();
                    get_vec();
                    VecCopy(tmp_vec, surf->diff);
                }
                break;
            case SPECULAR :
                get_token();    /* peek ahead */
                if(cur_token == LEFT_BRACE) {
                    push_token();
                    surf->tm_spec = yy_texmap();
                    surf->flags |= S_TM_SPEC;
                } else {
                    push_token();
                    get_vec();
                    VecCopy(tmp_vec, surf->spec);
                    if(surf->shine != 0
                       && surf->cshine[0] != 0
                       && surf->cshine[1] != 0
                       && surf->cshine[2] != 0) {
                        VecCopy(tmp_vec, surf->cshine);
                    }
                }
                break;
            case TRANSPARENT :
                get_token();    /* peek ahead */
                if(cur_token == LEFT_BRACE) {
                    push_token();
                    surf->tm_trans = yy_texmap();
                    surf->flags |= S_TM_TRANS;
                } else {
                    push_token();
                    get_vec();
                    VecCopy(tmp_vec, surf->trans);
                }
                surf->flags &= ~S_CACHE;
                break;
            case AMBIENT :
                get_token();    /* peek ahead */
                if(cur_token == LEFT_BRACE) {
                    push_token();
                    surf->tm_amb = yy_texmap();
                    surf->flags |= S_TM_AMB;
                } else {
                    push_token();
                    get_vec();
                    VecCopy(tmp_vec, surf->amb);
                }
                break;
            case SHINE :
                get_num();
                surf->shine = cur_value;
                get_token();    /* peek ahead */
                push_token();
                if(cur_token == NUMBER) {
                    get_vec();
                    VecCopy(tmp_vec, surf->cshine);
                } else {
                    VecCopy(surf->spec, surf->cshine);
                }
                break;
            case IOR :
                get_num();
                surf->ior = cur_value;
                break;
            case FUZZ :
                get_num();
                surf->fuzz = cur_value;
                break;
            case NO_ANTIALIAS :
                surf->flags |= S_NO_ANTIALIAS;
                break;
            case TEXTURE :
                surf->tex = yy_texture();
                break;
            case BUMP :
                surf->bump = yy_bump();
                break;
            default :
                yyerror("Unknown surface element.");
                break;
        }       /* end of bump map switch */
    }       /* end of while loop looking for left brace */

    CurrentSurface = surf;  /* so primitives can find it */
    return surf;
}       /* end of yy_surface() */

/*
    yy_texture() -- Parse procedural texture definition.
*/
Texture *yy_texture()
{
    Texture *cur_tex;
    Wave    *tmp_wave;

    cur_tex = (Texture *)vmalloc(sizeof(Texture));
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
    cur_tex->func = NULL;
    cur_tex->surf[0] = NULL;
    cur_tex->surf[1] = NULL;

    /* grab and toss left brace */
    if(get_token() != LEFT_BRACE) {
        yyerror("Left brace expected.");
    }

    while(get_token() != RIGHT_BRACE) {
        switch(cur_token) {
            case SURFACE :
                if(cur_tex->surf[0] == NULL) {
                    cur_tex->surf[0] = yy_surface();
                } else if(cur_tex->surf[1] == NULL) {
                    cur_tex->surf[1] = yy_surface();
                } else {
                    yyerror("Sorry, only two surfaces per texture allowed.\n");
                }
                break;
            case PATTERN :
                get_token();
                switch(cur_token) {
                    case CHECKER :
                        cur_tex->func = tex_checker;
                        break;
                    case SPHERICAL :
                        cur_tex->func = tex_spherical;
                        break;
                    case NOISE :
                        cur_tex->func = tex_noise;
                        break;
                    default :
                        yyerror("Unknown texture type.");
                        break;
                }
                break;
            case SCALE :
                get_vec();
                VecCopy(tmp_vec, cur_tex->scale);
                break;
            case TRANSLATE :
                get_vec();
                VecCopy(tmp_vec, cur_tex->trans);
                break;
            case BLEND :
                get_num();
                cur_tex->blur = cur_value;
                break;
            case FUZZ :
                get_num();
                cur_tex->fuzz = cur_value;
                break;
            case TERMS :
                get_num();
                cur_tex->terms = cur_value;
                break;
            case RADIUS :
                get_num();
                if(cur_tex->r1 < 0) {
                    cur_tex->r1 = cur_value;
                } else if(cur_tex->r2 < 0) {
                    cur_tex->r2 = cur_value;
                } else {
                    yyerror("Thanks anyway but only two radii are needed.");
                }
                break;
            case TURBULENCE :
                cur_tex->turbulence = yy_turbulence();
                break;
            case WAVE :
                /* get new wave structure and add to head of list */
                tmp_wave = yy_wave();
                tmp_wave->next = cur_tex->waves;
                cur_tex->waves = tmp_wave;
                break;
            default :
                yyerror("Unknown texture element.");
                break;
        }       /* end of texture switch() */
    }       /* end of loop looking for right brace */

    /* clean up loose ends */
    if(cur_tex->func == NULL) {
        yyerror("No pattern declared for texture.");
    }
    if(cur_tex->func == tex_spherical) {
        if(cur_tex->r2 < 0) {
            cur_tex->r2 = cur_tex->r1;
        }
    }

    return cur_tex;

}       /* end of yy_texture() */

/*
    yy_bump() -- Parse bump map structure definition.
*/
Bump    *yy_bump()
{
    Bump    *cur_bump;
    Wave    *tmp_wave;

    cur_bump = (Bump *)vmalloc(sizeof(Bump));
    ptrchk(cur_bump, "surface normal bump structure");

    /* fill in defaults */
    MakeVector(1, 1, 1, cur_bump->scale);
    MakeVector(0, 0, 0, cur_bump->trans);
    cur_bump->turbulence = NULL;
    cur_bump->waves = NULL;

    /* grab and toss left brace */
    if(get_token() != LEFT_BRACE) {
        yyerror("Left brace expected.");
    }

    while(get_token() != RIGHT_BRACE) {
        switch(cur_token) {
            case SCALE :
                get_vec();
                VecCopy(tmp_vec, cur_bump->scale);
                break;
            case TRANSLATE :
                get_vec();
                VecCopy(tmp_vec, cur_bump->trans);
                break;
            case TURBULENCE :
                cur_bump->turbulence = yy_turbulence();
                break;
            case WAVE :
                /* get new wave structure and add to head of list */
                tmp_wave = yy_wave();
                tmp_wave->next = cur_bump->waves;
                cur_bump->waves = tmp_wave;
                break;
            default :
                yyerror("Unknown bump map element.");
                break;
        }       /* end of bump map switch */
    }       /* end of while loop looking for right brace */

    return cur_bump;

}       /* end of yy_bump() */

/*
    yy_texmap() -- Parse a texure map structure.
*/
Texmap  *yy_texmap()
{
    Texmap  *cur_texmap;

    /* allocate and fill in the defaults */
    cur_texmap = (Texmap *)vmalloc(sizeof(Texmap));
    ptrchk(cur_texmap, "texture map structure");
    MakeVector(0.0, 0.0, 0.0, cur_texmap->position);
    MakeVector(0.0, 0.0, 1.0, cur_texmap->normal);
    MakeVector(1.0, 0.0, 0.0, cur_texmap->across);
    MakeVector(0.0, -1.0, 0.0, cur_texmap->down);
    if(TransTop) {
        cur_texmap->scale = 1.0/TransTop->mat[3][3];
    } else {
        cur_texmap->scale = 1.0;
    }
    cur_texmap->red = NULL;
    cur_texmap->grn = NULL;
    cur_texmap->blu = NULL;

    /* grab and toss left brace */
    if(get_token() != LEFT_BRACE) {
        yyerror("Left brace expected.");
    }

    while(get_token() != RIGHT_BRACE) {
        switch(cur_token) {
            case IMAGE :
                get_token();
                tex_read_img(cur_text, cur_texmap);
                break;
            case CENTER :
                get_vec();
                VecCopy(tmp_vec, cur_texmap->position);
                if(TransTop) {
                    trans_vector(TransTop->mat, cur_texmap->position, cur_texmap->position);
                }
                break;
            case NORMAL :
                get_vec();
                VecCopy(tmp_vec, cur_texmap->normal);
                if(TransTop) {
                    trans_normal(TransTop->mat, cur_texmap->normal, cur_texmap->normal);
                }
                break;
            case ACROSS :
                get_vec();
                VecCopy(tmp_vec, cur_texmap->across);
                if(TransTop) {
                    trans_normal(TransTop->mat, cur_texmap->across, cur_texmap->across);
                }
                break;
            case SCALE :
                get_num();
                cur_texmap->scale *= cur_value;
                break;
            default :
                yyerror("Unkown texture map structure element.");
                break;
        }       /* end of texture map switch */
    }       /* end of while loop looking for right brace */

    /* make sure everything is normalized and perpendicular */

    VecNormalize(cur_texmap->normal);
    VecNormalize(cur_texmap->across);
    VecCross(cur_texmap->across, cur_texmap->normal, cur_texmap->down);
    VecCross(cur_texmap->normal, cur_texmap->down, cur_texmap->across);

    return cur_texmap;

}       /* end of yy_texmap() */

/*
    yy_turbulence() -- Parse a turbulence structure.
*/
Turbulence      *yy_turbulence()
{
    Turbulence      *cur_turb;

    /* allocate and fill in defaults */
    cur_turb = (Turbulence *)vmalloc(sizeof(Turbulence));
    ptrchk(cur_turb, "turbulence structure");

    cur_turb->terms = 1;
    cur_turb->amp = 1.0;
    MakeVector(1, 1, 1, cur_turb->scale);
    MakeVector(0, 0, 0, cur_turb->trans);

    /* grab and toss left brace */
    if(get_token() != LEFT_BRACE) {
        yyerror("Left brace expected.");
    }

    while(get_token() != RIGHT_BRACE) {
        switch(cur_token) {
            case TERMS :
                get_num();
                cur_turb->terms = cur_value;
                break;
            case AMP :
                get_num();
                cur_turb->amp = cur_value;
                break;
            case SCALE :
                get_vec();
                VecCopy(tmp_vec, cur_turb->scale);
                break;
            case TRANSLATE :
                get_vec();
                VecCopy(tmp_vec, cur_turb->trans);
                break;
            default :
                yyerror("Unknown turbulence structure element.");
                break;
        }       /* end of turbulence switch */
    }       /* end of while loop looking for right brace */

    return cur_turb;

}       /* end of yy_turbulence() */

/*
    yy_wave() -- Parse a wave structure.
*/
Wave    *yy_wave()
{
    Wave    *cur_wave;

    /* allocate and fill in defaults */
    cur_wave = (Wave *)vmalloc(sizeof(Wave));
    ptrchk(cur_wave, "wave structure");

    MakeVector(0, 0, 0, cur_wave->center);
    cur_wave->wavelength = 1.0;
    cur_wave->amp        = 0.1;
    cur_wave->damp       = 1.0;
    cur_wave->phase      = 0.0;

    /* grab and toss left brace */
    if(get_token() != LEFT_BRACE) {
        yyerror("Left brace expected.");
    }

    while(get_token() != RIGHT_BRACE) {
        switch(cur_token) {
            case CENTER :
                get_vec();
                VecCopy(tmp_vec, cur_wave->center);
                break;
            case LENGTH :
                get_num();
                cur_wave->wavelength = cur_value;
                break;
            case AMP :
                get_num();
                cur_wave->amp = cur_value;
                break;
            case DAMPING :
                get_num();
                cur_wave->damp = cur_value;
                break;
            case PHASE :
                get_num();
                cur_wave->phase = cur_value;
                break;
            default :
                yyerror("Unknown wave structure element.");
                break;
        }       /* end of wave switch */
    }       /* end of while loop looking for right brace */

    return cur_wave;

}       /* end of yy_wave() */

/*
    yy_transform() -- Parse a transformation.
*/
void yy_transform()
{
    Transform       *cur_trans;
    Matrix          m;
    Flt             theta, tmp;

    cur_trans = (Transform *)vmalloc(sizeof(Transform));
    ptrchk(cur_trans, "transform structure");
    cur_trans->next = TransTop;
    TransTop = cur_trans;
    identity(cur_trans->mat);

    /* grab and toss left brace */
    if(get_token() != LEFT_BRACE) {
        yyerror("Left brace expected.");
    }

    while(get_token() != RIGHT_BRACE) {
        switch(cur_token) {
            case ROTATE :
                get_vec();

                /* do x axis rotation */
                identity(m);
                theta = degtorad(tmp_vec[0]);
                m[1][1] = cos(theta);
                m[2][1] = -sin(theta);
                m[1][2] = sin(theta);
                m[2][2] = cos(theta);
                matrix_cat(TransTop->mat, m, TransTop->mat);

                /* do y axis rotation */
                identity(m);
                theta = degtorad(tmp_vec[1]);
                m[0][0] = cos(theta);
                m[2][0] = sin(theta);
                m[0][2] = -sin(theta);
                m[2][2] = cos(theta);
                matrix_cat(TransTop->mat, m, TransTop->mat);

                /* do z axis rotation */
                identity(m);
                theta = degtorad(tmp_vec[2]);
                m[0][0] = cos(theta);
                m[1][0] = -sin(theta);
                m[0][1] = sin(theta);
                m[1][1] = cos(theta);
                matrix_cat(TransTop->mat, m, TransTop->mat);

                break;
            case TRANSLATE :
                get_vec();
                identity(m);    /* make identity matrix */
                m[3][0] = tmp_vec[0];
                m[3][1] = tmp_vec[1];
                m[3][2] = tmp_vec[2];

                matrix_cat(TransTop->mat, m, TransTop->mat);

                break;
            case SCALE :
                get_num();
                tmp = cur_value;
                get_token();
                if(cur_token == NUMBER) {
                    identity(m);    /* make identity matrix */
                    m[0][0] = tmp;
                    m[1][1] = cur_value;
                    get_num();
                    m[2][2] = cur_value;

                    matrix_cat(TransTop->mat, m, TransTop->mat);
                } else {
                    push_token();
                    TransTop->mat[3][3] /= tmp;
                }
                break;
            default :
                yyerror("Unknown transformation element.");
                break;
        }       /* end of transformation switch */
    }       /* end of while loop looking for right brace */

    /* concatenate new transformation onto previous ones */

    if(TransTop->next) {
        matrix_cat(TransTop->mat, TransTop->next->mat, TransTop->mat);
    }

}       /* end of yy_transform() */

/*
    yy_transform_pop() -- Pop the current transformation.
*/
void yy_transform_pop()
{
    trans_pop();
}       /* end of yy_transform_pop() */

/*
    yy_global_clip() -- Parse a global clip.
*/
void yy_global_clip()
{
    GlobalClip      *ptr;
    Clip            *new_clip;

    ptr = (GlobalClip *)vmalloc(sizeof(GlobalClip));
    ptrchk(ptr, "global clipping linked list structure");
    ptr->next       = GlobalClipTop;
    ptr->clip       = NULL;
    GlobalClipTop   = ptr;

    /* grab and toss left brace */
    if(get_token() != LEFT_BRACE) {
        yyerror("Left brace expected.");
    }

    while(get_token() != RIGHT_BRACE) {
        if(cur_token == CLIP) {
            yy_clip();
        } else {
            yyerror("Clipping structure expected.");
        }
    }       /* end of while loop looking for right brace */

    ptr->clip = ClipTop;

}       /* end of yy_global_clip() */

/*
    yy_clip_pop() -- Pop the current clip.
*/
void yy_clip_pop()
{
    GlobalClipTop = GlobalClipTop->next;
    ClipTop = GlobalClipTop->clip;
}       /* end of yy_clip_pop() */

/*
    yy_clip() -- Parse a clipping structure.
*/
Clip    *yy_clip()
{
    Clip    *cur_clip;

    cur_clip = (Clip *)vmalloc(sizeof(Clip));
    ptrchk(cur_clip, "clipping structure");
    MakeVector(0, 0, 0, cur_clip->center);
    MakeVector(0, 0, 1, cur_clip->normal);
    MakeVector(0, 0, 0, cur_clip->apex);
    MakeVector(0, 0, 0, cur_clip->base);
    cur_clip->radius1 = 0.0;
    cur_clip->radius2 = 0.0;
    cur_clip->length  = 0.0;
    cur_clip->type    = 0;

    /* grab and toss left brace */
    if(get_token() != LEFT_BRACE) {
        yyerror("Left brace expected.");
    }

    while(get_token() != RIGHT_BRACE) {
        switch(cur_token) {
            case CENTER :
                get_vec();
                VecCopy(tmp_vec, cur_clip->center);
                if(TransTop) {
                    trans_vector(TransTop->mat, cur_clip->center, cur_clip->center);
                }
        		if(!(cur_clip->type & C_PLANE)) {
					cur_clip->type |= C_SPHERE;
				}
				break;
			case NORMAL :
				get_vec();
				VecCopy(tmp_vec, cur_clip->normal);
				if(TransTop) {
					trans_normal(TransTop->mat, cur_clip->normal, cur_clip->normal);
				}
				cur_clip->type |= C_PLANE;
				if(cur_clip->type & C_SPHERE) {
					cur_clip->type ^= C_SPHERE;
				}
				break;
			case APEX :
				get_vec();
				VecCopy(tmp_vec, cur_clip->apex);
				if(TransTop) {
					trans_vector(TransTop->mat, cur_clip->apex, cur_clip->apex);
				}
				ClipTop->type |= C_CONE;
				break;
			case BASE :
				get_vec();
				VecCopy(tmp_vec, cur_clip->base);
				if(TransTop) {
					trans_vector(TransTop->mat, cur_clip->base, cur_clip->base);
				}
				cur_clip->type |= C_CONE;
				break;
			case RADIUS :
				get_num();
				cur_clip->radius1 = cur_value;
				cur_clip->radius2 = cur_value;
				break;
			case APEX_RADIUS :
				get_num();
				cur_clip->radius1 = cur_value;
				cur_clip->type |= C_CONE;
				break;
			case BASE_RADIUS :
				get_num();
				cur_clip->radius2 = cur_value;
				cur_clip->type |= C_CONE;
				break;
			case INSIDE :
				cur_clip->type |= C_INSIDE;
				break;
			case OUTSIDE :          /* this is the default */
				break;
			default :
				yyerror("Unknown clipping structure element.");
				break;
		}       /* end of clip switch */
	}       /* end of while loop looking for right brace */

	if(cur_clip->type & C_CONE) {
		VecSub(cur_clip->base, cur_clip->apex, cur_clip->normal);
		cur_clip->length = VecNormalize(cur_clip->normal);
	}
	if(cur_clip->type & C_SPHERE) {
		cur_clip->radius1 = cur_clip->radius1 * cur_clip->radius1;
	}

	cur_clip->next = ClipTop;
	ClipTop = cur_clip;

	return cur_clip;
}       /* end of yy_clip() */

/*
	yy_sphere() -- Parse a sphere
*/
void yy_sphere()
{
	Vec     center;
	Flt     radius, fuzz;
	Object  *new_obj;

	fuzz = 0.0;

	/* grab and toss left brace */
	if(get_token() != LEFT_BRACE) {
		yyerror("Left brace expected.");
	}

	while(get_token() != RIGHT_BRACE) {
		switch(cur_token) {
			case CENTER :
				get_vec();
				VecCopy(tmp_vec, center);
				break;
			case RADIUS :
				get_num();
				radius = cur_value;
				break;
			case FUZZ :
				get_num();
				fuzz = cur_value;
				break;
			case CLIP :
				yy_clip();
				break;
			default :
				yyerror("Unknown sphere element.");
				break;
		}       /* end of sphere switch */
	}       /* end of while loop looking for right brace */

	if(TransTop) {
		trans_vector(TransTop->mat, center, center);
		radius  /= TransTop->mat[3][3];
		fuzz    /= TransTop->mat[3][3];
	}

	new_obj = MakeSphere(center, radius, fuzz);
	new_obj->next = Root;
	Root = new_obj;

	++nPrims;
	yystats();
}       /* end of yy_sphere() */

/*
	yy_cone() -- Parse a cone.
*/
void yy_cone()
{
	Vec     apex, base;
	Flt     arad, brad;
	Object  *new_obj;

	/* grab and toss left brace */
	if(get_token() != LEFT_BRACE) {
		yyerror("Left brace expected.");
	}

	while(get_token() != RIGHT_BRACE) {
		switch(cur_token) {
			case APEX :
				get_vec();
				VecCopy(tmp_vec, apex);
				break;
			case BASE :
				get_vec();
				VecCopy(tmp_vec, base);
				break;
			case APEX_RADIUS :
				get_num();
				arad = cur_value;
				break;
			case BASE_RADIUS :
				get_num();
				brad = cur_value;
				break;
			case RADIUS :
				get_num();
				arad = cur_value;
				brad = cur_value;
				break;
			case CLIP :
				yy_clip();
				break;
			default :
				yyerror("Unknown cone element.");
				break;
		}       /* end of cone switch */
	}       /* end of while loop looking for right brace */

	if(TransTop) {
		trans_vector(TransTop->mat, apex, apex);
		trans_vector(TransTop->mat, base, base);
		arad /= TransTop->mat[3][3];
		brad /= TransTop->mat[3][3];
	}

	new_obj = MakeCone(apex, arad, base, brad);
	new_obj->next = Root;
	Root = new_obj;

	++nPrims;
	yystats();
}       /* end of yy_cone() */

/*
	yy_ring() -- Parse a ring.
*/
void yy_ring()
{
	Vec     center, normal;
	Flt     min_rad, max_rad;
	Object  *new_obj;

	min_rad = 0.0;

	/* grab and toss left brace */
	if(get_token() != LEFT_BRACE) {
		yyerror("Left brace expected.");
	}

	while(get_token() != RIGHT_BRACE) {
		switch(cur_token) {
			case CENTER :
				get_vec();
				VecCopy(tmp_vec, center);
				break;
			case NORMAL :
				get_vec();
				VecCopy(tmp_vec, normal);
				break;
			case MIN_RADIUS :
				get_num();
				min_rad = cur_value;
				break;
			case MAX_RADIUS :
			case RADIUS :
				get_num();
				max_rad = cur_value;
				break;
			case CLIP :
				yy_clip();
				break;
			default :
				yyerror("Unknown ring element.");
				break;
		}       /* end of ring switch */
	}       /* end of while loop looking for right brace */

	if(TransTop) {
		trans_vector(TransTop->mat, center, center);
		trans_normal(TransTop->mat, normal, normal);
		min_rad /= TransTop->mat[3][3];
		max_rad /= TransTop->mat[3][3];
	}

	new_obj = MakeRing(center, normal, min_rad, max_rad);
	new_obj->next = Root;
	Root = new_obj;

	++nPrims;
	yystats();
}       /* end of yy_ring() */

/*
	yy_polygon() -- Parse a polygon.
*/
void yy_polygon()
{
	int     num_pnts, i;
	Vec     *vlist;
	Object  *new_obj;

	/* grab and toss left brace */
	if(get_token() != LEFT_BRACE) {
		yyerror("Left brace expected.");
	}

	/* get number of points */

	get_token();
	if(cur_token != POINTS) {
		yyerror("Number of points in polygon needed.");
	}
	get_num();
	num_pnts = cur_value;

	vlist = (Vec *)vmalloc(num_pnts * sizeof(Vec));
	ptrchk(vlist, "polygon vertex list");
	i = 0;

	while(get_token() != RIGHT_BRACE) {
		switch(cur_token) {
			case VERTEX :
				get_vec();
				VecCopy(tmp_vec, vlist[i]);
				if(TransTop) {
					trans_vector(TransTop->mat, vlist[i], vlist[i]);
				}
				i++;
				break;
			case CLIP :
				yy_clip();
				break;
			default :
				yyerror("Unknown polygon element.  Vertex expected.");
				break;
		}       /* end of ring switch */
	}       /* end of while loop looking for right brace */

	if(i != num_pnts) {
		yyerror("Number of vertices expected does not match data.");
	}

	new_obj = MakePoly(num_pnts, vlist);
	new_obj->next = Root;
	Root = new_obj;

	++nPrims;
	yystats();
}       /* end of yy_polygon() */

/*
	yy_patch() -- Parse a triangular patch.
*/
void yy_patch()
{
	Vec     data[6];
	int     v, n;
	Object  *new_obj;

	/* grab and toss left brace */
	if(get_token() != LEFT_BRACE) {
		yyerror("Left brace expected.");
	}

	v = n = 0;
	while(get_token() != RIGHT_BRACE) {
		switch(cur_token) {
			case VERTEX :
				get_vec();
				VecCopy(tmp_vec, data[v*2]);
				++v;
				break;
			case NORMAL :
				get_vec();
				VecCopy(tmp_vec, data[n*2+1]);
				++n;
				break;
			case CLIP :
				yy_clip();
				break;
			default :
				yyerror("Unknown triangular patch element.");
				break;
		}       /* end of patch switch */
	}       /* end of while loop looking for right brace */

	if(n!=3 || v!=3) {
		yyerror("Patches must have 3 vertices and normals each.");
	}

	if(TransTop) {
		trans_vector(TransTop->mat, data[0], data[0]);
		trans_vector(TransTop->mat, data[2], data[2]);
		trans_vector(TransTop->mat, data[4], data[4]);
		trans_normal(TransTop->mat, data[1], data[1]);
		trans_normal(TransTop->mat, data[3], data[3]);
		trans_normal(TransTop->mat, data[5], data[5]);
	}

	new_obj = MakeTri(data);
	new_obj->next = Root;
	Root = new_obj;

	++nPrims;
	yystats();
}       /* end of yy_patch() */

/*
	yyparse() -- The main entry point into the parser.
*/

int     yyparse()
{
	while(get_token() != END_OF_FILE) {
		switch(cur_token) {
			case NEWFILE :
				get_token();    /* get file name */
				yy_newfile(cur_text);
				break;
			case POPFILE :
				yy_popfile();
				break;
			case STUDIO :
				yy_studio();
				break;
			case LIGHT :
				yy_light();
				break;
			case SURFACE :
				yy_surface();
				break;
			case TRANSFORM :
				yy_transform();
				break;
			case TRANS_POP :
				yy_transform_pop();
				break;
			case GLOBAL_CLIP :
				yy_global_clip();
				break;
			case CLIP_POP :
				yy_clip_pop();
				break;
			case SPHERE :
				yy_sphere();
				break;
			case CONE :
				yy_cone();
				break;
			case RING :
				yy_ring();
				break;
			case POLYGON :
				yy_polygon();
				break;
			case PATCH :
				yy_patch();
				break;
			default :
				fprintf(stderr, "\nError parsing, yyparse() found token %d '%s'\n", cur_token, cur_text);
				yyerror("Unknown token.\n");
				break;
		}       /* end of big switch */
	}       /* end of big loop */

	return 0;       /* successful */

}       /* end of yyparse() */

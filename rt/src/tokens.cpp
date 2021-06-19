/*
�������������������������������������������
�                                                                         �
�                             Bob Ray Tracer                              �
�                                                                         �
�             Tokens.C = input file converted to tokens for parser        �
�                                                                         �
�       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     �
�                                                                         �
�       ALL RIGHTS RESERVED.   This software is published, but is NOT     �
�         Public Domain and remains the propery of ALGORITHM, Inc.,       �
�   Christopher D. Watkins and Stephen B. Coy.  This software may not be  �
�  reproduced or integrated into other packages without the prior written �
�          consent of Christopher D. Watkins and Stephen B. Coy.          �
�                                                                         �
�                       Requires: defs.h, extern.h                        �
�                                                                         �
�������������������������������������������

    tokens -- This is where the input file is read in and
        broken up into tokens which are then fed to the
        parser as it asks for them.  Notice that it treats
        parentheses as white space.
*/

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <ctype.h>
#include "Bob.hpp"
#include "defs.hpp"
#include "extern.hpp"
#include "tokens.hpp"
#include "proto.hpp"

#define NUM_TOKENS      (98)

static int      token_pushed=0;

typedef struct t_token {
    const char    *name;
    int     value;
} Token;

Token token_list[] = {
    "bb_newfile",   NEWFILE,
    "bb_popfile",   POPFILE,
    "studio",       STUDIO,
    "from",         FROM,
    "at",           AT,
    "up",           UP,
    "angle",        ANGLE,
    "resolution",   RESOLUTION,
    "background",   BKG,
    "start",        START,
    "stop",         STOP,
    "aspect",       ASPECT,
    "ambient",      AMBIENT,
    "haze",         HAZE,
    "aperture",     APERTURE,
    "focal_length", FOCAL_LENGTH,
    "antialias",    ANTIALIAS,
    "depth",        DEPTH,
    "no_shadows",   NO_SHADOWS,
    "samples",      SAMPLES,
    "jitter",       JITTER,
    "threshold",    THRESHOLD,
    "caustics",     CAUSTICS,
    "no_exp_trans", NO_EXP_TRANS,
    "no_antialias", NO_ANTIALIAS,
    "none",         NONE,
    "corners",      CORNERS,
    "adaptive",     ADAPTIVE,
    "quick",        QUICK,
    "bunching",     BUNCHING,
    "projection",   PROJECTION,
    "flat",         FLAT,
    "orthogonal",   ORTHO,
    "width",        WIDTH,
    "fisheye",      FISHEYE,
    "no_parallax",  NO_PARALLAX,
    "transform",    TRANSFORM,
    "transform_pop",TRANS_POP,
    "rotate",       ROTATE,
    "translate",    TRANSLATE,
    "scale",        SCALE,
    "light",        LIGHT,
    "type",         TYPE,
    "point",        POINT,
    "spherical",    SPHERICAL,
    "spot",         SPOT,
    "direction",    DIRECTION,
    "directional",  DIRECTIONAL,
    "min_angle",    MIN_ANGLE,
    "max_angle",    MAX_ANGLE,
    "no_spec",      NOSPEC,
    "color",        COLOR,
    "falloff",      FALLOFF,
    "surface",      SURFACE,
    "diffuse",      DIFFUSE,
    "specular",     SPECULAR,
    "transparent",  TRANSPARENT,
    "shine",        SHINE,
    "ior",          IOR,
    "fuzz",         FUZZ,
    "texture",      TEXTURE,
    "bump",         BUMP,
    "pattern",      PATTERN,
    "checker",      CHECKER,
    "noise",        NOISE,
    "turbulence",   TURBULENCE,
    "offset",       OFFSET,
    "terms",        TERMS,
    "amplitude",    AMP,
    "wave",         WAVE,
    "wavelength",   LENGTH,
    "blend",        BLEND,
    "image",        IMAGE,
    "across",       ACROSS,
    "damping",      DAMPING,
    "phase",        PHASE,
    "cone",         CONE,
    "sphere",       SPHERE,
    "polygon",      POLYGON,
    "patch",        PATCH,
    "ring",         RING,
    "vertex",       VERTEX,
    "points",       POINTS,
    "center",       CENTER,
    "position",     CENTER,         /* synonym for center */
    "radius",       RADIUS,
    "min_radius",   MIN_RADIUS,
    "max_radius",   MAX_RADIUS,
    "base",         BASE,
    "apex",         APEX,
    "base_radius",  BASE_RADIUS,
    "apex_radius",  APEX_RADIUS,
    "normal",       NORMAL,
    "clip",         CLIP,
    "inside",       INSIDE,
    "outside",      OUTSIDE,
    "global_clip",  GLOBAL_CLIP,
    "clip_pop",     CLIP_POP
};

/*
    match_token() -- compares the string in cur_test with the list
        of valid tokens.  If a match is found then the token's
        value is returned.  If no match is found then UNKNOWN
        is returned.  This could be sped up quite a bit by sorting
        the list of tokens alphabetically and doing a binary search.
        Note that this only compares the first "length" characters.
        This allows the parser to recognize abbreviations such
        as "surf" for "surface".  This feature can cause some
        problems eg the token "transform" could be mistaken for
        an abbreviation for the token "transform_pop".  Therefore,
        "transform" must come in the list before "transform_pop".
        Since a binary search will not ensure the order of the
        tokens being compared (with a LOT of work) the ability to
        recognize abbreviations would have to be eliminated before
        a binary search was used.  Speed vs flexibility.  Classic.
        Of course a dozen other ways to get both speed and
        flexibilty should be quite easy to come up with.
*/

int     match_token()
{
    int     i, length;

    length = strlen(cur_text);
    for(i=0; i<NUM_TOKENS; i++) {
        if(!strncmp(cur_text, token_list[i].name, length)) {
            cur_token = token_list[i].value;
            return cur_token;
        }
    }
    return UNKNOWN;
}       /* end of match_token() */


/*
    get_token() -- Gets the next token from the input stream and
        returns it's value.  If the previous token has been
        pushed back, then it is returned again.
*/

int     get_token()
{
    int     c, i;

    if(token_pushed) {
        token_pushed = 0;
        return cur_token;
    }

    /* no token waiting so we have to build one */

    cur_text[0] = 0;
    i = 0;

    /* get rid of any whitespace */
    while(isspace(c = fgetc(yyin)) || c=='(' || c==')') {
        if(c == '\n') {
            yylinecount++;
        }
    }

    /* check for end of file */
    if(c == EOF) {
        cur_token = END_OF_FILE;
        return cur_token;
    }

    if(c == '{') {
        cur_token = LEFT_BRACE;
        return LEFT_BRACE;
    } else if(c == '}') {
        cur_token = RIGHT_BRACE;
        return RIGHT_BRACE;
    } else if(c== '\"') {   /* start of quoted string, most likely a file name */
        while((c = fgetc(yyin)) != '\"') {
            cur_text[i++] = c;
        }
        cur_text[i] = 0;        /* null terminate the string */
        cur_token = UNKNOWN;
        return cur_token;
    } else if(isalpha(c)) {         /* must be a keyword */
        do {
            cur_text[i++] = c;
            c = fgetc(yyin);
        } while(isalnum(c) || c=='_' || c=='.' || c=='\\' || c==':');
        ungetc(c, yyin);        /* push back the character that doesn't belong */

        cur_text[i] = 0;        /* null terminate */

        cur_token = match_token();
        return cur_token;
    } else if(isdigit(c) || c=='+' || c=='.' || c=='-') {
        do {
            cur_text[i++] = c;
            c = fgetc(yyin);
        } while(isdigit(c) || c=='+' || c=='.' || c=='-' || c=='e' || c=='E');
        ungetc(c, yyin);        /* push back the character that doesn't belong */

        cur_text[i] = 0;
        cur_value = atof(cur_text);
        cur_token = NUMBER;
        return cur_token;
    }

    /* if we get down here something is really wrong */

    fprintf(stderr, "\nError parsing.  Found the character '%c' aka 0x%02x and\n", c, c);
    fprintf(stderr, "I don't know what to do with it.\n");
    Bob::getApp().parser.yyerror("");
    return 0;
}       /* end of get_token() */

/*
    push_token() -- Sets a flag which indicates that the previous
        token was not used by the parser and should therefore
        be returned by the next call to get_token.
*/

int     push_token()
{
    token_pushed = 1;

    return cur_token;       /* why?  I don't know */
}       /* end of push_token() */


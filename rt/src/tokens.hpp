/*
*******************************************
*                                                                         *
*                             Bob Ray Tracer                              *
*                                                                         *
*          Tokens.H = contains the token values used by the parser        *
*               as returned by the lexer.                                 *
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

/* misc stuff */

#define END_OF_FILE     (-1)
#define UNKNOWN         (0)
#define NEWFILE         (1)
#define POPFILE         (2)
#define NUMBER          (3)
#define COMMENT         (4)

/* punctuation */

#define LEFT_BRACE      (10)
#define RIGHT_BRACE     (11)

/* studio */

#define STUDIO          (20)
#define FROM            (21)
#define AT              (22)
#define UP              (23)
#define ANGLE           (24)
#define RESOLUTION      (25)
#define BKG             (26)
#define START           (27)
#define STOP            (28)
#define ASPECT          (29)
#define AMBIENT         (30)
#define HAZE            (31)
#define APERTURE        (32)
#define FOCAL_LENGTH    (33)
#define ANTIALIAS       (34)
#define DEPTH           (35)
#define NO_SHADOWS      (36)
#define SAMPLES         (37)
#define JITTER          (38)
#define THRESHOLD       (39)
#define CAUSTICS        (40)
#define NO_EXP_TRANS    (41)
#define NO_ANTIALIAS    (42)
#define NONE            (43)
#define CORNERS         (44)
#define ADAPTIVE        (45)
#define QUICK           (46)
#define BUNCHING        (47)
#define PROJECTION      (48)
#define FLAT            (49)
#define ORTHO           (50)
#define WIDTH           (51)
#define FISHEYE         (52)
#define NO_PARALLAX     (53)

/* transformations */

#define TRANSFORM       (70)
#define TRANS_POP       (71)
#define ROTATE          (72)
#define TRANSLATE       (73)
#define SCALE           (74)

/* lights */

#define LIGHT           (80)
#define TYPE            (81)
#define POINT           (82)
#define SPHERICAL       (83)
#define SPOT            (84)
#define DIRECTIONAL     (85)
#define MIN_ANGLE       (86)
#define MAX_ANGLE       (87)
#define NOSPEC          (88)
#define COLOR           (89)
#define FALLOFF         (90)
#define DIRECTION       (91)

/* surfaces */

#define SURFACE         (100)
#define DIFFUSE         (101)
#define SPECULAR        (102)
#define TRANSPARENT     (103)
#define SHINE           (104)
#define IOR             (105)
#define FUZZ            (106)
#define TEXTURE         (107)
#define BUMP            (108)
#define PATTERN         (109)
#define CHECKER         (110)
#define NOISE           (111)
#define TURBULENCE      (112)
#define OFFSET          (113)
#define TERMS           (114)
#define AMP             (115)
#define WAVE            (116)
#define LENGTH          (117)
#define BLEND           (118)
#define IMAGE           (119)
#define ACROSS          (120)
#define DAMPING         (121)
#define PHASE           (122)

/* primitives */

#define CONE            (130)
#define SPHERE          (131)
#define POLYGON         (132)
#define PATCH           (133)
#define RING            (134)
#define VERTEX          (135)
#define POINTS          (136)
#define CENTER          (137)
#define RADIUS          (138)
#define MIN_RADIUS      (139)
#define MAX_RADIUS      (140)
#define NORMAL          (141)
#define BASE            (142)
#define APEX            (143)
#define BASE_RADIUS     (144)
#define APEX_RADIUS     (145)

/* clips */

#define CLIP            (160)
#define INSIDE          (161)
#define OUTSIDE         (162)
#define GLOBAL_CLIP     (163)
#define CLIP_POP        (164)

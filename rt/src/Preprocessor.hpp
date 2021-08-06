#pragma once
#include <cstdio>
#include "String.hpp"

/* preprocessor macro structure */
typedef struct t_macro {
    char *macro,          /* define'd word */
        *text;            /* text to replace macro with */
    int mlen,             /* length of macro */
        tlen;             /* length of text */
    struct t_macro *next; /* stack link */
} Macro;

class Preprocessor {
    private:
    static void  expand (char *src);
    static void  sub_macro (Macro *mptr , char *loc);
    static void  add_macro (char *txt);
    static void  remove_macro (char *str);
    static char *brute (char *text , const char *pat , int tlen , int plen);
    static void  clean_up (void);
    static int   vfgets (char *dst , int max_count , FILE *fp);
    static char *get_next_token (char *text);
    static char *brute(char *text, char *pattern, int tlen, int plen);
    static int   cpy_tok(char *dst, char *src);
    static int   is_tok(int c);

    public:
    static int preproc(const String &infile, const String &outfile);

};

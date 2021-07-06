/*
�������������������������������������������
�                                                                         �
�                             Bob Ray Tracer                              �
�                                                                         �
�   PreProc.C = preprocessor for Bob.  Handles macros and include files.  �
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

    Expects 2 arguments: infile outfile
    Returns 0 if ok, exits otherwise

    Macros must start with an alpha and may contain alphanum and '_'

    Note: Macros are sorted longest to shortest.
*/

#include "Bob.hpp"
#include "Exception.hpp"
#include "defs.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <iostream>
//#include "Object_3D.hpp"
//#include "struct_defs.hpp"
#include "extern.hpp"
#include "proto.hpp"

using std::cerr;
using std::cout;
using std::endl;

extern char *brute(char *text, char *pattern, int tlen, int plen);
extern char *get_next_token(char *text);
extern int cpy_tok(char *dst, char *src);
extern int is_tok(int c);

#define BLANK ' '

#define MAX_LEVEL (20)
#define MAX_LINE (1024) /* longest input line */
#define MAX_TOK (80)
#define MAX_EXPAND (32767) /* largest size to expand a line to */

static Macro *MacroHead = NULL;

static char *line, /* fully expanded line */
    *mline;        /* multi-line macro */
static int cur;
static FILE *fp[MAX_LEVEL], *outfp;

int preproc(const String &infile, const String &outfile) {
    char str[MAX_LINE], newfile[MAX_LINE];
    char command[MAX_TOK];
    char *rest, *cptr;

    if (tickflag) {
        cout << "Preprocessing file:\n\t" << infile << endl;
    }

    line = new char[MAX_EXPAND]();
    Bob::getApp().parser.ptrchk(line, "preprocessor space");
    mline = new char[MAX_EXPAND]();
    Bob::getApp().parser.ptrchk(mline, "preprocessor space");

    outfp = fopen(outfile.c_str(), "w");
    if (!outfp) {
        cerr << "Preprocessor error opening temp file " << outfile << " for output." << endl;
        throw Exception("thrown from preproc");
    } else {
        cout << "Opened " << outfile << " for writing." << endl;
    }

    cur = 0;
    fp[cur] = env_fopen(infile, "r");
    if (!fp[cur]) {
        cerr << "fp[cur]: " << fp[cur] << endl;
        cerr << "Preprocessor error opening file, infile(" << infile << "), for input." << endl;
        throw Exception("thrown from preproc");
    }

    fprintf(outfp, "bb_newfile \"%s\"\n", infile.c_str());
    do {
        if (vfgets(str, MAX_LINE, fp[cur]) == 0) {
            fclose(fp[cur]);
            cur--;
            fputs("bb_popfile\n", outfp);
        } else {
            rest = str;
            /* strip leading whitespace  */
            while (*rest == ' ' || *rest == '\t')
                rest++;

            /* take out eol type comments */
            if ((cptr = brute(rest, "//", strlen(rest), 2)) != NULL) {
                strcpy(cptr, "\n\0");
            }

            if (*rest == '#') { /* preprocessor command? */
                sscanf(rest, "#%s", command);
                rest += strlen(command) + 1;
                if (strcmp(command, "include") == 0) {
                    sscanf(rest, "%s", newfile);
                    cur++;
                    fp[cur] = env_fopen(newfile, "r");
                    if (!fp[cur]) {
                        cerr << "fp[cur]: " << fp[cur] << endl;
                        cerr << "Preprocessor error opening file, newfile(" << newfile << "), for input." << endl;
                        throw Exception("thrown from preproc");
                    }
                    fprintf(outfp, "bb_newfile \"%s\"\n", newfile);
                    if (tickflag) {
                        cout << "\t" << newfile << endl;
                    }
                } else if (strcmp(command, "define") == 0) {
                    while (*rest == ' ' || *rest == '\t')
                        rest++;
                    add_macro(rest);
                    fputc('\n', outfp);
                } else if (strcmp(command, "undef") == 0) {
                    while (*rest == ' ' || *rest == '\t')
                        rest++;
                    remove_macro(rest);
                    fputc('\n', outfp);
                } else {
                    cerr << "Unknown preprocessor command \"" << command << "\"" << endl;
                    throw Exception("Thrown from preproc");
                }
            } else {          /* just a normal line */
                expand(rest); /* expands to line */
                fputs(line, outfp);
            }
        }
    } while (cur >= 0);

    fclose(outfp);

    clean_up();

    for (int i = 0; i < 30; i++)
        cout << endl;
    return 0;
} /* end of preproc() */

/*
    expand() -- Remove excess white space and expand any macros in
        the line passed.  Return new text in line.
*/

void expand(char *src) {
    char *ptr, token[MAX_TOK];
    int len, found;
    Macro *mptr;

    /* waste leading white space */
    while (*src == ' ' || *src == '\t') {
        src++;
    }
    strcpy(line, src);

    /* do the expansion */
    ptr = line;
    while (ptr = get_next_token(ptr)) {
        len = cpy_tok(token, ptr); /* get token and length */
        /* go to macros with matching length */
        mptr = MacroHead;
        while (mptr && mptr->mlen > len) {
            mptr = mptr->next;
        }
        /* look for a matching macro */
        found = 0;
        while (mptr && mptr->mlen == len) {
            if (strcmp(mptr->macro, token) == 0) { /* match? */
                sub_macro(mptr, ptr);
                found = 1;
                break; /* found it so break out of loop */
            }
            mptr = mptr->next;
        }
        if (!found) { /* move beyond token since not a macro */
            ptr += len;
        }
    } /* end of tokens in line */
} /* end of expand() */

/*
    sub_macro() -- do a macro substitution into a line.
*/
void sub_macro(Macro *mptr, char *loc) {
    int len, offset, i;
    char *src, *dst;

    /* push rest of line */
    len = strlen(loc);
    offset = mptr->tlen - mptr->mlen + 2; /* calc how much to push */
    if (offset > 0) {
        src = loc + len; /* point to NULL */
        dst = src + offset;
        i = len - mptr->mlen + 1;
        while (i--) {
            *dst-- = *src--;
        }
    } else if (offset < 0) {
        strncpy(loc, loc - offset, len);
    }

    /* paste in new text with surrounding blanks */
    strncpy(loc + 1, mptr->text, mptr->tlen);
    *loc = BLANK;
    loc += mptr->tlen + 1;
    *loc = BLANK;

} /* end of sub_macro() */

/*
    add_macro() -- add a new macro to the list.
        Macros are sorted largest to smallest.
*/

void add_macro(char *txt) {
    Macro *mptr, *sptr;
    char macro[MAX_TOK];
    char *cptr, *cptr2;

    /* copy to mline and keep adding until end of macro */
    strcpy(mline, txt);
    while (1) {
        txt = mline + strlen(mline) - 1;
        while (isspace(*txt)) { /* move txt back to last non-white char */
            txt--;
        }
        if (*txt == '\\') { /* continuation character? */
            txt--;
            while (isspace(*txt)) { /* move back to last non-white char */
                txt--;
            }
            txt++;
            *txt = ' ';
            txt++; /* put a blank between lines */
            cptr = line;
            vfgets(cptr, MAX_LINE, fp[cur]);
            while (isspace(*cptr)) {
                cptr++;
            }

            /* take out eol type comments */
            if ((cptr2 = brute(cptr, "//", strlen(cptr), 2)) != NULL) {
                strcpy(cptr2, "\n\0");
            }
            strcpy(txt, cptr);
            fputc('\n', outfp); /* keep those line #'s straight */
        } else {
            break;
        }
    }
    txt = mline;

    /* create new link and fill it in */

    mptr = new Macro();
    Bob::getApp().parser.ptrchk(mptr, "macro structure");

    sscanf(txt, "%s", macro);
    mptr->macro = strdup(macro);
    Bob::getApp().parser.ptrchk(mptr->macro, "macro define");
    mptr->mlen = strlen(mptr->macro);

    txt += mptr->mlen;
    expand(txt); /* expand the macro def to line */
    mptr->text = strdup(line);
    Bob::getApp().parser.ptrchk(mptr->text, "macro text");
    mptr->tlen = strlen(mptr->text);

    /* toss \n \c at end of macro by replacing with ' ' */
    txt = mptr->text;
    while (*txt) {
        if (*txt == '\n' || *txt == 13)
            *txt = BLANK;
        txt++;
    }

    /* add to list, sort by mlen, largest to smallest */

    if (MacroHead == NULL || MacroHead->mlen <= mptr->mlen) { /* put at top of list */
        mptr->next = MacroHead;
        MacroHead = mptr;
    } else { /* have to sort */
        sptr = MacroHead;
        while (sptr->next && sptr->next->mlen > mptr->mlen) {
            sptr = sptr->next;
        }
        mptr->next = sptr->next;
        sptr->next = mptr;
    }

} /* end of add_macro() */

void remove_macro(char *str) {
    Macro *mptr, *mptr2;
    int len;
    char *cptr;

    /* clean off any following garbage */
    cptr = str;
    while (is_tok(*cptr)) {
        cptr++;
    }
    *cptr = 0; /* put a NULL at first non-token character */

    mptr = MacroHead;
    len = strlen(str);

    /* check if at top of list */
    if (strcmp(str, mptr->macro) == 0) { /* winner! */
        MacroHead = MacroHead->next;
        free(mptr->macro);
        free(mptr->text);
        free(mptr);
    } else {                                           /* gotta look for it */
        while (mptr->next && mptr->next->mlen > len) { /* skip cruft */
            mptr = mptr->next;
        }
        while (mptr->next && mptr->next->mlen == len) { /* while possible */
            if (strcmp(str, mptr->next->macro) == 0) {  /* wiener! */
                mptr2 = mptr->next;
                mptr->next = mptr2->next; /* remove from list */
                free(mptr2->macro);       /* free up */
                free(mptr2->text);
                free(mptr2);

                return;
            } else {
                mptr = mptr->next;
            }
        }
    }
} /* end of remove_macro() */

/*
    brute() -- brute force pattern matching
*/

char *brute(char *text, const char *pat, int tlen, int plen) {
    int i = 0, j = 0;

    i = j = 0;
    do {
        if (text[i] == pat[j] && (j != 0 || i == 0 || isspace(text[i - 1]) || (ispunct(text[i - 1]) && text[i - 1] != '_'))) {
            i++;
            j++;
        } else {
            i = i - j + 1;
            j = 0;
        }
    } while (j < plen && i < tlen);

    if (j == plen)
        return text + i - plen;
    else
        return NULL;
}

/*
    clean_up() -- free up memory allocated for preprocessing
*/

void clean_up() {
    Macro *mptr;

    free(line);
    free(mline);

    while (MacroHead) {
        mptr = MacroHead;
        MacroHead = MacroHead->next;
        free(mptr->macro);
        free(mptr->text);
        free(mptr);
    }
}

/*
    homegrown fgets type function which takes care of eliminating
    comments and purging whitespace.
*/

int vfgets(char *dst, int max_count, FILE *fp) {
    int c = 0, p = 0, /* current and previous characters */
        count;        /* current line count */

    if (feof(fp)) {
        return 0;
    }

    c = p = count = 0;
    do {
        p = c;
        c = fgetc(fp);
        if (c == EOF) {
            *dst++ = '\n';
            *dst = '\0';
            return 1;
        }
        if (p == '/' && c == '*') { /* we're in a comment */
            dst--;                  /* back off the '/' */
            count--;
            p = c = 0;
            do { /* go until end of comment */
                p = c;
                c = fgetc(fp);
                if (c == '\n') {
                    fputc('\n', outfp); /* keep those line #'s straight */
                }
            } while (p != '*' || c != '/');
        } else {
            *dst++ = c;
            count++;
            if (count >= max_count) {
                cerr << "Preprocessor error, input line too long." << endl;
                cerr << "Input lines must be less than " << MAX_LINE << " characters in length." << endl;
                throw Exception("Thrown from vfgets");
            }
        }
    } while (c != '\n');
    *dst = '\0';

    return 1;
} /* end of vfgets() */

/*
    get_next_token() -- find the beginning of the next token in the
        text string and return its location.
*/

char *get_next_token(char *text) {
    while (*text && isalpha(*text) == 0) {
        text++;
    }
    if (*text) {
        return text;
    } else {
        return NULL;
    }
} /* end of get_next_token */

/*
    cpy_tok() -- Copy the token at src to dst and return its length.
*/

int cpy_tok(char *dst, char *src) {
    int cnt = 0;

    while (is_tok(*src)) {
        *dst++ = *src++;
        cnt++;
    }
    *dst = '\0';

    return cnt;
} /* end of cpu_tok() */

/*
    is_tok(c) -- determine if c is a valid token character.
        Valid token characters are:
            A..Z a..z _ 0..9
        returns 1 is yes, 0 if no
*/

int is_tok(int c) {
    if (isalnum(c) || c == '_' || c == '.' || c == '\\' || c == ':') {
        return 1;
    } else {
        return 0;
    }
} /* end of is_tok() */

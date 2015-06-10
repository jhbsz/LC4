#ifndef _LANG_H
#define _LANG_H

#define LANG_MAP_FILE "./map"

enum lang {
    EN = 0,
    TW,
    GE,
    GB,
    SP,
    FR,
    EN_PROX,
    JP,
    _FR,
    DE,
    IT,
    NL,
    ES,
    KO,
    C_T,
    C_S,
    AR,
    TK
};

struct language {
    char *name;
    int value;
    char *charset;
};

extern struct language lang_map[];
extern char *lang_charset;
extern int lang_get_index(const char *lang);
extern int lang_construct_translator(const char *fname, const char *lang);
extern char *lang_translate(const char *key);

#endif /*_LANG_H*/

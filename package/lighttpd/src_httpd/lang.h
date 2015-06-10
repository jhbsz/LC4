#ifndef _LANG_H
#define _LANG_H

#define LANG_MAP_FILE "/tmp/mnt/map"

enum lang {
    EN = 0,
    CE,
    DA,
    DE,
    EP,
    ES,
    EX,
    FR,
    IT,
    MA,
    NE,
    NO,
    PO,
    PB,
    RO,
    SU,
    SV,
    TU,
    PY,
    TA,
    JP,
    KR,
    SC,
    TW
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

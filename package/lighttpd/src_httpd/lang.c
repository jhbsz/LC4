#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __USE_GNU
#include <search.h>
#include "fcgi_common.h"
#include "fcgi_stdio.h" 

#include "lang.h"

#define BUFSIZE 4096

static char *get_lang_charset(const char *lang);
static void create_hash(int max);
static void destroy_hash();
static int insert_key_val_into_hash(const char *key, const char *val, int pos);

char *lang_charset = NULL;
int lang_index = -1;
struct language lang_map[] = {
    { "EN", EN, "utf-8" }, 
    { "CE", CE, "utf-8" }, 
    { "DA", DA, "utf-8" }, 
    { "DE", DE, "utf-8" }, 
    { "EP", EP, "utf-8" }, 
    { "ES", ES, "utf-8" }, 
    { "EX", EX, "utf-8" }, 
    { "FR", FR, "utf-8" }, 
    { "IT", IT, "utf-8" }, 
    { "MA", MA, "utf-8" }, 
    { "NE", NE, "utf-8" }, 
    { "NO", NO, "utf-8" }, 
    { "PO", PO, "utf-8" }, 
    { "PB", PB, "utf-8" }, 
    { "RO", RO, "utf-8" }, 
    { "SU", SU, "utf-8" }, 
    { "SV", SV, "utf-8" }, 
    { "TU", TU, "utf-8" }, 
    { "PY", PY, "utf-8" }, 
    { "TA", TA, "utf-8" }, 
    { "JP", JP, "utf-8" }, 
    { "KR", KR, "utf-8" }, 
    { "SC", SC, "utf-8" }, 
    { "TW", TW, "utf-8" }, 
    { NULL, -1, NULL }
};

struct mapstruct {
    struct hsearch_data htab;
    char **key_array;
    char **val_array;
    int num;
};

static struct mapstruct map = {
    key_array: NULL, val_array: NULL, num: 0
};

int
lang_get_index(const char *lang)
{
    const struct language *l;
    for (l = &lang_map[0]; l->name != NULL; l++) {
        if (strcmp(l->name, lang) == 0) {
            return l->value;
        }
    }

    return -1;
}

static char *
get_lang_charset(const char *lang)
{
    const struct language *l;
    for (l = &lang_map[0]; l->name != NULL; l++) {
        if (strcmp(l->name, lang) == 0) {
            return l->charset;
        }
    }
    return NULL;
}

static void
create_hash(int max)
{
    destroy_hash();

    hcreate_r(max, &map.htab);

    map.key_array = (char **)calloc(max, sizeof(char *));
    map.val_array = (char **)calloc(max, sizeof(char *));

    map.num = max;
}

static void
destroy_hash()
{
    int i;

    hdestroy_r(&map.htab);

    if (map.key_array) {
        for (i = 0; i < map.num; i++) {
            free(map.key_array[i]);
        }
        free(map.key_array);
        map.key_array = NULL;
    }

    if (map.val_array) {
        for (i = 0; i < map.num; i++) {
            free(map.val_array[i]);
        }
        free(map.val_array);
        map.val_array = NULL;
    }
}

/**
 * @return 0 for existing key/value. -1 for hash full. or otherwise.
 */
static int
insert_key_val_into_hash(const char *key, const char *val, int pos)
{
    ENTRY e, *ep;
    int rtn;

    map.key_array[pos] = (char *)malloc(strlen(key) + 1);
    strcpy(map.key_array[pos], key);
    map.val_array[pos] = (char *)malloc(strlen(val) + 1);
    strcpy(map.val_array[pos], val);

    e.key = map.key_array[pos];
    hsearch_r(e, FIND, &ep, &map.htab);
    if (ep) {
        ep->data = (void *)pos;
        return 0;
    } else {
        e.data = (void *)pos;
        rtn = hsearch_r(e, ENTER, &ep, &map.htab);
        if (rtn == 0) {
            return -1;
        } else {
            return rtn;
        }
    }
}

char *
lang_translate(const char *key)
{
    ENTRY e, *ep;
    e.key = (char *)key;
    hsearch_r(e, FIND, &ep, &map.htab);
#ifdef DEBUG_MAP
    if (!ep) {
        return "no map and lang ok";
    }
    return "map ok and lang ok ";
#else
    if (!ep) {
        return (char *) key;
    }
    return map.val_array[(int)ep->data];
#endif

}

int
lang_construct_translator(const char *fname, const char *lang)
{
    FILE *fp;
    char buf[BUFSIZE];
    char *tok, *running;
    int ln, count;
    int idx;
    int rtn;

    if (!fname || !lang) {
#ifdef DEBUG
        printf("%s: %s: NULL parameter (%s) (%s)\n", 
               __FILE__, __FUNCTION__, fname, lang);
#endif
        return -1;
    }

    ln = lang_get_index(lang);
    if(ln == lang_index) {
        /* Language map is not changed */
        return 0;
    }
    lang_index = ln;
    /* charset is used in HTTP header. */
    lang_charset = get_lang_charset(lang);

    if (!(fp = fopen(fname, "r"))) {
        FCGI_LOG("File (%s) open error\n", fname);
        return -1;
    }

    /* Create the hash table. */
    count = 0;
    while (fgets(buf, BUFSIZE, fp)) {
        count++;
    }

    create_hash(count);

    /* Build up the translation rules with the hash table. */
    fp = freopen(fname, "r", fp);
    idx = 0;
    while (fgets(buf, BUFSIZE, fp)) {
        int n = 0;
        char *key = NULL, *val = NULL;

        running = buf;
        while ((tok = strsep(&running, "\t\n"))) {
            if (n == 0) {
               key = tok;
            }

            if (n == ln) {
               val = tok;
               break;
            } 

            n++;
        }
        if (!*val || !*key) {
            FCGI_LOG("%i) Parsing error at (%s, %s)\n", idx+1, key, val);
            idx++;
            continue;
        }

        rtn = insert_key_val_into_hash(key, val, idx++);
        if (rtn == 0) {
            FCGI_LOG("%s: existing key/val = %s/%s\n", __FUNCTION__, key, val);
        } else if (rtn == -1) {
            FCGI_LOG("%s: hash full\n", __FUNCTION__);
            return 0;
        }
    }

    fclose(fp);
    return 0;
}

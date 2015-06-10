#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>
 
#define gisdigit    isdigit
#define T(s)                s
typedef char                char_t;

#define STR_REALLOC     0x1             /* Reallocate the buffer as required */
#define STR_INC         64              /* Growth increment */

#define BUFSIZE            1000        

#define balloc(B_ARGS, num) malloc(num)
#define bfree(B_ARGS, p) free(p)
#define brealloc(B_ARGS, p, num) realloc(p, num)

typedef struct {
    char_t  *s;                         /* Pointer to buffer */
    int     size;                       /* Current buffer size */
    int     max;                        /* Maximum buffer size */
    int     count;                      /* Buffer count */
    int     flags;                      /* Allocation flags */
} strbuf_t;

/*
 *  Sprintf formatting flags
 */
enum flag {
    flag_none = 0,
    flag_minus = 1,
    flag_plus = 2,
    flag_space = 4,
    flag_hash = 8,
    flag_zero = 16,
    flag_short = 32,
    flag_long = 64
};

static void put_char(strbuf_t *buf, char_t c)
{
    if (buf->count >= (buf->size - 1)) {
        if (! (buf->flags & STR_REALLOC)) {
            return;
        }
        buf->size += STR_INC;
        if (buf->size > buf->max && buf->size > STR_INC) {
            /*  Caller should increase
             *  the size of the calling buffer
             */
            buf->size -= STR_INC;
            return;
        }
        if (buf->s == NULL) {
            buf->s = balloc(B_L, buf->size * sizeof(char_t));
        } else {
            buf->s = brealloc(B_L, buf->s, buf->size * sizeof(char_t));
        }
    }
    buf->s[buf->count] = c;
    if (c != '\0') {
        ++buf->count;
    }
}

/*****************************************************************************/
/* Add a string to a string buffer
 */

static void put_string(strbuf_t *buf, char_t *s, int len, int width,
        int prec, enum flag f)
{
    int     i;

    if (len < 0) {
        len = strnlen(s, prec >= 0 ? prec : ULONG_MAX);
    } else if (prec >= 0 && prec < len) {
        len = prec;
    }
    if (width > len && !(f & flag_minus)) {
        for (i = len; i < width; ++i) {
            put_char(buf, ' ');
        }
    }
    for (i = 0; i < len; ++i) {
        put_char(buf, s[i]);
    }
    if (width > len && f & flag_minus) {
        for (i = len; i < width; ++i) {
            put_char(buf, ' ');
        }
    }
}



static void put_ulong(strbuf_t *buf, unsigned long int value, int base,
        int upper, char_t *prefix, int width, int prec, enum flag f)
{
    unsigned long   x, x2;
    int             len, zeros, i;

    for (len = 1, x = 1; x < ULONG_MAX / base; ++len, x = x2) {
        x2 = x * base;
        if (x2 > value) {
            break;
        }
    }
    zeros = (prec > len) ? prec - len : 0;
    width -= zeros + len;
    if (prefix != NULL) {
        width -= strnlen(prefix, ULONG_MAX);
    }
    if (!(f & flag_minus)) {
        if (f & flag_zero) {
            for (i = 0; i < width; ++i) {
                put_char(buf, '0');
            }
        } else {
            for (i = 0; i < width; ++i) {
                put_char(buf, ' ');
            }
        }
    }
    if (prefix != NULL) {
        put_string(buf, prefix, -1, 0, -1, flag_none);
    }
    for (i = 0; i < zeros; ++i) {
        put_char(buf, '0');
    }
    for ( ; x > 0; x /= base) {
        int digit = (value / x) % base;
        put_char(buf, (char) ((digit < 10 ? '0' : (upper ? 'A' : 'a') - 10) +
                    digit));
    }
    if (f & flag_minus) {
        for (i = 0; i < width; ++i) {
            put_char(buf, ' ');
        }
    }
}



static int dsnprintf(char_t **s, int size, char_t *fmt, va_list arg, int msize)
{
    strbuf_t    buf;
    char_t      c;


    memset(&buf, 0, sizeof(buf));
    buf.s = *s;

    if (*s == NULL || msize != 0) {
        buf.max = size;
        buf.flags |= STR_REALLOC;
        if (msize != 0) {
            buf.size = max(msize, 0);
        }
        if (*s != NULL && msize != 0) {
            buf.count = gstrlen(*s);
        }
    } else {
        buf.size = size;
    }

    while ((c = *fmt++) != '\0') {
        if (c != '%' || (c = *fmt++) == '%') {
            put_char(&buf, c);
        } else {
            enum flag f = flag_none;
            int width = 0;
            int prec = -1;
            for ( ; c != '\0'; c = *fmt++) {
                if (c == '-') {
                    f |= flag_minus;
                } else if (c == '+') {
                    f |= flag_plus;
                } else if (c == ' ') {
                    f |= flag_space;
                } else if (c == '#') {
                    f |= flag_hash;
                } else if (c == '0') {
                    f |= flag_zero;
                } else {
                    break;
                }
            }
            if (c == '*') {
                width = va_arg(arg, int);
                if (width < 0) {
                    f |= flag_minus;
                    width = -width;
                }
                c = *fmt++;
            } else {
                for ( ; gisdigit((int)c); c = *fmt++) {
                    width = width * 10 + (c - '0');
                }
            }
            if (c == '.') {
                f &= ~flag_zero;
                c = *fmt++;
                if (c == '*') {
                    prec = va_arg(arg, int);
                    c = *fmt++;
                } else {
                    for (prec = 0; gisdigit((int)c); c = *fmt++) {
                        prec = prec * 10 + (c - '0');
                    }
                }
            }
            if (c == 'h' || c == 'l') {
                f |= (c == 'h' ? flag_short : flag_long);
                c = *fmt++;
            }
            if (c == 'd' || c == 'i') {
                long int value;
                if (f & flag_short) {
                    value = (short int) va_arg(arg, int);
                } else if (f & flag_long) {
                    value = va_arg(arg, long int);
                } else {
                    value = va_arg(arg, int);
                }
                if (value >= 0) {
                    if (f & flag_plus) {
                        put_ulong(&buf, value, 10, 0, T("+"), width, prec, f);
                    } else if (f & flag_space) {
                        put_ulong(&buf, value, 10, 0, T(" "), width, prec, f);
                    } else {
                        put_ulong(&buf, value, 10, 0, NULL, width, prec, f);
                    }
                } else {
                    put_ulong(&buf, -value, 10, 0, T("-"), width, prec, f);
                }
            } else if (c == 'o' || c == 'u' || c == 'x' || c == 'X') {
                unsigned long int value;
                if (f & flag_short) {
                    value = (unsigned short int) va_arg(arg, unsigned int);
                } else if (f & flag_long) {
                    value = va_arg(arg, unsigned long int);
                } else {
                    value = va_arg(arg, unsigned int);
                }
                if (c == 'o') {
                    if (f & flag_hash && value != 0) {
                        put_ulong(&buf, value, 8, 0, T("0"), width, prec, f);
                    } else {
                        put_ulong(&buf, value, 8, 0, NULL, width, prec, f);
                    }
                } else if (c == 'u') {
                    put_ulong(&buf, value, 10, 0, NULL, width, prec, f);
                } else {
                    if (f & flag_hash && value != 0) {
                        if (c == 'x') {
                            put_ulong(&buf, value, 16, 0, T("0x"), width,
                                    prec, f);
                        } else {
                            put_ulong(&buf, value, 16, 1, T("0X"), width,
                                    prec, f);
                        }
                    } else {
                        put_ulong(&buf, value, 16, ('X' == c) , NULL, width, prec, f);
                    }
                }
            } else if (c == 'c') {
                char_t value = va_arg(arg, int);
                put_char(&buf, value);

            } else if (c == 's' || c == 'S') {
                char_t *value = va_arg(arg, char_t *);
                if (value == NULL) {
                    put_string(&buf, T("(null)"), -1, width, prec, f);
                } else if (f & flag_hash) {
                    put_string(&buf,
                            value + 1, (char_t) *value, width, prec, f);
                } else {
                    put_string(&buf, value, -1, width, prec, f);
                }
            } else if (c == 'p') {
                void *value = va_arg(arg, void *);
                put_ulong(&buf,
                        (unsigned long int) value, 16, 0, T("0x"), width, prec, f);
            } else if (c == 'n') {
                if (f & flag_short) {
                    short int *value = va_arg(arg, short int *);
                    *value = buf.count;
                } else if (f & flag_long) {
                    long int *value = va_arg(arg, long int *);
                    *value = buf.count;
                } else {
                    int *value = va_arg(arg, int *);
                    *value = buf.count;
                }
            } else {
                put_char(&buf, c);
            }
        }
    }
    if (buf.s == NULL) {
        put_char(&buf, '\0');
    }

    /*
     *  If the user requested a dynamic buffer (*s == NULL), ensure it
     *  is returned.
     */
    if (*s == NULL || msize != 0) {
        *s = buf.s;
    }

    if (*s != NULL && size > 0) {
        if (buf.count < size) {
            (*s)[buf.count] = '\0';
        } else {
            (*s)[buf.size - 1] = '\0';
        }
    }

    if (msize != 0) {
        return buf.size;
    }
    return buf.count;
}

int fmtValloc(char_t **s, int n, char_t *fmt, va_list arg)
{

    *s = NULL;
    return dsnprintf(s, n, fmt, arg, 0);
}

int
doSystem(char *fmt, ...)
{
    va_list     vargs;                                                            char_t      *cmd = NULL;
    int         rc = 0;

    va_start(vargs, fmt);
    if (fmtValloc(&cmd, BUFSIZE, fmt, vargs) >= BUFSIZE) {
        perror("doSystem: lost data, buffer overflow\n");
    }
    va_end(vargs);

    if (cmd) {
        printf("cmd:%s\n", cmd);
        rc = system(cmd);
        free(cmd);
    }
    if(WIFEXITED(rc))
        return WEXITSTATUS(rc);
    else
        return -1;
}

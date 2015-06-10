#define DBG_MEMORY_ALLOC
#include <string.h>
//static void*__malloc_dbg_ptr = NULL;
#warning "Using dbg malloc ------------------------------------------------------------------------ "
//static char __memdbgbuf[256];
//static int __memdbgbuf_strlen = 0;
#define __ax_malloc malloc
#define __ax_calloc calloc
#define __ax_realloc realloc
#define __ax_strdup strdup
#define __ax_strndup strndup
#define __ax_asprintf asprintf
#define __ax_free free

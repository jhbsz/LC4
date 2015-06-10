#define DBG_MEMORY_ALLOC
#ifdef DBG_MEMORY_ALLOC
#include <string.h>
static void*__malloc_dbg_ptr = NULL;
#warning "Using dbg malloc ------------------------------------------------------------------------ "
//#define __ax_malloc(__SZ__) ((__malloc_dbg_ptr = (void*)malloc(__SZ__)) + ((__fprintf_status = fprintf(stderr, "%s(%d) malloc:%p\n", __func__, __LINE__, __malloc_dbg_ptr)) * 0))
//#define __ax_calloc(__NUM__, __SZ__) ((__malloc_dbg_ptr = (void*)calloc(__NUM__, __SZ__)) + ((__fprintf_status = fprintf(stderr, "%s(%d) calloc:%p\n", __func__, __LINE__, __malloc_dbg_ptr)) * 0))
//#define __ax_realloc(__PTR__, __SZ__) ((__malloc_dbg_ptr = (void*)realloc(__PTR__, __SZ__)) + ((__fprintf_status = fprintf(stderr, "%s(%d) realloc:%p\n", __func__, __LINE__, __malloc_dbg_ptr)) * 0))
//#define __ax_strdup(__STR__) ((__malloc_dbg_ptr = (void*)strdup(__STR__)) + ((__fprintf_status = fprintf(stderr, "%s(%d) strdup alloc:%p\n", __func__, __LINE__, __malloc_dbg_ptr)) * 0))
//#define __ax_strndup(__STR__, __SZ__) ((__malloc_dbg_ptr = (void*)strndup(__STR__, __SZ__)) + ((__fprintf_status = fprintf(stderr, "%s(%d) strndup alloc:%p\n", __func__, __LINE__, __malloc_dbg_ptr)) * 0))
#define __ax_malloc(__SZ__) (fprintf(stderr, "%s(%d) malloc:%p\n", __func__, __LINE__, (__malloc_dbg_ptr = (void*)malloc(__SZ__))) ?  __malloc_dbg_ptr : NULL)
#define __ax_calloc(__NUM__, __SZ__) (fprintf(stderr, "%s(%d) calloc:%p\n", __func__, __LINE__,(__malloc_dbg_ptr = (void*)calloc(__NUM__, __SZ__))) ? __malloc_dbg_ptr : NULL)
#define __ax_realloc(__PTR__, __SZ__) (fprintf(stderr, "%s(%d) realloc:%p\n", __func__, __LINE__, (__malloc_dbg_ptr = (void*)realloc(__PTR__, __SZ__))) ? __malloc_dbg_ptr : NULL)
#define __ax_strdup(__STR__) (fprintf(stderr, "%s(%d) strdup alloc:%p\n", __func__, __LINE__, (__malloc_dbg_ptr = (void*)strdup(__STR__))) ? __malloc_dbg_ptr : NULL)
#define __ax_strndup(__STR__, __SZ__) (fprintf(stderr, "%s(%d) strndup alloc:%p\n", __func__, __LINE__, (__malloc_dbg_ptr = (void*)strndup(__STR__, __SZ__))) ? __malloc_dbg_ptr : NULL)
#define __ax_asprintf(__STRBUF__, __FMT__, ...) { asprintf(__STRBUF__, __FMT__, ##__VA_ARGS__);   fprintf(stderr, "%s(%d) asprintf alloc:%p\n", __func__, __LINE__, * __STRBUF__); }
#define __ax_free(__PTR__) {fprintf(stderr, "%s(%d) freed:%p\n", __func__, __LINE__, __PTR__); free(__PTR__); __PTR__ = NULL; }
#else
#define __ax_malloc malloc
#define __ax_calloc calloc
#define __ax_realloc realloc
#define __ax_strdup strdup
#define __ax_strndup strndup
#define __ax_asprintf asprintf
#define __ax_free free
#endif

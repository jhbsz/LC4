/*
 * log.h
 *
 *  Created on: Oct 1, 2012
 *      Author: ch
 */

#ifndef LOG_H_
#define LOG_H_


#include <stdio.h>
#include <stdint.h>
#include <syslog.h>
#include <time.h>
#include "const.h"


/*
 * Constant defines.
 */
#define SYSLOG_BUFF_LEN             128     // max len for syslog buffer data

/*
 * Syslog parameters.
 */
#define SYSLOG_OPTION               (LOG_CONS | LOG_PID)
#define SYSLOG_FACILITY             LOG_USER

/*
 * Output with colors
 */
#define COLOR_ESCAPE                0x1B
#define COLOR_RESET                 0

#define BLACK                       "0;30"
#define RED                         "0;31"
#define GREEN                       "0;32"
#define YELLOW                      "0;33"
#define BLUE                        "0;34"
#define MAGENTA                     "0;35"
#define CYAN                        "0;36"
#define GRAY                        "0;37"
#define WHITE                       "0;38"
#define BGRED                       "0;41"

#define B_BLACK                     "1;30"  // preifx B_ means bright
#define B_RED                       "1;31"
#define B_GREEN                     "1;32"
#define B_YELLOW                    "1;33"
#define B_BLUE                      "1;34"
#define B_MAGENTA                   "1;35"
#define B_CYAN                      "1;36"
#define B_WHITE                     "1:37"
#define B_GRAY                      "1;38"
#define B_BGRED                     "1;41"


/*
 * Structure for debug variable.
 */
typedef struct _qnap_log_st {
  int       level;                      // level of debugging
  uint8_t   usrlog;                     // TRUE if user provide log function
  uint8_t   syslog;                     // TRUE if debug on system log
  uint8_t   detail;                     // TRUE if verbose debug
  uint8_t   filler;                     // alignment
  void      *svr_cb;                    // log server connection control block
  char      *log_file_name;             // filename for the log file
  FILE      *log_fp;                    // file descriptor
  void      (*log_func)(int level, const char *file, int line,
                        const char *func, char *fmt, ...);
} QNAP_LOG_T;


/*
 * Macro defines.
 */
#ifdef ENABLE_QNAP_LOG
/*
 * QNAP_LOG()      - debug info with filename, line number & function name
 * QNAP_LOG_NL()   - same as QNAP_LOG() plus newline
 * QNAP_LOG_RAW()  - debug info without prefixing filename, line number ...
 * QNAP_LOG_DATA() - detail debug, display data with hex value & ASCII char.
 *
 * QNAP_LOG_NSTRING() - display debug info with fix length of data
 */
#ifdef BACKEND_SERVICE
#define QNAP_LOG(Lvl, Color, Fmt, Arg...)                                  \
  if (QNAP_LOG_LEVEL_IS_ALLOWED((Lvl))) {                                  \
    if (QNAP_LOG_USRLOG_IS_ON()) {                                         \
      pqnap_log->log_func(Lvl, __FILE__, __LINE__, __func__, Fmt, ##Arg);  \
    } else if (QNAP_LOG_SYSLOG_IS_ON()) {                                  \
      syslog((SYSLOG_FACILITY| (Lvl)), "%c[%sm%s: %d: %s():" Fmt "%c[%dm",  \
             COLOR_ESCAPE, (Color), __FILE__, __LINE__, __func__,  ##Arg,   \
             COLOR_ESCAPE, COLOR_RESET);                                    \
    } else {                                                                \
      fprintf(pqnap_log->log_fp, "%c[%sm%s: %d: %s(): " Fmt "%c[%dm",      \
              COLOR_ESCAPE, (Color), __FILE__, __LINE__, __func__, ##Arg,   \
              COLOR_ESCAPE, COLOR_RESET);                                   \
      fflush(pqnap_log->log_fp);                                           \
    }                                                                       \
  }

#define QNAP_LOG_NL(Lvl, Color, Fmt, Arg...)                               \
  if (QNAP_LOG_LEVEL_IS_ALLOWED((Lvl))) {                                  \
    if (QNAP_LOG_USRLOG_IS_ON()) {                                         \
      pqnap_log->log_func(Lvl, __FILE__, __LINE__, __func__, Fmt, ##Arg);  \
    } else if (QNAP_LOG_SYSLOG_IS_ON()) {                                  \
      syslog((SYSLOG_FACILITY| (Lvl)), "%c[%sm%s: %d: %s():" Fmt "%c[%dm\n",\
             COLOR_ESCAPE, (Color), __FILE__, __LINE__, __func__,  ##Arg,   \
             COLOR_ESCAPE, COLOR_RESET);                                    \
    } else {                                                                \
      fprintf(pqnap_log->log_fp, "%c[%sm%s: %s: %d: %s(): " Fmt "%c[%dm\n",    \
              COLOR_ESCAPE, (Color), qnap_log_now_to_str(), __FILE__, __LINE__, __func__, ##Arg,   \
              COLOR_ESCAPE, COLOR_RESET);                                   \
      fflush(pqnap_log->log_fp);                                           \
    }                                                                       \
  }

#define QNAP_LOG_RAW(Lvl, Color, Fmt, Arg...)                              \
  if (QNAP_LOG_LEVEL_IS_ALLOWED((Lvl))) {                                  \
    if (QNAP_LOG_USRLOG_IS_ON()) {                                         \
      pqnap_log->log_func(Lvl, __FILE__, __LINE__, __func__, Fmt, ##Arg);  \
    } else if (QNAP_LOG_SYSLOG_IS_ON()) {                                  \
      syslog((SYSLOG_FACILITY| (Lvl)), "%c[%sm" Fmt "%c[%dm",               \
             COLOR_ESCAPE, (Color), ##Arg, COLOR_ESCAPE, COLOR_RESET);      \
    } else {                                                                \
      fprintf(pqnap_log->log_fp, "%c[%sm" Fmt "%c[%dm",                    \
              COLOR_ESCAPE, (Color), ##Arg, COLOR_ESCAPE, COLOR_RESET);     \
      fflush(pqnap_log->log_fp);                                           \
    }                                                                       \
  }
#else
#define QNAP_LOG(Lvl, Color, Fmt, Arg...)                                  \
  if (QNAP_LOG_LEVEL_IS_ALLOWED((Lvl))) {                                  \
    qnap_log_chk_file_size();                                              \
    fprintf(pqnap_log->log_fp, "%c[%sm%s: %s: %d: %s(): " Fmt "%c[%dm",      \
            COLOR_ESCAPE, (Color), qnap_log_now_to_str(), __FILE__, __LINE__, __func__, ##Arg,   \
            COLOR_ESCAPE, COLOR_RESET);                                   \
    fflush(pqnap_log->log_fp);                                           \
  }

#define QNAP_LOG_NL(Lvl, Color, Fmt, ...)                               \
    qnap_log(Lvl, Color, __FILE__, __LINE__, __func__, Fmt, ##__VA_ARGS__);

#define QNAP_LOG_RAW(Lvl, Color, Fmt, Arg...)                              \
  if (QNAP_LOG_LEVEL_IS_ALLOWED((Lvl))) {                                  \
    fprintf(pqnap_log->log_fp, "%c[%sm" Fmt "%c[%dm",                    \
            COLOR_ESCAPE, (Color), ##Arg, COLOR_ESCAPE, COLOR_RESET);     \
    fflush(pqnap_log->log_fp);                                           \
  }
#endif

#define QNAP_LOG_DATA(Lvl, Color, Data, Len)                               \
  if (QNAP_LOG_DETAIL_IS_ON() && QNAP_LOG_LEVEL_IS_ALLOWED((Lvl))) {      \
    qnap_log_dump_data((Lvl), (Color), (Data), (Len));                     \
  }

#define QNAP_LOG_NSTRING(Lvl, Color, Data, Len)                            \
  if (QNAP_LOG_DETAIL_IS_ON() && QNAP_LOG_LEVEL_IS_ALLOWED((Lvl))) {      \
    qnap_log_dump_nstring((Lvl), (Color), (Data), (Len));                  \
  }

#define QNAP_LOG_EMERG(Fmt, Arg...)                                        \
  QNAP_LOG(LOG_EMERG, B_BGRED, Fmt, ##Arg)
#define QNAP_LOG_ALERT(Fmt, Arg...)                                        \
  QNAP_LOG(LOG_ALERT, B_RED, Fmt, ##Arg)
#define QNAP_LOG_CRIT(Fmt, Arg...)                                         \
  QNAP_LOG(LOG_CRIT, RED, Fmt, ##Arg)
#define QNAP_LOG_ERR(Fmt, Arg...)                                          \
  QNAP_LOG(LOG_ERR, B_MAGENTA, Fmt, ##Arg)
#define QNAP_LOG_WARN(Fmt, Arg...)                                         \
  QNAP_LOG(LOG_WARNING, B_YELLOW, Fmt, ##Arg)
#define QNAP_LOG_INFO(Fmt, Arg...)                                         \
  QNAP_LOG(LOG_INFO, B_GREEN, Fmt, ##Arg)
#define QNAP_LOG_DEBUG(Fmt, Arg...)                                        \
  QNAP_LOG(LOG_DEBUG, B_BLACK, Fmt, ##Arg)

#define QNAP_LOG_EMERG_NL(Fmt, Arg...)                                     \
  QNAP_LOG_NL(LOG_EMERG, B_BGRED, Fmt, ##Arg)
#define QNAP_LOG_ALERT_NL(Fmt, Arg...)                                     \
  QNAP_LOG_NL(LOG_ALERT, B_RED, Fmt, ##Arg)
#define QNAP_LOG_CRIT_NL(Fmt, Arg...)                                      \
  QNAP_LOG_NL(LOG_CRIT, RED, Fmt, ##Arg)
#define QNAP_LOG_ERR_NL(Fmt, Arg...)                                       \
  QNAP_LOG_NL(LOG_ERR, B_MAGENTA, Fmt, ##Arg)
#define QNAP_LOG_WARN_NL(Fmt, Arg...)                                      \
  QNAP_LOG_NL(LOG_WARNING, B_YELLOW, Fmt, ##Arg)
#define QNAP_LOG_INFO_NL(Fmt, Arg...)                                      \
  QNAP_LOG_NL(LOG_INFO, B_GREEN, Fmt, ##Arg)
#define QNAP_LOG_DEBUG_NL(Fmt, Arg...)                                     \
  QNAP_LOG_NL(LOG_DEBUG, B_BLACK, Fmt, ##Arg)

/*
 * Useful macro to detect if detail debug is on before calling debug func.
 */
#define QNAP_LOG_DETAIL_IS_ON()            (pqnap_log->detail == TRUE)
#define QNAP_LOG_DETAIL_IS_OFF()           (pqnap_log->detail == FALSE)
#define QNAP_LOG_LEVEL_IS_ALLOWED(L)       ((L) <= pqnap_log->level)
#define QNAP_LOG_LEVEL_IS_NOT_ALLOWED(L)   ((L) > pqnap_log->level)
#define QNAP_LOG_SYSLOG_IS_ON()            (pqnap_log->syslog == TRUE)
#define QNAP_LOG_SYSLOG_IS_OFF()           (pqnap_log->syslog == FALSE)
#define QNAP_LOG_USRLOG_IS_ON()            ((pqnap_log->usrlog == TRUE) && \
                                             (pqnap_log->log_func))
#define QNAP_LOG_USRLOG_IS_OFF()           ! QNAP_LOG_USRLOG_IS_ON()

#else  /* ENABLE_QNAP_LOG */
#define QNAP_LOG(Lvl, Color, Fmt, Arg...)
#define QNAP_LOG_NL(Lvl, Color, Fmt, Arg...)
#define QNAP_LOG_RAW(Lvl, Color, Fmt, Arg...)
#define QNAP_LOG_DATA(Lvl, Color, Data, Len)
#define QNAP_LOG_NSTRING(Lvl, Color, Data, Len)

#define QNAP_LOG_EMERG(Fmt, Arg...)
#define QNAP_LOG_ALERT(Fmt, Arg...)
#define QNAP_LOG_CRIT(Fmt, Arg...)
#define QNAP_LOG_ERR(Fmt, Arg...)
#define QNAP_LOG_WARN(Fmt, Arg...)
#define QNAP_LOG_INFO(Fmt, Arg...)
#define QNAP_LOG_DEBUG(Fmt, Arg...)

#define QNAP_LOG_EMERG_NL(Fmt, Arg...)
#define QNAP_LOG_ALERT_NL(Fmt, Arg...)
#define QNAP_LOG_CRIT_NL(Fmt, Arg...)
#define QNAP_LOG_ERR_NL(Fmt, Arg...)
#define QNAP_LOG_WARN_NL(Fmt, Arg...)
#define QNAP_LOG_INFO_NL(Fmt, Arg...)
#define QNAP_LOG_DEBUG_NL(Fmt, Arg...)

#define QNAP_LOG_DETAIL_IS_ON()            FALSE
#define QNAP_LOG_DETAIL_IS_OFF()           TRUE
#define QNAP_LOG_LEVEL_IS_ALLOWED(L)       FALSE
#define QNAP_LOG_LEVEL_IS_NOT_ALLOWED(L)   TRUE
#define QNAP_LOG_SYSLOG_IS_ON()            FALSE
#define QNAP_LOG_SYSLOG_IS_OFF()           TRUE
#define QNAP_LOG_USRLOG_IS_ON()            FALSE
#define QNAP_LOG_USRLOG_IS_OFF()           TRUE
#endif /* ENABLE_QNAP_LOG */

/*
 * Access macros.
 */
#define QNAP_LOG_SET_LEVEL(L)              (pqnap_log->level = (L))

#define QNAP_LOG_SET_USRLOG()              (pqnap_log->usrlog = TRUE)
#define QNAP_LOG_CLR_USRLOG()              (pqnap_log->usrlog = FALSE)

#define QNAP_LOG_SET_SYSLOG()              (pqnap_log->syslog = TRUE)
#define QNAP_LOG_CLR_SYSLOG()              (pqnap_log->syslog = FALSE)

#define QNAP_LOG_SET_DETAIL()              (pqnap_log->detail = TRUE)
#define QNAP_LOG_CLR_DETAIL()              (pqnap_log->detail = FALSE)

#define QNAP_LOG_SET_LOG_FUNC(F)           (pqnap_log->log_func = (F))
#define QNAP_LOG_CLR_LOG_FUNC()            (pqnap_log->log_func = NULL)

#define QNAP_LOG_SET_SVR_CB(C)             (pqnap_log->svr_cb = (C))
#define QNAP_LOG_GET_SVR_CB()              (pqnap_log->svr_cb)

#define QNAP_LOG_LEVEL_IS_VALID(L)         ((L) >= LOG_EMERG &&            \
                                             (L) <= LOG_DEBUG)
#define QNAP_LOG_LEVEL_IS_INVALID(L)       ! QNAP_LOG_LEVEL_IS_VALID(L)

/*
 * External variables declaration.
 */
extern QNAP_LOG_T  *pqnap_log;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * External functions declaration.
 */
extern void qnap_log_init_setting(QNAP_LOG_T *log_cb);
extern int  qnap_log_init(char *ident, void *svr_cb,
                           void (*log_func)(int level, const char *file,
                                            int line, const char *func,
                                            char *fmt, ...));
extern void qnap_log_to_file(const char* file_name);
extern void qnap_log_chk_file_size();
extern void qnap_log_cleanup(void);
extern void qnap_log_dump_data(int level, char *color, void *data,
                                ssize_t len);
extern void qnap_log_dump_nstring(int level, char *color, void *data,
                                   ssize_t len);
extern char* qnap_log_now_to_str();

void qnap_log(int level, char *color, char *file, int line, const char *function, const char *fmt,...);

#ifdef __cplusplus
}
#endif

#endif /* LOG_H_ */

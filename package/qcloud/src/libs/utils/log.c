
/*
 * log.c --- syslog/debug utility routines.
 */

#include "std_hdr.h"
#include <syslog.h>
#include <ctype.h>
#include <sys/stat.h>
#include <stdarg.h>

#include "comm_hdr.h"

#define IS_LOG_TO_FILE(fp)  ((fileno((fp)))>STDERR_FILENO)


/*
 * Variables declaration.
 */
QNAP_LOG_T *pqnap_log = NULL;         // log control blcok

/*
 * qnap_log_init_setting()
 *
 * Every component need to call this function first to set up log info.
 *
 * Input:
 *      @ log_cb -- caller provided log control block.
 *
 * Output:
 *      @ log_cb -- contains init value of each field.
 */
void qnap_log_init_setting (QNAP_LOG_T *log_cb)
{
  /*
   * ignore the second init. setting
   */
  if(!pqnap_log){
    /*
     * Set to default value.
     */
    memset(log_cb, 0, sizeof(QNAP_LOG_T));
    if(log_cb->level == 0){
      log_cb->level = LOG_ERR;
    }

    log_cb->log_fp = stderr;
    pqnap_log = log_cb;
  }

}

/*
 * qnap_log_init()
 *
 * Open connection to system logger or get console file handle.
 *
 * Input:
 *      @ ident    -- process identifier string.
 *      @ svr_cb   -- log server connection control block.
 *      @ log_func -- user provided log function.
 *
 * Output:
 *      @ QNAP_NO_ERROR.
 */
int qnap_log_init (char *ident, void *svr_cb,
                    void (*log_func)(int level, const char *file, int line,
                                     const char *func, char *fmt, ...))
{
  QNAP_LOG_SET_LOG_FUNC(log_func);
  QNAP_LOG_SET_SVR_CB(svr_cb);

  if (QNAP_LOG_SYSLOG_IS_ON()) {
    openlog(ident, SYSLOG_OPTION, SYSLOG_FACILITY);
  }

  return(QNAP_NO_ERROR);
}

/*
 * qnap_log_to_file()
 *
 * Create file fp and save file name.
 *
 * Input: file name for log.
 * Output: None.
 */
void qnap_log_to_file(const char* file_name)
{
  if (!file_name || !strlen(file_name))
    return;

  if((pqnap_log->log_fp) && IS_LOG_TO_FILE(pqnap_log->log_fp)){
    fclose(pqnap_log->log_fp);
    pqnap_log->log_fp = NULL;
  }

  if(pqnap_log->log_file_name){
    free(pqnap_log->log_file_name);
  }

  pqnap_log->log_fp = fopen(file_name, "a");

  if(pqnap_log->log_fp){
    /*
     * change the log file permission. No one can read but owner.
     */
    chmod(file_name, 0600);
    pqnap_log->log_file_name = calloc(1, strlen(file_name)+1);
    strcpy(pqnap_log->log_file_name, file_name);
  }else{
    pqnap_log->log_fp = stderr;
  }

}

/*
 * qnap_log_chk_file_size()
 *
 * Check file size not over MAXLOGFILESIZE.
 *
 * Input:  None.
 * Output: None.
 */
void qnap_log_chk_file_size()
{
  if(pqnap_log->log_fp && IS_LOG_TO_FILE(pqnap_log->log_fp) &&
     pqnap_log->log_file_name){
    struct stat statbuf;
    int res = -1;
    res = lstat(pqnap_log->log_file_name, &statbuf);
    if(res == 0 && (statbuf.st_size > MAXLOGFILESIZE)){
      fclose(pqnap_log->log_fp);

      //re-open the file
      pqnap_log->log_fp = fopen(pqnap_log->log_file_name, "w");

      fprintf(pqnap_log->log_fp, "%c[%sm%s: %s: %d: %s(): log restart %c[%dm\n",
              COLOR_ESCAPE, B_GREEN, qnap_log_now_to_str(), __FILE__, __LINE__,
              __func__, COLOR_ESCAPE, COLOR_RESET);
      fflush(pqnap_log->log_fp);
    }
  }
}

/*
 * qnap_log_cleanup()
 *
 * Close connection to system logger.
 * Release console.
 *
 * Input:  None.
 * Output: None.
 */
void qnap_log_cleanup (void)
{

  if(!pqnap_log){
    return;
  }

  if (QNAP_LOG_SYSLOG_IS_ON()) {
    closelog();
  }

  if((pqnap_log->log_fp) && IS_LOG_TO_FILE(pqnap_log->log_fp)){
    fclose(pqnap_log->log_fp);
    pqnap_log->log_fp = NULL;
  }

  if(pqnap_log->log_file_name){
    free(pqnap_log->log_file_name);
  }

  pqnap_log = NULL;
}

/*
 * qnap_log_dump_data()
 *
 * Log data in hex format followed by ASCII character.
 * i.e in the format
 *   XX XX XX XX XX XX XX XX  XX XX XX XX XX XX XX XX ...... CCCCCCCC  CCCCCCCC
 *
 * Input:
 *      @ level -- log level.
 *      @ color -- color to be used to display logs.
 *      @ data  -- data to log.
 *      @ len   -- length of data.
 * Output:
 *      None.
 */
void qnap_log_dump_data (int level, char *color, void *data, ssize_t len)
{
  uint8_t   *ud = (uint8_t *)data;              // cast to unsigned
  char      output[DWORD_ALIGN(SYSLOG_BUFF_LEN)], *out;// tmp log buffer
  ssize_t   i;

  while (len > 0) {
    /*
     * Output data line-by-line, format to tmp buffer first.
     */
    memset(output, ' ', sizeof(output));        // set to all spaces first
    out = &output[2];                           // first 2 chars are spaces

    /*
     * Display the first 16 bytes in hex.
     */
    for (i = 0; (i < len) && (i < 16); i++) {
      out += sprintf(out, "%02X ", ud[i]);
      if (i == 7) {
        /*
         * Note: sprintf() will null terminated the string, need to reset
         *       back to space character
         */
        *out++ = ' ';
      }
    }
    *out = ' ';                                 // Same reason as above

    /*
     * Jump to position 51 because the last line of data might not be
     * 16 bytes.
     */
    out = &output[51];
    for (i = 0; i < 3; i++) {
      *out++ = ' ';                             // output 3 spaces
    }

    out++;                                      // one space

    /*
     * Now for ASCII characters display.
     */
    for (i = 0; (i < len) && (i < 16); i++) {
      *out++ = isprint(ud[i]) ? ud[i] : '.';
      if (i == 7) {
        out++;
      }
    }
    *out++ = '\0';

    /*
     * Display the line.
     */
    QNAP_LOG_RAW(level, color, "%s\n", output);

    /*
     * Advance to next line.
     */
    ud += 16;
    len -= 16;
  }
}

/*
 * qnap_log_dump_nstring()
 *
 * Display 'n' length bytes of string without hex data.
 * Usually the string is not null terminated.
 *
 * Input:
 *      @ level -- log level.
 *      @ color -- color to be used to display logs.
 *      @ data  -- data to log.
 *      @ len   -- length of data.
 *
 * Output:
 *      None.
 */
void qnap_log_dump_nstring (int level, char *color, void *data, ssize_t len)
{
  char  output[DWORD_ALIGN(SYSLOG_BUFF_LEN + 1)];// tmp buff for null-terminated
  char  *data_ptr = (char *)data;           // working pointer

  /*
   * Is string null-terminated ?
   * If yes, no need to process data, just display it.
   */
  if (*(data_ptr + len) == '\0') {
    QNAP_LOG_RAW(level, color, "%s", data_ptr);
    return;
  }

  /*
   * Null-terminated our output buffer.
   */
  output[SYSLOG_BUFF_LEN] = '\0';

  /*
   * Display SYSLOG_BUFF_LEN bytes data each time.
   */
  while (len >= SYSLOG_BUFF_LEN) {
    strncpy(output, data_ptr, SYSLOG_BUFF_LEN);
    QNAP_LOG_RAW(level, color, "%s", output);
    len -= SYSLOG_BUFF_LEN;
    data_ptr += SYSLOG_BUFF_LEN;
  }

  /*
   * Display the rest of data.
   */
  if (len > 0) {
    strncpy(output, data_ptr, len);
    output[len] = '\0';
    QNAP_LOG_RAW(level, color, "%s", output);
  }
}

/*
 * qnap_log_now_to_str()
 *
 * Return current time in string
 *
 * Input: None
 * Output: char *
 */
char* qnap_log_now_to_str()
{
  time_t now;
  struct tm* localnow;
  static char hdr_date[25] = "";

  time(&now);
  localnow = localtime(&now);

  strftime(hdr_date, 24, "%D %T", localnow);

  return hdr_date;
}

void qnap_log(int level, char *color, char *file, int line, const char *function, const char *fmt,...)
{
  // do extra check to fix segfault
  // in some flow, the log init function may not be called first
  if( pqnap_log == NULL ) return;

  // do the normal log flow
  if (QNAP_LOG_LEVEL_IS_ALLOWED((level))) {
    va_list args;
    va_start(args, fmt);
    qnap_log_chk_file_size();
    fprintf(pqnap_log->log_fp, "%c[%sm%s: %s: %d: %s():" ,
            COLOR_ESCAPE, color, qnap_log_now_to_str(), file, line, function);
    vfprintf(pqnap_log->log_fp, fmt, args);
    fprintf(pqnap_log->log_fp, "%c[%dm\n", COLOR_ESCAPE, COLOR_RESET);
    va_end(args);
    fflush(pqnap_log->log_fp);
  }
}

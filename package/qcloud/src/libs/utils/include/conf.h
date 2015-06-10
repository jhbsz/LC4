/*
 * conf.h
 *
 *  Created on: Oct 29, 2012
 *      Author: ch
 */

#ifndef CONF_H_
#define CONF_H_

#include "const.h"


/*
 * Path of configuration.
 */
#define PATH                  "/etc/"

#define CONF_PATH             PATH "config/"

#define CONF_RUN_TIME_PATH    "/tmp/"

#define CONF_FILE_QID         CONF_PATH "qid.conf"
#define CONF_FILE_MCN         CONF_PATH "mcn.conf"

/*
 * File access mode.
 */
#define FILE_MODE_READ              "r"
#define FILE_MODE_WRITE             "w"

/*
 * Constant defines.
 */
#define CONF_KW_LEFT_DELIMITER      '<'
#define CONF_KW_RIGHT_DELIMITER     '>'
#define CONF_KW_VAL_SEPERATOR       '='

#define CONF_KW_VAL_TRUE            "TRUE"
#define CONF_KW_VAL_FALSE           "FALSE"

/*
 * Keyword string.
 */
#define CONF_KW_DEBUG_LEVEL         "DebugLevel"
#define CONF_KW_DEBUG_DETAIL        "DebugDetail"
#define CONF_KW_DEBUG_SYSLOG        "DebugOnSyslogger"

#define CONF_KW_QID_SVR             "QIDServer"
#define CONF_KW_MCN_SVR             "MCNServer"

#define CONF_KW_MAC_ADDRESS         "MacAddress"
#define CONF_KW_SESSION_ID          "SessionId"


/*
 * Type of parameters.
 */
enum {
  CONF_TYPE_STRING,
  CONF_TYPE_INT,
  CONF_TYPE_BOOL,
  CONF_TYPE_IP,
  CONF_TYPE_URL,
  CONF_TYPE_ADDR_LIST,
  CONF_TYPE_MAC,
};

/*
 * Structure for configuration variables.
 */
typedef struct _conf_param_st {
  char      *keyword;                   // configuration keyword
  int       type;                       // type of parameter
  int       max_len;                    // maximum length of string data
  size_t    offset;                     // parameter's offset of config block
} CONF_PARAM_T;


/*
 * Forward functions declaration.
 */
extern int  conf_init( char *conf_file, void *conf_base,
                       CONF_PARAM_T *params);
extern int  conf_save_run_time_info( char *conf_file, void *conf_base,
                                     CONF_PARAM_T *params);



#endif /* CONF_H_ */

/*
 * conf.c --- configuration utility routines.
 */

#include "std_hdr.h"

#include "comm_hdr.h"
#include "conf.h"


/*
 * conf_get_value()
 *
 * Return pointer to value associated with the given keyword.
 * The format is  <Keyword=Value>.
 *
 * Input:
 *      @ buf      -- buffer contains configuration to be parsed.
 *      @ keyword  -- keyword string to be looking for or NULL for any keyword.
 *      @ key_pptr -- used for output.
 *      @ val_pptr -- used for output.
 *
 * Output:
 *      @ NO_ERROR for valid format and put returned info in
 *      @ key_pptr -- keyword string in the buffer.
 *      @ val_pptr -- keyword value in the buffer.
 *
 *      @ ERR_xxxx for any kind of error.
 */
static int conf_get_value (char *buf, char *keyword, char **key_pptr,
                                 char **val_pptr)
{
  char  *start;                         // working pointer
  char  *end;                           // working pointer
  char  *value = NULL;                  // keyword value

  /*
   * Check if valid format first.
   */
  start = strchr(buf, CONF_KW_LEFT_DELIMITER);
  end = start ? strchr(start + 1, CONF_KW_RIGHT_DELIMITER) : NULL;
  if (!start || !end)
    return(ERR_INVALID_FORMAT);

  /*
   * Advance start pointer to skip the left delimiter, and remove
   * the leading space.
   */
  start++;
  start = util_trim(start, TRUE);

  /*
   * Look for value starts.
   */
  if (start) {
    value = strchr(start, CONF_KW_VAL_SEPERATOR);

    /*
     * Null terminated for string comparison.
     */
    if (value) {
      *value++ = '\0';
    }
  }

  /*
   * Don't include the right delimiter, mask it out.
   */
  *end = '\0';

  /*
   * Trim out leading and trailing spaces of value.
   */
  if (value) {
    value = util_trim(value, FALSE);
  }

  /*
   * Trim out trailing spaces of keyword.
   */
  if (start) {
    start = util_trim(start, FALSE);
  }

  /*
   * Return pointer to point to start of keyword and value.
   */
  *key_pptr = start;
  *val_pptr = value;

  /*
   * Does keyword match ?
   */
  if (start && (!keyword || (strcmp(start, keyword) == 0)))
    return(NO_ERROR);

  return(ERR_NOT_FOUND);
}

/*
 * conf_set_param()
 *
 * Read in parameter configuration and save in local variable.
 *
 * Input:
 *      @ keyword   -- parameter keyword string.
 *      @ value     -- data value of keyword.
 *      @ conf_base -- base offset of configuration block to store parameter.
 *      @ params    -- parameters definition array.
 *
 * Output:
 *      None.
 */
static void conf_set_param(char *keyword, char *value, void *conf_base,
                                 CONF_PARAM_T *params)
{
  CONF_PARAM_T  *param = params;        // working pointer for parameters
  char          mac_str[DWORD_ALIGN(MAC_STR_LEN + 1)];// mac string without ':'
  uint8_t       *base = conf_base;      // cast base to uint8_t *
  int           found;                  // flag indicating if keyword found
  int           err;                    // flag indicating if any error
  int           len;                    // length of keyword value
  int           max_len;                // maximum length allowed for value
  char          *tmp_ptr;               // temp working pointer
  int           tmp_value;              // temp working data

  found = FALSE;
  err = FALSE;

  /*
   * Walk through parameter list.
   */
  while (param->keyword && !found) {
    /*
     * Do we get the required parameter ?
     */
    if (strcmp(keyword, param->keyword) != 0) {
      param++;
      continue;
    }

    found = TRUE;
    value = util_trim_quotes(value);
    if (!value) {
      err = TRUE;
      break;
    }

    switch (param->type) {
      case CONF_TYPE_MAC:
        util_strip_mac(value, mac_str);
        if (strlen(mac_str)) {
          strcpy((char *)(base + param->offset), mac_str);
        } else {
          err = TRUE;
        }
        break;

      case CONF_TYPE_URL:
        max_len = URL_STR_LEN;
        goto save_str;

      case CONF_TYPE_ADDR_LIST:
        max_len = ADDR_STR_LEN;
        goto save_str;

      case CONF_TYPE_IP:
        max_len = IP_HOST_LEN;
        goto save_str;

      case CONF_TYPE_STRING:
        max_len = param->max_len;

save_str:
        len = strlen(value);
        if (len > max_len) {
          len = max_len;
          QNAP_LOG_WARN_NL("Keyword[%s] value is truncated", keyword);
        }
        memcpy((base + param->offset), value, len);
        *(base + param->offset + len) = '\0';
        break;

      case CONF_TYPE_INT:
        tmp_value = strtol(value, &tmp_ptr, 10);
        if (*value && tmp_ptr && (*tmp_ptr == '\0')) {
          *((int *)(base + param->offset)) = atoi(value);
        } else {
          err = TRUE;
        }
        break;

      case CONF_TYPE_BOOL:
        if (strcasecmp(value, CONF_KW_VAL_TRUE) == 0) {
          tmp_value = 1;
        } else if (strcasecmp(value, CONF_KW_VAL_FALSE) == 0) {
          tmp_value = 0;
        } else {
          tmp_value = strtol(value, &tmp_ptr, 10);
          if (!(*value && tmp_ptr && (*tmp_ptr == '\0'))) {
            tmp_value = -1;
          }
        }

        if ((tmp_value == 0) || (tmp_value == 1)) {
          *(base + param->offset) = tmp_value;
        } else {
          err = TRUE;
        }
        break;

      default:
        err = TRUE;
        break;
    }

    /*
     * Advance to next parameter.
     */
    param++;
  }

  if (!found) {
    QNAP_LOG_INFO_NL("Unknown keyword[%s]", keyword);
  } else if (err) {
    QNAP_LOG_WARN_NL("Keyword[%s] value is invalid", keyword);
  } else {
    QNAP_LOG_DEBUG_NL("Get configuration [%s = %s]", keyword, value);
  }
}

/*
 * conf_init()
 *
 * Read in configuration.
 *
 * Input:
 *      @ conf_file -- configuration file name.
 *      @ conf_base -- configuration block base for storing parameters value.
 *      @ params    -- parameters definition array.
 *
 * Output:
 *      @ NO_ERROR for successfully readding in configuration.
 *      @ ERR_xxxx for any kind of error.
 */
int conf_init (char *conf_file, void *conf_base, CONF_PARAM_T *params)
{
  FILE  *fp;                            // configuration file descriptor
  char  line[DWORD_ALIGN(CONF_BUFF_LEN)];   // read buffer for keywords
  char  *key;                           // keyword
  char  *value;                         // keyword value
  char  *ptr;                           // working pointer

  /*
   * Open the configuration file.
   */
  if ((fp = util_open_file(conf_file, FILE_MODE_READ)) == NULL)
    return(ERR_NO_FILE);

  /*
   * Read in one line at a time until end of file.
   */
  while (fgets(line, CONF_BUFF_LEN, fp)) {
    /*
     * Skip comment line or space line.
     */
    ptr = util_trim(line, TRUE);
    if (!ptr || (*ptr == COMMENT_CHAR))
      continue;

    /*
     * Skip invalid format data.
     */
    if (conf_get_value(line, NULL, &key, &value) != NO_ERROR)
      continue;

    /*
     * Put in local variable.
     */
    if (key && value) {
      conf_set_param(key, value, conf_base, params);
    }
  }

  /*
   * Close the file.
   */
  util_close_file(fp);

  return(NO_ERROR);
}

/*
 * conf_save_run_time_info()
 *
 * Save run time configuration to the given file.
 *
 * Input:
 *      @ conf_file -- run-time configuration file name.
 *      @ conf_base -- configuration block base for parameters value.
 *      @ params    -- parameters definition array.
 *
 * Output:
 *      @ NO_ERROR for successfully storing run time info to file.
 *      @ ERR_xxxx for any kind of error.
 */
int conf_save_run_time_info (char *conf_file, void *conf_base,
                                   CONF_PARAM_T *params)
{
  FILE          *fp;                    // run-time config. file descriptor
  char          line[DWORD_ALIGN(CONF_BUFF_LEN)]; // write buffer for parameters
  CONF_PARAM_T  *param = params;        // working pointer for parameters
  char          *base = conf_base;      // cast base to char *

  /*
   * Open run-time configuration file.
   */
  if ((fp = util_open_file(conf_file, FILE_MODE_WRITE)) == NULL)
    return(ERR_NO_FILE);

  /*
   * Walk through parameters definition array to save each parameter.
   */
  while (param->keyword) {
    switch (param->type) {
      case CONF_TYPE_INT:
        QNAP_LOG_DEBUG_NL("Set run-time configuration [%s%c%d]",
                           param->keyword, CONF_KW_VAL_SEPERATOR,
                           *((int *)(base + param->offset)));
        sprintf(line, "%s%c%d",
                param->keyword, CONF_KW_VAL_SEPERATOR,
                *((int *)(base + param->offset)));
        break;

      case CONF_TYPE_STRING:
      case CONF_TYPE_URL:
      default:
        QNAP_LOG_DEBUG_NL("Set run-time configuration [%s%c%s]",
                           param->keyword, CONF_KW_VAL_SEPERATOR,
                           (base + param->offset));
        sprintf(line, "%s%c%s",
                param->keyword, CONF_KW_VAL_SEPERATOR, (base + param->offset));
        break;
    }

    fputs(line, fp);
  fputc('\n', fp);

    /*
     * Advance to next parameter.
     */
    param++;
  }

  /*
   * Close the file.
   */
  util_close_file(fp);

  return(NO_ERROR);
}

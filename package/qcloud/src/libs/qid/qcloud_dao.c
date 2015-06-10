/*
 * qcloud_dao.c
 *
 *  Created on: Aug 6, 2013
 *      Author: kevinchien
 */

#include <errno.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <ctype.h>

#include "sqlite3.h"

#include "std_hdr.h"
#include "comm_hdr.h"
#include "qid_system.h"
#include "qcloud_dao.h"

static int db_client_create_table(char *db_name, char * table_name,
                                  key_pair_st *key_list, int list_size)
{
  sqlite3 *database;
  int rc;
  char *error;
  char *statement = NULL;
  int count = 0;
  // "2*" is made to avoid overflow
  char* schema = (char*)malloc( 2*sizeof(char)* list_size* sizeof(key_pair_st));
  statement = (char*)malloc( 2*sizeof(char)* list_size* sizeof(key_pair_st));
  for( count = 0; count < list_size; count++){
    if(0 ==count){
      sprintf(schema, "%s char(%d)", key_list[count].key, PAIR_KEY_LEN);
    }else{
      sprintf(schema, "%s, %s char(%d)", schema, key_list[count].key, PAIR_KEY_LEN);
    }
  }
  sprintf(statement, DB_CREATE_TABLE, table_name, schema);

  /* Open the sqlite db */
  if ((rc = sqlite3_open (db_name, &database)) != SQLITE_OK)
  {
    QNAP_LOG_ERR_NL("* ERROR (%d) - sqlite3_open() failed.", rc);
    goto exit;
  }
  /* Create a new table */
  if ((rc = sqlite3_exec (database,
                          statement,
                          NULL,
                          NULL,
                          &error)) != SQLITE_OK){
    QNAP_LOG_ERR_NL("* ERROR (%d) - sqlite3_exec() failed.", rc);
    QNAP_LOG_ERR_NL("statment:%s\n", statement);
    goto exit;
  }

exit:
  if(database){
    /* Open the sqlite db */
    if ((rc = sqlite3_close (database)) != SQLITE_OK){
      QNAP_LOG_ERR_NL("* ERROR (%d) - sqlite3_close() failed.", rc);
    }
  }
  if(error != NULL ) sqlite3_free(error);
  free( statement );
  statement = NULL;
  free( schema );
  schema = NULL;
  return rc;
}

int config_client_create_file(char *config_path){
  char set_cmd[MAX_SYSTEM_CMD+1];
  char data[MAX_SYSTEM_CMD+1];
  snprintf(set_cmd, MAX_SYSTEM_CMD, "/bin/touch %s", config_path);
  return util_system_cmd(set_cmd, data, MAX_SYSTEM_CMD);
}

static int db_sqlquery_callback(void* ret,int argc,char** query_ret,char** column_name)
{
  int count = 0;
  key_pair_st *result = (key_pair_st *)ret;
  for(count = 0; count < argc; count++)
  {
    snprintf( result[count].key, PAIR_KEY_LEN, "%s", *(column_name+count));
    snprintf( result[count].value, PAIR_VALUE_LEN, "%s", *(query_ret+count));
    QNAP_LOG_DEBUG_NL("cnt:%d key:%s value:%s", count, result[count].key, result[count].value);
  }
  return NO_ERROR;
}

static int db_client_search_string(char *file_path, char *table_name, key_pair_st *condition_list,
                            key_pair_st *key_list, int cond_list_size)
{
  int rc = ERR_NULL_POINTER, count = 0;
  sqlite3 *database;
  char *error;
  char* cond = (char*)calloc( 2, sizeof(char)* cond_list_size* sizeof(key_pair_st));
  char* statement = (char*)calloc( 2, sizeof(char)* cond_list_size* sizeof(key_pair_st));
  if(!file_path || !table_name || !key_list || !condition_list){
    goto exit;
  }

  /* Open the sqlite db */
  if ((rc = sqlite3_open (file_path, &database)) != SQLITE_OK){
    QNAP_LOG_ERR_NL("* ERROR (%d) - sqlite3_open() failed.", rc);
    goto exit;
  }

  for( count = 0; count < cond_list_size; count++){
    if(0 ==count){
      sprintf(cond, "%s == '%s'", condition_list[count].key, condition_list[count].value);
    }else{
      sprintf(cond, "%s AND %s == '%s'", cond, condition_list[count].key, condition_list[count].value);
    }
  }

  sprintf( statement, DB_QUERY_DATA, "*", table_name, cond);
  QNAP_LOG_INFO_NL("statment:%s\n", statement);
  /* insert sql entry */
    if ((rc = sqlite3_exec (database,
                            statement,
                            db_sqlquery_callback,
                            key_list,
                            &error)) != SQLITE_OK){
    QNAP_LOG_ERR_NL("* ERROR (%d) - sqlite3_exec() failed.", rc);
    QNAP_LOG_ERR_NL("statment:%s\n", statement);
    goto exit;
  }
  memset( statement, 0,2*sizeof(char)* cond_list_size* sizeof(key_pair_st));


exit:
  if(database){
    /* Close the sqlite db */
    if ((rc = sqlite3_close (database)) != SQLITE_OK){
      QNAP_LOG_ERR_NL("* ERROR (%d) - sqlite3_close() failed.", rc);
    }
  }
  if(error != NULL ) sqlite3_free(error);
  if(statement != NULL ) free(statement);
  return rc;
}

static int db_insert_string_value(char *file_path, char *table_name,
                                  key_pair_st *key_list, int list_size){
  int rc = ERR_NULL_POINTER, count = 0;
  sqlite3 *database;
  char *error;
  // "2*" is made to avoid overflow
  char* key = (char*)calloc( 2, sizeof(char)* list_size* sizeof(key_pair_st));
  char* value = (char*)calloc( 2, sizeof(char)* list_size* sizeof(key_pair_st));
  char* statement = (char*)calloc( 2, sizeof(char)* list_size* sizeof(key_pair_st));
  if(!file_path || !table_name || !key_list){
    goto exit;
  }

  /* Open the sqlite db */
  if ((rc = sqlite3_open (file_path, &database)) != SQLITE_OK){
    QNAP_LOG_ERR_NL("* ERROR (%d) - sqlite3_open() failed.", rc);
    goto exit;
  }

  for( count = 0; count < list_size; count++){
    if(0 ==count){
      sprintf(key, "%s", key_list[count].key);
      sprintf(value, "'%s'", key_list[count].value);
    }else{
      sprintf(key, "%s, %s", key, key_list[count].key);
      sprintf(value, "%s, '%s'", value, key_list[count].value);
    }
  }
  sprintf( statement, DB_INSERT_DATA, table_name, key, value);
  QNAP_LOG_INFO_NL("statment:%s\n", statement);
  /* insert sql entry */
  if ((rc = sqlite3_exec (database,
                          statement,
                          NULL,
                          NULL,
                          &error)) != SQLITE_OK){
    QNAP_LOG_ERR_NL("* ERROR (%d) - sqlite3_exec() failed.", rc);
    QNAP_LOG_ERR_NL("statment:%s\n", statement);
    goto exit;
  }
exit:
  if(database){
    /* Close the sqlite db */
    if ((rc = sqlite3_close (database)) != SQLITE_OK){
      QNAP_LOG_ERR_NL("* ERROR (%d) - sqlite3_close() failed.", rc);
    }
  }
  if(error != NULL ) sqlite3_free(error);
  if(key != NULL ) free(key);
  if(value != NULL ) free(value);
  if(statement != NULL ) free(statement);
  return rc;
}

static int db_delete_string_value(char *file_path, char *table_name, key_pair_st *cond_list, int list_size)
{
  int rc = ERR_NULL_POINTER, count = 0;
  sqlite3 *database;
  char *error;
  // "2*" is made to avoid overflow
  char* cond = (char*)calloc( 2, sizeof(char)* list_size* sizeof(key_pair_st));
  char* statement = (char*)calloc( 2, sizeof(char)* list_size* sizeof(key_pair_st));
  if(!file_path || !table_name || !cond_list){
    goto exit;
  }

  /* Open the sqlite db */
  if ((rc = sqlite3_open (file_path, &database)) != SQLITE_OK){
    QNAP_LOG_ERR_NL("* ERROR (%d) - sqlite3_open() failed.", rc);
    goto exit;
  }
  sprintf(cond, "%s == '%s'", cond_list[count].key, cond_list[count].value);
  for( count = 1; count < list_size; count++){
    if(strlen( cond_list[count].key) >0 && strlen(cond_list[count].value) > 0 ){
      sprintf(cond, "%s AND %s == '%s'", cond, cond_list[count].key, cond_list[count].value);
    }
  }

  sprintf( statement, DB_DELETE_DATA, table_name, cond);
  QNAP_LOG_INFO_NL("statment:%s\n", statement);
  if ((rc = sqlite3_exec (database,
                          statement,
                          NULL,
                          NULL,
                          &error)) != SQLITE_OK){
    QNAP_LOG_ERR_NL("* ERROR (%d) - sqlite3_exec() failed.", rc);
    QNAP_LOG_ERR_NL("statment:%s\n", statement);
    goto exit;
  }

exit:
  if(database){
    /* Close the sqlite db */
    if ((rc = sqlite3_close (database)) != SQLITE_OK){
      QNAP_LOG_ERR_NL("* ERROR (%d) - sqlite3_close() failed.", rc);
    }
  }
  if(error != NULL ) sqlite3_free(error);
  if(cond != NULL ) free(cond);
  if(statement != NULL ) free(statement);
  return rc;
}

static int db_update_string_value(char *file_path, char *table_name,
                                  key_pair_st *cond_list, int cond_list_size,
                                  key_pair_st *data_list, int data_list_size)
{
  int rc = ERR_NULL_POINTER, count = 0;
  sqlite3 *database;
  char *error;
  // "2*" is made to avoid overflow
  char* cond = (char*)calloc( 2, sizeof(char)* cond_list_size* sizeof(key_pair_st));
  char* set_data = (char*)calloc( 2, sizeof(char)* data_list_size* sizeof(key_pair_st));
  char* statement = (char*)calloc( 2, sizeof(char)* (cond_list_size+data_list_size)* sizeof(key_pair_st));
  if(!file_path || !table_name || !cond_list){
    goto exit;
  }

  /* Open the sqlite db */
  if ((rc = sqlite3_open (file_path, &database)) != SQLITE_OK){
    QNAP_LOG_ERR_NL("* ERROR (%d) - sqlite3_open() failed.", rc);
    goto exit;
  }
  sprintf(cond, "%s = '%s'", cond_list[count].key, cond_list[count].value);
  sprintf(set_data, "%s = '%s'", data_list[count].key, data_list[count].value);

  for( count = 1; count < data_list_size; count++){
    sprintf(set_data, "%s , %s = '%s'", set_data, data_list[count].key, data_list[count].value);
  }

  for( count = 1; count < cond_list_size; count++){
    sprintf(cond, "%s AND %s = '%s'", cond, cond_list[count].key, cond_list[count].value);
  }

  sprintf( statement, DB_UPDATE_DATA, table_name, set_data, cond);
  QNAP_LOG_INFO_NL("statment:%s\n", statement);
  if ((rc = sqlite3_exec (database,
                          statement,
                          NULL,
                          NULL,
                          &error)) != SQLITE_OK){
    QNAP_LOG_ERR_NL("* ERROR (%d) - sqlite3_exec() failed.", rc);
    QNAP_LOG_ERR_NL("statment:%s\n", statement);
    goto exit;
  }

exit:
  if(database){
    /* Close the sqlite db */
    if ((rc = sqlite3_close (database)) != SQLITE_OK){
      QNAP_LOG_ERR_NL("* ERROR (%d) - sqlite3_close() failed.", rc);
    }
  }
  if(error != NULL ) sqlite3_free(error);
  if(cond != NULL ) free(cond);
  if(set_data != NULL ) free(set_data);
  if(statement != NULL ) free(statement);
  return rc;
}

static int db_drop_table(char *file_path, char *table_name)
{
  int rc = ERR_NULL_POINTER;

  sqlite3 *database = NULL;
  char *error = NULL;
  // "2*" is made to avoid overflow
  char* statement = (char*)calloc( 2, sizeof(char)* PAIR_VALUE_LEN);
  if(!file_path || !table_name ){
    goto exit;
  }

   //Open the sqlite db
  if ((rc = sqlite3_open (file_path, &database)) != SQLITE_OK){
    QNAP_LOG_ERR_NL("* ERROR (%d) - sqlite3_open() failed.", rc);
    goto exit;
  }
  sprintf( statement, DB_DROP_TABLE, table_name);
  QNAP_LOG_INFO_NL("statment:%s\n", statement);

  if ((rc = sqlite3_exec (database,
                          statement,
                          NULL,
                          NULL,
                          &error)) != SQLITE_OK){
    QNAP_LOG_ERR_NL("* ERROR (%d) - sqlite3_exec() failed.", rc);
    QNAP_LOG_ERR_NL("statment:%s\n", statement);
    goto exit;
  }

exit:
  if(database){
    /* Close the sqlite db */
    if ((rc = sqlite3_close (database)) != SQLITE_OK){
      QNAP_LOG_ERR_NL("* ERROR (%d) - sqlite3_close() failed.", rc);
    }
  }
  if(error != NULL ) sqlite3_free(error);
  if(statement != NULL ) free(statement);

  return rc;
}

#define SET_CFG_FMT "/sbin/setcfg -f %s %s \"%s\" \"%s\""
#define GET_CFG_FMT "/sbin/getcfg -f %s %s \"%s\" -d \"\""

static int config_insert_string_value(char *file_path, char * section,
                                      char *key, char *value)
{
  char cmd[MAX_SYSTEM_CMD+1];
  char data[MAX_SYSTEM_CMD+1];
  if(!file_path || !section || !key || !value){
    return ERR_NULL_POINTER;
  }
  snprintf(cmd, MAX_SYSTEM_CMD, SET_CFG_FMT, file_path, section, key, value);
  return util_system_cmd(cmd, data, MAX_SYSTEM_CMD);
}

static int config_get_string_value(char *file_path, char *section, char *key,
                                   char*return_buff, size_t buff_size)
{
  char cmd[MAX_SYSTEM_CMD+1];
  snprintf(cmd, MAX_SYSTEM_CMD, GET_CFG_FMT, file_path, section, key);
  return util_system_cmd(cmd, return_buff, buff_size);
}

int dao_create_container(qcloud_dao_type type, char *file_path, char * table_name,
                         key_pair_st *key_list, int list_size)
{
  int ret = NO_ERROR;
  switch(type)
  {
    default:
      break;
    case dao_type_db:
      ret = db_client_create_table(file_path, table_name, key_list, list_size);
      break;
    case dao_type_config:
      break;
  }
  return ret;
}

int dao_search_string(qcloud_dao_type type, char *file_path, char *table_name,
                      key_pair_st *condition_list, key_pair_st *key_list, int cond_list_size)
{
  int ret = NO_ERROR;
  switch(type)
  {
    default:
      break;
    case dao_type_db:
      ret = db_client_search_string(file_path, table_name, condition_list,
                                      key_list, cond_list_size);
      break;
    case dao_type_config:
      ret = config_get_string_value(file_path, table_name, PAIR_GET_KEY(key_list, 0),
                                    PAIR_GET_VALUE(key_list, 0), PAIR_VALUE_LEN);
      break;
  }
  return ret;
}

int dao_insert_string_value(qcloud_dao_type type, char *file_path, char *table_name,
                            key_pair_st *key_list, int list_size)
{
  int ret = NO_ERROR;
  key_pair_st * item;
  switch(type)
  {
    default:
      break;
    case dao_type_db:
      ret = db_insert_string_value(file_path, table_name, key_list, list_size);
      break;
    case dao_type_config:
    {
      int i = 0;
      item = key_list;
      for(; item && i < list_size; i++, item++){
        ret = config_insert_string_value(file_path, table_name, item->key,
                                         item->value);
      }
    }
    break;
  }
  return ret;
}

int dao_delete_string_value(qcloud_dao_type type, char *file_path, char *table_name,
                            key_pair_st *cond_list, int list_size)
{
  int ret = NO_ERROR;
  switch(type)
  {
    default:
      break;
    case dao_type_db:
      ret = db_delete_string_value(file_path, table_name, cond_list, list_size);
      break;
    case dao_type_config:
      break;
  }
  return ret;
}

int dao_update_string_value(qcloud_dao_type type, char *file_path, char *table_name,
                            key_pair_st *cond_list, int cond_list_size,
                            key_pair_st *data_list, int data_list_size)
{
  int ret = NO_ERROR;
  switch(type)
  {
    default:
      break;
    case dao_type_db:
      ret = db_update_string_value(file_path, table_name, cond_list,
                                   cond_list_size, data_list, data_list_size);
      break;
    case dao_type_config:
      break;
  }
  return ret;
}

int dao_drop_container(qcloud_dao_type type, char *file_path, char *table_name)
{
  int ret = NO_ERROR;
  switch(type)
  {
    default:
      break;
    case dao_type_db:
      ret = db_drop_table(file_path, table_name);
      break;
    case dao_type_config:
      break;
  }
  return ret;
}


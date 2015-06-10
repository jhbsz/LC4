
#ifndef QCLOUD_DAO_H_
#define QCLOUD_DAO_H_

// QID DB table name
#define DB_QID_TABLE            "QID"
// table_name + schema[key_list]
#define DB_CREATE_TABLE         "CREATE TABLE IF NOT EXISTS %s (%s);"
// table_name + (schema{name1, name2}) + (values{value, value2})'%s', '%s'
#define DB_INSERT_DATA          "INSERT OR REPLACE INTO %s (%s) values (%s);"
// table_name, row name[key_list], condition[key_pair].
#define DB_QUERY_DATA           "SELECT %s FROM %s WHERE %s;"
// table_name, condition[key_pair].
#define DB_DELETE_DATA          "DELETE FROM %s WHERE %s;"
// table_name, update[key_pair], condition[key_pair],
#define DB_UPDATE_DATA          "UPDATE %s SET %s WHERE %s;"
// table_name
#define DB_DROP_TABLE           "DROP TABLE %s;"


#define QCLOUD_DAO_DB_PATH                      "/etc/config/qidDB.db"
#define QCLOUD_DAO_DB_QID                       "user_account"
#define QCLOUD_DAO_DB_LOCAL_USER                "local_user"


#ifdef __cplusplus
extern "C" {
#endif

typedef enum _qcloud_dao_type{
  dao_type_db = 0,
  dao_type_config
}qcloud_dao_type;

typedef enum _qcloud_qid_table_column{
  db_column_qid = 0,
  db_column_local_account
}qcloud_qid_table_column;


/*
  Opens/creates the container and a new table (if it doesn't exist yet)
  Returns  sqlite3 error codes - in event of errors
  Returns  0 - success
*/
int dao_create_container(qcloud_dao_type type, char *file_path, char * table_name,
                         key_pair_st *key_list, int list_size);

/*
  Looks in the given container for key_value in the key column

  @param: type            Type of dao container.
  @param: file_path       File path of dao container.
  @param: table_name      Table name in DB/ section name in config file.
  @param: condition_list  Key value of select condition in DB. NULL for config file.
  @param: key_list        Returned value.
  @param: cond_list_size  The size of cond_list.

  Returns sqlite3 error codes - in event of error
  Returns  0                  - if the element exists
  Returns -1                  - if the element doesn't exist
*/
int dao_search_string(qcloud_dao_type type, char *file_path, char *table_name,
                      key_pair_st *condition_list, key_pair_st *key_list, int cond_list_size);

/*
 * Inserts new elements into the given container.
 *
  @param: type            Type of dao container.
  @param: file_path       File path of dao container.
  @param: table_name      Table name in DB/ section name in config file.
  @param: key_list        Inserted key value pair.
  @param: list_size       The size of cond_list.
 *
 */
int dao_insert_string_value(qcloud_dao_type type, char *file_path, char *table_name,
                            key_pair_st *key_list, int list_size);

/*
 * Deletes elements in the given container.
 *
  @param: type            Type of dao container.
  @param: file_path       File path of dao container.
  @param: table_name      Table name in DB/ section name in config file.
  @param: cond_list       the conditional key value pair.
  @param: list_size       The size of cond_list.
 *
 */
int dao_delete_string_value(qcloud_dao_type type, char *file_path, char *table_name,
                            key_pair_st *cond_list, int list_size);

/*
 * Updates elements in the given container.
 *
  @param: type            Type of dao container.
  @param: file_path       File path of dao container.
  @param: table_name      Table name in DB/ section name in config file.
  @param: cond_list       the conditional key value pair.
  @param: list_size       The size of cond_list.
 *
 */
int dao_update_string_value(qcloud_dao_type type, char *file_path, char *table_name,
                              key_pair_st *cond_list, int cond_list_size,
                              key_pair_st *data_list, int data_list_size);


int dao_drop_container(qcloud_dao_type type, char *file_path, char *table_name);


#ifdef __cplusplus
}
#endif

#endif


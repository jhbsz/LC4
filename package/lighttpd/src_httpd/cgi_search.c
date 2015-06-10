#include "cgi_search.h"
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <ctype.h>
#include <utime.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "memdbg.h"

#define YES 1
#define NO  0

static int  total_match = 0, total_search = 0;
static int  total_file = 0, total_folder = 0;
static char     (*folder_list)[MAX_FILE_PATH] = NULL;
static char (*file_list)[MAX_FILE_PATH] = NULL;

extern char     remote_user[MAX_USER_LEN];

JSON_FILE_OBJECT        *all_file;
JSON_FILE_OBJECT        *all_folder;

static int sorting_by_filename(char *sort_mode, int total);
static int sorting_by_filesize(char *sort_mode, int total);
static int sorting_by_filetype(char *sort_mode, int total);
static int sorting_by_modifiedtime(char *sort_mode, int total);
static int sorting_by_privilege(char *sort_mode, int total);
static int sorting_by_owner(char *sort_mode, int total);
static int sorting_by_group(char *sort_mode, int total);

static SORT_FUNC sort_func[] = { {"filename", sorting_by_filename},
    {"filesize", sorting_by_filesize},
    {"filetype", sorting_by_filetype},
    {"mt", sorting_by_modifiedtime},
    {"privilege", sorting_by_privilege},
    {"owner", sorting_by_owner},
    {"group", sorting_by_group} };

static int filename_compare_asc(const void *f1, const void *f2)
{
    JSON_FILE_OBJECT *a = (JSON_FILE_OBJECT *)f1;
    JSON_FILE_OBJECT *b = (JSON_FILE_OBJECT *)f2;

    if (strcmp(a->filename, b->filename) > 0) return 1;
    if (strcmp(a->filename, b->filename) < 0) return -1;

    return 0;
}

static int filename_compare_desc(const void *f1, const void *f2)
{
    JSON_FILE_OBJECT *a = (JSON_FILE_OBJECT *)f1;
    JSON_FILE_OBJECT *b = (JSON_FILE_OBJECT *)f2;

    if (strcmp(a->filename, b->filename) < 0) return 1;
    if (strcmp(a->filename, b->filename) > 0) return -1;

    return 0;
}

static int sorting_by_filename(char *sort_type, int total)
{
    if (!strcmp(sort_type, "ASC")) {
        qsort(all_folder, total_folder, sizeof(JSON_FILE_OBJECT), filename_compare_asc);
        qsort(all_file, total_file, sizeof(JSON_FILE_OBJECT), filename_compare_asc);
    }
    else if (!strcmp(sort_type, "DESC")) {
        qsort(all_folder, total_folder, sizeof(JSON_FILE_OBJECT), filename_compare_desc);
        qsort(all_file, total_file, sizeof(JSON_FILE_OBJECT), filename_compare_desc);
    }
    return 0;
}

static int filesize_compare_asc(const void *f1, const void *f2)
{
    JSON_FILE_OBJECT *a = (JSON_FILE_OBJECT *)f1;
    JSON_FILE_OBJECT *b = (JSON_FILE_OBJECT *)f2;

    if (a->filesize > b->filesize) return 1;
    if (a->filesize < b->filesize) return -1;

    return 0;
}

static int filesize_compare_desc(const void *f1, const void *f2)
{
    JSON_FILE_OBJECT *a = (JSON_FILE_OBJECT *)f1;
    JSON_FILE_OBJECT *b = (JSON_FILE_OBJECT *)f2;

    if (a->filesize < b->filesize) return 1;
    if (a->filesize > b->filesize) return -1;

    return 0;
}

static int sorting_by_filesize(char *sort_type, int total)
{
    if (!strcmp(sort_type, "ASC")) {
        qsort(all_folder, total_folder, sizeof(JSON_FILE_OBJECT), filesize_compare_asc);
        qsort(all_file, total_file, sizeof(JSON_FILE_OBJECT), filesize_compare_asc);
    }
    else if (!strcmp(sort_type, "DESC")) {
        qsort(all_folder, total_folder, sizeof(JSON_FILE_OBJECT), filesize_compare_desc);
        qsort(all_file, total_file, sizeof(JSON_FILE_OBJECT), filesize_compare_desc);
    }
    return 0;
}

static int filetype_compare_asc(const void *f1, const void *f2)
{
    JSON_FILE_OBJECT *a = (JSON_FILE_OBJECT *)f1;
    JSON_FILE_OBJECT *b = (JSON_FILE_OBJECT *)f2;

    if (strcmp(a->type_str, b->type_str) > 0) return 1;
    if (strcmp(a->type_str, b->type_str) < 0) return -1;

    return 0;
}

static int filetype_compare_desc(const void *f1, const void *f2)
{
    JSON_FILE_OBJECT *a = (JSON_FILE_OBJECT *)f1;
    JSON_FILE_OBJECT *b = (JSON_FILE_OBJECT *)f2;

    if (strcmp(a->type_str, b->type_str) < 0) return 1;
    if (strcmp(a->type_str, b->type_str) > 0) return -1;

    return 0;
}

static int sorting_by_filetype(char *sort_type, int total)
{
    if (!strcmp(sort_type, "ASC")) {
        qsort(all_folder, total_folder, sizeof(JSON_FILE_OBJECT), filetype_compare_asc);
        qsort(all_file, total_file, sizeof(JSON_FILE_OBJECT), filetype_compare_asc);
    }
    else if (!strcmp(sort_type, "DESC")) {
        qsort(all_folder, total_folder, sizeof(JSON_FILE_OBJECT), filetype_compare_desc);
        qsort(all_file, total_file, sizeof(JSON_FILE_OBJECT), filetype_compare_desc);
    }
    return 0;
}

static int modifiedtime_compare_asc(const void *f1, const void *f2)
{
    JSON_FILE_OBJECT *a = (JSON_FILE_OBJECT *)f1;
    JSON_FILE_OBJECT *b = (JSON_FILE_OBJECT *)f2;

    if (a->mt > b->mt) return 1;
    if (a->mt < b->mt) return -1;

    return 0;
}

static int modifiedtime_compare_desc(const void *f1, const void *f2)
{
    JSON_FILE_OBJECT *a = (JSON_FILE_OBJECT *)f1;
    JSON_FILE_OBJECT *b = (JSON_FILE_OBJECT *)f2;

    if (a->mt < b->mt) return 1;
    if (a->mt > b->mt) return -1;

    return 0;
}

static int sorting_by_modifiedtime(char *sort_type, int total)
{
    if (!strcmp(sort_type, "ASC")) {
        qsort(all_folder, total_folder, sizeof(JSON_FILE_OBJECT), modifiedtime_compare_asc);
        qsort(all_file, total_file, sizeof(JSON_FILE_OBJECT), modifiedtime_compare_asc);
    }
    else if (!strcmp(sort_type, "DESC")) {
        qsort(all_folder, total_folder, sizeof(JSON_FILE_OBJECT), modifiedtime_compare_desc);
        qsort(all_file, total_file, sizeof(JSON_FILE_OBJECT), modifiedtime_compare_desc);
    }
    return 0;
}

static int privilege_compare_asc(const void *f1, const void *f2)
{
    JSON_FILE_OBJECT *a = (JSON_FILE_OBJECT *)f1;
    JSON_FILE_OBJECT *b = (JSON_FILE_OBJECT *)f2;

    if (strcmp(a->privilege, b->privilege) > 0) return 1;
    if (strcmp(a->privilege, b->privilege) < 0) return -1;

    return 0;
}

static int privilege_compare_desc(const void *f1, const void *f2)
{
    JSON_FILE_OBJECT *a = (JSON_FILE_OBJECT *)f1;
    JSON_FILE_OBJECT *b = (JSON_FILE_OBJECT *)f2;

    if (strcmp(a->privilege, b->privilege) < 0) return 1;
    if (strcmp(a->privilege, b->privilege) > 0) return -1;

    return 0;
}

static int sorting_by_privilege(char *sort_type, int total)
{
    if (!strcmp(sort_type, "ASC")) {
        qsort(all_folder, total_folder, sizeof(JSON_FILE_OBJECT), privilege_compare_asc);
        qsort(all_file, total_file, sizeof(JSON_FILE_OBJECT), privilege_compare_asc);
    }
    else if (!strcmp(sort_type, "DESC")) {
        qsort(all_folder, total_folder, sizeof(JSON_FILE_OBJECT), privilege_compare_desc);
        qsort(all_file, total_file, sizeof(JSON_FILE_OBJECT), privilege_compare_desc);
    }
    return 0;
}

static int owner_compare_asc(const void *f1, const void *f2)
{
    JSON_FILE_OBJECT *a = (JSON_FILE_OBJECT *)f1;
    JSON_FILE_OBJECT *b = (JSON_FILE_OBJECT *)f2;

    if (strcmp(a->owner, b->owner) > 0) return 1;
    if (strcmp(a->owner, b->owner) < 0) return -1;

    return 0;
}

static int owner_compare_desc(const void *f1, const void *f2)
{
    JSON_FILE_OBJECT *a = (JSON_FILE_OBJECT *)f1;
    JSON_FILE_OBJECT *b = (JSON_FILE_OBJECT *)f2;

    if (strcmp(a->owner, b->owner) < 0) return 1;
    if (strcmp(a->owner, b->owner) > 0) return -1;

    return 0;
}

static int sorting_by_owner(char *sort_type, int total)
{
    if (!strcmp(sort_type, "ASC")) {
        qsort(all_folder, total_folder, sizeof(JSON_FILE_OBJECT), owner_compare_asc);
        qsort(all_file, total_file, sizeof(JSON_FILE_OBJECT), owner_compare_asc);
    }
    else if (!strcmp(sort_type, "DESC")) {
        qsort(all_folder, total_folder, sizeof(JSON_FILE_OBJECT), owner_compare_desc);
        qsort(all_file, total_file, sizeof(JSON_FILE_OBJECT), owner_compare_desc);
    }
    return 0;
}

static int group_compare_asc(const void *f1, const void *f2)
{
    JSON_FILE_OBJECT *a = (JSON_FILE_OBJECT *)f1;
    JSON_FILE_OBJECT *b = (JSON_FILE_OBJECT *)f2;

    if (strcmp(a->group, b->group) > 0) return 1;
    if (strcmp(a->group, b->group) < 0) return -1;

    return 0;
}

static int group_compare_desc(const void *f1, const void *f2)
{
    JSON_FILE_OBJECT *a = (JSON_FILE_OBJECT *)f1;
    JSON_FILE_OBJECT *b = (JSON_FILE_OBJECT *)f2;

    if (strcmp(a->group, b->group) < 0) return 1;
    if (strcmp(a->group, b->group) > 0) return -1;

    return 0;
}

static int sorting_by_group(char *sort_type, int total)
{
    if (!strcmp(sort_type, "ASC")) {
        qsort(all_folder, total_folder, sizeof(JSON_FILE_OBJECT), group_compare_asc);
        qsort(all_file, total_file, sizeof(JSON_FILE_OBJECT), group_compare_asc);
    }
    else if (!strcmp(sort_type, "DESC")) {
        qsort(all_folder, total_folder, sizeof(JSON_FILE_OBJECT), group_compare_desc);
        qsort(all_file, total_file, sizeof(JSON_FILE_OBJECT), group_compare_desc);
    }
    return 0;
}

// return value:
// 1: match
// 0: un-match
int is_match(char *filename, INPUT *keyword)
{
    INPUT   *tmp;
    int ret = 1, i;
    char    tmpfilename[MAX_FILE_LENGTH], tmpkeyword[MAX_FILE_LENGTH];
    //fixed bug #12352 seach file should be without case sensitive
    for (i=0; i<strlen(filename)+1; i++)
        tmpfilename[i] = toupper(filename[i]);

    tmp = keyword;
    while (tmp != NULL)
    {
        for (i=0; i<strlen(tmp->val)+1; i++) {
            tmpkeyword[i] = toupper((tmp->val)[i]);
        }
     
        if (!strcmp(tmpkeyword, "SEARCH"))
            break;

        if (strstr(tmpfilename, tmpkeyword) == NULL)
        {
            DBG("Not The same!!!");
            ret = 0;
            break;
        }
        tmp = tmp->next;
    }
    if (ret) total_match++;
    total_search++;
    DBG("total_match %d", total_match);
    return ret;
}

int output_search_to_json(int start, int end, int sort_mode, char *sort_type, int permission)
{
    int     total_obj = 0, total = total_file + total_folder, i = 0, j = 0, k = 0;
    int file_start = 0, folder_start = 0, file_end = 0, folder_end = 0;
    JSON_FILE_OBJECT    *file;
    struct stat buf;
    struct passwd   *user;
    struct group    *data;
    char    *p = NULL;

    all_file = (JSON_FILE_OBJECT *)__ax_malloc(total_file * sizeof(JSON_FILE_OBJECT));
    all_folder = (JSON_FILE_OBJECT *)__ax_malloc(total_folder * sizeof(JSON_FILE_OBJECT));

    for (k=0; k<total_folder; k++) {
        lstat(folder_list[k], &buf);
        if (S_ISDIR(buf.st_mode)) {
            strcpy((all_folder+k)->filename, folder_list[k]);

            (all_folder+k)->isfolder = 1;
            strcpy((all_folder+k)->type_str, " ");

            (all_folder+k)->filesize = (unsigned long long)(buf.st_size);

            user = getpwuid(buf.st_uid);
            if (user == NULL)
                sprintf((all_folder+k)->owner, "%d", buf.st_uid);
            else
                strcpy((all_folder+k)->owner, user->pw_name);

            data = getgrgid(buf.st_gid);
            if (data == NULL)
                sprintf((all_folder+k)->group, "%d", buf.st_gid);
            else
                strcpy((all_folder+k)->group, data->gr_name);

            (all_folder+k)->mt = buf.st_mtime;
/*
            {
                char sDateStr[64]={0};
                char sTimeStr[64]={0};

                Change_Date_To_String_By_Format(buf.st_mtime, sDateStr, nDate_format);
                Change_Time_To_String_By_Format(buf.st_mtime, sTimeStr, 0, nTime_format);
                sprintf((all_folder+k)->mt_str, "%s %s", sDateStr, sTimeStr);
            }
*/
            sprintf((all_folder+k)->privilege, "%o", (buf.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO)));
        }
    }
    for (k=0; k<total_file; k++) {
        lstat(file_list[k], &buf);
        if (S_ISDIR(buf.st_mode) == 0) {
            strcpy((all_file+k)->filename, file_list[k]);

            (all_file+k)->isfolder = 0;
            p = strrchr(file_list[k], '.');
            if (p != 0) {
                p = p + 1;
                strncpy((all_file+k)->type_str, p, 11);
                (all_file+k)->type_str[11] = '\0';
            }
            else{
                strcpy((all_file+k)->type_str, " ");
            }

            (all_file+k)->filesize = (unsigned long long)(buf.st_size);

            user = getpwuid(buf.st_uid);
            if (user == NULL)
                sprintf((all_file+k)->owner, "%d", buf.st_uid);
            else
                strcpy((all_file+k)->owner, user->pw_name);

            data = getgrgid(buf.st_gid);
            if (data == NULL)
                sprintf((all_file+k)->group, "%d", buf.st_gid);
            else
                strcpy((all_file+k)->group, data->gr_name);

            (all_file+k)->mt = buf.st_mtime;
/*
            {
                char sDateStr[64]={0};
                char sTimeStr[64]={0};

                Change_Date_To_String_By_Format(buf.st_mtime, sDateStr, nDate_format);
                Change_Time_To_String_By_Format(buf.st_mtime, sTimeStr, 0, nTime_format);
                sprintf((all_file+k)->mt_str, "%s %s", sDateStr, sTimeStr);
            }
*/
            sprintf((all_file+k)->privilege, "%o", (buf.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO)));

        }
    }
    //sorting by type
    (sort_func[sort_mode].function)(sort_type, total);

    if (total > end)
        total_obj = end - start + 1;
    else if (total <= end && total >= start)
        total_obj = total - start;
    else {
        if (all_file) __ax_free(all_file);
        if (all_folder) __ax_free(all_folder);
        return -1;
    }

    file = (JSON_FILE_OBJECT *)__ax_malloc(total_obj * sizeof(JSON_FILE_OBJECT));

    memset(file, 0, (total_obj * sizeof(JSON_FILE_OBJECT)));
    //output file and folder count.
    k = 0;

    //output folder first.
    if (start < total_folder) {
        folder_start = start;
        if (total_folder > end)
            folder_end = end + 1;
        else if (total_folder <= end && total_folder >= start)
            folder_end = total_folder;
        else
            goto output_file;
        for (i=folder_start; i<folder_end; i++) {
            strcpy((file+k)->filename, (all_folder+folder_start)->filename);

            (file+k)->isfolder = 1;

            (file+k)->filesize = (all_folder+folder_start)->filesize;

            strcpy((file+k)->owner, (all_folder+folder_start)->owner);

            strcpy((file+k)->group, (all_folder+folder_start)->group);

            (file+k)->mt = (all_folder+folder_start)->mt;

            strcpy((file+k)->mt_str, (all_folder+folder_start)->mt_str);

            strcpy((file+k)->privilege, (all_folder+folder_start)->privilege);

            folder_start++;
            k++;
        }
    }

output_file:
    if (start < (total_folder + total_file)) {

        if (start < total_folder)
            file_start = 0;
        else
            file_start = start - total_folder;
        if ((total_folder+total_file) > end)
            file_end = end + 1;
        else if ((total_folder+total_file) <= end && (total_folder+total_file) >= start)
            file_end = total_folder+total_file;
        else
            goto output_end;

        if (start > folder_end)
            folder_end = start;
        for (j=folder_end; j<file_end; j++) {
            strcpy((file+k)->filename, (all_file+file_start)->filename);

            (file+k)->isfolder = 0;

            (file+k)->filesize = (all_file+file_start)->filesize;

            strcpy((file+k)->owner, (all_file+file_start)->owner);

            strcpy((file+k)->group, (all_file+file_start)->group);

            (file+k)->mt = (all_file+file_start)->mt;

            strcpy((file+k)->mt_str, (all_file+file_start)->mt_str);

            strcpy((file+k)->privilege, (all_file+file_start)->privilege);

            file_start++;
            k++;
        }
    }

    File_List_To_JSON(file, total_obj, total, permission, 0);

output_end:
    if (file) __ax_free(file);
    if (all_file) __ax_free(all_file);
    if (all_folder) __ax_free(all_folder);

    return 0;
}

int _search_by_time(char *source_path, time_t start, time_t end)
{
    int     ret = 0;
    struct stat     statbuf;
    struct dirent   *dir = NULL;
    DIR     *file;
    char    fullpath[2048];

    if ((file=opendir(source_path))==NULL)
        return 0;
    while ((dir=readdir(file)))
    {
        sprintf(fullpath, "%s/%s", source_path, dir->d_name);
        fullpath[strlen(fullpath)] = '\0';
        if (lstat(fullpath, &statbuf)<0) {
            ret = -1;
            break;
        }
        if (S_ISREG(statbuf.st_mode)) {
            if (end != 0) {
                if (statbuf.st_mtime > start && statbuf.st_mtime < end) {
                    total_file++;
                    file_list = (char (*)[MAX_FILE_PATH]) __ax_realloc((void*)file_list, total_file * MAX_FILE_PATH * sizeof(char));
                    strcpy(file_list[total_file - 1], fullpath);
                }
            }
            else if (statbuf.st_mtime > start) {
                total_file++;
                file_list = (char (*)[MAX_FILE_PATH]) __ax_realloc((void*)file_list, total_file * MAX_FILE_PATH * sizeof(char));
                strcpy(file_list[total_file - 1], fullpath);
            }
        }
        else if (S_ISDIR(statbuf.st_mode)) {
            if (!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..") || !strcmp(dir->d_name, ".@__thumb"))
                continue;
            _search_by_time(fullpath, start, end);
        }
    }

    closedir(file);

    return ret;
}

int special_search(INPUT *input)
{
    INPUT *tmp = NULL;
    char *ptr = NULL, tmp_time[12], *folder = NULL;
    struct tm *p;
    time_t timep_begin, timep_end;
    int i;
    //SECTION_INFO *shareList=NULL;
    JSON_FILE_OBJECT *file;
    struct stat buf;
    struct passwd   *user;
    struct group    *data;

    tmp = CGI_Find_Parameter(input, "Time_begin");
    if (tmp == NULL)
        return -1;
    else {
        ptr = tmp->val;
        strncpy(tmp_time, ptr, 4);
        tmp_time[4] = 0x0;
        p = (struct tm *)__ax_malloc(sizeof(struct tm));
        p->tm_year = atoi(tmp_time) - 1900;
        ptr = ptr + 4;
        strncpy(tmp_time, ptr, 2);
        tmp_time[2] = 0x0;
        p->tm_mon = atoi(tmp_time) - 1;
        ptr = ptr + 2;
        strncpy(tmp_time, ptr, 2);
        tmp_time[2] = 0x0;
        p->tm_mday = atoi(tmp_time);
        p->tm_hour = 0;
        p->tm_min = 0;
        p->tm_sec = 0;
        timep_begin = mktime(p);
        if (p) __ax_free(p);
    }
    tmp = CGI_Find_Parameter(input, "Time_end");
    if (tmp == NULL)
        return -1;
    else {
        ptr = tmp->val;
        strncpy(tmp_time, ptr, 4);
        tmp_time[4] = 0x0;
        p = (struct tm *)__ax_malloc(sizeof(struct tm));
        p->tm_year = atoi(tmp_time) - 1900;
        ptr = ptr + 4;
        strncpy(tmp_time, ptr, 2);
        tmp_time[2] = 0x0;
        p->tm_mon = atoi(tmp_time) - 1;
        ptr = ptr + 2;
        strncpy(tmp_time, ptr, 2);
        tmp_time[2] = 0x0;
        p->tm_mday = atoi(tmp_time);
        p->tm_hour = 0;
        p->tm_min = 0;
        p->tm_sec = 0;
        timep_end = mktime(p);
        if (p) __ax_free(p);
    }

    tmp = CGI_Find_Parameter(input, "Folder_name");
    if (tmp == NULL)
        return -1;
    else
        folder = tmp->val;

    /*
    Get_NAS_Share_List_V2(&shareList, 0, 0, NULL, &total_share);

    for (i=0; i<total_share; i++) {
        sprintf(buffer, "/%s/%s", shareList[i].section_name, folder);
        full_path = (char *)__ax_malloc(strlen(folder) + MAX_FILE_LENGTH + 15);
        exist = check_exist(buffer, full_path);
        if (exist < 0) {
            //Status_To_JSON(WFM2_FILE_NO_EXIST);
            //if (full_path) __ax_free(full_path);
            //  return -1;
        }
        else
            _search_by_time(full_path, timep_begin, timep_end);
        if (full_path) __ax_free(full_path);
    }
    */

    file = (JSON_FILE_OBJECT *)__ax_malloc( total_file * sizeof(JSON_FILE_OBJECT));
    for (i=0;i<total_file;i++) {
        lstat(file_list[i], &buf);
        strcpy((file+i)->filename, file_list[i]);
        (file+i)->isfolder = 0;
        (file+i)->filesize = (unsigned long long)(buf.st_size);
        user = getpwuid(buf.st_uid);
        if (user == NULL)
            sprintf((file+i)->owner, "%d", buf.st_uid);
        else
            strcpy((file+i)->owner, user->pw_name);

        data = getgrgid(buf.st_gid);
        if (data == NULL)
            sprintf((file+i)->group, "%d", buf.st_gid);
        else
            strcpy((file+i)->group, data->gr_name);
/*
        {
            char sDateStr[64]={0};
            char sTimeStr[64]={0};

            Change_Date_To_String_By_Format(buf.st_mtime, sDateStr, nDate_format);
            Change_Time_To_String_By_Format(buf.st_mtime, sTimeStr, 0, nTime_format);
            sprintf((file+i)->mt_str, "%s %s", sDateStr, sTimeStr);
        }
*/
        sprintf((file+i)->privilege, "%o", (buf.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO)));

    }
    File_List_To_JSON(file, total_file, total_file, 0, 0);

    if (file) __ax_free(file);
    //if(shareList != NULL)   Release_List(shareList);

    return 0;
}

int _search(char *source_path, INPUT *keyword)
{
    int ret = 0;
    struct stat     statbuf;
    struct dirent   *dir = NULL;
    DIR     *file;
    char    *fullpath;

    if ((file=opendir(source_path))==NULL)
        return 0;
    fullpath = (char*) __ax_malloc(MAX_FILE_PATH * sizeof(char));
    while ((dir=readdir(file)))
    {
        if((strlen(source_path) + strlen(dir->d_name) + 2) > MAX_FILE_PATH) {
            DBG("%s", "Error: path size too large");
            continue;
        }
        sprintf(fullpath, "%s/%s", source_path, dir->d_name);
        fullpath[strlen(fullpath)] = '\0';
        if (lstat(fullpath, &statbuf)<0)
        {
            ret = -1;
            break;
        }
        if (S_ISDIR(statbuf.st_mode)==0)
        {   // file or link
            if (is_match(dir->d_name, keyword)) {
                total_file++;
                file_list = (char (*)[MAX_FILE_PATH]) __ax_realloc((void*)file_list, total_file * MAX_FILE_PATH * sizeof(char));
                strcpy(file_list[total_file - 1], fullpath);
            }
            if (S_ISLNK(statbuf.st_mode))
            {   // do nothing
                continue;
            }
            else
            {
            }
        }
        else
        {   // dir
            //do not include "." and ".." and ".@__thumb"
            if (!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..") || !strcmp(dir->d_name, ".@__thumb"))
                continue;
            if (is_match(dir->d_name, keyword)) {
                //print_file_xml(fullpath);
                //sort_by_type(fullpath, &list);
                total_folder++;
                folder_list = (char (*)[MAX_FILE_PATH]) __ax_realloc((void*)folder_list, total_folder * MAX_FILE_PATH * sizeof(char));
                strcpy(folder_list[total_folder - 1], fullpath);
            }
            _search(fullpath, keyword);
        }
    }
    closedir(file);

    if (fullpath) __ax_free(fullpath);

    return ret;
}

int op_search(INPUT *input)
{
    int ret = 0;
    INPUT   *keyword = NULL, *start = NULL, *limit = NULL;
    INPUT   *source = NULL, *tmp = NULL, *time = NULL;
    char    *full_path = NULL, *source_tmp = NULL, *sort_type = "ASC";
    int exist = 0, end = 0, sort_mode = 0, permission = NO, is_iso = 0;

    //only for internal use
    ret = special_search(input);
    if (ret == 0)
        return 0;

    source = CGI_Find_Parameter(input, "source_path");
    if (source == NULL)
        return -1;

    if(!Check_Illegal_Path(source->val)) {
        Status_To_JSON(WFM2_ILLEGAL_NAME);
        return -1;
    }

    time = CGI_Find_Parameter(input, "time");
    if (time != NULL) {
        char *ptr = NULL, tmp[12];
        struct tm *p;
        time_t timep;
        struct stat buf;
        struct passwd   *user;
        struct group    *data;
        int i;
        JSON_FILE_OBJECT    *file;

        ptr = time->val;
        strncpy(tmp, ptr, 4);
        tmp[4] = 0x0;
        p = (struct tm *)__ax_malloc(sizeof(struct tm));
        p->tm_year = atoi(tmp) - 1900;
        ptr = ptr + 4;
        strncpy(tmp, ptr, 2);
        tmp[2] = 0x0;
        p->tm_mon = atoi(tmp) - 1;
        ptr = ptr + 2;
        strncpy(tmp, ptr, 2);
        tmp[2] = 0x0;
        p->tm_mday = atoi(tmp);
        p->tm_hour = 0;
        p->tm_min = 0;
        p->tm_sec = 0;
        timep = mktime(p);
        if (p) __ax_free(p);

        full_path = (char *)__ax_malloc(strlen(source->val) + MAX_FILE_LENGTH + 15);
        exist = check_exist(source->val, full_path);
        if (exist < 0) {
            Status_To_JSON(WFM2_FILE_NO_EXIST);
            if (full_path) __ax_free(full_path);
            return -1;
        }

        _search_by_time(full_path, timep, 0);

        file = (JSON_FILE_OBJECT *)__ax_malloc( total_file * sizeof(JSON_FILE_OBJECT));
        for (i=0;i<total_file;i++) {
            lstat(file_list[i], &buf);
            strcpy((file+i)->filename, file_list[i]);
            (file+i)->isfolder = 0;
            (file+i)->filesize = (unsigned long long)(buf.st_size);
            user = getpwuid(buf.st_uid);
            if (user == NULL)
                sprintf((file+i)->owner, "%d", buf.st_uid);
            else
                strcpy((file+i)->owner, user->pw_name);

            data = getgrgid(buf.st_gid);
            if (data == NULL)
                sprintf((file+i)->group, "%d", buf.st_gid);
            else
                strcpy((file+i)->group, data->gr_name);
/*
            {
                char sDateStr[64]={0};
                char sTimeStr[64]={0};

                Change_Date_To_String_By_Format(buf.st_mtime, sDateStr, nDate_format);
                Change_Time_To_String_By_Format(buf.st_mtime, sTimeStr, 0, nTime_format);
                sprintf((file+i)->mt_str, "%s %s", sDateStr, sTimeStr);
            }
*/
            sprintf((file+i)->privilege, "%o", (buf.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO)));

        }
        File_List_To_JSON(file, total_file, total_file, 0, 0);

        if (full_path) __ax_free(full_path);
        if (file) __ax_free(file);


        return 0;
    }

    if((tmp = CGI_Find_Parameter(input, "is_iso")))
        is_iso = atoi(tmp->val);
    /*
    source_tmp = (char *)__ax_malloc(strlen(source->val) + 1);

    strcpy(source_tmp, source->val);
    g_access = Get_NAS_User_Security_For_Share_Ex(remote_user, strtok(source_tmp, delim));
    if (g_access == SHARE_NOACCESS) {
        Status_To_JSON(WFM2_PERMISSION_DENY);
        if(source_tmp)
            __ax_free(source_tmp);

        return -1;
    }
    */
    full_path = (char *)__ax_malloc(strlen(source->val) + MAX_FILE_LENGTH + 15);
    exist = check_exist(source->val, full_path);
    if (exist < 0) {
        Status_To_JSON(WFM2_FILE_NO_EXIST);
        if (full_path) __ax_free(full_path);
        return -1;
    }
#ifdef ACL
    //if not iso folder, check permission
    if (Is_ACL_Enabled() && No_Support_ACL(source_tmp) == 0) {
        if (!is_iso) {
            permission= Get_ACL_User_Group_Security_For_Share(remote_user, full_path);
            if (permission < 5) {
                Status_To_JSON(WFM2_PERMISSION_DENY);
                if (full_path) __ax_free(full_path);
                return -1;
            }
        }
    }
#endif
    if(source_tmp)
        __ax_free(source_tmp);

    keyword = CGI_Find_Parameter(input, "keyword");
    if (keyword == NULL)
    {
        //CGI_Free_Input(source);
        if (full_path) __ax_free(full_path);
        return -1;
    }
    start = CGI_Find_Parameter(input, "start");
    if (start == NULL) {
        if (full_path) __ax_free(full_path);
        return -1;
    }
    limit = CGI_Find_Parameter(input, "limit");
    if (limit == NULL) {
        if (full_path) __ax_free(full_path);
        return -1;
    }

    _search(full_path, keyword);

    /*
    if (keyword!=NULL)
        CGI_Free_Input(keyword);
    if (source!=NULL)
        CGI_Free_Input(source);
    */
    int i;

    end = atoi(start->val) + atoi(limit->val) - 1;

    if ((tmp = CGI_Find_Parameter(input, "sort"))) {
        for (i=0; i<sizeof(sort_func)/sizeof(SORT_FUNC); i++) {
            if (!strcmp(sort_func[i].sort_mode, tmp->val)) {
                sort_mode = i;
                break;
            }
        }
    }

    if ((tmp = CGI_Find_Parameter(input, "dir")))
        sort_type = tmp->val;

    output_search_to_json(atoi(start->val), end, sort_mode, sort_type, permission);

    if (full_path) __ax_free(full_path);

    return ret;
}

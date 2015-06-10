#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nas_cgiutil.h"
#include "qbox_op.h"
#include "upload.h"
#include "download.h"
#include "file_op.h"
#include "cgi_search.h"

int main(int argc, char *argv[]) {

    INPUT *input = CGI_INIT();
	INPUT *tmp;
 
    char *sid;

    // Query SID
	if ((tmp = CGI_Find_Parameter(input, "sid")))
    	sid = tmp->val;    
 
    if ((tmp = CGI_Find_Parameter(input, "func"))) {
        // Parsing function command and make opertation
        DBG("funcname: %s", tmp->val);
        if (!strcmp(tmp->val, "qbox_get_sid")) 
            op_qbox_get_sid(input);
        else 
        if (!strcmp(tmp->val, "get_domain_ip_list"))
            op_get_domain_ip_list(NULL);
        else 
        if (!strcmp(tmp->val, "qbox_get_qbox_info"))
            op_qbox_get_qbox_info(input);
        else 
        if (!strcmp(tmp->val, "get_tree"))
            op_get_tree(input);
        else
        if (!strcmp(tmp->val, "get_list"))
            op_get_file_list(input);
        else 
		if (!strcmp(tmp->val, "start_chunked_upload"))
	    	op_get_chunked_upload_id(input);
        else
        if (!strcmp(tmp->val, "chunked_upload"))
            op_chunked_upload(input);
        else 
        if (!strcmp(tmp->val, "download"))
            op_download(input);
        else 
        if (!strcmp(tmp->val, "move"))
            op_move(input);
        else
        if (!strcmp(tmp->val, "get_viewer"))
            op_viewer(input);
        else
        if (!strcmp(tmp->val, "createdir"))
            op_createdir(input);
        else
        if (!strcmp(tmp->val, "delete"))
            op_delete(input);
        else
        if (!strcmp(tmp->val, "copy"))
            op_copy(input);
        else
        if (!strcmp(tmp->val, "get_copy_status"))
            op_get_copy_and_move_status(input);
        else
        if (!strcmp(tmp->val, "get_move_status"))
            op_get_copy_and_move_status(input);
        else
        if (!strcmp(tmp->val, "rename"))
            op_rename(input);
        else
        if (!strcmp(tmp->val, "cancel_copy"))
            op_cancel_move_or_copy(input);
        else
        if (!strcmp(tmp->val, "cancel_move"))
            op_cancel_move_or_copy(input);
        else
        if (!strcmp(tmp->val, "search"))
            op_search(input);
        else
        if (!strcmp(tmp->val, "get_file_size")) 
           op_get_file_size(input);
	else
	if (!strcmp(tmp->val, "delete_chunked_upload_file"))
           op_delete_chunked_upload_file(input);
	else
        if (!strcmp(tmp->val, "get_chunked_upload_status"))
	   op_get_chunked_upload_status(input);
    }
    CGI_RELEASE(input);
    return 0;   
}

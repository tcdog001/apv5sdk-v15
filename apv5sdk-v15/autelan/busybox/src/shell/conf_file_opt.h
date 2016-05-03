/*
set : getnode
	    setoption...
	    setnode
	    destroynode
	    
get : getnode
	    getoption...
	    destroynode
*/

#ifndef _CONF_FILE_OPT_H
#define _CONF_FILE_OPT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include "mxml.h"
#include "mxml_file.h"

#define XERR_NO_ERROR        0
#define XERR_FILE_READ       1
#define XERR_FILE_WRITE      2
#define XERR_XPATH_NOT_FOUND 3
#define XERR_NODE_NOT_FOUND  4
#define XERR_NODE_NOT_LEAF   5
#define XERR_NODE_NO_DATA    6
#define XERR_MEM_ALLOC       7
#define XERR_CLI_SYNTAX      9


MXML_NODE* getnode(const char *str_filename, const char *str_xpath);
int getoption( MXML_NODE *pst_node, const char *str_opt_name, char *str_opt_value, unsigned int un_val_len );
int setoption( MXML_NODE *pst_node, const char *str_opt_name, char *str_opt_value );
int setnode( const char *str_filename, const char *str_xpath, MXML_NODE *pst_node_new );
void destroynode( MXML_NODE *pst_node );
int get_data_by_path( const char *str_file_name, const char *str_xpath, const char *str_arg_name, char *str_value, 
unsigned int un_val_len);
void destroyDoc( MXML_DOCUMENT *pst_doc );


#ifdef __cplusplus
}
#endif

#endif

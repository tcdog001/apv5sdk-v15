

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "conf_file_opt.h"

#define MAX_STR_LEN 32
#define MAX_PATH_LEN 512

MXML_DOCUMENT* loadfile( const char *str_filename )
{
    MXML_DOCUMENT *pst_doc = NULL;
    FILE *fp = NULL;
    int fd_conf;
    
    if ( NULL == (fp=fopen(str_filename, "r")) )
    {
        return NULL;
    }
    
    pst_doc = mxml_document_new();

    fd_conf = fileno( fp );
    if( flock( fd_conf, LOCK_SH ) != 0 )
    {
        fclose( fp );
        return NULL;
    }
    
    mxml_read_file(fp, pst_doc, 0);

    flock( fd_conf, LOCK_UN );
    fclose(fp);
    

    return pst_doc;
}

MXML_NODE* findnode(MXML_DOCUMENT *pst_doc, const char *str_xpath)
{
    MXML_NODE *pst_node = NULL;
    MXML_ITERATOR st_iter;
    char str_path[MAX_PATH_LEN];

    mxml_iterator_setup(&st_iter, pst_doc);
    memset( str_path, 0, MAX_PATH_LEN );

    //can optimize by extend function mxml_iterator_find_first
    while ((pst_node = mxml_iterator_next(&st_iter)) != NULL)
    {
        mxml_node_get_path(pst_node, str_path);
        if (strcmp(str_xpath, str_path) == 0)
        {
            break;
        }
    }
    return pst_node;
}

MXML_NODE* getnode(const char *str_filename, const char *str_xpath)
{
    MXML_DOCUMENT *pst_doc = NULL;
    MXML_NODE *pst_node = NULL;
    MXML_NODE *pst_ret_node = NULL;
    FILE *fp = NULL;

    if( NULL == (pst_doc=loadfile(str_filename)))
    {
        return NULL;
    }

    if( NULL == (pst_node=findnode(pst_doc, str_xpath)) )
    {
        destroyDoc( pst_doc );
        return NULL;
    }

	pst_ret_node = mxml_node_clone_tree( pst_node );

    destroyDoc(pst_doc); 
    
    return pst_ret_node;
}

int getoption( 
               MXML_NODE *pst_node, 
               const char *str_opt_name, 
               char *str_opt_value, 
               unsigned int un_val_len )
{
    MXML_ITERATOR st_iter;
    MXML_NODE *pst_cur_node = NULL;
    unsigned int un_cur_val_len = 0;
    
    mxml_iterator_set_top( &st_iter, pst_node );
    if ( NULL == (pst_cur_node = mxml_iterator_scan_node( &st_iter, str_opt_name )) )
    {
        return XERR_NODE_NOT_FOUND;
    }

    if( pst_cur_node->child != NULL )
    {
        return XERR_NODE_NOT_LEAF;
    }

    if( NULL == pst_cur_node->data )
    {
        return XERR_NODE_NO_DATA;
    }
    
    un_cur_val_len = strlen(pst_cur_node->data)+1;
    if( un_val_len < un_cur_val_len )
    {
        return -un_cur_val_len;
    }
    
    strcpy( str_opt_value, pst_cur_node->data );
    
    return XERR_NO_ERROR;
}

int setoption( 
               MXML_NODE *pst_node, 
               const char *str_opt_name, 
               char *str_opt_value )
{
    MXML_ITERATOR st_iter;
    MXML_NODE *pst_cur_node = NULL;
    
    mxml_iterator_set_top( &st_iter, pst_node );
    if ( NULL == (pst_cur_node = mxml_iterator_scan_node( &st_iter, str_opt_name )) )
    {
        return XERR_NODE_NOT_FOUND;
    }

    if( pst_cur_node->child != NULL )
    {
        return XERR_NODE_NOT_LEAF;
    }

    pst_cur_node->data = MXML_REALLOCATOR( pst_cur_node->data, 
                                           strlen(str_opt_value)+1 );
    
    strcpy( pst_cur_node->data, str_opt_value );
    return XERR_NO_ERROR;
}

int setnode( const char *str_filename, const char *str_xpath, MXML_NODE *pst_node_new )
{
    MXML_DOCUMENT *pst_doc = NULL;
    MXML_NODE *tmp = NULL;
    MXML_NODE *pst_node_old = NULL;
    MXML_ITERATOR st_iter;
    FILE *fp = NULL;
    int fd_conf;

    if( NULL == (pst_doc=loadfile(str_filename)))
    {
        return XERR_FILE_READ;
    }

    if( NULL == (pst_node_old=findnode(pst_doc, str_xpath)) )
    {
        destroyDoc( pst_doc );
        return XERR_NODE_NOT_FOUND;
    }

    mxml_node_insert_after( pst_node_old, pst_node_new );
    mxml_node_unlink( pst_node_old );
    mxml_node_destroy( pst_node_old );

    if ( NULL == (fp=fopen(str_filename, "w+")) )
    {
        destroyDoc( pst_doc );
        return XERR_FILE_WRITE;
    }
    
    fd_conf = fileno( fp );
    if( flock( fd_conf, LOCK_EX ) != 0 )
    {
        destroyDoc( pst_doc );
        fclose( fp );
        return XERR_FILE_WRITE;
    }
    mxml_write( pst_doc, str_filename, MXML_STYLE_INDENT );

    flock( fd_conf, LOCK_UN );
    fclose( fp );
    
    destroyDoc( pst_doc );
    
    return XERR_NO_ERROR;
}

void destroyDoc( MXML_DOCUMENT *pst_doc )
{
    if( NULL != pst_doc->root )
    {
        destroynode( pst_doc->root );
    }
    mxml_document_destroy( pst_doc );
}

void destroynode( MXML_NODE *pst_node )
{
    mxml_node_destroy( pst_node );
}

int get_data_by_path( const char *str_conf_file, const char *str_xpath, const char *str_arg_name, char *str_value, 
unsigned int un_val_len)
{
    MXML_NODE* pst_node = NULL;
	int res = XERR_NO_ERROR;
	
	if( NULL == (pst_node=getnode(str_conf_file, str_xpath)) )
	{
		return XERR_XPATH_NOT_FOUND;
    }
    
    res = getoption( pst_node, str_arg_name, str_value, un_val_len );
           
    destroynode(pst_node);

    if( XERR_NO_ERROR == res || XERR_NODE_NO_DATA == res || res < 0 )
    {
        return res;
    }
    
    return XERR_NODE_NOT_FOUND;
}

int set_data_by_path( const char *str_conf_file, const char *str_xpath, const char *str_arg_name, char *str_value )
{
    MXML_NODE* pst_node = NULL;
	int res = XERR_NO_ERROR;
	
	if( NULL == (pst_node=getnode(str_conf_file, str_xpath)) )
	{
		return XERR_XPATH_NOT_FOUND;
    }
    
    res = setoption( pst_node, str_arg_name, str_value );
    
    if( res != XERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return XERR_NODE_NOT_FOUND;
    }
    
    if( setnode( str_conf_file, str_xpath, pst_node ) != XERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return XERR_FILE_WRITE;
    }

    return XERR_NO_ERROR;
}
/*
int get_block_val( const char *str_xpath )
{
}

int getblock_by_path( const char *str_xpath, pst_data_block )
{
}*/

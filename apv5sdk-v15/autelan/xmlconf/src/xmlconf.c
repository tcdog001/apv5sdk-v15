/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: xmlconf.c
* description:  implementation for xmlconf command
* 
*
* 
************************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mxml.h>
#include <mxml_file.h>

#define USAGE "\n\
Usage:\n\
    xmlconf readconf <file> <XPath>\n\
    xmlconf writeconf <file> <XPath> <filesubset>\n\
    xmlconf getoption <file> <name>\n\
    xmlconf setoption <file> <name> <value>\n\
\n\
Return Value:\n\
    0 No Error\n\
    1 File Read Error\n\
    2 File Write Error\n\
    3 XML Path Not Found\n\
    4 XML Node Not Found\n\
    5 XML Node Not Leaf\n\
    6 Memory Alloc Error\n\
    9 CLI Syntax Error\n\
\n"

#define XERR_NO_ERROR        0
#define XERR_FILE_READ       1
#define XERR_FILE_WRITE      2
#define XERR_XPATH_NOT_FOUND 3
#define XERR_NODE_NOT_FOUND  4
#define XERR_NODE_NOT_LEAF   5
#define XERR_MEM_ALLOC       6
#define XERR_CLI_SYNTAX      9

MXML_DOCUMENT* xml_loadfromfile(char *filename);
MXML_NODE* xml_querybypath(MXML_DOCUMENT *doc, char *xpath);
MXML_NODE* xml_querybyname(MXML_DOCUMENT *doc, char *nodename);

int readconf(char *filename, char *xpath)
{
MXML_DOCUMENT *doc;
MXML_NODE *node;
MXML_OUTPUT out;

    doc = xml_loadfromfile(filename);
    if (doc == NULL)
        return XERR_FILE_READ;

    node = xml_querybypath(doc, xpath);
    if (node == NULL)
    {
        mxml_document_destroy(doc);
        return XERR_XPATH_NOT_FOUND;
    }

    mxml_output_setup(&out, mxml_output_func_to_stream, 0);
    out.data = stdout;
    mxml_node_write(&out, node, MXML_STYLE_INDENT);

    mxml_document_destroy(doc);

    return XERR_NO_ERROR;
}

int writeconf(char *filename, char *xpath, char *filenamesubset)
{
MXML_DOCUMENT *doc1;
MXML_DOCUMENT *doc2;
MXML_NODE *node1;
MXML_NODE *node2;

    /* load the original file */
    doc1 = xml_loadfromfile(filename);
    if (doc1 == NULL)
        return XERR_FILE_READ;

    /* locate the portion to be updated */
    node1 = xml_querybypath(doc1, xpath);
    if (node1 == NULL)
    {
        mxml_document_destroy(doc1);
        return XERR_XPATH_NOT_FOUND;
    }

    /* load the file contain updates */
    doc2 = xml_loadfromfile(filenamesubset);
    if (doc2 == NULL)
    {
        mxml_document_destroy(doc1);
        return XERR_FILE_READ;
    }

    /* node2 is clone from doc2 */
    node2 = mxml_node_clone_tree(doc2->root->child);

    /* insert node2 after node1 */
    mxml_node_insert_after(node1, node2);

    /* unlink node1 from doc1, and destory */
    mxml_node_unlink(node1);
    mxml_node_destroy(node1);

    /* write back to disk */
    mxml_write(doc1, filename, MXML_STYLE_INDENT);

    mxml_document_destroy(doc1);
    mxml_document_destroy(doc2);

    return XERR_NO_ERROR;
}

int getoption(char *filename, char *optname)
{
MXML_DOCUMENT *doc;
MXML_NODE *node;
MXML_OUTPUT out;

    doc = xml_loadfromfile(filename);
    if (doc == NULL)
        return XERR_FILE_READ;

    node = xml_querybyname(doc, optname);
    if (node == NULL)
    {
        mxml_document_destroy(doc);
        return XERR_NODE_NOT_FOUND;
    }

    if (node->child != NULL)
    {
        mxml_document_destroy(doc);
        return XERR_NODE_NOT_LEAF;
    }

    mxml_output_setup(&out, mxml_output_func_to_stream, 0);
    out.data = stdout;
    if (node->data != NULL) mxml_output_string(&out, node->data);

    mxml_document_destroy(doc);

    return XERR_NO_ERROR;
}

int setoption(char *filename, char *optname, char *optvalue)
{
MXML_DOCUMENT *doc;
MXML_NODE *node;
int len;

    doc = xml_loadfromfile(filename);
    if (doc == NULL)
        return XERR_FILE_READ;

    node = xml_querybyname(doc, optname);
    if (node == NULL)
    {
        /* create an empty option */
        node = mxml_node_new();
        if (node == NULL)
        {
            mxml_document_destroy(doc);
            return XERR_NODE_NOT_FOUND;
        }

        /* set option name */
        len = strlen(optname);
        node->name = MXML_ALLOCATOR(len + 1);
        if (node->name == NULL)
        {
            mxml_node_destroy(node);
            mxml_document_destroy(doc);
            return XERR_MEM_ALLOC;
        }
        else
        {
            strcpy(node->name, optname);
            mxml_node_add_below(doc->root->child, node);
        }
    }

    if (node->child != NULL)
    {
        mxml_document_destroy(doc);
        return XERR_NODE_NOT_LEAF;
    }

    if (node->data != NULL)
    {
        MXML_DELETOR(node->data);
    }

    /* set option value */
    len = strlen(optvalue);
    node->data = MXML_ALLOCATOR(len + 1);
    if (node->data == NULL)
    {
        mxml_document_destroy(doc);
        return XERR_MEM_ALLOC;
    }
    else
    {
        strcpy(node->data, optvalue);
        node->data_length = len;
    }

    mxml_write_file(doc, stdout, MXML_STYLE_INDENT);

    mxml_document_destroy(doc);

    return XERR_NO_ERROR;
}

void usage()
{
    fprintf(stderr, USAGE);
}

int main(int argc, char *argv[])
{
    if (argc == 4 && strcmp(argv[1], "readconf") == 0)
    {
        return readconf(argv[2], argv[3]);
    }
    else if (argc == 5 && strcmp(argv[1], "writeconf") == 0)
    {
        return writeconf(argv[2], argv[3], argv[4]);
    }
    else if (argc == 4 && strcmp(argv[1], "getoption") == 0)
    {
        return getoption(argv[2], argv[3]);
    }
    else if (argc == 5 && strcmp(argv[1], "setoption") == 0)
    {
        return setoption(argv[2], argv[3], argv[4]);
    }
    else
    {
        usage();
        return XERR_CLI_SYNTAX;
    }

}

MXML_DOCUMENT* xml_loadfromfile(char *filename)
{
MXML_DOCUMENT *doc;
FILE *fp;

    if (strcmp(filename, "-") == 0)
        fp = stdin;
    else
    {
        fp = fopen(filename, "r");
        if ( fp == NULL )
        {
            return NULL;
        }
    }

    doc = mxml_document_new();
    mxml_read_file(fp, doc, 0);

    if (fp != stdin) fclose(fp);

    return doc;
}

MXML_NODE* xml_querybypath(MXML_DOCUMENT *doc, char *nodepath)
{
MXML_ITERATOR iter;
MXML_NODE *node;
char path[1024];
int len;

    mxml_iterator_setup(&iter, doc);
    while ((node = mxml_iterator_next(&iter)) != NULL)
    {
        len = mxml_node_get_path(node, path);
        if (strcmp(nodepath, path) == 0)
        {
            break;
        }
    }

    return node;
}

MXML_NODE* xml_querybyname(MXML_DOCUMENT *doc, char *nodename)
{
MXML_ITERATOR iter;

    mxml_iterator_setup(&iter, doc);
    return mxml_iterator_scan_node(&iter, nodename);
}



/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: TreeFunctions.h
* description:  the header file for Tree functions on gSoap DOM
* 
*
* 
************************************************************************************/
#ifndef _TREEFUNCTIONS_H
#define _TREEFUNCTIONS_H

#include "StubH.h"
#include "const.h"

// dom function declaration
SOAP_FMAC1 struct soap_dom_element * SOAP_FMAC2 soap_in_xsd__anyType(struct soap*, const char*, struct soap_dom_element *, const char*);
SOAP_FMAC1 void SOAP_FMAC2 soap_default_xsd__anyType(struct soap*, struct soap_dom_element *);


struct soap_dom_attribute *copy_attributes(struct soap *_soap, struct soap_dom_attribute *_att_src);
struct soap_dom_attribute *add_attributes(struct soap *_soap, struct soap_dom_element *_node, const char *_name, const char *_data);
struct soap_dom_attribute *search_attribute(struct soap_dom_element *_node, const char *_attributeName);
struct soap_dom_element *find_attributes_in_branch(struct soap_dom_element *_node);
short AttributeExists(struct soap_dom_attribute *_atts, char *strToFind);

struct soap_dom_element *search_entire_element(struct soap_dom_element *_node, const char *_name);
struct soap_dom_element *search_element(struct soap_dom_element *_node, const char *_name);
struct soap_dom_element *tree_next_element(struct soap_dom_element *_elt);
void print_tree(struct soap_dom_element *_node, struct soap_dom_element *_parent, int _nlevel_spaces);

struct soap_dom_element *copy_branch(struct soap *_soap, struct soap_dom_element *_src, struct soap_dom_element *_elt);
void merge_branch(struct soap *_soap, struct soap_dom_element *_root, struct soap_dom_element *_src);

void manage_instances_in_branch(struct soap *_soap, struct soap_dom_element *_node, unsigned int _Id);
struct soap_dom_element *merge_branch_in_tree(struct soap *_soap, struct soap_dom_element *_tree, struct soap_dom_element *_branch);

struct soap_dom_element *str_parameter_path_original(struct soap_dom_element *_node, char *_szSrcPath, char *_szDestPath);

#endif // _TREEFUNCTIONS_H

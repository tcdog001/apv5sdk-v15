
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: TreeFunctions.c 
* description:  implementation for Tree functions on gSoap DOM  for parsing, adding node(s) and attribute(s) in DOM tree 
* 
*
* 
************************************************************************************/

#include <stdio.h>
#include "const.h"
#include "TreeFunctions.h"
#include "tools.h"

/*********************************************/
/*** copy_attributes                       ***/
/*** copy attributes of source DOM node    ***/
/*** in destination node                   ***/
/*********************************************/
struct soap_dom_attribute *copy_attributes(struct soap *_soap, struct soap_dom_attribute *_att_src)
{
struct soap_dom_attribute *src, *dst, *previous = NULL, *atts = NULL;

  for (src = _att_src; (src != NULL); src = src->next)
  {
    dst = (struct soap_dom_attribute*)soap_malloc(_soap, sizeof(struct soap_dom_attribute));
    if (atts == NULL) // atts points on attributes list
      atts = dst;

    if (previous != NULL)
      previous->next = dst;
    dst->next = NULL;
    if (src->nstr != NULL)
      dst->nstr = soap_strdup(_soap, src->nstr); /* namespace string */
    if (src->name)
      dst->name = soap_strdup(_soap, src->name);
    if (src->data)
      dst->data = soap_strdup(_soap, src->data);

    dst->soap = _soap;
    previous = dst;
  }
  return atts;
}


/*********************************************/
/*** add_attributes                        ***/
/*** add attributes of source DOM node     ***/
/*********************************************/
struct soap_dom_attribute *add_attributes(struct soap *_soap, struct soap_dom_element *_node, const char *_name, const char *_data)
{
struct soap_dom_attribute *elt;

  // go at end of list
  elt = _node->atts; // atts points on attributes list
  if (elt != NULL)
  {
    while (elt->next)
    {
      elt = elt->next;
    }

    // allocate new attribute element
    elt->next = (struct soap_dom_attribute*)soap_malloc(_soap, sizeof(struct soap_dom_attribute));
    elt = elt->next;
  }
  else
  { // first element attribute
    _node->atts = (struct soap_dom_attribute*)soap_malloc(_soap, sizeof(struct soap_dom_attribute));
    elt = _node->atts;
  }

  memset(elt, 0x00, sizeof(struct soap_dom_attribute));

  if (elt != NULL)
  {
    elt->next = NULL;
    elt->nstr = NULL;
    elt->name = soap_strdup(_soap, _name);
    elt->data = soap_strdup(_soap, _data);

    elt->soap = _soap;
  }
  return elt;
}


/*********************************************/
/*** search_attribute                      ***/
/*** parse DOM node attributes list        ***/
/*** return value corresponding to         ***/
/*** attribute name                        ***/
/*** NULL if attribute name not found      ***/
/*** attribute element else                ***/
/*********************************************/
struct soap_dom_attribute *search_attribute(struct soap_dom_element *_node, const char *_attributeName)
{
struct soap_dom_attribute *elt;

  // search in DOM attribute list
  for (elt = _node->atts; elt; elt = elt->next)
  {
    // is word same than node name
    if (elt && elt->name && (strcmp(_attributeName, elt->name) == 0))
    {
      // copy data
      if (elt->data) return elt;
      else return NULL;
    }
  }
  return NULL;
}


//add by wangjr 091124
struct soap_dom_attribute *update_notification(struct soap_dom_element *_node, const char *_attributeName,int notification)
{
	struct soap_dom_attribute *elt;
	char tmp[10]={0};
	if(notification==2)
	{
		strcpy(tmp,"ACTIVE");
	}
	else if(notification==1)
	{
		strcpy(tmp,"PASSIVE");
	}
	else
	{
		strcpy(tmp,"NONE");
	}

  // search in DOM attribute list
  for (elt = _node->atts; elt; elt = elt->next)
  {
    // is word same than node name
    if (elt && elt->name && (strcmp(_attributeName, elt->name) == 0))
    {
      // copy data
      if (elt->data)
      {
      	strcpy(elt->data,tmp);
	  	return elt;
      }
      else return NULL;
    }
  }
  return NULL;
}
//add by wangjr 091124
struct soap_dom_attribute *update_isexist_no(struct soap_dom_element *_node, const char *_attributeName)
{
struct soap_dom_attribute *elt;

  // search in DOM attribute list
  for (elt = _node->atts; elt; elt = elt->next)
  {
    // is word same than node name
    if (elt && elt->name && (strcmp(_attributeName, elt->name) == 0))
    {
      // copy data
      if (elt->data)
      {
        if(atoi(elt->data)==1)
        {
          strcpy(elt->data,"0");
        }
		else
		{
		  strcpy(elt->data,"0");
		}
	  	return elt;
      }
      else return NULL;
    }
  }
  return NULL;
}
//add by wangjr  091124
struct soap_dom_attribute *update_isexist_yes(struct soap_dom_element *_node, const char *_attributeName)
{
struct soap_dom_attribute *elt;

  // search in DOM attribute list
  for (elt = _node->atts; elt; elt = elt->next)
  {
    // is word same than node name
    if (elt && elt->name && (strcmp(_attributeName, elt->name) == 0))
    {
      // copy data
      if (elt->data)
      {
        if(atoi(elt->data)==0)
        {
          strcpy(elt->data,"1");
        }
		else
		{
		  strcpy(elt->data,"1");
		}
	  	return elt;
      }
      else return NULL;
    }
  }
  return NULL;
}

/*********************************************/
/*** find_attributes_in_branch             ***/
/*** parse DOM branch                      ***/
/*** return first node with attributes     ***/
/*** pointer different of NULL             ***/
/*** NULL if no attributes list found      ***/
/*********************************************/
struct soap_dom_element *find_attributes_in_branch(struct soap_dom_element *_node)
{
struct soap_dom_element *elt=NULL;

  if (_node->atts != NULL)
    elt = _node;
  else if (_node->elts != NULL)
    return find_attributes_in_branch(_node->elts);
  return elt;
}


/*********************************************/
/*** AttributeExists                       ***/
/*** parse node Attributes to see if       ***/
/***  attribute name exists                ***/
/*********************************************/
short AttributeExists(struct soap_dom_attribute *_atts, char *strToFind)
{
short bAttributeFound = 0;
struct soap_dom_attribute *attr;

  for (attr = _atts; attr; attr = soap_dom_next_attribute(attr))
  {
    if (strcmp((*attr).name, strToFind) == 0)
    {
      bAttributeFound = 1;
      break;
    }
  }
  return bAttributeFound;
}


/*********************************************/
/*** search_entire_element                 ***/
/*** search a name in DOM tree             ***/
/*** return element found                  ***/
/*** else NULL                             ***/
/*** ex.: Device.DeviceInfo.Manufacturer   ***/
/*********************************************/
struct soap_dom_element *search_entire_element(struct soap_dom_element *_node, const char *_name)
{
char *ptr;
struct soap_dom_element *iter=NULL;
struct soap_dom_element *elt=NULL;
char szSubString[ARGSIZE];

  memset(szSubString, 0x00, ARGSIZE);
  if(_name)
  dprintf(LOG_LEVEL1,"tree:name:%s\n",_name);
  ptr = strExtractWord((char *)_name, '.', szSubString);
  if(szSubString)
  dprintf(LOG_LEVEL1,"tree:%s\n",szSubString);
  // search recursively in DOM tree
  // stop when node name found (elt != NULL)
  for (iter = _node; iter && !elt; iter = tree_next_element(iter))
  {
    // is word same than node name
    dprintf(LOG_LEVEL1,"tree:itername:%s\n",iter->name);
    if (iter && iter->name && (strcmp(szSubString, iter->name) == 0))
    {
      // search next part of string
	  if(ptr)
	  dprintf(LOG_LEVEL1,"tree:left:%s\n",ptr);
      if (ptr) elt = search_entire_element(iter->elts, ptr);
      else {
		dprintf(LOG_LEVEL1,"return iter\n");
		return iter;
      }
    }
  }

  return elt;
}

/*********************************************/
/*** search_element                        ***/
/*** search a node name in DOM branch      ***/
/*** return element found                  ***/
/*** else NULL                             ***/
/*** ex.: Manufacturer                     ***/
/*********************************************/
struct soap_dom_element *search_element(struct soap_dom_element *_node, const char *_name)
{
struct soap_dom_element *elt=NULL;

  if ((_node->name) && strcmp(_node->name, _name) == 0)
    elt = _node;
  else if (_node->elts != NULL)
    return search_element(_node->elts, _name);

  return elt;
}


/*********************************************/
/*** tree_next_element                     ***/
/*** get next element                      ***/
/*** different than soap_dom_next_element!!***/
/*********************************************/
struct soap_dom_element *tree_next_element(struct soap_dom_element *_elt)
{ 
  if (_elt->next)
    return _elt->next;
  return NULL;
}


/*********************************************/
/*** print_tree                            ***/
/*** print recursively nodes set tree      ***/
/*** arguments:                            ***/
/***  dom node                             ***/
/***  parent dom node                      ***/
/***  number of padding char               ***/
/*********************************************/
void print_tree(struct soap_dom_element *_node, struct soap_dom_element *_parent, int _nlevel_spaces)
{
struct soap_dom_element *iter;
struct soap_dom_attribute *attr;
int i;

  for (iter = _node; iter; iter = tree_next_element(iter))
  {
    if (/*(iter->prnt == _parent) &&*/ (iter && iter->name))
    {
      // padding
      if (_nlevel_spaces > 0)
      {
        for (i=0; i < (_nlevel_spaces-strlen(_parent->name)); i++)
          dprintf(LOG_LEVEL2, " ");
        dprintf(LOG_LEVEL2, "|");
        for (i=1; i < strlen(_parent->name); i++)
          dprintf(LOG_LEVEL2, "-");
      }

      // node informations
      dprintf(LOG_LEVEL2, "%s", (*iter).name);

      // value
      if (iter && iter->data)
        dprintf(LOG_LEVEL2, " ===> %s", (*iter).data);

      // attributes
      if (iter && iter->atts)
      {
        for (attr = iter->atts; attr; attr = soap_dom_next_attribute(attr))
        {
          dprintf(LOG_LEVEL2, ", %s=%s", (*attr).name, (*attr).data);
        }
      }

      dprintf(LOG_LEVEL2, "\n");

      // recursive call
      print_tree(iter->elts, iter, _nlevel_spaces+strlen((*iter).name));
    }
  }
}


/*********************************************/
/*** copy_branch                           ***/
/*** copy a DOM branch in another branch   ***/
/*** new elements are allocated in another ***/
/*** soap context !                        ***/
/*********************************************/
struct soap_dom_element *copy_branch(struct soap *_soap, struct soap_dom_element *_src, struct soap_dom_element *_elt)
{
struct soap_dom_element *dst;

  if (_src == NULL) 
  {
    return NULL;
  }
  else
  {
    dst = (struct soap_dom_element*)soap_malloc(_soap, sizeof(struct soap_dom_element) );

    soap_default_xsd__anyType(_soap, dst);

    dst->next = NULL;  /* next sibling */
    dst->prnt = _elt;  /* parent */
    if (_src->atts != NULL) 
      dst->atts = copy_attributes(_soap, _src->atts);

    dst->name = soap_strdup(_soap, _src->name);       /* element tag name */

    if ((_src->elts != NULL) && (dst != NULL))
      dst->elts = copy_branch(_soap, _src->elts, dst);

    dst->nstr = soap_strdup(_soap, _src->nstr); /* namespace string */
    dst->data = soap_strdup(_soap, _src->data); /* element content data (with SOAP_C_UTFSTRING flag set) */
    dst->type = _src->type;
    return dst;
  }
}

/*********************************************/
/*** merge_branch                          ***/
/*** merge a DOM branch in dom tree        ***/
/*** new elements are allocated in another ***/
/*** soap context !                        ***/
/*********************************************/
void merge_branch(struct soap *_soap, struct soap_dom_element *_root, struct soap_dom_element *_src)
{
  // parse dom tree node to find name
  if ((_root!= NULL) && (strcmp(_root->name, _src->name) == 0))
  {
    // next step : compare root childs and node child
    if ((_root->elts != NULL) && (_src->elts != NULL))
      merge_branch(_soap, _root->elts, _src->elts);
    else if (_root->elts == NULL)
      // copy branch in child
      _root->elts = copy_branch(_soap, _src, _root);
    else return;
  }
  else
  {
    // copy branch in next sibling
    if (_root->next != NULL)
    {
      while (_root->next != NULL)
        _root = _root->next;
    }

    _root->next = copy_branch(_soap, _src, _root->prnt);
  }
}



//-----------------------------
// MULTIPLE INSTANCES FUNCTIONS
//

/*********************************************/
/*** manage_instances_in_branch            ***/
/*** parse DOM branch to manage            ***/
/*** objects instances                     ***/
/*********************************************/
void manage_instances_in_branch(struct soap *_soap, struct soap_dom_element *_branch, unsigned int _Id)
{
	struct soap_dom_element *iter, *prnt;
	struct soap_dom_attribute *attr;
	char *ptr;
	short bAttributeFound;
	short NumberOfEntries;
	char szTemp[10];

  for (iter = _branch; iter; iter = tree_next_element(iter))
  {
    if (iter && iter->name)
    {
      // Test if name begin with "INSTANCE"
      if (strncmp(iter->name, SZ_INSTANCE, strlen(SZ_INSTANCE)) ==0)
      {
        dprintf(LOG_LEVEL3, "INSTANCE found\n");
        ptr = &iter->name[strlen(SZ_INSTANCE)];
        dprintf(LOG_LEVEL3, "ptr: %s\n", ptr);

        // retrieve attribute NumberOfEntries of its parent
        if (iter->prnt) {
          prnt = iter->prnt;
          dprintf(LOG_LEVEL3, "parent->name: %s\n", prnt->name);
          bAttributeFound = 0;
          // attributes
          if (prnt->atts) {
            for (attr = prnt->atts; attr; attr = soap_dom_next_attribute(attr))
            {
              if (strcmp((*attr).name, SZ_NUMBEROFENTRIES) == 0) {
                dprintf(LOG_LEVEL3, ", %s=%s", (*attr).name, (*attr).data);
                NumberOfEntries = atoi((*attr).data);
                bAttributeFound = 1;
                break;
              }
            }
          }

          // If attribute NumberOfEntries doesn't exist then create it
          if (!bAttributeFound) 
		  {
            // attribute NumberOfEntries doesn't exist, add it
            dprintf(LOG_LEVEL3, "attribute NumberOfEntries not found, add it\n");
            NumberOfEntries = 1;
            sprintf(szTemp, "%d", NumberOfEntries);
            add_attributes(_soap, prnt, SZ_NUMBEROFENTRIES, szTemp);
          }
          else
		  {
            dprintf(LOG_LEVEL3, "\nattribute NumberOfEntries found\n");
            // force NumberOfEntries value to 1
            sprintf((*attr).data, "1");
          }

          // add to the instance object, the attribute Id of its manager 
          sprintf(szTemp, "%d", _Id);
          add_attributes(_soap, iter, "Id", szTemp);
  
          // add to the instance object, the original value (manager declaration)
          add_attributes(_soap, iter, SZ_INSTANCE, iter->name);
  
          // rename the instance object
          sprintf(iter->name, "%d", atoi(ptr));
          dprintf(LOG_LEVEL3, "iter->name: %s\n", iter->name);

        }
      }

      dprintf(LOG_LEVEL3, "\n");
    }
    // recursive call
    manage_instances_in_branch(_soap, iter->elts, _Id);
  }
}


/*********************************************/
/*** merge_branch_in_tree                  ***/
/*** parse DOM branch and DOM tree         ***/
/*** take care of object instances         ***/
/*** make a new instance if id different   ***/
/*** or instance different                 ***/
/*********************************************/
struct soap_dom_element *merge_branch_in_tree(struct soap *_soap, struct soap_dom_element *_tree, struct soap_dom_element *_branch)
{
struct soap_dom_element *iter_branch, *iter_tree;
struct soap_dom_attribute *attr;
short bInstanceFound;
short nInstanceBranch, nInstanceTree;
short nIdBranch, nIdTree;
short NumberOfEntries;
char *ptr;

  short bAdd_Child = 0;
  iter_branch = _branch;
  iter_tree= _tree;

  if ((iter_tree == NULL) || (iter_tree->name == NULL))
    return NULL;
  if ((iter_branch == NULL) || (iter_branch->name == NULL))
    return NULL;

  bInstanceFound = 0;
  // test first character to know if instance node
  dprintf(LOG_LEVEL3, "branch name:%s\n",iter_branch->name);
  dprintf(LOG_LEVEL3, "tree name:%s\n",iter_tree->name);
  if ((iter_branch->name[0] >= '0') && (iter_branch->name[0] <= '9')) {
  	dprintf(LOG_LEVEL3, "----->is instance\n");
   // bInstanceFound = 1;
  }

  // search instance value of branch node
  for (attr = iter_branch->atts; attr; attr = soap_dom_next_attribute(attr)) {
    if (strcmp((*attr).name, SZ_INSTANCE) == 0)
    {
      ptr = (*attr).data+strlen(SZ_INSTANCE);
      nInstanceBranch = atoi(ptr);
      break;
    }
  }

  // search instance value of tree node
  for (attr = iter_tree->atts; attr; attr = soap_dom_next_attribute(attr)) {
    if (strcmp((*attr).name, SZ_INSTANCE) == 0) {
      ptr = (*attr).data+strlen(SZ_INSTANCE);
      nInstanceTree = atoi(ptr);
      break;
    }
  }

  // search id value of branch node
  for (attr = iter_branch->atts; attr; attr = soap_dom_next_attribute(attr)) {
    if (strcmp((*attr).name, "Id") == 0) {
      nIdBranch = atoi((*attr).data);
      break;
    }
  }

  // search id value of tree node
  for (attr = iter_tree->atts; attr; attr = soap_dom_next_attribute(attr)) {
    if (strcmp((*attr).name, "Id") == 0) {
      nIdTree = atoi((*attr).data);
      break;
    }
  }

  // if not instance object, compare branch node name and tree node name
  // else compare instance values and id values
  if (((bInstanceFound == 0) && (strcmp(iter_branch->name, iter_tree->name) == 0)) ||
      ((bInstanceFound == 1) && ((nInstanceBranch == nInstanceTree) && (nIdBranch == nIdTree))))
  {
    // same name or same instance and same manager, so go ahead in branch
    return merge_branch_in_tree(_soap, iter_tree->elts, iter_branch->elts);
  }
  else
  {
    // compare branch node and tree next child
    if (iter_tree->next == NULL) 
    {
      bAdd_Child = 1;
      goto add_child;
    }
    else return merge_branch_in_tree(_soap, iter_tree->next, iter_branch);
  }

add_child:
  if (bAdd_Child)
  {
    // different node, copy branch
    // search attribute in branch node (for instance objects)
    bInstanceFound = AttributeExists(iter_branch->atts, SZ_INSTANCE);
    if (bInstanceFound) // instance object
    {
      // add NumberOfEntries of parent
      for (attr = iter_tree->prnt->atts; attr; attr = soap_dom_next_attribute(attr))
      {
        if (strcmp((*attr).name, SZ_NUMBEROFENTRIES) == 0)
        {
          NumberOfEntries = atoi((*attr).data)+1;
          sprintf((*attr).data, "%d", NumberOfEntries);
          break;
        }
      }

      // add 1 to instance name of branch node 
      sprintf(iter_branch->name, "%d", NumberOfEntries);
    }

    merge_branch(_soap, iter_tree, iter_branch);
    return iter_tree;
  }

  return NULL;
}



/*********************************************/
/*** str_parameter_path_original           ***/
/*** parse DOM branch with parameter path  ***/
/*** take care of object instances         ***/
/*** return parameter path                 ***/
/*********************************************/
struct soap_dom_element *str_parameter_path_original(struct soap_dom_element *_node, char *_szSrcPath, char *_szDestPath)
{
char *ptr;
struct soap_dom_element *iter=NULL;
struct soap_dom_element *elt=NULL;
struct soap_dom_attribute *attr;
char szSubString[ARGSIZE];
char szPartialName[ARGSIZE];

  memset(szSubString, 0x00, ARGSIZE);
  ptr = strExtractWord((char *)_szSrcPath, '.', szSubString);
  if (ptr != NULL)
  {
    dprintf(LOG_LEVEL3, "str_parameter_path_original, ptr != NULL\n");
  }

  // search recursively in DOM tree
  // stop when node name found (elt != NULL)
  for (iter = _node; iter && !elt; iter = tree_next_element(iter))
  {
    // is word same than node name
    if (iter && iter->name && (strcmp(szSubString, iter->name) == 0))
    {
      strcpy(szPartialName, szSubString);
      attr = search_attribute(iter, SZ_INSTANCE); // search for "INSTANCE" attribute in attribute list
      if (attr != NULL)
      {
        strcpy(szPartialName, attr->data);
      }

      // add INSTANCE value to destination string
      if (strcmp(_szDestPath, "") == 0)
        strcat(_szDestPath, szPartialName);
      else
      {
        strcat(_szDestPath, ".");
        strcat(_szDestPath, szPartialName);
      }

      dprintf(LOG_LEVEL3, "_szDestPath: %s\n", _szDestPath);

      // search next part of string
      if (ptr)
      {
        elt = str_parameter_path_original(iter->elts, ptr, _szDestPath);
      }
      else return iter;
    }
  }

  return elt;
}


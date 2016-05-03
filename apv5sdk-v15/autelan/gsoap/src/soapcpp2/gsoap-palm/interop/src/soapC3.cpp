
#include "soapH.h"

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoStructArray(struct soap *soap, const struct ns__echoStructArray *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_ArrayOfSOAPStruct(soap, &a->inputStructArray);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoStructArray(struct soap *soap, struct ns__echoStructArray *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_ArrayOfSOAPStruct(soap, &a->inputStructArray);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoStructArray(struct soap *soap, const struct ns__echoStructArray *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoStructArray);
	if (soap_out_ns__echoStructArray(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoStructArray(struct soap *soap, const char *tag, int id, const struct ns__echoStructArray *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoStructArray), type);
	soap_out_ArrayOfSOAPStruct(soap, "inputStructArray", -1, &a->inputStructArray, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoStructArray * SOAP_FMAC4 soap_get_ns__echoStructArray(struct soap *soap, struct ns__echoStructArray *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoStructArray(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoStructArray * SOAP_FMAC4 soap_in_ns__echoStructArray(struct soap *soap, const char *tag, struct ns__echoStructArray *a, const char *type)
{
	short soap_flag_inputStructArray = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoStructArray *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoStructArray, sizeof(struct ns__echoStructArray), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoStructArray(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_inputStructArray && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_ArrayOfSOAPStruct(soap, "inputStructArray", &a->inputStructArray, "s:SOAPStruct"))
				{	soap_flag_inputStructArray--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_inputStructArray > 0))
		{	soap->error = SOAP_OCCURS;
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct ns__echoStructArray *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoStructArray, 0, sizeof(struct ns__echoStructArray), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoStructArray * SOAP_FMAC6 soap_new_ns__echoStructArray(struct soap *soap, int n)
{	return soap_instantiate_ns__echoStructArray(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoStructArray(struct soap *soap, struct ns__echoStructArray *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoStructArray * SOAP_FMAC4 soap_instantiate_ns__echoStructArray(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoStructArray(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoStructArray, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoStructArray;
		if (size)
			*size = sizeof(struct ns__echoStructArray);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoStructArray[n];
		if (size)
			*size = n * sizeof(struct ns__echoStructArray);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoStructArray*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoStructArray(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoStructArray %p -> %p\n", q, p));
	*(struct ns__echoStructArray*)p = *(struct ns__echoStructArray*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoStructArrayResponse(struct soap *soap, const struct ns__echoStructArrayResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_ArrayOfSOAPStruct(soap, &a->_return);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoStructArrayResponse(struct soap *soap, struct ns__echoStructArrayResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_ArrayOfSOAPStruct(soap, &a->_return);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoStructArrayResponse(struct soap *soap, const struct ns__echoStructArrayResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoStructArrayResponse);
	if (soap_out_ns__echoStructArrayResponse(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoStructArrayResponse(struct soap *soap, const char *tag, int id, const struct ns__echoStructArrayResponse *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoStructArrayResponse), type);
	soap_element_result(soap, "return");
	soap_out_ArrayOfSOAPStruct(soap, "return", -1, &a->_return, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoStructArrayResponse * SOAP_FMAC4 soap_get_ns__echoStructArrayResponse(struct soap *soap, struct ns__echoStructArrayResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoStructArrayResponse(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoStructArrayResponse * SOAP_FMAC4 soap_in_ns__echoStructArrayResponse(struct soap *soap, const char *tag, struct ns__echoStructArrayResponse *a, const char *type)
{
	short soap_flag__return = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoStructArrayResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoStructArrayResponse, sizeof(struct ns__echoStructArrayResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoStructArrayResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag__return && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_ArrayOfSOAPStruct(soap, NULL, &a->_return, "s:SOAPStruct"))
				{	soap_flag__return--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if ((soap->mode & SOAP_XML_STRICT) && (soap_flag__return > 0))
		{	soap->error = SOAP_OCCURS;
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct ns__echoStructArrayResponse *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoStructArrayResponse, 0, sizeof(struct ns__echoStructArrayResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoStructArrayResponse * SOAP_FMAC6 soap_new_ns__echoStructArrayResponse(struct soap *soap, int n)
{	return soap_instantiate_ns__echoStructArrayResponse(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoStructArrayResponse(struct soap *soap, struct ns__echoStructArrayResponse *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoStructArrayResponse * SOAP_FMAC4 soap_instantiate_ns__echoStructArrayResponse(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoStructArrayResponse(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoStructArrayResponse, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoStructArrayResponse;
		if (size)
			*size = sizeof(struct ns__echoStructArrayResponse);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoStructArrayResponse[n];
		if (size)
			*size = n * sizeof(struct ns__echoStructArrayResponse);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoStructArrayResponse*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoStructArrayResponse(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoStructArrayResponse %p -> %p\n", q, p));
	*(struct ns__echoStructArrayResponse*)p = *(struct ns__echoStructArrayResponse*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ArrayOfSOAPStruct(struct soap *soap, struct ArrayOfSOAPStruct const*a)
{
	int i;
	if (a->__ptr && !soap_array_reference(soap, a, (struct soap_array*)&a->__ptr, 1, SOAP_TYPE_ArrayOfSOAPStruct))
		for (i = 0; i < a->__size; i++)
		{
			soap_serialize_PointerTos__SOAPStruct(soap, a->__ptr + i);
		}
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ArrayOfSOAPStruct(struct soap *soap, struct ArrayOfSOAPStruct *a)
{
	a->__size = 0;
	a->__ptr = NULL;
	a->__offset = 0;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ArrayOfSOAPStruct(struct soap *soap, const struct ArrayOfSOAPStruct *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, (struct soap_array*)&a->__ptr, 1, tag, SOAP_TYPE_ArrayOfSOAPStruct);
	if (soap_out_ArrayOfSOAPStruct(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ArrayOfSOAPStruct(struct soap *soap, const char *tag, int id, const struct ArrayOfSOAPStruct *a, const char *type)
{
	int i, n = a->__size;
	char *t = soap_putsize(soap, "s:SOAPStruct", n + a->__offset);
	id = soap_element_id(soap, tag, id, a, (struct soap_array*)&a->__ptr, 1, type, SOAP_TYPE_ArrayOfSOAPStruct);
	if (id < 0)
		return soap->error;
	soap_array_begin_out(soap, tag, id, t, soap_putoffset(soap, a->__offset));
	for (i = 0; i < n; i++)
	{
		soap->position = 1;
		soap->positions[0] = i;
		soap_out_PointerTos__SOAPStruct(soap, "item", -1, &a->__ptr[i], "");
	}
	soap->position = 0;
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ArrayOfSOAPStruct * SOAP_FMAC4 soap_get_ArrayOfSOAPStruct(struct soap *soap, struct ArrayOfSOAPStruct *p, const char *tag, const char *type)
{
	if ((p = soap_in_ArrayOfSOAPStruct(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ArrayOfSOAPStruct * SOAP_FMAC4 soap_in_ArrayOfSOAPStruct(struct soap *soap, const char *tag, struct ArrayOfSOAPStruct *a, const char *type)
{	int i, j;
	struct s__SOAPStruct **p;
	if (soap_element_begin_in(soap, tag, 1))
		return NULL;
	if (soap_match_array(soap, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ArrayOfSOAPStruct *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ArrayOfSOAPStruct, sizeof(struct ArrayOfSOAPStruct), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ArrayOfSOAPStruct(soap, a);
	if (soap->body && !*soap->href)
	{
		a->__size = soap_getsize(soap->arraySize, soap->arrayOffset, &j);
		a->__offset = j;
		if (a->__size >= 0)
		{	a->__ptr = (struct s__SOAPStruct **)soap_malloc(soap, sizeof(struct s__SOAPStruct *) * a->__size);
			for (i = 0; i < a->__size; i++)
				a->__ptr[i] = NULL;
			for (i = 0; i < a->__size; i++)
			{	soap_peek_element(soap);
				if (soap->position)
				{	i = soap->positions[0]-j;
					if (i < 0 || i >= a->__size)
					{	soap->error = SOAP_IOB;
						return NULL;
					}
				}
				if (!soap_in_PointerTos__SOAPStruct(soap, NULL, a->__ptr + i, "s:SOAPStruct"))
				{	if (soap->error != SOAP_NO_TAG)
						return NULL;
					soap->error = SOAP_OK;
					break;
				}
			}
		}
		else
		{	soap_new_block(soap);
			for (a->__size = 0; ; a->__size++)
			{	p = (struct s__SOAPStruct **)soap_push_block(soap, sizeof(struct s__SOAPStruct *));
				if (!p)
					return NULL;
				*p = NULL;
				if (!soap_in_PointerTos__SOAPStruct(soap, NULL, p, "s:SOAPStruct"))
				{	if (soap->error != SOAP_NO_TAG)
						return NULL;
					soap->error = SOAP_OK;
					break;
				}
			}
			soap_pop_block(soap);
			a->__ptr = (struct s__SOAPStruct **)soap_malloc(soap, soap->blist->size);
			soap_save_block(soap, (char*)a->__ptr, 1);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct ArrayOfSOAPStruct *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ArrayOfSOAPStruct, 0, sizeof(struct ArrayOfSOAPStruct), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ArrayOfSOAPStruct * SOAP_FMAC6 soap_new_ArrayOfSOAPStruct(struct soap *soap, int n)
{	return soap_instantiate_ArrayOfSOAPStruct(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ArrayOfSOAPStruct(struct soap *soap, struct ArrayOfSOAPStruct *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ArrayOfSOAPStruct * SOAP_FMAC4 soap_instantiate_ArrayOfSOAPStruct(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ArrayOfSOAPStruct(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ArrayOfSOAPStruct, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ArrayOfSOAPStruct;
		if (size)
			*size = sizeof(struct ArrayOfSOAPStruct);
	}
	else
	{	cp->ptr = (void*)new struct ArrayOfSOAPStruct[n];
		if (size)
			*size = n * sizeof(struct ArrayOfSOAPStruct);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ArrayOfSOAPStruct*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ArrayOfSOAPStruct(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ArrayOfSOAPStruct %p -> %p\n", q, p));
	*(struct ArrayOfSOAPStruct*)p = *(struct ArrayOfSOAPStruct*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoStruct(struct soap *soap, const struct ns__echoStruct *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_embedded(soap, &a->_inputStruct, SOAP_TYPE_s__SOAPStruct);
	soap_serialize_s__SOAPStruct(soap, &a->_inputStruct);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoStruct(struct soap *soap, struct ns__echoStruct *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_s__SOAPStruct(soap, &a->_inputStruct);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoStruct(struct soap *soap, const struct ns__echoStruct *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoStruct);
	if (soap_out_ns__echoStruct(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoStruct(struct soap *soap, const char *tag, int id, const struct ns__echoStruct *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoStruct), type);
	soap_out_s__SOAPStruct(soap, "inputStruct", -1, &a->_inputStruct, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoStruct * SOAP_FMAC4 soap_get_ns__echoStruct(struct soap *soap, struct ns__echoStruct *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoStruct(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoStruct * SOAP_FMAC4 soap_in_ns__echoStruct(struct soap *soap, const char *tag, struct ns__echoStruct *a, const char *type)
{
	short soap_flag__inputStruct = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoStruct *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoStruct, sizeof(struct ns__echoStruct), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoStruct(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag__inputStruct && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_s__SOAPStruct(soap, NULL, &a->_inputStruct, "s:SOAPStruct"))
				{	soap_flag__inputStruct--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if ((soap->mode & SOAP_XML_STRICT) && (soap_flag__inputStruct > 0))
		{	soap->error = SOAP_OCCURS;
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct ns__echoStruct *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoStruct, 0, sizeof(struct ns__echoStruct), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoStruct * SOAP_FMAC6 soap_new_ns__echoStruct(struct soap *soap, int n)
{	return soap_instantiate_ns__echoStruct(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoStruct(struct soap *soap, struct ns__echoStruct *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoStruct * SOAP_FMAC4 soap_instantiate_ns__echoStruct(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoStruct(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoStruct, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoStruct;
		if (size)
			*size = sizeof(struct ns__echoStruct);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoStruct[n];
		if (size)
			*size = n * sizeof(struct ns__echoStruct);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoStruct*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoStruct(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoStruct %p -> %p\n", q, p));
	*(struct ns__echoStruct*)p = *(struct ns__echoStruct*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoStructResponse(struct soap *soap, const struct ns__echoStructResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_embedded(soap, &a->_return, SOAP_TYPE_s__SOAPStruct);
	soap_serialize_s__SOAPStruct(soap, &a->_return);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoStructResponse(struct soap *soap, struct ns__echoStructResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_s__SOAPStruct(soap, &a->_return);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoStructResponse(struct soap *soap, const struct ns__echoStructResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoStructResponse);
	if (soap_out_ns__echoStructResponse(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoStructResponse(struct soap *soap, const char *tag, int id, const struct ns__echoStructResponse *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoStructResponse), type);
	soap_element_result(soap, "return");
	soap_out_s__SOAPStruct(soap, "return", -1, &a->_return, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoStructResponse * SOAP_FMAC4 soap_get_ns__echoStructResponse(struct soap *soap, struct ns__echoStructResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoStructResponse(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoStructResponse * SOAP_FMAC4 soap_in_ns__echoStructResponse(struct soap *soap, const char *tag, struct ns__echoStructResponse *a, const char *type)
{
	short soap_flag__return = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoStructResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoStructResponse, sizeof(struct ns__echoStructResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoStructResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag__return && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_s__SOAPStruct(soap, NULL, &a->_return, "s:SOAPStruct"))
				{	soap_flag__return--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if ((soap->mode & SOAP_XML_STRICT) && (soap_flag__return > 0))
		{	soap->error = SOAP_OCCURS;
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct ns__echoStructResponse *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoStructResponse, 0, sizeof(struct ns__echoStructResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoStructResponse * SOAP_FMAC6 soap_new_ns__echoStructResponse(struct soap *soap, int n)
{	return soap_instantiate_ns__echoStructResponse(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoStructResponse(struct soap *soap, struct ns__echoStructResponse *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoStructResponse * SOAP_FMAC4 soap_instantiate_ns__echoStructResponse(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoStructResponse(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoStructResponse, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoStructResponse;
		if (size)
			*size = sizeof(struct ns__echoStructResponse);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoStructResponse[n];
		if (size)
			*size = n * sizeof(struct ns__echoStructResponse);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoStructResponse*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoStructResponse(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoStructResponse %p -> %p\n", q, p));
	*(struct ns__echoStructResponse*)p = *(struct ns__echoStructResponse*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_s__SOAPStruct(struct soap *soap, const struct s__SOAPStruct *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_xsd__string(soap, &a->varString);
	soap_serialize_PointerToxsd__int(soap, &a->varInt);
	soap_serialize_PointerToxsd__float(soap, &a->varFloat);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_s__SOAPStruct(struct soap *soap, struct s__SOAPStruct *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_xsd__string(soap, &a->varString);
	a->varInt = NULL;
	a->varFloat = NULL;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_s__SOAPStruct(struct soap *soap, const struct s__SOAPStruct *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_s__SOAPStruct);
	if (soap_out_s__SOAPStruct(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_s__SOAPStruct(struct soap *soap, const char *tag, int id, const struct s__SOAPStruct *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_s__SOAPStruct), type);
	soap_out_xsd__string(soap, "varString", -1, &a->varString, "");
	soap_out_PointerToxsd__int(soap, "varInt", -1, &a->varInt, "");
	soap_out_PointerToxsd__float(soap, "varFloat", -1, &a->varFloat, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct s__SOAPStruct * SOAP_FMAC4 soap_get_s__SOAPStruct(struct soap *soap, struct s__SOAPStruct *p, const char *tag, const char *type)
{
	if ((p = soap_in_s__SOAPStruct(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct s__SOAPStruct * SOAP_FMAC4 soap_in_s__SOAPStruct(struct soap *soap, const char *tag, struct s__SOAPStruct *a, const char *type)
{
	short soap_flag_varString = 1, soap_flag_varInt = 1, soap_flag_varFloat = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct s__SOAPStruct *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_s__SOAPStruct, sizeof(struct s__SOAPStruct), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_s__SOAPStruct(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_varString && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_in_xsd__string(soap, "varString", &a->varString, "xsd:string"))
				{	soap_flag_varString--;
					continue;
				}
			if (soap_flag_varInt && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerToxsd__int(soap, "varInt", &a->varInt, "xsd:int"))
				{	soap_flag_varInt--;
					continue;
				}
			if (soap_flag_varFloat && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerToxsd__float(soap, "varFloat", &a->varFloat, "xsd:float"))
				{	soap_flag_varFloat--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct s__SOAPStruct *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_s__SOAPStruct, 0, sizeof(struct s__SOAPStruct), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct s__SOAPStruct * SOAP_FMAC6 soap_new_s__SOAPStruct(struct soap *soap, int n)
{	return soap_instantiate_s__SOAPStruct(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_s__SOAPStruct(struct soap *soap, struct s__SOAPStruct *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct s__SOAPStruct * SOAP_FMAC4 soap_instantiate_s__SOAPStruct(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_s__SOAPStruct(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_s__SOAPStruct, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct s__SOAPStruct;
		if (size)
			*size = sizeof(struct s__SOAPStruct);
	}
	else
	{	cp->ptr = (void*)new struct s__SOAPStruct[n];
		if (size)
			*size = n * sizeof(struct s__SOAPStruct);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct s__SOAPStruct*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_s__SOAPStruct(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct s__SOAPStruct %p -> %p\n", q, p));
	*(struct s__SOAPStruct*)p = *(struct s__SOAPStruct*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoFloatArray(struct soap *soap, const struct ns__echoFloatArray *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_ArrayOffloat(soap, &a->inputFloatArray);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoFloatArray(struct soap *soap, struct ns__echoFloatArray *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_ArrayOffloat(soap, &a->inputFloatArray);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoFloatArray(struct soap *soap, const struct ns__echoFloatArray *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoFloatArray);
	if (soap_out_ns__echoFloatArray(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoFloatArray(struct soap *soap, const char *tag, int id, const struct ns__echoFloatArray *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoFloatArray), type);
	soap_out_ArrayOffloat(soap, "inputFloatArray", -1, &a->inputFloatArray, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoFloatArray * SOAP_FMAC4 soap_get_ns__echoFloatArray(struct soap *soap, struct ns__echoFloatArray *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoFloatArray(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoFloatArray * SOAP_FMAC4 soap_in_ns__echoFloatArray(struct soap *soap, const char *tag, struct ns__echoFloatArray *a, const char *type)
{
	short soap_flag_inputFloatArray = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoFloatArray *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoFloatArray, sizeof(struct ns__echoFloatArray), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoFloatArray(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_inputFloatArray && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_ArrayOffloat(soap, "inputFloatArray", &a->inputFloatArray, "xsd:float"))
				{	soap_flag_inputFloatArray--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_inputFloatArray > 0))
		{	soap->error = SOAP_OCCURS;
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct ns__echoFloatArray *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoFloatArray, 0, sizeof(struct ns__echoFloatArray), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoFloatArray * SOAP_FMAC6 soap_new_ns__echoFloatArray(struct soap *soap, int n)
{	return soap_instantiate_ns__echoFloatArray(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoFloatArray(struct soap *soap, struct ns__echoFloatArray *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoFloatArray * SOAP_FMAC4 soap_instantiate_ns__echoFloatArray(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoFloatArray(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoFloatArray, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoFloatArray;
		if (size)
			*size = sizeof(struct ns__echoFloatArray);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoFloatArray[n];
		if (size)
			*size = n * sizeof(struct ns__echoFloatArray);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoFloatArray*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoFloatArray(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoFloatArray %p -> %p\n", q, p));
	*(struct ns__echoFloatArray*)p = *(struct ns__echoFloatArray*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoFloatArrayResponse(struct soap *soap, const struct ns__echoFloatArrayResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_ArrayOffloat(soap, &a->_return);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoFloatArrayResponse(struct soap *soap, struct ns__echoFloatArrayResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_ArrayOffloat(soap, &a->_return);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoFloatArrayResponse(struct soap *soap, const struct ns__echoFloatArrayResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoFloatArrayResponse);
	if (soap_out_ns__echoFloatArrayResponse(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoFloatArrayResponse(struct soap *soap, const char *tag, int id, const struct ns__echoFloatArrayResponse *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoFloatArrayResponse), type);
	soap_element_result(soap, "return");
	soap_out_ArrayOffloat(soap, "return", -1, &a->_return, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoFloatArrayResponse * SOAP_FMAC4 soap_get_ns__echoFloatArrayResponse(struct soap *soap, struct ns__echoFloatArrayResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoFloatArrayResponse(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoFloatArrayResponse * SOAP_FMAC4 soap_in_ns__echoFloatArrayResponse(struct soap *soap, const char *tag, struct ns__echoFloatArrayResponse *a, const char *type)
{
	short soap_flag__return = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoFloatArrayResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoFloatArrayResponse, sizeof(struct ns__echoFloatArrayResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoFloatArrayResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag__return && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_ArrayOffloat(soap, NULL, &a->_return, "xsd:float"))
				{	soap_flag__return--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if ((soap->mode & SOAP_XML_STRICT) && (soap_flag__return > 0))
		{	soap->error = SOAP_OCCURS;
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct ns__echoFloatArrayResponse *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoFloatArrayResponse, 0, sizeof(struct ns__echoFloatArrayResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoFloatArrayResponse * SOAP_FMAC6 soap_new_ns__echoFloatArrayResponse(struct soap *soap, int n)
{	return soap_instantiate_ns__echoFloatArrayResponse(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoFloatArrayResponse(struct soap *soap, struct ns__echoFloatArrayResponse *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoFloatArrayResponse * SOAP_FMAC4 soap_instantiate_ns__echoFloatArrayResponse(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoFloatArrayResponse(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoFloatArrayResponse, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoFloatArrayResponse;
		if (size)
			*size = sizeof(struct ns__echoFloatArrayResponse);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoFloatArrayResponse[n];
		if (size)
			*size = n * sizeof(struct ns__echoFloatArrayResponse);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoFloatArrayResponse*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoFloatArrayResponse(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoFloatArrayResponse %p -> %p\n", q, p));
	*(struct ns__echoFloatArrayResponse*)p = *(struct ns__echoFloatArrayResponse*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ArrayOffloat(struct soap *soap, struct ArrayOffloat const*a)
{
	int i;
	if (a->__ptr && !soap_array_reference(soap, a, (struct soap_array*)&a->__ptr, 1, SOAP_TYPE_ArrayOffloat))
		for (i = 0; i < a->__size; i++)
		{
			soap_serialize_PointerToxsd__float(soap, a->__ptr + i);
		}
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ArrayOffloat(struct soap *soap, struct ArrayOffloat *a)
{
	a->__size = 0;
	a->__ptr = NULL;
	a->__offset = 0;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ArrayOffloat(struct soap *soap, const struct ArrayOffloat *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, (struct soap_array*)&a->__ptr, 1, tag, SOAP_TYPE_ArrayOffloat);
	if (soap_out_ArrayOffloat(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ArrayOffloat(struct soap *soap, const char *tag, int id, const struct ArrayOffloat *a, const char *type)
{
	int i, n = a->__size;
	char *t = soap_putsize(soap, "xsd:float", n + a->__offset);
	id = soap_element_id(soap, tag, id, a, (struct soap_array*)&a->__ptr, 1, type, SOAP_TYPE_ArrayOffloat);
	if (id < 0)
		return soap->error;
	soap_array_begin_out(soap, tag, id, t, soap_putoffset(soap, a->__offset));
	for (i = 0; i < n; i++)
	{
		soap->position = 1;
		soap->positions[0] = i;
		soap_out_PointerToxsd__float(soap, "item", -1, &a->__ptr[i], "");
	}
	soap->position = 0;
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ArrayOffloat * SOAP_FMAC4 soap_get_ArrayOffloat(struct soap *soap, struct ArrayOffloat *p, const char *tag, const char *type)
{
	if ((p = soap_in_ArrayOffloat(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ArrayOffloat * SOAP_FMAC4 soap_in_ArrayOffloat(struct soap *soap, const char *tag, struct ArrayOffloat *a, const char *type)
{	int i, j;
	float **p;
	if (soap_element_begin_in(soap, tag, 1))
		return NULL;
	if (soap_match_array(soap, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ArrayOffloat *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ArrayOffloat, sizeof(struct ArrayOffloat), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ArrayOffloat(soap, a);
	if (soap->body && !*soap->href)
	{
		a->__size = soap_getsize(soap->arraySize, soap->arrayOffset, &j);
		a->__offset = j;
		if (a->__size >= 0)
		{	a->__ptr = (float **)soap_malloc(soap, sizeof(float *) * a->__size);
			for (i = 0; i < a->__size; i++)
				a->__ptr[i] = NULL;
			for (i = 0; i < a->__size; i++)
			{	soap_peek_element(soap);
				if (soap->position)
				{	i = soap->positions[0]-j;
					if (i < 0 || i >= a->__size)
					{	soap->error = SOAP_IOB;
						return NULL;
					}
				}
				if (!soap_in_PointerToxsd__float(soap, NULL, a->__ptr + i, "xsd:float"))
				{	if (soap->error != SOAP_NO_TAG)
						return NULL;
					soap->error = SOAP_OK;
					break;
				}
			}
		}
		else
		{	soap_new_block(soap);
			for (a->__size = 0; ; a->__size++)
			{	p = (float **)soap_push_block(soap, sizeof(float *));
				if (!p)
					return NULL;
				*p = NULL;
				if (!soap_in_PointerToxsd__float(soap, NULL, p, "xsd:float"))
				{	if (soap->error != SOAP_NO_TAG)
						return NULL;
					soap->error = SOAP_OK;
					break;
				}
			}
			soap_pop_block(soap);
			a->__ptr = (float **)soap_malloc(soap, soap->blist->size);
			soap_save_block(soap, (char*)a->__ptr, 1);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct ArrayOffloat *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ArrayOffloat, 0, sizeof(struct ArrayOffloat), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ArrayOffloat * SOAP_FMAC6 soap_new_ArrayOffloat(struct soap *soap, int n)
{	return soap_instantiate_ArrayOffloat(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ArrayOffloat(struct soap *soap, struct ArrayOffloat *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ArrayOffloat * SOAP_FMAC4 soap_instantiate_ArrayOffloat(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ArrayOffloat(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ArrayOffloat, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ArrayOffloat;
		if (size)
			*size = sizeof(struct ArrayOffloat);
	}
	else
	{	cp->ptr = (void*)new struct ArrayOffloat[n];
		if (size)
			*size = n * sizeof(struct ArrayOffloat);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ArrayOffloat*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ArrayOffloat(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ArrayOffloat %p -> %p\n", q, p));
	*(struct ArrayOffloat*)p = *(struct ArrayOffloat*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoFloat(struct soap *soap, const struct ns__echoFloat *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_embedded(soap, &a->inputFloat, SOAP_TYPE_xsd__float);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoFloat(struct soap *soap, struct ns__echoFloat *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_xsd__float(soap, &a->inputFloat);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoFloat(struct soap *soap, const struct ns__echoFloat *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoFloat);
	if (soap_out_ns__echoFloat(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoFloat(struct soap *soap, const char *tag, int id, const struct ns__echoFloat *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoFloat), type);
	soap_out_xsd__float(soap, "inputFloat", -1, &a->inputFloat, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoFloat * SOAP_FMAC4 soap_get_ns__echoFloat(struct soap *soap, struct ns__echoFloat *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoFloat(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoFloat * SOAP_FMAC4 soap_in_ns__echoFloat(struct soap *soap, const char *tag, struct ns__echoFloat *a, const char *type)
{
	short soap_flag_inputFloat = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoFloat *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoFloat, sizeof(struct ns__echoFloat), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoFloat(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_inputFloat && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_xsd__float(soap, "inputFloat", &a->inputFloat, "xsd:float"))
				{	soap_flag_inputFloat--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_inputFloat > 0))
		{	soap->error = SOAP_OCCURS;
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct ns__echoFloat *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoFloat, 0, sizeof(struct ns__echoFloat), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoFloat * SOAP_FMAC6 soap_new_ns__echoFloat(struct soap *soap, int n)
{	return soap_instantiate_ns__echoFloat(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoFloat(struct soap *soap, struct ns__echoFloat *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoFloat * SOAP_FMAC4 soap_instantiate_ns__echoFloat(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoFloat(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoFloat, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoFloat;
		if (size)
			*size = sizeof(struct ns__echoFloat);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoFloat[n];
		if (size)
			*size = n * sizeof(struct ns__echoFloat);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoFloat*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoFloat(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoFloat %p -> %p\n", q, p));
	*(struct ns__echoFloat*)p = *(struct ns__echoFloat*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoFloatResponse(struct soap *soap, const struct ns__echoFloatResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_embedded(soap, &a->_return, SOAP_TYPE_xsd__float);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoFloatResponse(struct soap *soap, struct ns__echoFloatResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_xsd__float(soap, &a->_return);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoFloatResponse(struct soap *soap, const struct ns__echoFloatResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoFloatResponse);
	if (soap_out_ns__echoFloatResponse(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoFloatResponse(struct soap *soap, const char *tag, int id, const struct ns__echoFloatResponse *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoFloatResponse), type);
	soap_element_result(soap, "return");
	soap_out_xsd__float(soap, "return", -1, &a->_return, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoFloatResponse * SOAP_FMAC4 soap_get_ns__echoFloatResponse(struct soap *soap, struct ns__echoFloatResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoFloatResponse(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoFloatResponse * SOAP_FMAC4 soap_in_ns__echoFloatResponse(struct soap *soap, const char *tag, struct ns__echoFloatResponse *a, const char *type)
{
	short soap_flag__return = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoFloatResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoFloatResponse, sizeof(struct ns__echoFloatResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoFloatResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag__return && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_xsd__float(soap, NULL, &a->_return, "xsd:float"))
				{	soap_flag__return--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if ((soap->mode & SOAP_XML_STRICT) && (soap_flag__return > 0))
		{	soap->error = SOAP_OCCURS;
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct ns__echoFloatResponse *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoFloatResponse, 0, sizeof(struct ns__echoFloatResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoFloatResponse * SOAP_FMAC6 soap_new_ns__echoFloatResponse(struct soap *soap, int n)
{	return soap_instantiate_ns__echoFloatResponse(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoFloatResponse(struct soap *soap, struct ns__echoFloatResponse *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoFloatResponse * SOAP_FMAC4 soap_instantiate_ns__echoFloatResponse(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoFloatResponse(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoFloatResponse, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoFloatResponse;
		if (size)
			*size = sizeof(struct ns__echoFloatResponse);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoFloatResponse[n];
		if (size)
			*size = n * sizeof(struct ns__echoFloatResponse);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoFloatResponse*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoFloatResponse(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoFloatResponse %p -> %p\n", q, p));
	*(struct ns__echoFloatResponse*)p = *(struct ns__echoFloatResponse*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoIntegerArray(struct soap *soap, const struct ns__echoIntegerArray *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_ArrayOfint(soap, &a->inputIntegerArray);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoIntegerArray(struct soap *soap, struct ns__echoIntegerArray *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_ArrayOfint(soap, &a->inputIntegerArray);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoIntegerArray(struct soap *soap, const struct ns__echoIntegerArray *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoIntegerArray);
	if (soap_out_ns__echoIntegerArray(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoIntegerArray(struct soap *soap, const char *tag, int id, const struct ns__echoIntegerArray *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoIntegerArray), type);
	soap_out_ArrayOfint(soap, "inputIntegerArray", -1, &a->inputIntegerArray, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoIntegerArray * SOAP_FMAC4 soap_get_ns__echoIntegerArray(struct soap *soap, struct ns__echoIntegerArray *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoIntegerArray(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoIntegerArray * SOAP_FMAC4 soap_in_ns__echoIntegerArray(struct soap *soap, const char *tag, struct ns__echoIntegerArray *a, const char *type)
{
	short soap_flag_inputIntegerArray = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoIntegerArray *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoIntegerArray, sizeof(struct ns__echoIntegerArray), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoIntegerArray(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_inputIntegerArray && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_ArrayOfint(soap, "inputIntegerArray", &a->inputIntegerArray, "xsd:int"))
				{	soap_flag_inputIntegerArray--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_inputIntegerArray > 0))
		{	soap->error = SOAP_OCCURS;
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct ns__echoIntegerArray *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoIntegerArray, 0, sizeof(struct ns__echoIntegerArray), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoIntegerArray * SOAP_FMAC6 soap_new_ns__echoIntegerArray(struct soap *soap, int n)
{	return soap_instantiate_ns__echoIntegerArray(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoIntegerArray(struct soap *soap, struct ns__echoIntegerArray *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoIntegerArray * SOAP_FMAC4 soap_instantiate_ns__echoIntegerArray(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoIntegerArray(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoIntegerArray, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoIntegerArray;
		if (size)
			*size = sizeof(struct ns__echoIntegerArray);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoIntegerArray[n];
		if (size)
			*size = n * sizeof(struct ns__echoIntegerArray);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoIntegerArray*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoIntegerArray(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoIntegerArray %p -> %p\n", q, p));
	*(struct ns__echoIntegerArray*)p = *(struct ns__echoIntegerArray*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoIntegerArrayResponse(struct soap *soap, const struct ns__echoIntegerArrayResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_ArrayOfint(soap, &a->_return);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoIntegerArrayResponse(struct soap *soap, struct ns__echoIntegerArrayResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_ArrayOfint(soap, &a->_return);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoIntegerArrayResponse(struct soap *soap, const struct ns__echoIntegerArrayResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoIntegerArrayResponse);
	if (soap_out_ns__echoIntegerArrayResponse(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoIntegerArrayResponse(struct soap *soap, const char *tag, int id, const struct ns__echoIntegerArrayResponse *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoIntegerArrayResponse), type);
	soap_element_result(soap, "return");
	soap_out_ArrayOfint(soap, "return", -1, &a->_return, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoIntegerArrayResponse * SOAP_FMAC4 soap_get_ns__echoIntegerArrayResponse(struct soap *soap, struct ns__echoIntegerArrayResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoIntegerArrayResponse(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoIntegerArrayResponse * SOAP_FMAC4 soap_in_ns__echoIntegerArrayResponse(struct soap *soap, const char *tag, struct ns__echoIntegerArrayResponse *a, const char *type)
{
	short soap_flag__return = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoIntegerArrayResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoIntegerArrayResponse, sizeof(struct ns__echoIntegerArrayResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoIntegerArrayResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag__return && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_ArrayOfint(soap, NULL, &a->_return, "xsd:int"))
				{	soap_flag__return--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if ((soap->mode & SOAP_XML_STRICT) && (soap_flag__return > 0))
		{	soap->error = SOAP_OCCURS;
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct ns__echoIntegerArrayResponse *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoIntegerArrayResponse, 0, sizeof(struct ns__echoIntegerArrayResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoIntegerArrayResponse * SOAP_FMAC6 soap_new_ns__echoIntegerArrayResponse(struct soap *soap, int n)
{	return soap_instantiate_ns__echoIntegerArrayResponse(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoIntegerArrayResponse(struct soap *soap, struct ns__echoIntegerArrayResponse *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoIntegerArrayResponse * SOAP_FMAC4 soap_instantiate_ns__echoIntegerArrayResponse(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoIntegerArrayResponse(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoIntegerArrayResponse, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoIntegerArrayResponse;
		if (size)
			*size = sizeof(struct ns__echoIntegerArrayResponse);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoIntegerArrayResponse[n];
		if (size)
			*size = n * sizeof(struct ns__echoIntegerArrayResponse);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoIntegerArrayResponse*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoIntegerArrayResponse(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoIntegerArrayResponse %p -> %p\n", q, p));
	*(struct ns__echoIntegerArrayResponse*)p = *(struct ns__echoIntegerArrayResponse*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ArrayOfint(struct soap *soap, struct ArrayOfint const*a)
{
	int i;
	if (a->__ptr && !soap_array_reference(soap, a, (struct soap_array*)&a->__ptr, 1, SOAP_TYPE_ArrayOfint))
		for (i = 0; i < a->__size; i++)
		{
			soap_serialize_PointerToxsd__int(soap, a->__ptr + i);
		}
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ArrayOfint(struct soap *soap, struct ArrayOfint *a)
{
	a->__size = 0;
	a->__ptr = NULL;
	a->__offset = 0;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ArrayOfint(struct soap *soap, const struct ArrayOfint *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, (struct soap_array*)&a->__ptr, 1, tag, SOAP_TYPE_ArrayOfint);
	if (soap_out_ArrayOfint(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ArrayOfint(struct soap *soap, const char *tag, int id, const struct ArrayOfint *a, const char *type)
{
	int i, n = a->__size;
	char *t = soap_putsize(soap, "xsd:int", n + a->__offset);
	id = soap_element_id(soap, tag, id, a, (struct soap_array*)&a->__ptr, 1, type, SOAP_TYPE_ArrayOfint);
	if (id < 0)
		return soap->error;
	soap_array_begin_out(soap, tag, id, t, soap_putoffset(soap, a->__offset));
	for (i = 0; i < n; i++)
	{
		soap->position = 1;
		soap->positions[0] = i;
		soap_out_PointerToxsd__int(soap, "item", -1, &a->__ptr[i], "");
	}
	soap->position = 0;
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ArrayOfint * SOAP_FMAC4 soap_get_ArrayOfint(struct soap *soap, struct ArrayOfint *p, const char *tag, const char *type)
{
	if ((p = soap_in_ArrayOfint(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ArrayOfint * SOAP_FMAC4 soap_in_ArrayOfint(struct soap *soap, const char *tag, struct ArrayOfint *a, const char *type)
{	int i, j;
	long **p;
	if (soap_element_begin_in(soap, tag, 1))
		return NULL;
	if (soap_match_array(soap, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ArrayOfint *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ArrayOfint, sizeof(struct ArrayOfint), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ArrayOfint(soap, a);
	if (soap->body && !*soap->href)
	{
		a->__size = soap_getsize(soap->arraySize, soap->arrayOffset, &j);
		a->__offset = j;
		if (a->__size >= 0)
		{	a->__ptr = (long **)soap_malloc(soap, sizeof(long *) * a->__size);
			for (i = 0; i < a->__size; i++)
				a->__ptr[i] = NULL;
			for (i = 0; i < a->__size; i++)
			{	soap_peek_element(soap);
				if (soap->position)
				{	i = soap->positions[0]-j;
					if (i < 0 || i >= a->__size)
					{	soap->error = SOAP_IOB;
						return NULL;
					}
				}
				if (!soap_in_PointerToxsd__int(soap, NULL, a->__ptr + i, "xsd:int"))
				{	if (soap->error != SOAP_NO_TAG)
						return NULL;
					soap->error = SOAP_OK;
					break;
				}
			}
		}
		else
		{	soap_new_block(soap);
			for (a->__size = 0; ; a->__size++)
			{	p = (long **)soap_push_block(soap, sizeof(long *));
				if (!p)
					return NULL;
				*p = NULL;
				if (!soap_in_PointerToxsd__int(soap, NULL, p, "xsd:int"))
				{	if (soap->error != SOAP_NO_TAG)
						return NULL;
					soap->error = SOAP_OK;
					break;
				}
			}
			soap_pop_block(soap);
			a->__ptr = (long **)soap_malloc(soap, soap->blist->size);
			soap_save_block(soap, (char*)a->__ptr, 1);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct ArrayOfint *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ArrayOfint, 0, sizeof(struct ArrayOfint), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ArrayOfint * SOAP_FMAC6 soap_new_ArrayOfint(struct soap *soap, int n)
{	return soap_instantiate_ArrayOfint(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ArrayOfint(struct soap *soap, struct ArrayOfint *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ArrayOfint * SOAP_FMAC4 soap_instantiate_ArrayOfint(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ArrayOfint(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ArrayOfint, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ArrayOfint;
		if (size)
			*size = sizeof(struct ArrayOfint);
	}
	else
	{	cp->ptr = (void*)new struct ArrayOfint[n];
		if (size)
			*size = n * sizeof(struct ArrayOfint);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ArrayOfint*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ArrayOfint(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ArrayOfint %p -> %p\n", q, p));
	*(struct ArrayOfint*)p = *(struct ArrayOfint*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoInteger(struct soap *soap, const struct ns__echoInteger *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_embedded(soap, &a->inputInteger, SOAP_TYPE_xsd__int);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoInteger(struct soap *soap, struct ns__echoInteger *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_xsd__int(soap, &a->inputInteger);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoInteger(struct soap *soap, const struct ns__echoInteger *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoInteger);
	if (soap_out_ns__echoInteger(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoInteger(struct soap *soap, const char *tag, int id, const struct ns__echoInteger *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoInteger), type);
	soap_out_xsd__int(soap, "inputInteger", -1, &a->inputInteger, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoInteger * SOAP_FMAC4 soap_get_ns__echoInteger(struct soap *soap, struct ns__echoInteger *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoInteger(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoInteger * SOAP_FMAC4 soap_in_ns__echoInteger(struct soap *soap, const char *tag, struct ns__echoInteger *a, const char *type)
{
	short soap_flag_inputInteger = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoInteger *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoInteger, sizeof(struct ns__echoInteger), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoInteger(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_inputInteger && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_xsd__int(soap, "inputInteger", &a->inputInteger, "xsd:int"))
				{	soap_flag_inputInteger--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_inputInteger > 0))
		{	soap->error = SOAP_OCCURS;
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct ns__echoInteger *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoInteger, 0, sizeof(struct ns__echoInteger), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoInteger * SOAP_FMAC6 soap_new_ns__echoInteger(struct soap *soap, int n)
{	return soap_instantiate_ns__echoInteger(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoInteger(struct soap *soap, struct ns__echoInteger *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoInteger * SOAP_FMAC4 soap_instantiate_ns__echoInteger(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoInteger(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoInteger, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoInteger;
		if (size)
			*size = sizeof(struct ns__echoInteger);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoInteger[n];
		if (size)
			*size = n * sizeof(struct ns__echoInteger);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoInteger*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoInteger(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoInteger %p -> %p\n", q, p));
	*(struct ns__echoInteger*)p = *(struct ns__echoInteger*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoIntegerResponse(struct soap *soap, const struct ns__echoIntegerResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_embedded(soap, &a->_return, SOAP_TYPE_xsd__int);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoIntegerResponse(struct soap *soap, struct ns__echoIntegerResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_xsd__int(soap, &a->_return);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoIntegerResponse(struct soap *soap, const struct ns__echoIntegerResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoIntegerResponse);
	if (soap_out_ns__echoIntegerResponse(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoIntegerResponse(struct soap *soap, const char *tag, int id, const struct ns__echoIntegerResponse *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoIntegerResponse), type);
	soap_element_result(soap, "return");
	soap_out_xsd__int(soap, "return", -1, &a->_return, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoIntegerResponse * SOAP_FMAC4 soap_get_ns__echoIntegerResponse(struct soap *soap, struct ns__echoIntegerResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoIntegerResponse(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoIntegerResponse * SOAP_FMAC4 soap_in_ns__echoIntegerResponse(struct soap *soap, const char *tag, struct ns__echoIntegerResponse *a, const char *type)
{
	short soap_flag__return = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoIntegerResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoIntegerResponse, sizeof(struct ns__echoIntegerResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoIntegerResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag__return && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_xsd__int(soap, NULL, &a->_return, "xsd:int"))
				{	soap_flag__return--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if ((soap->mode & SOAP_XML_STRICT) && (soap_flag__return > 0))
		{	soap->error = SOAP_OCCURS;
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct ns__echoIntegerResponse *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoIntegerResponse, 0, sizeof(struct ns__echoIntegerResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoIntegerResponse * SOAP_FMAC6 soap_new_ns__echoIntegerResponse(struct soap *soap, int n)
{	return soap_instantiate_ns__echoIntegerResponse(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoIntegerResponse(struct soap *soap, struct ns__echoIntegerResponse *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoIntegerResponse * SOAP_FMAC4 soap_instantiate_ns__echoIntegerResponse(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoIntegerResponse(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoIntegerResponse, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoIntegerResponse;
		if (size)
			*size = sizeof(struct ns__echoIntegerResponse);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoIntegerResponse[n];
		if (size)
			*size = n * sizeof(struct ns__echoIntegerResponse);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoIntegerResponse*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoIntegerResponse(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoIntegerResponse %p -> %p\n", q, p));
	*(struct ns__echoIntegerResponse*)p = *(struct ns__echoIntegerResponse*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoStringArray(struct soap *soap, const struct ns__echoStringArray *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_ArrayOfstring(soap, &a->inputStringArray);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoStringArray(struct soap *soap, struct ns__echoStringArray *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_ArrayOfstring(soap, &a->inputStringArray);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoStringArray(struct soap *soap, const struct ns__echoStringArray *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoStringArray);
	if (soap_out_ns__echoStringArray(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoStringArray(struct soap *soap, const char *tag, int id, const struct ns__echoStringArray *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoStringArray), type);
	soap_out_ArrayOfstring(soap, "inputStringArray", -1, &a->inputStringArray, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoStringArray * SOAP_FMAC4 soap_get_ns__echoStringArray(struct soap *soap, struct ns__echoStringArray *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoStringArray(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoStringArray * SOAP_FMAC4 soap_in_ns__echoStringArray(struct soap *soap, const char *tag, struct ns__echoStringArray *a, const char *type)
{
	short soap_flag_inputStringArray = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoStringArray *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoStringArray, sizeof(struct ns__echoStringArray), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoStringArray(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_inputStringArray && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_ArrayOfstring(soap, "inputStringArray", &a->inputStringArray, "xsd:string"))
				{	soap_flag_inputStringArray--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_inputStringArray > 0))
		{	soap->error = SOAP_OCCURS;
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct ns__echoStringArray *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoStringArray, 0, sizeof(struct ns__echoStringArray), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoStringArray * SOAP_FMAC6 soap_new_ns__echoStringArray(struct soap *soap, int n)
{	return soap_instantiate_ns__echoStringArray(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoStringArray(struct soap *soap, struct ns__echoStringArray *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoStringArray * SOAP_FMAC4 soap_instantiate_ns__echoStringArray(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoStringArray(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoStringArray, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoStringArray;
		if (size)
			*size = sizeof(struct ns__echoStringArray);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoStringArray[n];
		if (size)
			*size = n * sizeof(struct ns__echoStringArray);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoStringArray*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoStringArray(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoStringArray %p -> %p\n", q, p));
	*(struct ns__echoStringArray*)p = *(struct ns__echoStringArray*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoStringArrayResponse(struct soap *soap, const struct ns__echoStringArrayResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_ArrayOfstring(soap, &a->_return);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoStringArrayResponse(struct soap *soap, struct ns__echoStringArrayResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_ArrayOfstring(soap, &a->_return);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoStringArrayResponse(struct soap *soap, const struct ns__echoStringArrayResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoStringArrayResponse);
	if (soap_out_ns__echoStringArrayResponse(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoStringArrayResponse(struct soap *soap, const char *tag, int id, const struct ns__echoStringArrayResponse *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoStringArrayResponse), type);
	soap_element_result(soap, "return");
	soap_out_ArrayOfstring(soap, "return", -1, &a->_return, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoStringArrayResponse * SOAP_FMAC4 soap_get_ns__echoStringArrayResponse(struct soap *soap, struct ns__echoStringArrayResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoStringArrayResponse(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoStringArrayResponse * SOAP_FMAC4 soap_in_ns__echoStringArrayResponse(struct soap *soap, const char *tag, struct ns__echoStringArrayResponse *a, const char *type)
{
	short soap_flag__return = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoStringArrayResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoStringArrayResponse, sizeof(struct ns__echoStringArrayResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoStringArrayResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag__return && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_ArrayOfstring(soap, NULL, &a->_return, "xsd:string"))
				{	soap_flag__return--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if ((soap->mode & SOAP_XML_STRICT) && (soap_flag__return > 0))
		{	soap->error = SOAP_OCCURS;
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct ns__echoStringArrayResponse *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoStringArrayResponse, 0, sizeof(struct ns__echoStringArrayResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoStringArrayResponse * SOAP_FMAC6 soap_new_ns__echoStringArrayResponse(struct soap *soap, int n)
{	return soap_instantiate_ns__echoStringArrayResponse(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoStringArrayResponse(struct soap *soap, struct ns__echoStringArrayResponse *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoStringArrayResponse * SOAP_FMAC4 soap_instantiate_ns__echoStringArrayResponse(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoStringArrayResponse(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoStringArrayResponse, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoStringArrayResponse;
		if (size)
			*size = sizeof(struct ns__echoStringArrayResponse);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoStringArrayResponse[n];
		if (size)
			*size = n * sizeof(struct ns__echoStringArrayResponse);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoStringArrayResponse*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoStringArrayResponse(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoStringArrayResponse %p -> %p\n", q, p));
	*(struct ns__echoStringArrayResponse*)p = *(struct ns__echoStringArrayResponse*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ArrayOfstring(struct soap *soap, struct ArrayOfstring const*a)
{
	int i;
	if (a->__ptr && !soap_array_reference(soap, a, (struct soap_array*)&a->__ptr, 1, SOAP_TYPE_ArrayOfstring))
		for (i = 0; i < a->__size; i++)
		{
			soap_serialize_xsd__string(soap, a->__ptr + i);
		}
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ArrayOfstring(struct soap *soap, struct ArrayOfstring *a)
{
	a->__size = 0;
	a->__ptr = NULL;
	a->__offset = 0;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ArrayOfstring(struct soap *soap, const struct ArrayOfstring *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, (struct soap_array*)&a->__ptr, 1, tag, SOAP_TYPE_ArrayOfstring);
	if (soap_out_ArrayOfstring(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ArrayOfstring(struct soap *soap, const char *tag, int id, const struct ArrayOfstring *a, const char *type)
{
	int i, n = a->__size;
	char *t = soap_putsize(soap, "xsd:string", n + a->__offset);
	id = soap_element_id(soap, tag, id, a, (struct soap_array*)&a->__ptr, 1, type, SOAP_TYPE_ArrayOfstring);
	if (id < 0)
		return soap->error;
	soap_array_begin_out(soap, tag, id, t, soap_putoffset(soap, a->__offset));
	for (i = 0; i < n; i++)
	{
		soap->position = 1;
		soap->positions[0] = i;
		soap_out_xsd__string(soap, "item", -1, &a->__ptr[i], "");
	}
	soap->position = 0;
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ArrayOfstring * SOAP_FMAC4 soap_get_ArrayOfstring(struct soap *soap, struct ArrayOfstring *p, const char *tag, const char *type)
{
	if ((p = soap_in_ArrayOfstring(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ArrayOfstring * SOAP_FMAC4 soap_in_ArrayOfstring(struct soap *soap, const char *tag, struct ArrayOfstring *a, const char *type)
{	int i, j;
	char **p;
	if (soap_element_begin_in(soap, tag, 1))
		return NULL;
	if (soap_match_array(soap, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ArrayOfstring *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ArrayOfstring, sizeof(struct ArrayOfstring), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ArrayOfstring(soap, a);
	if (soap->body && !*soap->href)
	{
		a->__size = soap_getsize(soap->arraySize, soap->arrayOffset, &j);
		a->__offset = j;
		if (a->__size >= 0)
		{	a->__ptr = (char **)soap_malloc(soap, sizeof(char *) * a->__size);
			for (i = 0; i < a->__size; i++)
				a->__ptr[i] = NULL;
			for (i = 0; i < a->__size; i++)
			{	soap_peek_element(soap);
				if (soap->position)
				{	i = soap->positions[0]-j;
					if (i < 0 || i >= a->__size)
					{	soap->error = SOAP_IOB;
						return NULL;
					}
				}
				if (!soap_in_xsd__string(soap, NULL, a->__ptr + i, "xsd:string"))
				{	if (soap->error != SOAP_NO_TAG)
						return NULL;
					soap->error = SOAP_OK;
					break;
				}
			}
		}
		else
		{	soap_new_block(soap);
			for (a->__size = 0; ; a->__size++)
			{	p = (char **)soap_push_block(soap, sizeof(char *));
				if (!p)
					return NULL;
				*p = NULL;
				if (!soap_in_xsd__string(soap, NULL, p, "xsd:string"))
				{	if (soap->error != SOAP_NO_TAG)
						return NULL;
					soap->error = SOAP_OK;
					break;
				}
			}
			soap_pop_block(soap);
			a->__ptr = (char **)soap_malloc(soap, soap->blist->size);
			soap_save_block(soap, (char*)a->__ptr, 1);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct ArrayOfstring *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ArrayOfstring, 0, sizeof(struct ArrayOfstring), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ArrayOfstring * SOAP_FMAC6 soap_new_ArrayOfstring(struct soap *soap, int n)
{	return soap_instantiate_ArrayOfstring(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ArrayOfstring(struct soap *soap, struct ArrayOfstring *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ArrayOfstring * SOAP_FMAC4 soap_instantiate_ArrayOfstring(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ArrayOfstring(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ArrayOfstring, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ArrayOfstring;
		if (size)
			*size = sizeof(struct ArrayOfstring);
	}
	else
	{	cp->ptr = (void*)new struct ArrayOfstring[n];
		if (size)
			*size = n * sizeof(struct ArrayOfstring);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ArrayOfstring*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ArrayOfstring(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ArrayOfstring %p -> %p\n", q, p));
	*(struct ArrayOfstring*)p = *(struct ArrayOfstring*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoString(struct soap *soap, const struct ns__echoString *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_xsd__string(soap, &a->inputString);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoString(struct soap *soap, struct ns__echoString *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_xsd__string(soap, &a->inputString);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoString(struct soap *soap, const struct ns__echoString *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoString);
	if (soap_out_ns__echoString(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoString(struct soap *soap, const char *tag, int id, const struct ns__echoString *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoString), type);
	soap_out_xsd__string(soap, "inputString", -1, &a->inputString, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoString * SOAP_FMAC4 soap_get_ns__echoString(struct soap *soap, struct ns__echoString *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoString(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoString * SOAP_FMAC4 soap_in_ns__echoString(struct soap *soap, const char *tag, struct ns__echoString *a, const char *type)
{
	short soap_flag_inputString = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoString *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoString, sizeof(struct ns__echoString), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoString(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_inputString && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_in_xsd__string(soap, "inputString", &a->inputString, "xsd:string"))
				{	soap_flag_inputString--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct ns__echoString *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoString, 0, sizeof(struct ns__echoString), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoString * SOAP_FMAC6 soap_new_ns__echoString(struct soap *soap, int n)
{	return soap_instantiate_ns__echoString(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoString(struct soap *soap, struct ns__echoString *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoString * SOAP_FMAC4 soap_instantiate_ns__echoString(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoString(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoString, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoString;
		if (size)
			*size = sizeof(struct ns__echoString);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoString[n];
		if (size)
			*size = n * sizeof(struct ns__echoString);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoString*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoString(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoString %p -> %p\n", q, p));
	*(struct ns__echoString*)p = *(struct ns__echoString*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoStringResponse(struct soap *soap, const struct ns__echoStringResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_xsd__string(soap, &a->_return);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoStringResponse(struct soap *soap, struct ns__echoStringResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_xsd__string(soap, &a->_return);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoStringResponse(struct soap *soap, const struct ns__echoStringResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoStringResponse);
	if (soap_out_ns__echoStringResponse(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoStringResponse(struct soap *soap, const char *tag, int id, const struct ns__echoStringResponse *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoStringResponse), type);
	if (a->_return)
		soap_element_result(soap, "return");
	soap_out_xsd__string(soap, "return", -1, &a->_return, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoStringResponse * SOAP_FMAC4 soap_get_ns__echoStringResponse(struct soap *soap, struct ns__echoStringResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoStringResponse(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoStringResponse * SOAP_FMAC4 soap_in_ns__echoStringResponse(struct soap *soap, const char *tag, struct ns__echoStringResponse *a, const char *type)
{
	short soap_flag__return = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoStringResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoStringResponse, sizeof(struct ns__echoStringResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoStringResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag__return && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_in_xsd__string(soap, NULL, &a->_return, "xsd:string"))
				{	soap_flag__return--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if ((soap->mode & SOAP_XML_STRICT) && (soap_flag__return > 0))
		{	soap->error = SOAP_OCCURS;
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct ns__echoStringResponse *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoStringResponse, 0, sizeof(struct ns__echoStringResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoStringResponse * SOAP_FMAC6 soap_new_ns__echoStringResponse(struct soap *soap, int n)
{	return soap_instantiate_ns__echoStringResponse(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoStringResponse(struct soap *soap, struct ns__echoStringResponse *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoStringResponse * SOAP_FMAC4 soap_instantiate_ns__echoStringResponse(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoStringResponse(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoStringResponse, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoStringResponse;
		if (size)
			*size = sizeof(struct ns__echoStringResponse);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoStringResponse[n];
		if (size)
			*size = n * sizeof(struct ns__echoStringResponse);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoStringResponse*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoStringResponse(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoStringResponse %p -> %p\n", q, p));
	*(struct ns__echoStringResponse*)p = *(struct ns__echoStringResponse*)q;
}

#ifndef WITH_NOGLOBAL

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerToSOAP_ENV__Reason(struct soap *soap, struct SOAP_ENV__Reason *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_SOAP_ENV__Reason))
		soap_serialize_SOAP_ENV__Reason(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerToSOAP_ENV__Reason(struct soap *soap, struct SOAP_ENV__Reason *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerToSOAP_ENV__Reason);
	if (soap_out_PointerToSOAP_ENV__Reason(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerToSOAP_ENV__Reason(struct soap *soap, const char *tag, int id, struct SOAP_ENV__Reason *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_SOAP_ENV__Reason);
	if (id < 0)
		return soap->error;
	return soap_out_SOAP_ENV__Reason(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct SOAP_ENV__Reason ** SOAP_FMAC4 soap_get_PointerToSOAP_ENV__Reason(struct soap *soap, struct SOAP_ENV__Reason **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerToSOAP_ENV__Reason(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct SOAP_ENV__Reason ** SOAP_FMAC4 soap_in_PointerToSOAP_ENV__Reason(struct soap *soap, const char *tag, struct SOAP_ENV__Reason **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1))
		return NULL;
	if (!a)
		if (!(a = (struct SOAP_ENV__Reason **)soap_malloc(soap, sizeof(struct SOAP_ENV__Reason *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_SOAP_ENV__Reason(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct SOAP_ENV__Reason **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_SOAP_ENV__Reason, sizeof(struct SOAP_ENV__Reason), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

#endif

#ifndef WITH_NOGLOBAL

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerToSOAP_ENV__Detail(struct soap *soap, struct SOAP_ENV__Detail *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_SOAP_ENV__Detail))
		soap_serialize_SOAP_ENV__Detail(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerToSOAP_ENV__Detail(struct soap *soap, struct SOAP_ENV__Detail *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerToSOAP_ENV__Detail);
	if (soap_out_PointerToSOAP_ENV__Detail(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerToSOAP_ENV__Detail(struct soap *soap, const char *tag, int id, struct SOAP_ENV__Detail *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_SOAP_ENV__Detail);
	if (id < 0)
		return soap->error;
	return soap_out_SOAP_ENV__Detail(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct SOAP_ENV__Detail ** SOAP_FMAC4 soap_get_PointerToSOAP_ENV__Detail(struct soap *soap, struct SOAP_ENV__Detail **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerToSOAP_ENV__Detail(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct SOAP_ENV__Detail ** SOAP_FMAC4 soap_in_PointerToSOAP_ENV__Detail(struct soap *soap, const char *tag, struct SOAP_ENV__Detail **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1))
		return NULL;
	if (!a)
		if (!(a = (struct SOAP_ENV__Detail **)soap_malloc(soap, sizeof(struct SOAP_ENV__Detail *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_SOAP_ENV__Detail(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct SOAP_ENV__Detail **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_SOAP_ENV__Detail, sizeof(struct SOAP_ENV__Detail), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

#endif

#ifndef WITH_NOGLOBAL

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerToSOAP_ENV__Code(struct soap *soap, struct SOAP_ENV__Code *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_SOAP_ENV__Code))
		soap_serialize_SOAP_ENV__Code(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerToSOAP_ENV__Code(struct soap *soap, struct SOAP_ENV__Code *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerToSOAP_ENV__Code);
	if (soap_out_PointerToSOAP_ENV__Code(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerToSOAP_ENV__Code(struct soap *soap, const char *tag, int id, struct SOAP_ENV__Code *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_SOAP_ENV__Code);
	if (id < 0)
		return soap->error;
	return soap_out_SOAP_ENV__Code(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct SOAP_ENV__Code ** SOAP_FMAC4 soap_get_PointerToSOAP_ENV__Code(struct soap *soap, struct SOAP_ENV__Code **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerToSOAP_ENV__Code(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct SOAP_ENV__Code ** SOAP_FMAC4 soap_in_PointerToSOAP_ENV__Code(struct soap *soap, const char *tag, struct SOAP_ENV__Code **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1))
		return NULL;
	if (!a)
		if (!(a = (struct SOAP_ENV__Code **)soap_malloc(soap, sizeof(struct SOAP_ENV__Code *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_SOAP_ENV__Code(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct SOAP_ENV__Code **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_SOAP_ENV__Code, sizeof(struct SOAP_ENV__Code), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

#endif

SOAP_FMAC3 void SOAP_FMAC4 soap_default_xsd__decimal(struct soap *soap, char **a)
{	soap_default_string(soap, a);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_xsd__decimal(struct soap *soap, char *const*a)
{	soap_serialize_string(soap, a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_xsd__decimal(struct soap *soap, char *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_xsd__decimal);
	if (soap_out_xsd__decimal(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_xsd__decimal(struct soap *soap, const char *tag, int id, char *const*a, const char *type)
{
	return soap_outstring(soap, tag, id, a, type, SOAP_TYPE_xsd__decimal);
}

SOAP_FMAC3 char ** SOAP_FMAC4 soap_get_xsd__decimal(struct soap *soap, char **p, const char *tag, const char *type)
{
	if ((p = soap_in_xsd__decimal(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 char * * SOAP_FMAC4 soap_in_xsd__decimal(struct soap *soap, const char *tag, char **a, const char *type)
{
	return soap_instring(soap, tag, a, type, SOAP_TYPE_xsd__decimal, 1, -1, -1);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_xsd__dateTime(struct soap *soap, char **a)
{	soap_default_string(soap, a);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_xsd__dateTime(struct soap *soap, char *const*a)
{	soap_serialize_string(soap, a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_xsd__dateTime(struct soap *soap, char *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_xsd__dateTime);
	if (soap_out_xsd__dateTime(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_xsd__dateTime(struct soap *soap, const char *tag, int id, char *const*a, const char *type)
{
	return soap_outstring(soap, tag, id, a, type, SOAP_TYPE_xsd__dateTime);
}

SOAP_FMAC3 char ** SOAP_FMAC4 soap_get_xsd__dateTime(struct soap *soap, char **p, const char *tag, const char *type)
{
	if ((p = soap_in_xsd__dateTime(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 char * * SOAP_FMAC4 soap_in_xsd__dateTime(struct soap *soap, const char *tag, char **a, const char *type)
{
	return soap_instring(soap, tag, a, type, SOAP_TYPE_xsd__dateTime, 1, -1, -1);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTounsignedByte(struct soap *soap, unsigned char *const*a)
{
	soap_reference(soap, *a, SOAP_TYPE_unsignedByte);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTounsignedByte(struct soap *soap, unsigned char *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTounsignedByte);
	if (soap_out_PointerTounsignedByte(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTounsignedByte(struct soap *soap, const char *tag, int id, unsigned char *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_unsignedByte);
	if (id < 0)
		return soap->error;
	return soap_out_unsignedByte(soap, tag, id, *a, type);
}

SOAP_FMAC3 unsigned char ** SOAP_FMAC4 soap_get_PointerTounsignedByte(struct soap *soap, unsigned char **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTounsignedByte(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 unsigned char ** SOAP_FMAC4 soap_in_PointerTounsignedByte(struct soap *soap, const char *tag, unsigned char **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1))
		return NULL;
	if (!a)
		if (!(a = (unsigned char **)soap_malloc(soap, sizeof(unsigned char *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_unsignedByte(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (unsigned char **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_unsignedByte, sizeof(unsigned char), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerToPointerTos__SOAPStruct(struct soap *soap, struct s__SOAPStruct **const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_PointerTos__SOAPStruct))
		soap_serialize_PointerTos__SOAPStruct(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerToPointerTos__SOAPStruct(struct soap *soap, struct s__SOAPStruct **const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerToPointerTos__SOAPStruct);
	if (soap_out_PointerToPointerTos__SOAPStruct(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerToPointerTos__SOAPStruct(struct soap *soap, const char *tag, int id, struct s__SOAPStruct **const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_PointerTos__SOAPStruct);
	if (id < 0)
		return soap->error;
	return soap_out_PointerTos__SOAPStruct(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct s__SOAPStruct *** SOAP_FMAC4 soap_get_PointerToPointerTos__SOAPStruct(struct soap *soap, struct s__SOAPStruct ***p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerToPointerTos__SOAPStruct(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct s__SOAPStruct *** SOAP_FMAC4 soap_in_PointerToPointerTos__SOAPStruct(struct soap *soap, const char *tag, struct s__SOAPStruct ***a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1))
		return NULL;
	if (!a)
		if (!(a = (struct s__SOAPStruct ***)soap_malloc(soap, sizeof(struct s__SOAPStruct **))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_PointerTos__SOAPStruct(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct s__SOAPStruct ***)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_PointerTos__SOAPStruct, sizeof(struct s__SOAPStruct *), 1);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTos__SOAPStruct(struct soap *soap, struct s__SOAPStruct *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_s__SOAPStruct))
		soap_serialize_s__SOAPStruct(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTos__SOAPStruct(struct soap *soap, struct s__SOAPStruct *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTos__SOAPStruct);
	if (soap_out_PointerTos__SOAPStruct(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTos__SOAPStruct(struct soap *soap, const char *tag, int id, struct s__SOAPStruct *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_s__SOAPStruct);
	if (id < 0)
		return soap->error;
	return soap_out_s__SOAPStruct(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct s__SOAPStruct ** SOAP_FMAC4 soap_get_PointerTos__SOAPStruct(struct soap *soap, struct s__SOAPStruct **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTos__SOAPStruct(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct s__SOAPStruct ** SOAP_FMAC4 soap_in_PointerTos__SOAPStruct(struct soap *soap, const char *tag, struct s__SOAPStruct **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1))
		return NULL;
	if (!a)
		if (!(a = (struct s__SOAPStruct **)soap_malloc(soap, sizeof(struct s__SOAPStruct *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_s__SOAPStruct(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct s__SOAPStruct **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_s__SOAPStruct, sizeof(struct s__SOAPStruct), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerToPointerToxsd__float(struct soap *soap, float **const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_PointerToxsd__float))
		soap_serialize_PointerToxsd__float(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerToPointerToxsd__float(struct soap *soap, float **const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerToPointerToxsd__float);
	if (soap_out_PointerToPointerToxsd__float(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerToPointerToxsd__float(struct soap *soap, const char *tag, int id, float **const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_PointerToxsd__float);
	if (id < 0)
		return soap->error;
	return soap_out_PointerToxsd__float(soap, tag, id, *a, type);
}

SOAP_FMAC3 float *** SOAP_FMAC4 soap_get_PointerToPointerToxsd__float(struct soap *soap, float ***p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerToPointerToxsd__float(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 float *** SOAP_FMAC4 soap_in_PointerToPointerToxsd__float(struct soap *soap, const char *tag, float ***a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1))
		return NULL;
	if (!a)
		if (!(a = (float ***)soap_malloc(soap, sizeof(float **))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_PointerToxsd__float(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (float ***)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_PointerToxsd__float, sizeof(float *), 1);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerToxsd__float(struct soap *soap, float *const*a)
{
	soap_reference(soap, *a, SOAP_TYPE_xsd__float);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerToxsd__float(struct soap *soap, float *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerToxsd__float);
	if (soap_out_PointerToxsd__float(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerToxsd__float(struct soap *soap, const char *tag, int id, float *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_xsd__float);
	if (id < 0)
		return soap->error;
	return soap_out_xsd__float(soap, tag, id, *a, type);
}

SOAP_FMAC3 float ** SOAP_FMAC4 soap_get_PointerToxsd__float(struct soap *soap, float **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerToxsd__float(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 float ** SOAP_FMAC4 soap_in_PointerToxsd__float(struct soap *soap, const char *tag, float **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1))
		return NULL;
	if (!a)
		if (!(a = (float **)soap_malloc(soap, sizeof(float *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_xsd__float(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (float **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_xsd__float, sizeof(float), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerToPointerToxsd__int(struct soap *soap, long **const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_PointerToxsd__int))
		soap_serialize_PointerToxsd__int(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerToPointerToxsd__int(struct soap *soap, long **const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerToPointerToxsd__int);
	if (soap_out_PointerToPointerToxsd__int(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerToPointerToxsd__int(struct soap *soap, const char *tag, int id, long **const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_PointerToxsd__int);
	if (id < 0)
		return soap->error;
	return soap_out_PointerToxsd__int(soap, tag, id, *a, type);
}

SOAP_FMAC3 long *** SOAP_FMAC4 soap_get_PointerToPointerToxsd__int(struct soap *soap, long ***p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerToPointerToxsd__int(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 long *** SOAP_FMAC4 soap_in_PointerToPointerToxsd__int(struct soap *soap, const char *tag, long ***a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1))
		return NULL;
	if (!a)
		if (!(a = (long ***)soap_malloc(soap, sizeof(long **))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_PointerToxsd__int(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (long ***)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_PointerToxsd__int, sizeof(long *), 1);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerToxsd__int(struct soap *soap, long *const*a)
{
	soap_reference(soap, *a, SOAP_TYPE_xsd__int);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerToxsd__int(struct soap *soap, long *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerToxsd__int);
	if (soap_out_PointerToxsd__int(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerToxsd__int(struct soap *soap, const char *tag, int id, long *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_xsd__int);
	if (id < 0)
		return soap->error;
	return soap_out_xsd__int(soap, tag, id, *a, type);
}

SOAP_FMAC3 long ** SOAP_FMAC4 soap_get_PointerToxsd__int(struct soap *soap, long **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerToxsd__int(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 long ** SOAP_FMAC4 soap_in_PointerToxsd__int(struct soap *soap, const char *tag, long **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1))
		return NULL;
	if (!a)
		if (!(a = (long **)soap_malloc(soap, sizeof(long *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_xsd__int(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (long **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_xsd__int, sizeof(long), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerToxsd__string(struct soap *soap, char **const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_xsd__string))
		soap_serialize_xsd__string(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerToxsd__string(struct soap *soap, char **const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerToxsd__string);
	if (soap_out_PointerToxsd__string(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerToxsd__string(struct soap *soap, const char *tag, int id, char **const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_xsd__string);
	if (id < 0)
		return soap->error;
	return soap_out_xsd__string(soap, tag, id, *a, type);
}

SOAP_FMAC3 char *** SOAP_FMAC4 soap_get_PointerToxsd__string(struct soap *soap, char ***p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerToxsd__string(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 char *** SOAP_FMAC4 soap_in_PointerToxsd__string(struct soap *soap, const char *tag, char ***a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1))
		return NULL;
	if (!a)
		if (!(a = (char ***)soap_malloc(soap, sizeof(char **))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_xsd__string(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (char ***)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_xsd__string, sizeof(char *), 1);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_xsd__string(struct soap *soap, char **a)
{	soap_default_string(soap, a);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_xsd__string(struct soap *soap, char *const*a)
{	soap_serialize_string(soap, a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_xsd__string(struct soap *soap, char *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_xsd__string);
	if (soap_out_xsd__string(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_xsd__string(struct soap *soap, const char *tag, int id, char *const*a, const char *type)
{
	return soap_outstring(soap, tag, id, a, type, SOAP_TYPE_xsd__string);
}

SOAP_FMAC3 char ** SOAP_FMAC4 soap_get_xsd__string(struct soap *soap, char **p, const char *tag, const char *type)
{
	if ((p = soap_in_xsd__string(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 char * * SOAP_FMAC4 soap_in_xsd__string(struct soap *soap, const char *tag, char **a, const char *type)
{
	return soap_instring(soap, tag, a, type, SOAP_TYPE_xsd__string, 1, -1, -1);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__QName(struct soap *soap, char **a)
{	soap_default_string(soap, a);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__QName(struct soap *soap, char *const*a)
{	soap_serialize_string(soap, a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__QName(struct soap *soap, char *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__QName);
	if (soap_out__QName(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__QName(struct soap *soap, const char *tag, int id, char *const*a, const char *type)
{
	return soap_outstring(soap, tag, id, a, type, SOAP_TYPE__QName);
}

SOAP_FMAC3 char ** SOAP_FMAC4 soap_get__QName(struct soap *soap, char **p, const char *tag, const char *type)
{
	if ((p = soap_in__QName(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 char * * SOAP_FMAC4 soap_in__QName(struct soap *soap, const char *tag, char **a, const char *type)
{
	return soap_instring(soap, tag, a, type, SOAP_TYPE__QName, 2, -1, -1);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_string(struct soap *soap, char **a)
{	(void)soap; /* appease -Wall -Werror */
#ifdef SOAP_DEFAULT_string
	*a = SOAP_DEFAULT_string;
#else
	*a = (char *)0;
#endif
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_string(struct soap *soap, char *const*a)
{
	soap_reference(soap, *a, SOAP_TYPE_string);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_string(struct soap *soap, char *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_string);
	if (soap_out_string(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_string(struct soap *soap, const char *tag, int id, char *const*a, const char *type)
{
	return soap_outstring(soap, tag, id, a, type, SOAP_TYPE_string);
}

SOAP_FMAC3 char ** SOAP_FMAC4 soap_get_string(struct soap *soap, char **p, const char *tag, const char *type)
{
	if ((p = soap_in_string(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 char * * SOAP_FMAC4 soap_in_string(struct soap *soap, const char *tag, char **a, const char *type)
{
	return soap_instring(soap, tag, a, type, SOAP_TYPE_string, 1, -1, -1);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_Array2Ofint(struct soap *soap, int const a[2])
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_Array2Ofint(struct soap *soap, int a[2])
{
	int i;
	(void)soap; /* appease -Wall -Werror */
	for (i = 0; i < 2; i++)
	soap_default_int(soap, a+i);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_Array2Ofint(struct soap *soap, int const a[2], const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_Array2Ofint);
	if (soap_out_Array2Ofint(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_Array2Ofint(struct soap *soap, const char *tag, int id, int const a[2], const char *type)
{
	int i;
	soap_array_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_Array2Ofint), "xsd:int[2]", 0);
	for (i = 0; i < 2; i++)
	{
		soap_out_int(soap, "item", -1, a+i, "");
	}
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 int * SOAP_FMAC4 soap_get_Array2Ofint(struct soap *soap, int a[2], const char *tag, const char *type)
{	int (*p);
	if ((p = soap_in_Array2Ofint(soap, tag, a, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 int * SOAP_FMAC4 soap_in_Array2Ofint(struct soap *soap, const char *tag, int a[2], const char *type)
{
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (soap_match_array(soap, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (int (*))soap_id_enter(soap, soap->id, a, SOAP_TYPE_Array2Ofint, sizeof(int[2]), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_Array2Ofint(soap, a);
	if (soap->body && !*soap->href)
	{	int i;
		for (i = 0; i < 2; i++)
		{	soap_peek_element(soap);
			if (soap->position)
			{	i = soap->positions[0];
				if (i < 0 || i >= 2)
				{	soap->error = SOAP_IOB;
					return NULL;
				}
			}
			if (!soap_in_int(soap, NULL, a+i, "xsd:int"))
			{	if (soap->error != SOAP_NO_TAG)
					return NULL;
				soap->error = SOAP_OK;
				break;
			}
		}
		if (soap->mode & SOAP_C_NOIOB)
			while (soap_element_end_in(soap, tag) == SOAP_SYNTAX_ERROR)
			{	soap->peeked = 1;
				soap_ignore_element(soap);
			}
		else if (soap_element_end_in(soap, tag))
		{	if (soap->error == SOAP_SYNTAX_ERROR)
				soap->error = SOAP_IOB;
			return NULL;
		}
	}
	else
	{	a = (int (*))soap_id_forward(soap, soap->href, (void**)soap_id_enter(soap, soap->id, a, SOAP_TYPE_Array2Ofint, sizeof(int[2]), 0, NULL, NULL, NULL), 0, SOAP_TYPE_Array2Ofint, 0, sizeof(int[2]), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return (int *)a;
}

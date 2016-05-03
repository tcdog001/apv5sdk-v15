
#include "soapH.h"

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoNestedArray(struct soap *soap, const struct ns__echoNestedArray *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_s__SOAPArrayStruct(soap, &a->_inputStruct);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoNestedArray(struct soap *soap, struct ns__echoNestedArray *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_s__SOAPArrayStruct(soap, &a->_inputStruct);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoNestedArray(struct soap *soap, const struct ns__echoNestedArray *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoNestedArray);
	if (soap_out_ns__echoNestedArray(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoNestedArray(struct soap *soap, const char *tag, int id, const struct ns__echoNestedArray *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoNestedArray), type);
	soap_out_s__SOAPArrayStruct(soap, "inputStruct", -1, &a->_inputStruct, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoNestedArray * SOAP_FMAC4 soap_get_ns__echoNestedArray(struct soap *soap, struct ns__echoNestedArray *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoNestedArray(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoNestedArray * SOAP_FMAC4 soap_in_ns__echoNestedArray(struct soap *soap, const char *tag, struct ns__echoNestedArray *a, const char *type)
{
	short soap_flag__inputStruct = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoNestedArray *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoNestedArray, sizeof(struct ns__echoNestedArray), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoNestedArray(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag__inputStruct && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_s__SOAPArrayStruct(soap, NULL, &a->_inputStruct, "s:SOAPArrayStruct"))
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
	{	a = (struct ns__echoNestedArray *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoNestedArray, 0, sizeof(struct ns__echoNestedArray), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoNestedArray * SOAP_FMAC6 soap_new_ns__echoNestedArray(struct soap *soap, int n)
{	return soap_instantiate_ns__echoNestedArray(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoNestedArray(struct soap *soap, struct ns__echoNestedArray *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoNestedArray * SOAP_FMAC4 soap_instantiate_ns__echoNestedArray(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoNestedArray(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoNestedArray, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoNestedArray;
		if (size)
			*size = sizeof(struct ns__echoNestedArray);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoNestedArray[n];
		if (size)
			*size = n * sizeof(struct ns__echoNestedArray);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoNestedArray*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoNestedArray(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoNestedArray %p -> %p\n", q, p));
	*(struct ns__echoNestedArray*)p = *(struct ns__echoNestedArray*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoNestedArrayResponse(struct soap *soap, const struct ns__echoNestedArrayResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_s__SOAPArrayStruct(soap, &a->_return);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoNestedArrayResponse(struct soap *soap, struct ns__echoNestedArrayResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_s__SOAPArrayStruct(soap, &a->_return);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoNestedArrayResponse(struct soap *soap, const struct ns__echoNestedArrayResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoNestedArrayResponse);
	if (soap_out_ns__echoNestedArrayResponse(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoNestedArrayResponse(struct soap *soap, const char *tag, int id, const struct ns__echoNestedArrayResponse *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoNestedArrayResponse), type);
	soap_element_result(soap, "return");
	soap_out_s__SOAPArrayStruct(soap, "return", -1, &a->_return, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoNestedArrayResponse * SOAP_FMAC4 soap_get_ns__echoNestedArrayResponse(struct soap *soap, struct ns__echoNestedArrayResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoNestedArrayResponse(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoNestedArrayResponse * SOAP_FMAC4 soap_in_ns__echoNestedArrayResponse(struct soap *soap, const char *tag, struct ns__echoNestedArrayResponse *a, const char *type)
{
	short soap_flag__return = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoNestedArrayResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoNestedArrayResponse, sizeof(struct ns__echoNestedArrayResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoNestedArrayResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag__return && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_s__SOAPArrayStruct(soap, NULL, &a->_return, "s:SOAPArrayStruct"))
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
	{	a = (struct ns__echoNestedArrayResponse *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoNestedArrayResponse, 0, sizeof(struct ns__echoNestedArrayResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoNestedArrayResponse * SOAP_FMAC6 soap_new_ns__echoNestedArrayResponse(struct soap *soap, int n)
{	return soap_instantiate_ns__echoNestedArrayResponse(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoNestedArrayResponse(struct soap *soap, struct ns__echoNestedArrayResponse *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoNestedArrayResponse * SOAP_FMAC4 soap_instantiate_ns__echoNestedArrayResponse(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoNestedArrayResponse(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoNestedArrayResponse, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoNestedArrayResponse;
		if (size)
			*size = sizeof(struct ns__echoNestedArrayResponse);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoNestedArrayResponse[n];
		if (size)
			*size = n * sizeof(struct ns__echoNestedArrayResponse);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoNestedArrayResponse*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoNestedArrayResponse(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoNestedArrayResponse %p -> %p\n", q, p));
	*(struct ns__echoNestedArrayResponse*)p = *(struct ns__echoNestedArrayResponse*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_s__SOAPArrayStruct(struct soap *soap, const struct s__SOAPArrayStruct *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_xsd__string(soap, &a->varString);
	soap_serialize_PointerToxsd__int(soap, &a->varInt);
	soap_serialize_PointerToxsd__float(soap, &a->varFloat);
	soap_serialize_ArrayOfstring(soap, &a->varArray);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_s__SOAPArrayStruct(struct soap *soap, struct s__SOAPArrayStruct *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_xsd__string(soap, &a->varString);
	a->varInt = NULL;
	a->varFloat = NULL;
	soap_default_ArrayOfstring(soap, &a->varArray);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_s__SOAPArrayStruct(struct soap *soap, const struct s__SOAPArrayStruct *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_s__SOAPArrayStruct);
	if (soap_out_s__SOAPArrayStruct(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_s__SOAPArrayStruct(struct soap *soap, const char *tag, int id, const struct s__SOAPArrayStruct *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_s__SOAPArrayStruct), type);
	soap_out_xsd__string(soap, "varString", -1, &a->varString, "");
	soap_out_PointerToxsd__int(soap, "varInt", -1, &a->varInt, "");
	soap_out_PointerToxsd__float(soap, "varFloat", -1, &a->varFloat, "");
	soap_out_ArrayOfstring(soap, "varArray", -1, &a->varArray, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct s__SOAPArrayStruct * SOAP_FMAC4 soap_get_s__SOAPArrayStruct(struct soap *soap, struct s__SOAPArrayStruct *p, const char *tag, const char *type)
{
	if ((p = soap_in_s__SOAPArrayStruct(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct s__SOAPArrayStruct * SOAP_FMAC4 soap_in_s__SOAPArrayStruct(struct soap *soap, const char *tag, struct s__SOAPArrayStruct *a, const char *type)
{
	short soap_flag_varString = 1, soap_flag_varInt = 1, soap_flag_varFloat = 1, soap_flag_varArray = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct s__SOAPArrayStruct *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_s__SOAPArrayStruct, sizeof(struct s__SOAPArrayStruct), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_s__SOAPArrayStruct(soap, a);
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
			if (soap_flag_varArray && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_ArrayOfstring(soap, "varArray", &a->varArray, "xsd:string"))
				{	soap_flag_varArray--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_varArray > 0))
		{	soap->error = SOAP_OCCURS;
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct s__SOAPArrayStruct *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_s__SOAPArrayStruct, 0, sizeof(struct s__SOAPArrayStruct), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct s__SOAPArrayStruct * SOAP_FMAC6 soap_new_s__SOAPArrayStruct(struct soap *soap, int n)
{	return soap_instantiate_s__SOAPArrayStruct(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_s__SOAPArrayStruct(struct soap *soap, struct s__SOAPArrayStruct *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct s__SOAPArrayStruct * SOAP_FMAC4 soap_instantiate_s__SOAPArrayStruct(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_s__SOAPArrayStruct(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_s__SOAPArrayStruct, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct s__SOAPArrayStruct;
		if (size)
			*size = sizeof(struct s__SOAPArrayStruct);
	}
	else
	{	cp->ptr = (void*)new struct s__SOAPArrayStruct[n];
		if (size)
			*size = n * sizeof(struct s__SOAPArrayStruct);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct s__SOAPArrayStruct*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_s__SOAPArrayStruct(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct s__SOAPArrayStruct %p -> %p\n", q, p));
	*(struct s__SOAPArrayStruct*)p = *(struct s__SOAPArrayStruct*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoNestedStruct(struct soap *soap, const struct ns__echoNestedStruct *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_s__SOAPStructStruct(soap, &a->_inputStruct);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoNestedStruct(struct soap *soap, struct ns__echoNestedStruct *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_s__SOAPStructStruct(soap, &a->_inputStruct);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoNestedStruct(struct soap *soap, const struct ns__echoNestedStruct *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoNestedStruct);
	if (soap_out_ns__echoNestedStruct(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoNestedStruct(struct soap *soap, const char *tag, int id, const struct ns__echoNestedStruct *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoNestedStruct), type);
	soap_out_s__SOAPStructStruct(soap, "inputStruct", -1, &a->_inputStruct, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoNestedStruct * SOAP_FMAC4 soap_get_ns__echoNestedStruct(struct soap *soap, struct ns__echoNestedStruct *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoNestedStruct(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoNestedStruct * SOAP_FMAC4 soap_in_ns__echoNestedStruct(struct soap *soap, const char *tag, struct ns__echoNestedStruct *a, const char *type)
{
	short soap_flag__inputStruct = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoNestedStruct *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoNestedStruct, sizeof(struct ns__echoNestedStruct), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoNestedStruct(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag__inputStruct && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_s__SOAPStructStruct(soap, NULL, &a->_inputStruct, "s:SOAPStructStruct"))
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
	{	a = (struct ns__echoNestedStruct *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoNestedStruct, 0, sizeof(struct ns__echoNestedStruct), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoNestedStruct * SOAP_FMAC6 soap_new_ns__echoNestedStruct(struct soap *soap, int n)
{	return soap_instantiate_ns__echoNestedStruct(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoNestedStruct(struct soap *soap, struct ns__echoNestedStruct *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoNestedStruct * SOAP_FMAC4 soap_instantiate_ns__echoNestedStruct(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoNestedStruct(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoNestedStruct, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoNestedStruct;
		if (size)
			*size = sizeof(struct ns__echoNestedStruct);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoNestedStruct[n];
		if (size)
			*size = n * sizeof(struct ns__echoNestedStruct);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoNestedStruct*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoNestedStruct(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoNestedStruct %p -> %p\n", q, p));
	*(struct ns__echoNestedStruct*)p = *(struct ns__echoNestedStruct*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoNestedStructResponse(struct soap *soap, const struct ns__echoNestedStructResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_s__SOAPStructStruct(soap, &a->_return);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoNestedStructResponse(struct soap *soap, struct ns__echoNestedStructResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_s__SOAPStructStruct(soap, &a->_return);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoNestedStructResponse(struct soap *soap, const struct ns__echoNestedStructResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoNestedStructResponse);
	if (soap_out_ns__echoNestedStructResponse(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoNestedStructResponse(struct soap *soap, const char *tag, int id, const struct ns__echoNestedStructResponse *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoNestedStructResponse), type);
	soap_element_result(soap, "return");
	soap_out_s__SOAPStructStruct(soap, "return", -1, &a->_return, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoNestedStructResponse * SOAP_FMAC4 soap_get_ns__echoNestedStructResponse(struct soap *soap, struct ns__echoNestedStructResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoNestedStructResponse(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoNestedStructResponse * SOAP_FMAC4 soap_in_ns__echoNestedStructResponse(struct soap *soap, const char *tag, struct ns__echoNestedStructResponse *a, const char *type)
{
	short soap_flag__return = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoNestedStructResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoNestedStructResponse, sizeof(struct ns__echoNestedStructResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoNestedStructResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag__return && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_s__SOAPStructStruct(soap, NULL, &a->_return, "s:SOAPStructStruct"))
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
	{	a = (struct ns__echoNestedStructResponse *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoNestedStructResponse, 0, sizeof(struct ns__echoNestedStructResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoNestedStructResponse * SOAP_FMAC6 soap_new_ns__echoNestedStructResponse(struct soap *soap, int n)
{	return soap_instantiate_ns__echoNestedStructResponse(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoNestedStructResponse(struct soap *soap, struct ns__echoNestedStructResponse *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoNestedStructResponse * SOAP_FMAC4 soap_instantiate_ns__echoNestedStructResponse(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoNestedStructResponse(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoNestedStructResponse, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoNestedStructResponse;
		if (size)
			*size = sizeof(struct ns__echoNestedStructResponse);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoNestedStructResponse[n];
		if (size)
			*size = n * sizeof(struct ns__echoNestedStructResponse);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoNestedStructResponse*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoNestedStructResponse(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoNestedStructResponse %p -> %p\n", q, p));
	*(struct ns__echoNestedStructResponse*)p = *(struct ns__echoNestedStructResponse*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_s__SOAPStructStruct(struct soap *soap, const struct s__SOAPStructStruct *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_xsd__string(soap, &a->varString);
	soap_serialize_PointerToxsd__int(soap, &a->varInt);
	soap_serialize_PointerToxsd__float(soap, &a->varFloat);
	soap_serialize_PointerTos__SOAPStruct(soap, &a->varStruct);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_s__SOAPStructStruct(struct soap *soap, struct s__SOAPStructStruct *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_xsd__string(soap, &a->varString);
	a->varInt = NULL;
	a->varFloat = NULL;
	a->varStruct = NULL;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_s__SOAPStructStruct(struct soap *soap, const struct s__SOAPStructStruct *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_s__SOAPStructStruct);
	if (soap_out_s__SOAPStructStruct(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_s__SOAPStructStruct(struct soap *soap, const char *tag, int id, const struct s__SOAPStructStruct *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_s__SOAPStructStruct), type);
	soap_out_xsd__string(soap, "varString", -1, &a->varString, "");
	soap_out_PointerToxsd__int(soap, "varInt", -1, &a->varInt, "");
	soap_out_PointerToxsd__float(soap, "varFloat", -1, &a->varFloat, "");
	soap_out_PointerTos__SOAPStruct(soap, "varStruct", -1, &a->varStruct, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct s__SOAPStructStruct * SOAP_FMAC4 soap_get_s__SOAPStructStruct(struct soap *soap, struct s__SOAPStructStruct *p, const char *tag, const char *type)
{
	if ((p = soap_in_s__SOAPStructStruct(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct s__SOAPStructStruct * SOAP_FMAC4 soap_in_s__SOAPStructStruct(struct soap *soap, const char *tag, struct s__SOAPStructStruct *a, const char *type)
{
	short soap_flag_varString = 1, soap_flag_varInt = 1, soap_flag_varFloat = 1, soap_flag_varStruct = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct s__SOAPStructStruct *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_s__SOAPStructStruct, sizeof(struct s__SOAPStructStruct), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_s__SOAPStructStruct(soap, a);
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
			if (soap_flag_varStruct && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTos__SOAPStruct(soap, "varStruct", &a->varStruct, "s:SOAPStruct"))
				{	soap_flag_varStruct--;
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
	{	a = (struct s__SOAPStructStruct *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_s__SOAPStructStruct, 0, sizeof(struct s__SOAPStructStruct), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct s__SOAPStructStruct * SOAP_FMAC6 soap_new_s__SOAPStructStruct(struct soap *soap, int n)
{	return soap_instantiate_s__SOAPStructStruct(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_s__SOAPStructStruct(struct soap *soap, struct s__SOAPStructStruct *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct s__SOAPStructStruct * SOAP_FMAC4 soap_instantiate_s__SOAPStructStruct(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_s__SOAPStructStruct(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_s__SOAPStructStruct, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct s__SOAPStructStruct;
		if (size)
			*size = sizeof(struct s__SOAPStructStruct);
	}
	else
	{	cp->ptr = (void*)new struct s__SOAPStructStruct[n];
		if (size)
			*size = n * sizeof(struct s__SOAPStructStruct);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct s__SOAPStructStruct*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_s__SOAPStructStruct(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct s__SOAPStructStruct %p -> %p\n", q, p));
	*(struct s__SOAPStructStruct*)p = *(struct s__SOAPStructStruct*)q;
}


SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echo2DStringArray(struct soap *soap, const struct ns__echo2DStringArray *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_ArrayOfstring2D(soap, &a->_input2DStringArray);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echo2DStringArray(struct soap *soap, struct ns__echo2DStringArray *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_ArrayOfstring2D(soap, &a->_input2DStringArray);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echo2DStringArray(struct soap *soap, const struct ns__echo2DStringArray *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echo2DStringArray);
	if (soap_out_ns__echo2DStringArray(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echo2DStringArray(struct soap *soap, const char *tag, int id, const struct ns__echo2DStringArray *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echo2DStringArray), type);
	soap_out_ArrayOfstring2D(soap, "input2DStringArray", -1, &a->_input2DStringArray, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echo2DStringArray * SOAP_FMAC4 soap_get_ns__echo2DStringArray(struct soap *soap, struct ns__echo2DStringArray *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echo2DStringArray(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echo2DStringArray * SOAP_FMAC4 soap_in_ns__echo2DStringArray(struct soap *soap, const char *tag, struct ns__echo2DStringArray *a, const char *type)
{
	short soap_flag__input2DStringArray = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echo2DStringArray *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echo2DStringArray, sizeof(struct ns__echo2DStringArray), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echo2DStringArray(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag__input2DStringArray && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_ArrayOfstring2D(soap, NULL, &a->_input2DStringArray, "xsd:string"))
				{	soap_flag__input2DStringArray--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if ((soap->mode & SOAP_XML_STRICT) && (soap_flag__input2DStringArray > 0))
		{	soap->error = SOAP_OCCURS;
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct ns__echo2DStringArray *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echo2DStringArray, 0, sizeof(struct ns__echo2DStringArray), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echo2DStringArray * SOAP_FMAC6 soap_new_ns__echo2DStringArray(struct soap *soap, int n)
{	return soap_instantiate_ns__echo2DStringArray(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echo2DStringArray(struct soap *soap, struct ns__echo2DStringArray *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echo2DStringArray * SOAP_FMAC4 soap_instantiate_ns__echo2DStringArray(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echo2DStringArray(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echo2DStringArray, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echo2DStringArray;
		if (size)
			*size = sizeof(struct ns__echo2DStringArray);
	}
	else
	{	cp->ptr = (void*)new struct ns__echo2DStringArray[n];
		if (size)
			*size = n * sizeof(struct ns__echo2DStringArray);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echo2DStringArray*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echo2DStringArray(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echo2DStringArray %p -> %p\n", q, p));
	*(struct ns__echo2DStringArray*)p = *(struct ns__echo2DStringArray*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echo2DStringArrayResponse(struct soap *soap, const struct ns__echo2DStringArrayResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_ArrayOfstring2D(soap, &a->_return);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echo2DStringArrayResponse(struct soap *soap, struct ns__echo2DStringArrayResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_ArrayOfstring2D(soap, &a->_return);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echo2DStringArrayResponse(struct soap *soap, const struct ns__echo2DStringArrayResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echo2DStringArrayResponse);
	if (soap_out_ns__echo2DStringArrayResponse(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echo2DStringArrayResponse(struct soap *soap, const char *tag, int id, const struct ns__echo2DStringArrayResponse *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echo2DStringArrayResponse), type);
	soap_element_result(soap, "return");
	soap_out_ArrayOfstring2D(soap, "return", -1, &a->_return, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echo2DStringArrayResponse * SOAP_FMAC4 soap_get_ns__echo2DStringArrayResponse(struct soap *soap, struct ns__echo2DStringArrayResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echo2DStringArrayResponse(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echo2DStringArrayResponse * SOAP_FMAC4 soap_in_ns__echo2DStringArrayResponse(struct soap *soap, const char *tag, struct ns__echo2DStringArrayResponse *a, const char *type)
{
	short soap_flag__return = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echo2DStringArrayResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echo2DStringArrayResponse, sizeof(struct ns__echo2DStringArrayResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echo2DStringArrayResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag__return && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_ArrayOfstring2D(soap, NULL, &a->_return, "xsd:string"))
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
	{	a = (struct ns__echo2DStringArrayResponse *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echo2DStringArrayResponse, 0, sizeof(struct ns__echo2DStringArrayResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echo2DStringArrayResponse * SOAP_FMAC6 soap_new_ns__echo2DStringArrayResponse(struct soap *soap, int n)
{	return soap_instantiate_ns__echo2DStringArrayResponse(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echo2DStringArrayResponse(struct soap *soap, struct ns__echo2DStringArrayResponse *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echo2DStringArrayResponse * SOAP_FMAC4 soap_instantiate_ns__echo2DStringArrayResponse(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echo2DStringArrayResponse(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echo2DStringArrayResponse, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echo2DStringArrayResponse;
		if (size)
			*size = sizeof(struct ns__echo2DStringArrayResponse);
	}
	else
	{	cp->ptr = (void*)new struct ns__echo2DStringArrayResponse[n];
		if (size)
			*size = n * sizeof(struct ns__echo2DStringArrayResponse);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echo2DStringArrayResponse*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echo2DStringArrayResponse(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echo2DStringArrayResponse %p -> %p\n", q, p));
	*(struct ns__echo2DStringArrayResponse*)p = *(struct ns__echo2DStringArrayResponse*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ArrayOfstring2D(struct soap *soap, struct ArrayOfstring2D const*a)
{
	int i;
	if (a->__ptr && !soap_array_reference(soap, a, (struct soap_array*)&a->__ptr, 2, SOAP_TYPE_ArrayOfstring2D))
		for (i = 0; i < soap_size(a->__size, 2); i++)
		{
			soap_serialize_xsd__string(soap, a->__ptr + i);
		}
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ArrayOfstring2D(struct soap *soap, struct ArrayOfstring2D *a)
{
	a->__ptr = NULL;
	a->__size[0] = 0;
	a->__offset[0] = 0;
	a->__size[1] = 0;
	a->__offset[1] = 0;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ArrayOfstring2D(struct soap *soap, const struct ArrayOfstring2D *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, (struct soap_array*)&a->__ptr, 2, tag, SOAP_TYPE_ArrayOfstring2D);
	if (soap_out_ArrayOfstring2D(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ArrayOfstring2D(struct soap *soap, const char *tag, int id, const struct ArrayOfstring2D *a, const char *type)
{
	int i, n = soap_size(a->__size, 2);
	char *t = soap_putsizesoffsets(soap, "xsd:string", a->__size, a->__offset, 2);
	id = soap_element_id(soap, tag, id, a, (struct soap_array*)&a->__ptr, 2, type, SOAP_TYPE_ArrayOfstring2D);
	if (id < 0)
		return soap->error;
	soap_array_begin_out(soap, tag, id, t, soap_putoffsets(soap, a->__offset, 2));
	for (i = 0; i < n; i++)
	{
		soap->position = 2;
		soap->positions[0] = i/a->__size[1]%a->__size[0];
		soap->positions[1] = i%a->__size[1];
		soap_out_xsd__string(soap, "item", -1, &a->__ptr[i], "");
	}
	soap->position = 0;
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ArrayOfstring2D * SOAP_FMAC4 soap_get_ArrayOfstring2D(struct soap *soap, struct ArrayOfstring2D *p, const char *tag, const char *type)
{
	if ((p = soap_in_ArrayOfstring2D(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ArrayOfstring2D * SOAP_FMAC4 soap_in_ArrayOfstring2D(struct soap *soap, const char *tag, struct ArrayOfstring2D *a, const char *type)
{	int i, j, n;
	char **p;
	if (soap_element_begin_in(soap, tag, 1))
		return NULL;
	if (soap_match_array(soap, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ArrayOfstring2D *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ArrayOfstring2D, sizeof(struct ArrayOfstring2D), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ArrayOfstring2D(soap, a);
	if (soap->body && !*soap->href)
	{
		n = soap_getsizes(soap->arraySize, a->__size, 2);
		n -= j = soap_getoffsets(soap->arrayOffset, a->__size, a->__offset, 2);
		if (n >= 0)
		{	a->__ptr = (char **)soap_malloc(soap, n*sizeof(char *));
			for (i = 0; i < n; i++)
				a->__ptr[i] = NULL;
			for (i = 0; i < n; i++)
			{	soap_peek_element(soap);
				if (soap->position == 2)
				{	i = soap->positions[0]*a->__size[1]+soap->positions[1]-j;
					if (i < 0 || i >= n)
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
			for (a->__size[0] = 0; ; a->__size[0]++)
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
	{	a = (struct ArrayOfstring2D *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ArrayOfstring2D, 0, sizeof(struct ArrayOfstring2D), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ArrayOfstring2D * SOAP_FMAC6 soap_new_ArrayOfstring2D(struct soap *soap, int n)
{	return soap_instantiate_ArrayOfstring2D(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ArrayOfstring2D(struct soap *soap, struct ArrayOfstring2D *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ArrayOfstring2D * SOAP_FMAC4 soap_instantiate_ArrayOfstring2D(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ArrayOfstring2D(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ArrayOfstring2D, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ArrayOfstring2D;
		if (size)
			*size = sizeof(struct ArrayOfstring2D);
	}
	else
	{	cp->ptr = (void*)new struct ArrayOfstring2D[n];
		if (size)
			*size = n * sizeof(struct ArrayOfstring2D);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ArrayOfstring2D*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ArrayOfstring2D(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ArrayOfstring2D %p -> %p\n", q, p));
	*(struct ArrayOfstring2D*)p = *(struct ArrayOfstring2D*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoSimpleTypesAsStruct(struct soap *soap, const struct ns__echoSimpleTypesAsStruct *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_xsd__string(soap, &a->inputString);
	soap_serialize_PointerToxsd__int(soap, &a->inputInteger);
	soap_serialize_PointerToxsd__float(soap, &a->inputFloat);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoSimpleTypesAsStruct(struct soap *soap, struct ns__echoSimpleTypesAsStruct *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_xsd__string(soap, &a->inputString);
	a->inputInteger = NULL;
	a->inputFloat = NULL;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoSimpleTypesAsStruct(struct soap *soap, const struct ns__echoSimpleTypesAsStruct *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoSimpleTypesAsStruct);
	if (soap_out_ns__echoSimpleTypesAsStruct(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoSimpleTypesAsStruct(struct soap *soap, const char *tag, int id, const struct ns__echoSimpleTypesAsStruct *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoSimpleTypesAsStruct), type);
	soap_out_xsd__string(soap, "inputString", -1, &a->inputString, "");
	soap_out_PointerToxsd__int(soap, "inputInteger", -1, &a->inputInteger, "");
	soap_out_PointerToxsd__float(soap, "inputFloat", -1, &a->inputFloat, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoSimpleTypesAsStruct * SOAP_FMAC4 soap_get_ns__echoSimpleTypesAsStruct(struct soap *soap, struct ns__echoSimpleTypesAsStruct *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoSimpleTypesAsStruct(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoSimpleTypesAsStruct * SOAP_FMAC4 soap_in_ns__echoSimpleTypesAsStruct(struct soap *soap, const char *tag, struct ns__echoSimpleTypesAsStruct *a, const char *type)
{
	short soap_flag_inputString = 1, soap_flag_inputInteger = 1, soap_flag_inputFloat = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoSimpleTypesAsStruct *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoSimpleTypesAsStruct, sizeof(struct ns__echoSimpleTypesAsStruct), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoSimpleTypesAsStruct(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_inputString && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_in_xsd__string(soap, "inputString", &a->inputString, "xsd:string"))
				{	soap_flag_inputString--;
					continue;
				}
			if (soap_flag_inputInteger && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerToxsd__int(soap, "inputInteger", &a->inputInteger, "xsd:int"))
				{	soap_flag_inputInteger--;
					continue;
				}
			if (soap_flag_inputFloat && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerToxsd__float(soap, "inputFloat", &a->inputFloat, "xsd:float"))
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
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct ns__echoSimpleTypesAsStruct *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoSimpleTypesAsStruct, 0, sizeof(struct ns__echoSimpleTypesAsStruct), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoSimpleTypesAsStruct * SOAP_FMAC6 soap_new_ns__echoSimpleTypesAsStruct(struct soap *soap, int n)
{	return soap_instantiate_ns__echoSimpleTypesAsStruct(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoSimpleTypesAsStruct(struct soap *soap, struct ns__echoSimpleTypesAsStruct *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoSimpleTypesAsStruct * SOAP_FMAC4 soap_instantiate_ns__echoSimpleTypesAsStruct(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoSimpleTypesAsStruct(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoSimpleTypesAsStruct, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoSimpleTypesAsStruct;
		if (size)
			*size = sizeof(struct ns__echoSimpleTypesAsStruct);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoSimpleTypesAsStruct[n];
		if (size)
			*size = n * sizeof(struct ns__echoSimpleTypesAsStruct);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoSimpleTypesAsStruct*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoSimpleTypesAsStruct(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoSimpleTypesAsStruct %p -> %p\n", q, p));
	*(struct ns__echoSimpleTypesAsStruct*)p = *(struct ns__echoSimpleTypesAsStruct*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoSimpleTypesAsStructResponse(struct soap *soap, const struct ns__echoSimpleTypesAsStructResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_embedded(soap, &a->_return, SOAP_TYPE_s__SOAPStruct);
	soap_serialize_s__SOAPStruct(soap, &a->_return);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoSimpleTypesAsStructResponse(struct soap *soap, struct ns__echoSimpleTypesAsStructResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_s__SOAPStruct(soap, &a->_return);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoSimpleTypesAsStructResponse(struct soap *soap, const struct ns__echoSimpleTypesAsStructResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoSimpleTypesAsStructResponse);
	if (soap_out_ns__echoSimpleTypesAsStructResponse(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoSimpleTypesAsStructResponse(struct soap *soap, const char *tag, int id, const struct ns__echoSimpleTypesAsStructResponse *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoSimpleTypesAsStructResponse), type);
	soap_element_result(soap, "return");
	soap_out_s__SOAPStruct(soap, "return", -1, &a->_return, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoSimpleTypesAsStructResponse * SOAP_FMAC4 soap_get_ns__echoSimpleTypesAsStructResponse(struct soap *soap, struct ns__echoSimpleTypesAsStructResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoSimpleTypesAsStructResponse(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoSimpleTypesAsStructResponse * SOAP_FMAC4 soap_in_ns__echoSimpleTypesAsStructResponse(struct soap *soap, const char *tag, struct ns__echoSimpleTypesAsStructResponse *a, const char *type)
{
	short soap_flag__return = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoSimpleTypesAsStructResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoSimpleTypesAsStructResponse, sizeof(struct ns__echoSimpleTypesAsStructResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoSimpleTypesAsStructResponse(soap, a);
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
	{	a = (struct ns__echoSimpleTypesAsStructResponse *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoSimpleTypesAsStructResponse, 0, sizeof(struct ns__echoSimpleTypesAsStructResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoSimpleTypesAsStructResponse * SOAP_FMAC6 soap_new_ns__echoSimpleTypesAsStructResponse(struct soap *soap, int n)
{	return soap_instantiate_ns__echoSimpleTypesAsStructResponse(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoSimpleTypesAsStructResponse(struct soap *soap, struct ns__echoSimpleTypesAsStructResponse *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoSimpleTypesAsStructResponse * SOAP_FMAC4 soap_instantiate_ns__echoSimpleTypesAsStructResponse(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoSimpleTypesAsStructResponse(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoSimpleTypesAsStructResponse, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoSimpleTypesAsStructResponse;
		if (size)
			*size = sizeof(struct ns__echoSimpleTypesAsStructResponse);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoSimpleTypesAsStructResponse[n];
		if (size)
			*size = n * sizeof(struct ns__echoSimpleTypesAsStructResponse);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoSimpleTypesAsStructResponse*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoSimpleTypesAsStructResponse(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoSimpleTypesAsStructResponse %p -> %p\n", q, p));
	*(struct ns__echoSimpleTypesAsStructResponse*)p = *(struct ns__echoSimpleTypesAsStructResponse*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoStructAsSimpleTypes(struct soap *soap, const struct ns__echoStructAsSimpleTypes *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_embedded(soap, &a->_inputStruct, SOAP_TYPE_s__SOAPStruct);
	soap_serialize_s__SOAPStruct(soap, &a->_inputStruct);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoStructAsSimpleTypes(struct soap *soap, struct ns__echoStructAsSimpleTypes *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_s__SOAPStruct(soap, &a->_inputStruct);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoStructAsSimpleTypes(struct soap *soap, const struct ns__echoStructAsSimpleTypes *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoStructAsSimpleTypes);
	if (soap_out_ns__echoStructAsSimpleTypes(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoStructAsSimpleTypes(struct soap *soap, const char *tag, int id, const struct ns__echoStructAsSimpleTypes *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoStructAsSimpleTypes), type);
	soap_out_s__SOAPStruct(soap, "inputStruct", -1, &a->_inputStruct, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoStructAsSimpleTypes * SOAP_FMAC4 soap_get_ns__echoStructAsSimpleTypes(struct soap *soap, struct ns__echoStructAsSimpleTypes *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoStructAsSimpleTypes(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoStructAsSimpleTypes * SOAP_FMAC4 soap_in_ns__echoStructAsSimpleTypes(struct soap *soap, const char *tag, struct ns__echoStructAsSimpleTypes *a, const char *type)
{
	short soap_flag__inputStruct = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoStructAsSimpleTypes *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoStructAsSimpleTypes, sizeof(struct ns__echoStructAsSimpleTypes), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoStructAsSimpleTypes(soap, a);
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
	{	a = (struct ns__echoStructAsSimpleTypes *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoStructAsSimpleTypes, 0, sizeof(struct ns__echoStructAsSimpleTypes), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoStructAsSimpleTypes * SOAP_FMAC6 soap_new_ns__echoStructAsSimpleTypes(struct soap *soap, int n)
{	return soap_instantiate_ns__echoStructAsSimpleTypes(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoStructAsSimpleTypes(struct soap *soap, struct ns__echoStructAsSimpleTypes *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoStructAsSimpleTypes * SOAP_FMAC4 soap_instantiate_ns__echoStructAsSimpleTypes(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoStructAsSimpleTypes(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoStructAsSimpleTypes, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoStructAsSimpleTypes;
		if (size)
			*size = sizeof(struct ns__echoStructAsSimpleTypes);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoStructAsSimpleTypes[n];
		if (size)
			*size = n * sizeof(struct ns__echoStructAsSimpleTypes);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoStructAsSimpleTypes*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoStructAsSimpleTypes(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoStructAsSimpleTypes %p -> %p\n", q, p));
	*(struct ns__echoStructAsSimpleTypes*)p = *(struct ns__echoStructAsSimpleTypes*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoStructAsSimpleTypesResponse(struct soap *soap, const struct ns__echoStructAsSimpleTypesResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_xsd__string(soap, &a->_outputString);
	soap_serialize_PointerToxsd__int(soap, &a->_outputInteger);
	soap_serialize_PointerToxsd__float(soap, &a->_outputFloat);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoStructAsSimpleTypesResponse(struct soap *soap, struct ns__echoStructAsSimpleTypesResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_xsd__string(soap, &a->_outputString);
	a->_outputInteger = NULL;
	a->_outputFloat = NULL;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoStructAsSimpleTypesResponse(struct soap *soap, const struct ns__echoStructAsSimpleTypesResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoStructAsSimpleTypesResponse);
	if (soap_out_ns__echoStructAsSimpleTypesResponse(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoStructAsSimpleTypesResponse(struct soap *soap, const char *tag, int id, const struct ns__echoStructAsSimpleTypesResponse *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoStructAsSimpleTypesResponse), type);
	if (a->_outputString)
		soap_element_result(soap, "outputString");
	soap_out_xsd__string(soap, "outputString", -1, &a->_outputString, "");
	soap_out_PointerToxsd__int(soap, "outputInteger", -1, &a->_outputInteger, "");
	soap_out_PointerToxsd__float(soap, "outputFloat", -1, &a->_outputFloat, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoStructAsSimpleTypesResponse * SOAP_FMAC4 soap_get_ns__echoStructAsSimpleTypesResponse(struct soap *soap, struct ns__echoStructAsSimpleTypesResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoStructAsSimpleTypesResponse(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoStructAsSimpleTypesResponse * SOAP_FMAC4 soap_in_ns__echoStructAsSimpleTypesResponse(struct soap *soap, const char *tag, struct ns__echoStructAsSimpleTypesResponse *a, const char *type)
{
	short soap_flag__outputString = 1, soap_flag__outputInteger = 1, soap_flag__outputFloat = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoStructAsSimpleTypesResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoStructAsSimpleTypesResponse, sizeof(struct ns__echoStructAsSimpleTypesResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoStructAsSimpleTypesResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag__outputString && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_in_xsd__string(soap, NULL, &a->_outputString, "xsd:string"))
				{	soap_flag__outputString--;
					continue;
				}
			if (soap_flag__outputInteger && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerToxsd__int(soap, NULL, &a->_outputInteger, "xsd:int"))
				{	soap_flag__outputInteger--;
					continue;
				}
			if (soap_flag__outputFloat && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerToxsd__float(soap, NULL, &a->_outputFloat, "xsd:float"))
				{	soap_flag__outputFloat--;
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
	{	a = (struct ns__echoStructAsSimpleTypesResponse *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoStructAsSimpleTypesResponse, 0, sizeof(struct ns__echoStructAsSimpleTypesResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoStructAsSimpleTypesResponse * SOAP_FMAC6 soap_new_ns__echoStructAsSimpleTypesResponse(struct soap *soap, int n)
{	return soap_instantiate_ns__echoStructAsSimpleTypesResponse(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoStructAsSimpleTypesResponse(struct soap *soap, struct ns__echoStructAsSimpleTypesResponse *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoStructAsSimpleTypesResponse * SOAP_FMAC4 soap_instantiate_ns__echoStructAsSimpleTypesResponse(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoStructAsSimpleTypesResponse(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoStructAsSimpleTypesResponse, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoStructAsSimpleTypesResponse;
		if (size)
			*size = sizeof(struct ns__echoStructAsSimpleTypesResponse);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoStructAsSimpleTypesResponse[n];
		if (size)
			*size = n * sizeof(struct ns__echoStructAsSimpleTypesResponse);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoStructAsSimpleTypesResponse*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoStructAsSimpleTypesResponse(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoStructAsSimpleTypesResponse %p -> %p\n", q, p));
	*(struct ns__echoStructAsSimpleTypesResponse*)p = *(struct ns__echoStructAsSimpleTypesResponse*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoBoolean(struct soap *soap, const struct ns__echoBoolean *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoBoolean(struct soap *soap, struct ns__echoBoolean *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_xsd__boolean(soap, &a->inputBoolean);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoBoolean(struct soap *soap, const struct ns__echoBoolean *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoBoolean);
	if (soap_out_ns__echoBoolean(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoBoolean(struct soap *soap, const char *tag, int id, const struct ns__echoBoolean *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoBoolean), type);
	soap_out_xsd__boolean(soap, "inputBoolean", -1, &a->inputBoolean, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoBoolean * SOAP_FMAC4 soap_get_ns__echoBoolean(struct soap *soap, struct ns__echoBoolean *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoBoolean(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoBoolean * SOAP_FMAC4 soap_in_ns__echoBoolean(struct soap *soap, const char *tag, struct ns__echoBoolean *a, const char *type)
{
	short soap_flag_inputBoolean = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoBoolean *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoBoolean, sizeof(struct ns__echoBoolean), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoBoolean(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_inputBoolean && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_xsd__boolean(soap, "inputBoolean", &a->inputBoolean, "xsd:boolean"))
				{	soap_flag_inputBoolean--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_inputBoolean > 0))
		{	soap->error = SOAP_OCCURS;
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct ns__echoBoolean *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoBoolean, 0, sizeof(struct ns__echoBoolean), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoBoolean * SOAP_FMAC6 soap_new_ns__echoBoolean(struct soap *soap, int n)
{	return soap_instantiate_ns__echoBoolean(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoBoolean(struct soap *soap, struct ns__echoBoolean *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoBoolean * SOAP_FMAC4 soap_instantiate_ns__echoBoolean(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoBoolean(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoBoolean, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoBoolean;
		if (size)
			*size = sizeof(struct ns__echoBoolean);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoBoolean[n];
		if (size)
			*size = n * sizeof(struct ns__echoBoolean);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoBoolean*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoBoolean(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoBoolean %p -> %p\n", q, p));
	*(struct ns__echoBoolean*)p = *(struct ns__echoBoolean*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoBooleanResponse(struct soap *soap, const struct ns__echoBooleanResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoBooleanResponse(struct soap *soap, struct ns__echoBooleanResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_xsd__boolean(soap, &a->_return);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoBooleanResponse(struct soap *soap, const struct ns__echoBooleanResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoBooleanResponse);
	if (soap_out_ns__echoBooleanResponse(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoBooleanResponse(struct soap *soap, const char *tag, int id, const struct ns__echoBooleanResponse *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoBooleanResponse), type);
	soap_element_result(soap, "return");
	soap_out_xsd__boolean(soap, "return", -1, &a->_return, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoBooleanResponse * SOAP_FMAC4 soap_get_ns__echoBooleanResponse(struct soap *soap, struct ns__echoBooleanResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoBooleanResponse(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoBooleanResponse * SOAP_FMAC4 soap_in_ns__echoBooleanResponse(struct soap *soap, const char *tag, struct ns__echoBooleanResponse *a, const char *type)
{
	short soap_flag__return = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoBooleanResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoBooleanResponse, sizeof(struct ns__echoBooleanResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoBooleanResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag__return && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_xsd__boolean(soap, NULL, &a->_return, "xsd:boolean"))
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
	{	a = (struct ns__echoBooleanResponse *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoBooleanResponse, 0, sizeof(struct ns__echoBooleanResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoBooleanResponse * SOAP_FMAC6 soap_new_ns__echoBooleanResponse(struct soap *soap, int n)
{	return soap_instantiate_ns__echoBooleanResponse(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoBooleanResponse(struct soap *soap, struct ns__echoBooleanResponse *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoBooleanResponse * SOAP_FMAC4 soap_instantiate_ns__echoBooleanResponse(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoBooleanResponse(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoBooleanResponse, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoBooleanResponse;
		if (size)
			*size = sizeof(struct ns__echoBooleanResponse);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoBooleanResponse[n];
		if (size)
			*size = n * sizeof(struct ns__echoBooleanResponse);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoBooleanResponse*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoBooleanResponse(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoBooleanResponse %p -> %p\n", q, p));
	*(struct ns__echoBooleanResponse*)p = *(struct ns__echoBooleanResponse*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoDecimal(struct soap *soap, const struct ns__echoDecimal *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_xsd__decimal(soap, &a->inputDecimal);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoDecimal(struct soap *soap, struct ns__echoDecimal *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_xsd__decimal(soap, &a->inputDecimal);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoDecimal(struct soap *soap, const struct ns__echoDecimal *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoDecimal);
	if (soap_out_ns__echoDecimal(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoDecimal(struct soap *soap, const char *tag, int id, const struct ns__echoDecimal *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoDecimal), type);
	soap_out_xsd__decimal(soap, "inputDecimal", -1, &a->inputDecimal, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoDecimal * SOAP_FMAC4 soap_get_ns__echoDecimal(struct soap *soap, struct ns__echoDecimal *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoDecimal(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoDecimal * SOAP_FMAC4 soap_in_ns__echoDecimal(struct soap *soap, const char *tag, struct ns__echoDecimal *a, const char *type)
{
	short soap_flag_inputDecimal = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoDecimal *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoDecimal, sizeof(struct ns__echoDecimal), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoDecimal(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_inputDecimal && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_in_xsd__decimal(soap, "inputDecimal", &a->inputDecimal, "xsd:decimal"))
				{	soap_flag_inputDecimal--;
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
	{	a = (struct ns__echoDecimal *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoDecimal, 0, sizeof(struct ns__echoDecimal), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoDecimal * SOAP_FMAC6 soap_new_ns__echoDecimal(struct soap *soap, int n)
{	return soap_instantiate_ns__echoDecimal(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoDecimal(struct soap *soap, struct ns__echoDecimal *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoDecimal * SOAP_FMAC4 soap_instantiate_ns__echoDecimal(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoDecimal(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoDecimal, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoDecimal;
		if (size)
			*size = sizeof(struct ns__echoDecimal);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoDecimal[n];
		if (size)
			*size = n * sizeof(struct ns__echoDecimal);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoDecimal*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoDecimal(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoDecimal %p -> %p\n", q, p));
	*(struct ns__echoDecimal*)p = *(struct ns__echoDecimal*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoDecimalResponse(struct soap *soap, const struct ns__echoDecimalResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_xsd__decimal(soap, &a->_return);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoDecimalResponse(struct soap *soap, struct ns__echoDecimalResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_xsd__decimal(soap, &a->_return);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoDecimalResponse(struct soap *soap, const struct ns__echoDecimalResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoDecimalResponse);
	if (soap_out_ns__echoDecimalResponse(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoDecimalResponse(struct soap *soap, const char *tag, int id, const struct ns__echoDecimalResponse *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoDecimalResponse), type);
	if (a->_return)
		soap_element_result(soap, "return");
	soap_out_xsd__decimal(soap, "return", -1, &a->_return, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoDecimalResponse * SOAP_FMAC4 soap_get_ns__echoDecimalResponse(struct soap *soap, struct ns__echoDecimalResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoDecimalResponse(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoDecimalResponse * SOAP_FMAC4 soap_in_ns__echoDecimalResponse(struct soap *soap, const char *tag, struct ns__echoDecimalResponse *a, const char *type)
{
	short soap_flag__return = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoDecimalResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoDecimalResponse, sizeof(struct ns__echoDecimalResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoDecimalResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag__return && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_in_xsd__decimal(soap, NULL, &a->_return, "xsd:decimal"))
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
	{	a = (struct ns__echoDecimalResponse *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoDecimalResponse, 0, sizeof(struct ns__echoDecimalResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoDecimalResponse * SOAP_FMAC6 soap_new_ns__echoDecimalResponse(struct soap *soap, int n)
{	return soap_instantiate_ns__echoDecimalResponse(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoDecimalResponse(struct soap *soap, struct ns__echoDecimalResponse *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoDecimalResponse * SOAP_FMAC4 soap_instantiate_ns__echoDecimalResponse(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoDecimalResponse(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoDecimalResponse, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoDecimalResponse;
		if (size)
			*size = sizeof(struct ns__echoDecimalResponse);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoDecimalResponse[n];
		if (size)
			*size = n * sizeof(struct ns__echoDecimalResponse);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoDecimalResponse*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoDecimalResponse(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoDecimalResponse %p -> %p\n", q, p));
	*(struct ns__echoDecimalResponse*)p = *(struct ns__echoDecimalResponse*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoHexBinary(struct soap *soap, const struct ns__echoHexBinary *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_xsd__hexBinary(soap, &a->inputHexBinary);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoHexBinary(struct soap *soap, struct ns__echoHexBinary *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_xsd__hexBinary(soap, &a->inputHexBinary);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoHexBinary(struct soap *soap, const struct ns__echoHexBinary *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoHexBinary);
	if (soap_out_ns__echoHexBinary(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoHexBinary(struct soap *soap, const char *tag, int id, const struct ns__echoHexBinary *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoHexBinary), type);
	soap_out_xsd__hexBinary(soap, "inputHexBinary", -1, &a->inputHexBinary, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoHexBinary * SOAP_FMAC4 soap_get_ns__echoHexBinary(struct soap *soap, struct ns__echoHexBinary *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoHexBinary(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoHexBinary * SOAP_FMAC4 soap_in_ns__echoHexBinary(struct soap *soap, const char *tag, struct ns__echoHexBinary *a, const char *type)
{
	short soap_flag_inputHexBinary = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoHexBinary *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoHexBinary, sizeof(struct ns__echoHexBinary), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoHexBinary(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_inputHexBinary && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_xsd__hexBinary(soap, "inputHexBinary", &a->inputHexBinary, "xsd:hexBinary"))
				{	soap_flag_inputHexBinary--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_inputHexBinary > 0))
		{	soap->error = SOAP_OCCURS;
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct ns__echoHexBinary *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoHexBinary, 0, sizeof(struct ns__echoHexBinary), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoHexBinary * SOAP_FMAC6 soap_new_ns__echoHexBinary(struct soap *soap, int n)
{	return soap_instantiate_ns__echoHexBinary(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoHexBinary(struct soap *soap, struct ns__echoHexBinary *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoHexBinary * SOAP_FMAC4 soap_instantiate_ns__echoHexBinary(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoHexBinary(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoHexBinary, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoHexBinary;
		if (size)
			*size = sizeof(struct ns__echoHexBinary);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoHexBinary[n];
		if (size)
			*size = n * sizeof(struct ns__echoHexBinary);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoHexBinary*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoHexBinary(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoHexBinary %p -> %p\n", q, p));
	*(struct ns__echoHexBinary*)p = *(struct ns__echoHexBinary*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoHexBinaryResponse(struct soap *soap, const struct ns__echoHexBinaryResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_xsd__hexBinary(soap, &a->_return);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoHexBinaryResponse(struct soap *soap, struct ns__echoHexBinaryResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_xsd__hexBinary(soap, &a->_return);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoHexBinaryResponse(struct soap *soap, const struct ns__echoHexBinaryResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoHexBinaryResponse);
	if (soap_out_ns__echoHexBinaryResponse(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoHexBinaryResponse(struct soap *soap, const char *tag, int id, const struct ns__echoHexBinaryResponse *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoHexBinaryResponse), type);
	soap_element_result(soap, "return");
	soap_out_xsd__hexBinary(soap, "return", -1, &a->_return, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoHexBinaryResponse * SOAP_FMAC4 soap_get_ns__echoHexBinaryResponse(struct soap *soap, struct ns__echoHexBinaryResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoHexBinaryResponse(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoHexBinaryResponse * SOAP_FMAC4 soap_in_ns__echoHexBinaryResponse(struct soap *soap, const char *tag, struct ns__echoHexBinaryResponse *a, const char *type)
{
	short soap_flag__return = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoHexBinaryResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoHexBinaryResponse, sizeof(struct ns__echoHexBinaryResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoHexBinaryResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag__return && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_xsd__hexBinary(soap, NULL, &a->_return, "xsd:hexBinary"))
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
	{	a = (struct ns__echoHexBinaryResponse *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoHexBinaryResponse, 0, sizeof(struct ns__echoHexBinaryResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoHexBinaryResponse * SOAP_FMAC6 soap_new_ns__echoHexBinaryResponse(struct soap *soap, int n)
{	return soap_instantiate_ns__echoHexBinaryResponse(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoHexBinaryResponse(struct soap *soap, struct ns__echoHexBinaryResponse *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoHexBinaryResponse * SOAP_FMAC4 soap_instantiate_ns__echoHexBinaryResponse(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoHexBinaryResponse(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoHexBinaryResponse, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoHexBinaryResponse;
		if (size)
			*size = sizeof(struct ns__echoHexBinaryResponse);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoHexBinaryResponse[n];
		if (size)
			*size = n * sizeof(struct ns__echoHexBinaryResponse);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoHexBinaryResponse*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoHexBinaryResponse(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoHexBinaryResponse %p -> %p\n", q, p));
	*(struct ns__echoHexBinaryResponse*)p = *(struct ns__echoHexBinaryResponse*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_xsd__hexBinary(struct soap *soap, struct xsd__hexBinary const*a)
{
	if (a->__ptr)
		soap_array_reference(soap, a, (struct soap_array*)&a->__ptr, 1, SOAP_TYPE_xsd__hexBinary);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_xsd__hexBinary(struct soap *soap, struct xsd__hexBinary *a)
{
	a->__size = 0;
	a->__ptr = NULL;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_xsd__hexBinary(struct soap *soap, const struct xsd__hexBinary *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, (struct soap_array*)&a->__ptr, 1, tag, SOAP_TYPE_xsd__hexBinary);
	if (soap_out_xsd__hexBinary(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_xsd__hexBinary(struct soap *soap, const char *tag, int id, const struct xsd__hexBinary *a, const char *type)
{
	id = soap_element_id(soap, tag, id, a, (struct soap_array*)&a->__ptr, 1, type, SOAP_TYPE_xsd__hexBinary);
	if (id < 0)
		return soap->error;
	soap_element_begin_out(soap, tag, id, type);
	soap_puthex(soap, a->__ptr, a->__size);
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct xsd__hexBinary * SOAP_FMAC4 soap_get_xsd__hexBinary(struct soap *soap, struct xsd__hexBinary *p, const char *tag, const char *type)
{
	if ((p = soap_in_xsd__hexBinary(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct xsd__hexBinary * SOAP_FMAC4 soap_in_xsd__hexBinary(struct soap *soap, const char *tag, struct xsd__hexBinary *a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type) && soap_match_tag(soap, soap->type, ":hexBinary"))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct xsd__hexBinary *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_xsd__hexBinary, sizeof(struct xsd__hexBinary), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_xsd__hexBinary(soap, a);
	if (soap->body && !*soap->href)
	{
		a->__ptr = soap_gethex(soap, &a->__size);
		if ((!a->__ptr && soap->error) || soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct xsd__hexBinary *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_xsd__hexBinary, 0, sizeof(struct xsd__hexBinary), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct xsd__hexBinary * SOAP_FMAC6 soap_new_xsd__hexBinary(struct soap *soap, int n)
{	return soap_instantiate_xsd__hexBinary(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_xsd__hexBinary(struct soap *soap, struct xsd__hexBinary *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct xsd__hexBinary * SOAP_FMAC4 soap_instantiate_xsd__hexBinary(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_xsd__hexBinary(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_xsd__hexBinary, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct xsd__hexBinary;
		if (size)
			*size = sizeof(struct xsd__hexBinary);
	}
	else
	{	cp->ptr = (void*)new struct xsd__hexBinary[n];
		if (size)
			*size = n * sizeof(struct xsd__hexBinary);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct xsd__hexBinary*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_xsd__hexBinary(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct xsd__hexBinary %p -> %p\n", q, p));
	*(struct xsd__hexBinary*)p = *(struct xsd__hexBinary*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoDate(struct soap *soap, const struct ns__echoDate *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_xsd__dateTime(soap, &a->inputDate);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoDate(struct soap *soap, struct ns__echoDate *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_xsd__dateTime(soap, &a->inputDate);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoDate(struct soap *soap, const struct ns__echoDate *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoDate);
	if (soap_out_ns__echoDate(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoDate(struct soap *soap, const char *tag, int id, const struct ns__echoDate *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoDate), type);
	soap_out_xsd__dateTime(soap, "inputDate", -1, &a->inputDate, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoDate * SOAP_FMAC4 soap_get_ns__echoDate(struct soap *soap, struct ns__echoDate *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoDate(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoDate * SOAP_FMAC4 soap_in_ns__echoDate(struct soap *soap, const char *tag, struct ns__echoDate *a, const char *type)
{
	short soap_flag_inputDate = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoDate *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoDate, sizeof(struct ns__echoDate), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoDate(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_inputDate && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_in_xsd__dateTime(soap, "inputDate", &a->inputDate, "xsd:dateTime"))
				{	soap_flag_inputDate--;
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
	{	a = (struct ns__echoDate *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoDate, 0, sizeof(struct ns__echoDate), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoDate * SOAP_FMAC6 soap_new_ns__echoDate(struct soap *soap, int n)
{	return soap_instantiate_ns__echoDate(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoDate(struct soap *soap, struct ns__echoDate *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoDate * SOAP_FMAC4 soap_instantiate_ns__echoDate(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoDate(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoDate, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoDate;
		if (size)
			*size = sizeof(struct ns__echoDate);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoDate[n];
		if (size)
			*size = n * sizeof(struct ns__echoDate);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoDate*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoDate(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoDate %p -> %p\n", q, p));
	*(struct ns__echoDate*)p = *(struct ns__echoDate*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoDateResponse(struct soap *soap, const struct ns__echoDateResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_xsd__dateTime(soap, &a->_return);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoDateResponse(struct soap *soap, struct ns__echoDateResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_xsd__dateTime(soap, &a->_return);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoDateResponse(struct soap *soap, const struct ns__echoDateResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoDateResponse);
	if (soap_out_ns__echoDateResponse(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoDateResponse(struct soap *soap, const char *tag, int id, const struct ns__echoDateResponse *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoDateResponse), type);
	if (a->_return)
		soap_element_result(soap, "return");
	soap_out_xsd__dateTime(soap, "return", -1, &a->_return, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoDateResponse * SOAP_FMAC4 soap_get_ns__echoDateResponse(struct soap *soap, struct ns__echoDateResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoDateResponse(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoDateResponse * SOAP_FMAC4 soap_in_ns__echoDateResponse(struct soap *soap, const char *tag, struct ns__echoDateResponse *a, const char *type)
{
	short soap_flag__return = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoDateResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoDateResponse, sizeof(struct ns__echoDateResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoDateResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag__return && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_in_xsd__dateTime(soap, NULL, &a->_return, "xsd:dateTime"))
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
	{	a = (struct ns__echoDateResponse *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoDateResponse, 0, sizeof(struct ns__echoDateResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoDateResponse * SOAP_FMAC6 soap_new_ns__echoDateResponse(struct soap *soap, int n)
{	return soap_instantiate_ns__echoDateResponse(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoDateResponse(struct soap *soap, struct ns__echoDateResponse *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoDateResponse * SOAP_FMAC4 soap_instantiate_ns__echoDateResponse(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoDateResponse(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoDateResponse, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoDateResponse;
		if (size)
			*size = sizeof(struct ns__echoDateResponse);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoDateResponse[n];
		if (size)
			*size = n * sizeof(struct ns__echoDateResponse);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoDateResponse*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoDateResponse(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoDateResponse %p -> %p\n", q, p));
	*(struct ns__echoDateResponse*)p = *(struct ns__echoDateResponse*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoBase64(struct soap *soap, const struct ns__echoBase64 *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_xsd__base64Binary(soap, &a->inputBase64);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoBase64(struct soap *soap, struct ns__echoBase64 *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_xsd__base64Binary(soap, &a->inputBase64);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoBase64(struct soap *soap, const struct ns__echoBase64 *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoBase64);
	if (soap_out_ns__echoBase64(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoBase64(struct soap *soap, const char *tag, int id, const struct ns__echoBase64 *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoBase64), type);
	soap_out_xsd__base64Binary(soap, "inputBase64", -1, &a->inputBase64, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoBase64 * SOAP_FMAC4 soap_get_ns__echoBase64(struct soap *soap, struct ns__echoBase64 *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoBase64(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoBase64 * SOAP_FMAC4 soap_in_ns__echoBase64(struct soap *soap, const char *tag, struct ns__echoBase64 *a, const char *type)
{
	short soap_flag_inputBase64 = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoBase64 *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoBase64, sizeof(struct ns__echoBase64), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoBase64(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_inputBase64 && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_xsd__base64Binary(soap, "inputBase64", &a->inputBase64, "xsd:base64Binary"))
				{	soap_flag_inputBase64--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_inputBase64 > 0))
		{	soap->error = SOAP_OCCURS;
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct ns__echoBase64 *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoBase64, 0, sizeof(struct ns__echoBase64), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoBase64 * SOAP_FMAC6 soap_new_ns__echoBase64(struct soap *soap, int n)
{	return soap_instantiate_ns__echoBase64(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoBase64(struct soap *soap, struct ns__echoBase64 *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoBase64 * SOAP_FMAC4 soap_instantiate_ns__echoBase64(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoBase64(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoBase64, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoBase64;
		if (size)
			*size = sizeof(struct ns__echoBase64);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoBase64[n];
		if (size)
			*size = n * sizeof(struct ns__echoBase64);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoBase64*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoBase64(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoBase64 %p -> %p\n", q, p));
	*(struct ns__echoBase64*)p = *(struct ns__echoBase64*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoBase64Response(struct soap *soap, const struct ns__echoBase64Response *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_xsd__base64Binary(soap, &a->_return);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoBase64Response(struct soap *soap, struct ns__echoBase64Response *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_xsd__base64Binary(soap, &a->_return);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoBase64Response(struct soap *soap, const struct ns__echoBase64Response *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoBase64Response);
	if (soap_out_ns__echoBase64Response(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoBase64Response(struct soap *soap, const char *tag, int id, const struct ns__echoBase64Response *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoBase64Response), type);
	soap_element_result(soap, "return");
	soap_out_xsd__base64Binary(soap, "return", -1, &a->_return, "");
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoBase64Response * SOAP_FMAC4 soap_get_ns__echoBase64Response(struct soap *soap, struct ns__echoBase64Response *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoBase64Response(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoBase64Response * SOAP_FMAC4 soap_in_ns__echoBase64Response(struct soap *soap, const char *tag, struct ns__echoBase64Response *a, const char *type)
{
	short soap_flag__return = 1;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoBase64Response *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoBase64Response, sizeof(struct ns__echoBase64Response), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoBase64Response(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag__return && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_xsd__base64Binary(soap, NULL, &a->_return, "xsd:base64Binary"))
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
	{	a = (struct ns__echoBase64Response *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoBase64Response, 0, sizeof(struct ns__echoBase64Response), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoBase64Response * SOAP_FMAC6 soap_new_ns__echoBase64Response(struct soap *soap, int n)
{	return soap_instantiate_ns__echoBase64Response(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoBase64Response(struct soap *soap, struct ns__echoBase64Response *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoBase64Response * SOAP_FMAC4 soap_instantiate_ns__echoBase64Response(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoBase64Response(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoBase64Response, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoBase64Response;
		if (size)
			*size = sizeof(struct ns__echoBase64Response);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoBase64Response[n];
		if (size)
			*size = n * sizeof(struct ns__echoBase64Response);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoBase64Response*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoBase64Response(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoBase64Response %p -> %p\n", q, p));
	*(struct ns__echoBase64Response*)p = *(struct ns__echoBase64Response*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_xsd__base64Binary(struct soap *soap, struct xsd__base64Binary const*a)
{
	if (a->__ptr)
		soap_array_reference(soap, a, (struct soap_array*)&a->__ptr, 1, SOAP_TYPE_xsd__base64Binary);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_xsd__base64Binary(struct soap *soap, struct xsd__base64Binary *a)
{
	a->__size = 0;
	a->__ptr = NULL;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_xsd__base64Binary(struct soap *soap, const struct xsd__base64Binary *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, (struct soap_array*)&a->__ptr, 1, tag, SOAP_TYPE_xsd__base64Binary);
	if (soap_out_xsd__base64Binary(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_xsd__base64Binary(struct soap *soap, const char *tag, int id, const struct xsd__base64Binary *a, const char *type)
{
	id = soap_element_id(soap, tag, id, a, (struct soap_array*)&a->__ptr, 1, type, SOAP_TYPE_xsd__base64Binary);
	if (id < 0)
		return soap->error;
	soap_element_begin_out(soap, tag, id, type);
	soap_putbase64(soap, a->__ptr, a->__size);
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct xsd__base64Binary * SOAP_FMAC4 soap_get_xsd__base64Binary(struct soap *soap, struct xsd__base64Binary *p, const char *tag, const char *type)
{
	if ((p = soap_in_xsd__base64Binary(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct xsd__base64Binary * SOAP_FMAC4 soap_in_xsd__base64Binary(struct soap *soap, const char *tag, struct xsd__base64Binary *a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type) && soap_match_tag(soap, soap->type, ":base64Binary") && soap_match_tag(soap, soap->type, ":base64"))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct xsd__base64Binary *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_xsd__base64Binary, sizeof(struct xsd__base64Binary), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_xsd__base64Binary(soap, a);
	if (soap->body && !*soap->href)
	{
		a->__ptr = soap_getbase64(soap, &a->__size, 0);
		if ((!a->__ptr && soap->error) || soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct xsd__base64Binary *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_xsd__base64Binary, 0, sizeof(struct xsd__base64Binary), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct xsd__base64Binary * SOAP_FMAC6 soap_new_xsd__base64Binary(struct soap *soap, int n)
{	return soap_instantiate_xsd__base64Binary(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_xsd__base64Binary(struct soap *soap, struct xsd__base64Binary *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct xsd__base64Binary * SOAP_FMAC4 soap_instantiate_xsd__base64Binary(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_xsd__base64Binary(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_xsd__base64Binary, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct xsd__base64Binary;
		if (size)
			*size = sizeof(struct xsd__base64Binary);
	}
	else
	{	cp->ptr = (void*)new struct xsd__base64Binary[n];
		if (size)
			*size = n * sizeof(struct xsd__base64Binary);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct xsd__base64Binary*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_xsd__base64Binary(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct xsd__base64Binary %p -> %p\n", q, p));
	*(struct xsd__base64Binary*)p = *(struct xsd__base64Binary*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoVoid(struct soap *soap, const struct ns__echoVoid *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoVoid(struct soap *soap, struct ns__echoVoid *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoVoid(struct soap *soap, const struct ns__echoVoid *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoVoid);
	if (soap_out_ns__echoVoid(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoVoid(struct soap *soap, const char *tag, int id, const struct ns__echoVoid *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoVoid), type);
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoVoid * SOAP_FMAC4 soap_get_ns__echoVoid(struct soap *soap, struct ns__echoVoid *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoVoid(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoVoid * SOAP_FMAC4 soap_in_ns__echoVoid(struct soap *soap, const char *tag, struct ns__echoVoid *a, const char *type)
{;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoVoid *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoVoid, sizeof(struct ns__echoVoid), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoVoid(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
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
	{	a = (struct ns__echoVoid *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoVoid, 0, sizeof(struct ns__echoVoid), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoVoid * SOAP_FMAC6 soap_new_ns__echoVoid(struct soap *soap, int n)
{	return soap_instantiate_ns__echoVoid(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoVoid(struct soap *soap, struct ns__echoVoid *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoVoid * SOAP_FMAC4 soap_instantiate_ns__echoVoid(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoVoid(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoVoid, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoVoid;
		if (size)
			*size = sizeof(struct ns__echoVoid);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoVoid[n];
		if (size)
			*size = n * sizeof(struct ns__echoVoid);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoVoid*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoVoid(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoVoid %p -> %p\n", q, p));
	*(struct ns__echoVoid*)p = *(struct ns__echoVoid*)q;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__echoVoidResponse(struct soap *soap, const struct ns__echoVoidResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__echoVoidResponse(struct soap *soap, struct ns__echoVoidResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__echoVoidResponse(struct soap *soap, const struct ns__echoVoidResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns__echoVoidResponse);
	if (soap_out_ns__echoVoidResponse(soap, tag, id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__echoVoidResponse(struct soap *soap, const char *tag, int id, const struct ns__echoVoidResponse *a, const char *type)
{
	soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns__echoVoidResponse), type);
	soap_element_end_out(soap, tag);
	return SOAP_OK;
}

SOAP_FMAC3 struct ns__echoVoidResponse * SOAP_FMAC4 soap_get_ns__echoVoidResponse(struct soap *soap, struct ns__echoVoidResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in_ns__echoVoidResponse(soap, tag, p, type)))
		soap_getindependent(soap);
	return p;
}

SOAP_FMAC3 struct ns__echoVoidResponse * SOAP_FMAC4 soap_in_ns__echoVoidResponse(struct soap *soap, const char *tag, struct ns__echoVoidResponse *a, const char *type)
{;
	if (soap_element_begin_in(soap, tag, 0))
		return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type))
	{	soap->error = SOAP_TYPE;
		return NULL;
	}
	a = (struct ns__echoVoidResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns__echoVoidResponse, sizeof(struct ns__echoVoidResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_ns__echoVoidResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
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
	{	a = (struct ns__echoVoidResponse *)soap_id_forward(soap, soap->href, (void**)a, 0, SOAP_TYPE_ns__echoVoidResponse, 0, sizeof(struct ns__echoVoidResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC5 struct ns__echoVoidResponse * SOAP_FMAC6 soap_new_ns__echoVoidResponse(struct soap *soap, int n)
{	return soap_instantiate_ns__echoVoidResponse(soap, n, NULL, NULL, NULL);
}

SOAP_FMAC5 void SOAP_FMAC6 soap_delete_ns__echoVoidResponse(struct soap *soap, struct ns__echoVoidResponse *p)
{	soap_delete(soap, p);
}

SOAP_FMAC3 struct ns__echoVoidResponse * SOAP_FMAC4 soap_instantiate_ns__echoVoidResponse(struct soap *soap, int n, const char *type, const char *arrayType, size_t *size)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_instantiate_ns__echoVoidResponse(%d, %s, %s)\n", n, type?type:"", arrayType?arrayType:""));
	struct soap_clist *cp = soap_link(soap, NULL, SOAP_TYPE_ns__echoVoidResponse, n, soap_fdelete);
	if (!cp)
		return NULL;
	if (n < 0)
	{	cp->ptr = (void*)new struct ns__echoVoidResponse;
		if (size)
			*size = sizeof(struct ns__echoVoidResponse);
	}
	else
	{	cp->ptr = (void*)new struct ns__echoVoidResponse[n];
		if (size)
			*size = n * sizeof(struct ns__echoVoidResponse);
	}
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Instantiated location=%p\n", cp->ptr));
	return (struct ns__echoVoidResponse*)cp->ptr;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns__echoVoidResponse(struct soap *soap, int st, int tt, void *p, size_t len, const void *q, size_t n)
{
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copying struct ns__echoVoidResponse %p -> %p\n", q, p));
	*(struct ns__echoVoidResponse*)p = *(struct ns__echoVoidResponse*)q;
}


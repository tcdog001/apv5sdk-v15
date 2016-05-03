#include "soapH.h" 

extern int getQuote (char *symbol, char *Result);

SOAP_NMAC struct Namespace namespaces[] =
{
  {"SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/", "http://www.w3.org/*/soap-envelope"},
  {"SOAP-ENC", "http://schemas.xmlsoap.org/soap/encoding/", "http://www.w3.org/*/soap-encoding"},
  {"xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance"},
  {"xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema"},
  {"ns", "urn:xmethods-delayed-quotes"},
  {NULL, NULL}
};

void StdEtoA(char *text);

int getQuote (char *symbol, char *Result) {

	FlpCompDouble d;
	struct soap *soap;
	float result;
	int rc;
    char *addr = "http://services.xmethods.net/soap/";

	soap = soap_new();
	soap->namespaces = (struct Namespace *)namespaces;
	rc = soap_call_ns__getQuote(soap, addr, "", (char*)symbol, &result);
  
	if (rc==SOAP_OK) {
		d.d= result;
		FlpFToA (d.fd,Result);
		StdEtoA(Result);
	} else {
		soap_set_fault(soap);
		strcpy (Result, "Not connected");
	}
	soap_end(soap);
	free(soap);
	
	return rc;

}

void StdEtoA(char *text) {
	  int i;
	  int len = strlen(text);
	  char *ptr=text+(len-2);
	  int digits=atoi(ptr);
	  ptr=text+1;
	  for (i=0; i<digits; i++)
	  {
	    *ptr = *(ptr+1);
	    ptr++;
	  }
	  *ptr='.';
	  *(ptr+3)=0;
  
}   
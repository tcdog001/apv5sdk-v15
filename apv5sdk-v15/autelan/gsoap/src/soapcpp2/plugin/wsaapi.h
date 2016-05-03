/*

wsaapi.h

WS-Addressing plugin

gSOAP XML Web services tools
Copyright (C) 2000-2006, Robert van Engelen, Genivia Inc., All Rights Reserved.
This part of the software is released under one of the following licenses:
GPL, the gSOAP public license, or Genivia's license for commercial use.
--------------------------------------------------------------------------------
gSOAP public license.

The contents of this file are subject to the gSOAP Public License Version 1.3
(the "License"); you may not use this file except in compliance with the
License. You may obtain a copy of the License at
http://www.cs.fsu.edu/~engelen/soaplicense.html
Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
for the specific language governing rights and limitations under the License.

The Initial Developer of the Original Code is Robert A. van Engelen.
Copyright (C) 2000-2006, Robert van Engelen, Genivia Inc., All Rights Reserved.
--------------------------------------------------------------------------------
GPL license.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place, Suite 330, Boston, MA 02111-1307 USA

Author contact information:
engelen@genivia.com / engelen@acm.org
--------------------------------------------------------------------------------
A commercial use license is available from Genivia, Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

#include "soapH.h"

/** plugin identification for plugin registry */
#define SOAP_WSA_ID "SOAP-WSA-1.0"

/** plugin identification for plugin registry */
extern const char soap_wsa_id[];

/**
@struct soap_wsa_data
@brief plugin data to override callbacks
*/
struct soap_wsa_data
{ /** fheader callback is invoked immediately after parsing a SOAP Header */
  int (*fheader)(struct soap*);
  /** fseterror callback is used to inspect and change gSOAP error codes */
  void (*fseterror)(struct soap*, const char**, const char**);
  /** fresponse callback is used to change a HTTP response into a HTTP POST */
  int (*fresponse)(struct soap*, int, size_t);
};

int soap_wsa(struct soap *soap, struct soap_plugin *p, void *arg);

int soap_wsa_request(struct soap *soap, const char *id, const char *to, const char *action);
int soap_wsa_add_From(struct soap *soap, const char *endpoint);
int soap_wsa_add_ReplyTo(struct soap *soap, const char *endpoint);
int soap_wsa_add_FaultTo(struct soap *soap, const char *endpoint);

int soap_wsa_check(struct soap *soap);
int soap_wsa_reply(struct soap *soap, const char *id, const char *action);
int soap_wsa_sender_fault_subcode(struct soap *soap, const char *faultsubcode, const char *faultstring, const char *faultdetail);
int soap_wsa_receiver_fault_subcode(struct soap *soap, const char *faultsubcode, const char *faultstring, const char *faultdetail);
int soap_wsa_sender_fault(struct soap *soap, const char *faultstring, const char *faultdetail);
int soap_wsa_receiver_fault(struct soap *soap, const char *faultstring, const char *faultdetail);
int soap_wsa_error(struct soap *soap, wsa__FaultSubcodeValues fault);

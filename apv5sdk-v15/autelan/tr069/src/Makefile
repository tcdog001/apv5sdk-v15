#hacks required in gsoap to compile cleanly
#soapcpp2/src/Makefile.am 
#+ CC=$(HOST_CC)
#configure.in 
#- AC_FUNC_MALLOC


CFLAGS = -g -Wall -DDEBUG 
#debug flag removed by czm
PKGCONFIG = pkg-config
BUILD_USR_DIR = usr/include
GSOAP_DIR = gsoap
#TODO use pkgconfig & gsoap instead of static definitions
#SOAP_CFLAGS = `$(PKGCONFIG) gsoap --cflags`
#SOAP_LDFLAGS = `$(PKGCONFIG) gsoap --libs`
SOAP_CFLAGS = -DWITH_COOKIES -DWITH_DOM -DWITH_OPENSSL -I$(STAGING_DIR)/$(BUILD_USR_DIR) -I$(STAGING_DIR)/$(BUILD_USR_DIR)/$(GSOAP_DIR)
SOAP_CPPFLAGS = -I$(STAGING_DIR)/$(BUILD_USR_DIR)/$(GSOAP_DIR)/import 
#TODO add curl cflags and ldflags
CURL_CFLAGS = 
CURL_LDFLAGS = -L/$(STAGING_DIR)/usr/lib -lcurl -lgssapi 
CPPFLAGS = $(SOAP_CFLAGS)  $(SOAP_CPPFLAGS) 
LDFLAGS = $(SOAP_LDFLAGS) -lpthread
CC = $(TARGET_CC) 
LD = $(TARGET_LD)

#gsoap
SOAPCPP = $(STAGING_DIR)/usr/bin/soapcpp2
STUB = Stub
STUB_FILES = $(STUB)*
WSDL_FILES = cwmp.*.xml cwmp.wsdl cwmp.xsd
STATIC_LIB = -L$(STAGING_DIR)/usr/lib -lgsoapck -lcurl -lssl -lz -lcrypto -lapfunc
#-lgsoapck 

EXE=$(DM_EXE) $(CWMP_EXE) $(LMGR_EXE) $(DLMGR_EXE)
OBJ=$(COMMON_OBJ) $(DM_OBJ) $(CWMP_OBJ) $(LMGR_OBJ) $(DLMGR_OBJ)

VPATH=$(BUILD_DIR)/gsoap-2.7/soapcpp2

COMMON_OBJ = $(STUB)C.o tools.o FileConf.o messages.o TreeFunctions.o Manager.o Fault.o ACSInterface.o dom.o md5evp.o threads.o httpda.o

all: cwmp-rules datamodel-rules localmanager-rules downloadmanager-rules

gsoap: $(SOAPCPP)

stub: $(STUB)C.o

$(STUB)C.c: struct.h
	$(SOAPCPP) -n -c -p$(STUB) $(SOAP_CPPFLAGS) $<

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $< 

#datamodel
DM_CFLAGS=-DDEBUG_LEVEL=2
DM_OBJ=DataModel.o
DM_EXE=datamodel

datamodel-rules: gsoap $(DM_EXE)

$(DM_EXE): $(DM_OBJ) $(COMMON_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(STATIC_LIB)

$(DM_OBJ): $(subst .o,.c,$(DM_OBJ))
	$(CC) $(CFLAGS) $(CPPFLAGS) $(DM_CFLAGS) -c -o $@ $< 

#cwmp
CWMP_CFLAGS=-DDEBUG_LEVEL=3
CWMP_OBJ=cwmp.o
CWMP_EXE=cwmp

cwmp-rules: gsoap stub $(CWMP_EXE)

$(CWMP_EXE): $(CWMP_OBJ) $(COMMON_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(STATIC_LIB)

$(CWMP_OBJ): $(subst .o,.c,$(CWMP_OBJ))
	$(CC) $(CFLAGS) $(CPPFLAGS) $(CWMP_CFLAGS) -c -o $@ $< 

#localmanager
LMGR_CFLAGS=-DDEBUG_LEVEL=2
LMGR_OBJ=LocalManager.o 
LMGR_EXE=LocalManager

localmanager-rules: gsoap $(LMGR_EXE)

$(LMGR_EXE): $(LMGR_OBJ) $(COMMON_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(STATIC_LIB)

$(LMGR_OBJ): $(subst .o,.c,$(LMGR_OBJ))
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LMGR_CFLAGS) -c -o $@ $< 

#downloadmanager
DLMGR_CFLAGS=-DDEBUG_LEVEL=2
DLMGR_OBJ=DownloadManager.o
DLMGR_EXE=DownloadManager

downloadmanager-rules: gsoap $(DLMGR_EXE)

$(DLMGR_EXE): $(DLMGR_OBJ) $(COMMON_OBJ)
	$(CC) $(LDFLAGS) $(CURL_LDFLAGS) -o $@ $^ $(STATIC_LIB)

$(DLMGR_OBJ): $(subst .o,.c,$(DLMGR_OBJ))
	$(CC) $(CFLAGS) $(CPPFLAGS) $(CURL_CFLAGS) $(DLMGR_CFLAGS) -c -o $@ $< 


clean:
	-rm -f $(OBJ) $(EXE) $(STUB_FILES) $(WSDL_FILES)

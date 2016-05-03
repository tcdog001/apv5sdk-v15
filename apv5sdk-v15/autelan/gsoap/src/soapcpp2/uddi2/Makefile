.SUFFIXES: .wsdl .xsd
WSDL2H=../wsdl2h
SOAPCPP=../soapcpp2
CC=gcc
CPP=g++
LIBS=
#COFLAGS=-g -DDEBUG -fstack-check
COFLAGS=-O1
CWFLAGS=-Wall
CIFLAGS=-I..
CMFLAGS=-DWITH_NONAMESPACES
CFLAGS= $(CWFLAGS) $(COFLAGS) $(CIFLAGS) $(CMFLAGS)
all:			example1 example2
example1:		example1.cpp inqClientLib.o inquire_v2.o stdsoap2.o envC.o
			$(CPP) $(CFLAGS) -o example1 example1.cpp inqClientLib.o inquire_v2.o stdsoap2.o envC.o
example2:		example2.cpp inqClientLib.o inquire_v2.o stdsoap2.o envC.o
			$(CPP) $(CFLAGS) -o example2 example2.cpp inqClientLib.o inquire_v2.o stdsoap2.o envC.o
example3:		example3.cpp pubClientLib.o publish_v2.o ../stdsoap2.cpp envC.o
			$(CPP) $(CFLAGS) -DWITH_OPENSSL -o example3 example3.cpp pubClientLib.o publish_v2.o ../stdsoap2.cpp envC.o -lssl -lcrypto
uddiClientLib.cpp:	uddi_v2.h inquire_v2.h publish_v2.h
			$(SOAPCPP) -I.. -p uddi uddi_v2.h
			$(SOAPCPP) -I.. -p inq inquire_v2.h
			$(SOAPCPP) -I.. -p pub publish_v2.h
inqClientLib.cpp:	inquire_v2.h
			$(SOAPCPP) -I.. -p inq inquire_v2.h
pubClientLib.cpp:	publish_v2.h
			$(SOAPCPP) -I.. -p pub publish_v2.h
envC.cpp:
			echo '' > env.h
			$(SOAPCPP) -I.. -p env env.h
stdsoap2.o:		../stdsoap2.cpp
			$(CPP) $(CFLAGS) -c $<
.c.o:
			$(CPP) $(CFLAGS) -c $<
.cpp.o:
			$(CPP) $(CFLAGS) -c $<
uddi_v2.h:		inquire_v2.wsdl publish_v2.wsdl uddi_v2.xsd uddi2-typemap.dat
			$(WSDL2H) -tuddi2-typemap.dat -ouddi_v2.h inquire_v2.wsdl publish_v2.wsdl
inquire_v2.h:		inquire_v2.wsdl uddi_v2.xsd uddi2-typemap.dat
			$(WSDL2H) -tuddi2-typemap.dat inquire_v2.wsdl
publish_v2.h:		publish_v2.wsdl uddi_v2.xsd uddi2-typemap.dat
			$(WSDL2H) -tuddi2-typemap.dat publish_v2.wsdl
clean:		
			rm -f *.o *.log *.xml *.nsmap *Stub.h *Proxy.h *Object.h inqC*.cpp inqS*.cpp pubC*.cpp pubS*.cpp uddiC*.cpp uddiS*.cpp env*.h env*.cpp
distclean:		
			rm -f *.o *.log *.xml *.nsmap *Stub.h *Proxy.h *Object.h inqC*.cpp inqS*.cpp pubC*.cpp pubS*.cpp uddiC*.cpp uddiS*.cpp env*.h env*.cpp
			rm -f example1 example2 example3

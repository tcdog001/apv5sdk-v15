This directory contains Palm OS support.

Two example applications are included in the 'interop' and 'stockquote'
directories. MetroWerks CodeWarrior project files are included.

The 'interop' example performs interop checks with SOAP RPC 1.1 round 2 A/B/C
tests against interop servers. (Note: the debug mode reports a memory leak,
which is in the displayText() function for the GUI and not harmful.)

The 'stockquote' example reports the value of a stock (ticker symbol).

The stdsoap2.c/.cpp runtime library fits in a 64K segment. Some functionality
is not available (implicitly disabled with the WITH_LEAN compiler option): no
HTTP keep-alive, no HTTP cookies, no HTTP authentication, no message logging,
limited error diagnostics, no send/recv timeouts, and no time_t, unsigned
short, or Int64 serialization. To make the footprint as small as possible,
compile the sources with the /DWITH_LEANER compile option. This disables MIME
and DIME support.

To help develop multi-segmented applications, the stdsoap2.c/.cpp is split in
two parts: palmsoap1.c/.cpp and palmsoap2.c/.cpp. You should use these instead
of stdsoap2.c/.cpp.

To develop an application from a WSDL, run wsdl2h.exe (or a wsdl2h executable
for any other platform) on the WSDL file. Mind the command line options. For
example, wsdl2h.exe -c generates C code and wsdl2h.exe -s generates C++ code
without requiring STL. This command generated a C or C++ header file.

To generate serializers and stub routines, run soapcpp2.exe (or a soapcpp2
executable for any other platform) on the header file. The soapcpp2.exe
generates platform-independent code, including:
soapH.h		header file for serializers
soapC.cpp	serializers
soapStub.h	header file for stubs
soapClient.cpp	stubs

The 'interop' and 'stockquote' examples already included these files.

Because the generated code is platform-independent, it is strongly advised to
try to build a simple test application on a non-Palm platform first to verify
interoperability and data exchange. Because logging is disabled on Palm, it
will be hard to find the source of an interop problem. After testing the
application, you can use the same sources to build a Palm OS application.


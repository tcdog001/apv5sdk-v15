This directory contains custom serializers for common data types.

Custom serializers replace the soapcpp2-generated serialization routines.
See the notes in the source files on specific usage.

The following serializers are available:

long_double	Serializes long double (extended double) type
struct_tm.*	Serializes <time.h> struct tm

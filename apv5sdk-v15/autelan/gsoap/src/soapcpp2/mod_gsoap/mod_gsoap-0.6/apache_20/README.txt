apache_gsoap.h			mod_gsoap module for Apache v2.0
mod_gsoap.c			mod_gsoap module for Apache v2.0
mod_gsoap.vcproj		MSVC project


Here's how to compile mod_gsoap for apache 2:

/usr/local/apache2/bin/apxs -a -i -c -I/path/to/gsoap/installation mod_gsoap.c


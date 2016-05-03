SUBDIRS = talk 

all: clean
	for dir in $(SUBDIRS) ; do (cd $$dir && $(MAKE) all) || exit 1; done

clean:
	rm -rf *.a *.so *.cgi wapitalk

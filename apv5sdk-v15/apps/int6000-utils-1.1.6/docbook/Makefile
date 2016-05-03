#!/bin/make -f
# file: docbook/Makefile

# ====================================================================
# environment definitions;
# --------------------------------------------------------------------

include ../make.def

DOCBOOK=/usr/share/xml/docbook/stylesheet/nwalsh/html

# ====================================================================
# symbols;
# --------------------------------------------------------------------

TRASH=*~ *.[0-9][0-9][0-9] t t.* *.1.html *.c.html *.cpp.html *.h.html *.hpp.html *.7.html *.7a.html *.sh.html *.xml.html *.xsd.html ch[0-9][0-9]*.html *.pdf 
FILES=Makefile *.txt *.xml *.xsl
STYLES=param.xsl lists.xsl synop.xsl
PAGES=int6000-utils.xml overview.xml hardware.xml software.xml firmware.xml procedures.xml support.xml scripting.xml interface.xml embedding.xml security.xml library.xml management.xml

IMAGES=BootFromHost.png SimpleNetwork.png UpdateLocalHost.png UpgradeDevice.png
BOOKS=index.html int6000-utils.pdf int6000-utils.txt
BOOKS=index.html 

# ====================================================================
# targets;
# --------------------------------------------------------------------

.PHONY: all compile scripts manuals install uninstall clean fresh 

all compile: setup ${BOOKS} 
	change *.html -e "</html>0" -t "</html>"
	rm *.[0-9][0-9][0-9]
manuals: 
	manuals
scripts:
library:
	sudo install -m 0755 -o ${OWNER} -g ${GROUP} -d ${DOC}
install:  compile manuals
#	sudo if [ -d ${DOC} ]; then install -m 0664 -o ${OWNER} -g ${GROUP} *.html *.css *.png *.pdf ${DOC}; fi
	sudo if [ -d ${DOC} ]; then install -m 0664 -o ${OWNER} -g ${GROUP} *.html *.css *.png ${DOC}; fi
	sudo if [ -d ${DOC} ]; then install -m 0664 -o ${OWNER} -g ${GROUP} piboffset.xsd piboffset.xml ${DOC}; fi
uninstall:
	sudo rm -f ${DOC}/*.c.html ${DOC}/*.h.html ${DOC}/*.7.html
setup:
	sudo install ${STYLES} ${DOCBOOK}
clean:
	chmod 0755 ${PAGES} ${STYLES}
	rm -f ${TRASH} ${BOOKS}
fresh: clean compile

# ====================================================================
# targets;
# --------------------------------------------------------------------

index.html: ${PAGES}
	xmlto -o . html ${<} 
int6000-utils.pdf: ${PAGES} ${IMAGES}
	docbook2pdf ${<}
int6000-utils.txt: ${PAGES} 
	xmlto txt ${<}


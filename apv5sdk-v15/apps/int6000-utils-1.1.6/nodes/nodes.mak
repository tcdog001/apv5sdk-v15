# ====================================================================
# files;
# --------------------------------------------------------------------

reorder.o: reorder.c nodes.h
xmlattribute.o: xmlattribute.c nodes.h
xmlcontent.o: xmlcontent.c nodes.h
xmledit.o: xmledit.c nodes.h types.h error.h number.h memory.h
xmlfree.o: xmlfree.c nodes.h
xmlnode.o: xmlnode.c nodes.h types.h
xmlopen.o: xmlopen.c nodes.h types.h error.h 
xmlscan.o: xmlscan.c nodes.h types.h error.h number.h
xmlvalue.o: xmlvalue.c nodes.h
xmlschema.o: xmlschema.c nodes.h format.h 

# ====================================================================
# files;
# --------------------------------------------------------------------

nodes.h:
	touch ../nodes/${@}


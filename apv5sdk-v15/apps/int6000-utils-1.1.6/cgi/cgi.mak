# file: cgi/cgi.mak

# ====================================================================
# 
# --------------------------------------------------------------------

intellon.o: intellon.c page.h 
session.o: session.c session.h
echo.o: echo.c page.h session.h number.h 
DivisionHeader.o: DivisionHeader.c page.h 
DivisionOption.o: DivisionOption.c page.h session.h
DivisionDevice.o: DivisionDevice.c channel.h homeplug.h intellon.h memory.h page.h types.h version.h
DivisionFooter.o: DivisionFooter.c page.h version.h
EthernetHeader.o: EthernetHeader.c homeplug.h
IntellonHeader.o: IntellonHeader.c homeplug.h
SessionEdit.o: SessionEdit.c session.h scan.h error.h types.h int6k.h 
SessionLoad.o: SessionLoad.c session.h files.h int6k.h 
SessionPeek.o: SessionPeek.c page.h session.h memory.h types.h int6k.h 
SessionSave.o: SessionSave.c session.h files.h int6k.h 
SessionTask.o: SessionTask.c page.h session.h error.h
OptionTopology.o: OptionTopology.c int6k.h channel.h memory.h error.h page.h 
OptionFirmware.o: OptionFirmware.c page.h session.h 
OptionIdentity.o: OptionIdentity.c page.h session.h int6k.h memory.h 
OptionPriority.o: OptionPriority.c page.h session.h 
OptionSecurity.o: OptionSecurity.c page.h version.h types.h
CGILocalPLD.o: CGILocalPLD.c channel.h session.h types.h int6k.h HPAVKey.h homeplug.h intellon.h chipset.h nvm.h
CGIFlashMOD.o: CGIFlashMOD.c int6k.h session.h error.h memory.h homeplug.h intellon.h chipset.h types.h HPAVKey.h channel.h nvm.h
CGIWriteMOD.o: CGIWriteMOD.c int6k.h channel.h ether.h memory.h error.h session.h homeplug.h intellon.h types.h HPAVKey.h nvm.h
CGISetKey.o: CGISetKey.c channel.h HPAVKey.h session.h types.h int6k.h homeplug.h intellon.h nvm.h

# ====================================================================
# files;
# --------------------------------------------------------------------

page.h: types.h
	touch ../cgi/${@}
session.h: int6k.h types.h HPAVKey.h 
	touch ../cgi/${@}


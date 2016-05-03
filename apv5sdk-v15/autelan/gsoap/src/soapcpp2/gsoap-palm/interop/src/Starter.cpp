//===================================================================================
//
//    (C) COPYRIGHT International Business Machines Corp., 2002 All Rights Reserved
//    Licensed Materials - Property of IBM
//    US Government Users Restricted Rights - Use, duplication or
//    disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
//
//    IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
//    ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//    PURPOSE. IN NO EVENT SHALL IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR
//    CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
//    USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
//    OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE
//    OR PERFORMANCE OF THIS SOFTWARE.
//
//    The program may be used, executed, copied, modified, and distributed
//    without royalty for the purpose of developing, using, marketing, or distributing.
// 
//=======================================================================================
/******************************************************************************
 *
 * Copyright (c) 1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: Starter.cpp
 *
 *****************************************************************************/
#include <PalmOS.h>
#include "StarterRsc.h"
#include <unix_string.h>
#include <unix_stdlib.h>

//#include "interop2H.h"    // include the generated proxy
//#include "interop2BH.h"    // include the generated proxy
//#include "interop2CH.h"    // include the generated proxy

extern "C" int interoptA(const char *site);
extern "C" int interopB(const char *site);
extern "C" int interopC(const char *site);
extern "C" void displayText(char *text);
void initURLList();
char *getSite(char type);

/***********************************************************************
 *
 *	Entry Points
 *
 ***********************************************************************/


/***********************************************************************
 *
 *	Internal Structures
 *
 ***********************************************************************/
typedef struct 
	{
	UInt8 replaceme;
	} StarterPreferenceType;

typedef struct 
	{
	UInt8 replaceme;
	} StarterAppInfoType;

typedef StarterAppInfoType* StarterAppInfoPtr;


/***********************************************************************
 *
 *	Global variables
 *
 ***********************************************************************/
//static Boolean HideSecretRecords;

char **resultText ;
int resultLines=0;

#define URL_COUNT 6
 char * tryURLsA[]={
   "http://www.cs.fsu.edu/~engelen/interop2.cgi",
   "http://services.soaplite.com/interop.cgi",
   "http://interop.capeclear.com/ccx/soapbuilders-round2",
	"http://nagoya.apache.org:5049/axis/services/echoA",
	"http://easysoap.sourceforge.net/cgi-bin/interopserver",
	"http://soapinterop.simdb.com/round2",
 };

 char * tryURLsB[]={
   "http://www.cs.fsu.edu/~engelen/interop2B.cgi",
   "http://services.soaplite.com/interop.cgi",
   "http://interop.capeclear.com/ccx/soapbuilders-round2",
   "http://nagoya.apache.org:5049/axis/services/echoB",
	"http://easysoap.sourceforge.net/cgi-bin/interopserver",
	"http://soapinterop.simdb.com/round2B",
 };

 char * tryURLsC[]={
   "http://www.cs.fsu.edu/~engelen/interop2C.cgi",
   "http://services.soaplite.com/interop.cgi",
   "http://interop.capeclear.com/ccx/soapbuilders-round2",
   "http://nagoya.apache.org:5049/axis/services/echoB",
	"http://easysoap.sourceforge.net/cgi-bin/interopserver",
	"http://soapinterop.simdb.com/round2B",
 };


/***********************************************************************
 *
 *	Internal Constants
 *
 ***********************************************************************/
#define appFileCreator			'STRT'	// register your own at http://www.palmos.com/dev/creatorid/
#define appVersionNum			0x01
#define appPrefID				0x00
#define appPrefVersionNum		0x01

// Define the minimum OS version we support (2.0 for now).
#define kOurMinVersion	sysMakeROMVersion(2,0,0,sysROMStageRelease,0)
#define kPalmOS10Version	sysMakeROMVersion(1,0,0,sysROMStageRelease,0)


/***********************************************************************
 *
 *	Internal Functions
 *
 ***********************************************************************/

/***********************************************************************
 *
 * FUNCTION:    RomVersionCompatible
 *
 * DESCRIPTION: This routine checks that a ROM version is meet your
 *              minimum requirement.
 *
 * PARAMETERS:  requiredVersion - minimum rom version required
 *                                (see sysFtrNumROMVersion in SystemMgr.h 
 *                                for format)
 *              launchFlags     - flags that indicate if the application 
 *                                UI is initialized.
 *
 * RETURNED:    error code or zero if rom is compatible
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Err RomVersionCompatible(UInt32 requiredVersion, UInt16 launchFlags)
{
	UInt32 romVersion;
	// See if we're on in minimum required version of the ROM or later.
	FtrGet(sysFtrCreator, sysFtrNumROMVersion, &romVersion);
	if (romVersion < requiredVersion)
		{
		if ((launchFlags & (sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp)) ==
			(sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp))
			{
			FrmAlert (RomIncompatibleAlert);
		
			// Palm OS 1.0 will continuously relaunch this app unless we switch to 
			// another safe one.
			if (romVersion <= kPalmOS10Version)
				{
				AppLaunchWithCommand(sysFileCDefaultApp, sysAppLaunchCmdNormalLaunch, NULL);
				}
			}
		
		return sysErrRomIncompatible;
		}

	return errNone;
}


/***********************************************************************
 *
 * FUNCTION:    GetObjectPtr
 *
 * DESCRIPTION: This routine returns a pointer to an object in the current
 *              form.
 *
 * PARAMETERS:  formId - id of the form to display
 *
 * RETURNED:    void *
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void * GetObjectPtr(UInt16 objectID)
{
	FormPtr frmP;

	frmP = FrmGetActiveForm();
	return FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, objectID));
}


/***********************************************************************
 *
 * FUNCTION:    MainFormInit
 *
 * DESCRIPTION: This routine initializes the MainForm form.
 *
 * PARAMETERS:  frm - pointer to the MainForm form.
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void MainFormInit(FormPtr /*frmP*/)
{
 initURLList();
}


/***********************************************************************
 *
 * FUNCTION:    MainFormDoCommand
 *
 * DESCRIPTION: This routine performs the menu command specified.
 *
 * PARAMETERS:  command  - menu item id
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Boolean MainFormDoCommand(UInt16 command)
{
	Boolean handled = false;
	FormPtr frmP;

	switch (command)
		{
		case MainOptionsAboutStarterApp:
			MenuEraseStatus(0);					// Clear the menu status from the display.
			frmP = FrmInitForm (AboutForm);
			FrmDoDialog (frmP);					// Display the About Box.
			FrmDeleteForm (frmP);
			handled = true;
			break;

		}
	
	return handled;
}


/***********************************************************************
 *
 * FUNCTION:    MainFormHandleEvent
 *
 * DESCRIPTION: This routine is the event handler for the 
 *              "MainForm" of this application.
 *
 * PARAMETERS:  eventP  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event has handle and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Boolean MainFormHandleEvent(EventPtr eventP)
{
	Boolean handled = false;
	FormPtr frmP;

	switch (eventP->eType) 
		{
		case menuEvent:
			return MainFormDoCommand(eventP->data.menu.itemID);

		case frmOpenEvent:
			frmP = FrmGetActiveForm();
			MainFormInit( frmP);
			FrmDrawForm ( frmP);
			handled = true;
			break;
			
		case frmUpdateEvent:
			// To do any custom drawing here, first call FrmDrawForm(), then do your
			// drawing, and then set handled to true.
			break;

		case ctlSelectEvent:
			switch (eventP->data.ctlSelect.controlID)
			{
				case MainRunAButton:
					interoptA(getSite('A'));
					break;
				case MainRunBButton:
					interopB(getSite('B'));
					break;
				case MainRunCButton:
					interopC(getSite('C'));
					break;
			}
			break;

		default:
			break;
		
		}
	
	return handled;
}


/***********************************************************************
 *
 * FUNCTION:    AppHandleEvent
 *
 * DESCRIPTION: This routine loads form resources and set the event
 *              handler for the form loaded.
 *
 * PARAMETERS:  event  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event has handle and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Boolean AppHandleEvent(EventPtr eventP)
{
	UInt16 formId;
	FormPtr frmP;

	if (eventP->eType == frmLoadEvent)
		{
		// Load the form resource.
		formId = eventP->data.frmLoad.formID;
		frmP = FrmInitForm(formId);
		FrmSetActiveForm(frmP);

		// Set the event handler for the form.  The handler of the currently
		// active form is called by FrmHandleEvent each time is receives an
		// event.
		switch (formId)
			{
			case MainForm:
			
			    switch (eventP->data.ctlSelect.controlID)
		        {
//			        case InteroptARunAll:
//			        interoptA(1);
//				    break;
		        }
			
				FrmSetEventHandler(frmP, MainFormHandleEvent);
				break;

			default:
//				ErrFatalDisplay("Invalid Form Load Event");
				break;

			}
		return true;
		}
	
	return false;
}


/***********************************************************************
 *
 * FUNCTION:    AppEventLoop
 *
 * DESCRIPTION: This routine is the event loop for the application.  
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void AppEventLoop(void)
{
	UInt16 error;
	EventType event;

	do {
		EvtGetEvent(&event, evtWaitForever);

		if (! SysHandleEvent(&event))
			if (! MenuHandleEvent(0, &event, &error))
				if (! AppHandleEvent(&event))
					FrmDispatchEvent(&event);

	} while (event.eType != appStopEvent);
}


/***********************************************************************
 *
 * FUNCTION:     AppStart
 *
 * DESCRIPTION:  Get the current application's preferences.
 *
 * PARAMETERS:   nothing
 *
 * RETURNED:     Err value 0 if nothing went wrong
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Err AppStart(void)
{
	StarterPreferenceType prefs;
	UInt16 prefsSize;

	// Read the saved preferences / saved-state information.
	prefsSize = sizeof(StarterPreferenceType);
	if (PrefGetAppPreferences(appFileCreator, appPrefID, &prefs, &prefsSize, true) != 
		noPreferenceFound)
		{
		}
	
	return errNone;
}


/***********************************************************************
 *
 * FUNCTION:    AppStop
 *
 * DESCRIPTION: Save the current state of the application.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void AppStop(void)
{
	StarterPreferenceType prefs;

	// Write the saved preferences / saved-state information.  This data 
	// will saved during a HotSync backup.
	PrefSetAppPreferences (appFileCreator, appPrefID, appPrefVersionNum, 
		&prefs, sizeof (prefs), true);
		
	// Close all the open forms.
	FrmCloseAllForms ();
}


/***********************************************************************
 *
 * FUNCTION:    StarterPalmMain
 *
 * DESCRIPTION: This is the main entry point for the application.
 *
 * PARAMETERS:  cmd - word value specifying the launch code. 
 *              cmdPB - pointer to a structure that is associated with the launch code. 
 *              launchFlags -  word value providing extra information about the launch.
 *
 * RETURNED:    Result of launch
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static UInt32 StarterPalmMain(UInt16 cmd, MemPtr /*cmdPBP*/, UInt16 launchFlags)
{
	Err error;

	error = RomVersionCompatible (kOurMinVersion, launchFlags);
	if (error) return (error);

	switch (cmd)
		{
		case sysAppLaunchCmdNormalLaunch:
			error = AppStart();
			if (error) 
				return error;
				
			FrmGotoForm(MainForm);
			AppEventLoop();
			AppStop();
			break;

		default:
			break;

		}
	
	return errNone;
}


/***********************************************************************
 *
 * FUNCTION:    PilotMain
 *
 * DESCRIPTION: This is the main entry point for the application.
 *
 * PARAMETERS:  cmd - word value specifying the launch code. 
 *              cmdPB - pointer to a structure that is associated with the launch code. 
 *              launchFlags -  word value providing extra information about the launch.
 * RETURNED:    Result of launch
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
UInt32 PilotMain( UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
// int i1=sizeof(size_t);
// int i2=sizeof(int);
// int i3=sizeof(struct xsd__base64Binary);
	return StarterPalmMain(cmd, cmdPBP, launchFlags);
}

void displayText(char *text)
{
	ListPtr lst;
   char ** newText;
   char *str =(char*) malloc(1+StrLen(text));
   strcpy(str,text);
  int i=0;
  if (resultLines==0)
  {
    newText = (char**)malloc(sizeof(void*));
  } 
  else
  {
    int size=  resultLines*sizeof(void*);
    newText =(char**) malloc(size+sizeof(void*));
    memcpy(newText,resultText,size);
  }
  
  newText[resultLines]=str;
  resultLines++;
  resultText=newText;

	lst = (ListPtr) GetObjectPtr(MainResultsList);

	LstSetListChoices(lst, resultText, resultLines);
	if (resultLines>5)
	  LstSetTopItem(lst,resultLines-5);
	LstDrawList(lst);

}


char *getSite(char type)
{
  int listIndex;
  listIndex = LstGetSelection((ListPtr)GetObjectPtr(MainURLList));
  if (type=='A') return tryURLsA[listIndex];
  if (type=='B') return tryURLsB[listIndex];
  if (type=='C') return tryURLsC[listIndex];
  
  // Shouldn't ever get to this point. Just return A list
  return tryURLsA[listIndex];
}

void initURLList()
{
	ControlPtr ctl;
	ListPtr lst;
//	UInt32	defaultID;

	ctl = (ControlPtr)GetObjectPtr(MainURLPopTrigger);
	lst = (ListPtr)GetObjectPtr(MainURLList);

	LstSetListChoices(lst, tryURLsA, URL_COUNT);
	LstSetHeight(lst, 10);
	CtlSetLabel (ctl, tryURLsA[0]);
}


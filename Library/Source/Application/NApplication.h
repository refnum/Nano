/*	NAME:
		NApplication.h

	DESCRIPTION:
		Application object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NAPPLICATION_HDR
#define NAPPLICATION_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCarbonEventHandler.h"
#include "NWindowSplashScreen.h"
#include "NAppleEventHandler.h"
#include "NSoftwareUpdate.h"
#include "NUndoManager.h"
#include "NRecentItems.h"
#include "NMsgListener.h"
#include "NDictionary.h"
#include "NDocument.h"
#include "NDockTile.h"
#include "NTimer.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Properties
static const NString kAppNibFileKey								= "AppNibFile";
static const NString kAppNibMenuBarKey							= "AppNibMenuBar";
static const NString kAppNibHelpMenuKey							= "AppNibHelpMenu";
static const NString kAppNibSplashScreenKey						= "AppNibSplashScreen";
static const NString kAppNameKey								= "AppName";
static const NString kAppHomePageKey							= "AppHomePage";
static const NString kAppCreatorKey								= "AppCreator";
static const NString kAppFormatUTIsKey							= "AppFormatUTIs";
static const NString kAppCreateDocOnOpenKey						= "AppCreateDocOnOpen";
static const NString kAppCreateDocOnReopenKey					= "AppCreateDocOnReopen";
static const NString kAppDefaultPrefsKey						= "AppDefaultPrefs";


// Application stats
typedef enum {
	kAppStateStarting,
	kAppStateRunning,
	kAppStateQuitting,
	
	kAppStateQuitReviewSingle,
	kAppStateQuitReviewMultiple,
	kAppStateQuitDiscardChanges
} AppState;





//============================================================================
//      Macros
//----------------------------------------------------------------------------
// Standard main
#define CREATE_NAPPLICATION(__class)										\
	int main(void)															\
	{	__class		theApp;													\
																			\
																			\
		/* Run the application */											\
		theApp.Run();														\
																			\
		return(0);															\
	}





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NApplication : public NCarbonEventHandler,
					 public NAppleEventHandler,
					 public NMsgListener {
public:
										NApplication(void);
	virtual								~NApplication(void);


	// Get the application
	static NApplication					*GetApp(void);
	

	// Run the application
	void								Run(void);


	// Quit the application
	void								Quit(void);


	// Get the properties dictionary
	NDictionary							*GetProperties(void);


	// Get the application state
	AppState							GetState(void) const;


	// Get the application nib
	IBNibRef							GetNib(void) const;


	// Get the recent items menu
	NRecentItems						*GetRecentItems(void);


	// Get the Dock tile
	NDockTile							GetDockTile(void) const;
	

	// Cancel/continue a review
	//
	// Invoked by NDocument during a review of modified documents during a quit.
	void								ContinueReview(void);
	void								CancelReview(void);


protected:
	// Initialize ourselves
	//
	// Sub-classes should call through to initialize the base class.
	virtual OSStatus					InitializeProperties(void);
	virtual OSStatus					InitializeBegin(void);
	virtual OSStatus					InitializeMenuBar(void);
	virtual OSStatus					InitializeSelf(void);
	virtual OSStatus					InitializeEnd(void);


	// Shut down the app
	//
	// Sub-classes should call through to shut down the base class.
	virtual void						ShutDown(void);


	// Handle messages
	virtual void						DoMessage(BroadcastMsg theMsg, void *msgData);


	// Handle events
	virtual OSStatus					DoEventCommandProcess(NCarbonEvent &theEvent);
	virtual OSStatus					DoEventAppActivated(  NCarbonEvent &theEvent);


	// Handle Apple Events
	virtual OSStatus					DoAppleEventOpenApp(  const AppleEvent &theEvent);
	virtual OSStatus					DoAppleEventOpenDocs( const AppleEvent &theEvent);
	virtual OSStatus					DoAppleEventPrintDocs(const AppleEvent &theEvent);
	virtual OSStatus					DoAppleEventQuitApp(  const AppleEvent &theEvent);
	virtual OSStatus					DoAppleEventReopenApp(const AppleEvent &theEvent);


	// Get the status of a command
	virtual CommandStatus				GetCommandStatus(const HICommandExtended &hiCmd);


	// Handle commands
	virtual OSStatus					CommandNew(NDocument *&theDocument, const NFile *theFile=NULL);
	virtual OSStatus					CommandOpen(void);
	virtual OSStatus					CommandQuit(void);
	virtual OSStatus					CommandAbout(void);
	virtual OSStatus					CommandPreferences(void);
	virtual OSStatus					CommandSoftwareUpdate(UInt32 theCmd);
	virtual OSStatus					CommandToggleColorPanel(void);


	// Create a new document
	virtual NDocument					*CreateDocument(void);


	// Get the splash screen
	NWindowSplashScreen					*GetSplashScreen(void);
	

private:
	NDocument							*FindDocument(  const NFile     &theFile);
	OSStatus							CreateDocuments(const NFileList &theFiles, NCarbonEvent *theEvent);

	OSStatus							CloseDocuments(void);
	void								CloseWindows(void);

	void								SelectModifiedDocuments(void);
	void								SendEventToDocuments(NCarbonEvent &theEvent, bool onlyModified);
	
	void								ShowSplashScreen(void);
	void								HideSplashScreen(void);

	void								CheckForUpdates(void);

	NString								GetColorToggle(void) const;
	NString								GetFontToggle( void) const;


private:
	IBNibRef							mNib;
	AppState							mState;
	NTimer								mTimer;
	NDictionary							mProperties;

	NUndoManager						mUndoManager;
	NRecentItems						mRecentItems;
	NSoftwareUpdate						mSoftwareUpdate;
	
	EventTime							mSplashStart;
	NWindowSplashScreen					*mSplashScreen;

	static NApplication					*mApp;
};





#endif // NAPPLICATION_HDR



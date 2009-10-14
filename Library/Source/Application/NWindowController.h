/*	NAME:
		NWindowController.h

	DESCRIPTION:
		Window controller.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NWINDOWCONTROLLER_HDR
#define NWINDOWCONTROLLER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCarbonEventHandler.h"
#include "NStandardAlert.h"
#include "NMsgListener.h"
#include "NUndoManager.h"
#include "NIBWindow.h"
#include "NIBView.h"
#include "NHIWindow.h"





//============================================================================
//		Templates
//----------------------------------------------------------------------------
// Controller construction
template <class T> T*CreateController(const NString &nibName, const NString &resName)
{	T			*theObject;
	OSStatus	theErr;



	// Create the controller
	theObject = new T;
	if (theObject == NULL)
		return(NULL);



	// Initialize it
	//
	// If Initialize fails, theObject will destroy itself.
	theErr = theObject->Initialize(nibName, resName);
	if (theErr != noErr)
		theObject = NULL;
	
	return(theObject);
}





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// Controller construction
//
// Controller sub-classes can use these macros to declare standard methods
// for instantiating a singleton controller (e.g., a preferences window) or
// to create a controller from a nib (e.g., a sheet window).
//
// Usage:
//
//		CMyController.h
//		---------------
//		class CMyController : public NWindowController {
//		...
//			DECLARE_WINDOW_CONTROLLER(CMyController);
//		...
//		};
//
//		CMyController.cpp
//		-----------------
//		DEFINE_WINDOW_CONTROLLER(CMyController, "MyNib", "MyWindow");
//
//
// To instantiate a new instance of a controller, use:
//
//		CMyController *theController = CMyController::Create();
//
// To obtain a singleton instance of a controller, use:
//
//		CMyController *theController = CMyController::GetInstance();
//
// In both cases, the controller will be instantiated using the "MyWindow"
// window from the "MyNib" nib file within the application bundle.
//
//
// A "singleton" object is an object which has a single instance within an
// application - for example, a preferences window.
//
// Singleton NWindowController objects are destroyed as normal when they
// are closed, and re-created on the next invocation of GetInstance.
//
// To ensure that GetInstance always holds a valid pointer, or NULL, the
// address of the singleton pointer is stored within the controller itself
// so that the instance pointer can be cleared when it is destroyed.
#define DECLARE_WINDOW_CONTROLLER(_class)														\
			static _class *Create(void);														\
																								\
			static _class *GetInstance(void)


#define DEFINE_WINDOW_CONTROLLER(_class, _nibName, _resName)									\
			_class *_class::Create(void)														\
			{																					\
				return(CreateController<_class>(_nibName, _resName));							\
			}																					\
																								\
			_class *_class::GetInstance(void)													\
			{	static _class	*sInstance = NULL;												\
																								\
																								\
				if (sInstance == NULL)															\
					sInstance = _class::Create();												\
																								\
																								\
				if (sInstance != NULL)															\
					{																			\
					NN_ASSERT(sInstance->mInstance == NULL ||									\
							  sInstance->mInstance == (NWindowController **) &sInstance);		\
																								\
					sInstance->mInstance = (NWindowController **) &sInstance;					\
					}																			\
																								\
				return(sInstance);																\
			}





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NWindowController : public NCarbonEventHandler,
						  public NMsgListener,
						  public NIBWindow {
public:
										NWindowController(void);
	virtual								~NWindowController(void);


	// Initialize the controller
	OSStatus							Initialize(const NString &nibName, const NString &resName);
	OSStatus							Initialize(NHIWindow *theWindow);


	// Get our window
	NHIWindow							*GetWindow(void) const;


	// Get our undo manager
	const NUndoManager					*GetUndoManager(void) const;
	NUndoManager						*GetUndoManager(void);
	

	// Close the window
	void								Close(void);


	// Display an alert
	//
	// The alert will be displayed as a sheet if possible, or a modal alert if
	// this controller already has a sheet attached.
	//
	// In either situation, the alert result will be sent to CommandSheet.
	void								DisplayAlert(const NString	&textMajor,
													 const NString	&textMinor,
													 bool			 stringsAreKeys = true,
													 const NString	&buttonDefault  = kDefaultAlertText,
													 const NString	&buttonCancel   = "",
													 const NString	&buttonOther    = "",
													 AlertType		 theType        = kAlertNoteAlert,
													 bool			 hasHelp        = false);


	// Begin/end a sheet
	//
	// To dismiss the sheet, EndSheet can be invoked on either the sheet itself
	// or the parent controller.
	//
	// When the sheet is closed, the sheet controller will be deleted.
	OSStatus							BeginSheet(NStandardAlert    *theSheet);
	OSStatus							BeginSheet(NWindowController *theSheet);
	
	OSStatus							EndSheet(void);


	// Update the toolbar status
	void								UpdateToolbarStatus(void);


	// Record an undo action
	void								RecordUndo(const NFunctor	&theAction,
												   const NString	&theName       = "",
												   bool				stringsAreKeys = true);


	// Track an undo group
	//
	// Undo-able actions that are driven by mouse-tracking (e.g., dragging an object,
	// or using a color picker) should typically be grouped into a single operation
	// so that the user can undo/redo the entire sequence atomically.
	//
	// Since these operations can typically be represented by the first action in the
	// group, the group may also be flushed to minimise the number of actions that are
	// saved to the undo stacks.
	void								TrackUndo(bool beginTrack, bool canFlush=true);


protected:
	// Initialize ourselves
	//
	// Sub-classes do not need to call through to initalize the base class.
	virtual OSStatus					InitializeSelf(void);


	// Handle messages
	virtual void						DoMessage(BroadcastMsg theMsg, void *msgData);


	// Handle events
	virtual OSStatus					DoEventCommandProcess(NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowClose(   NCarbonEvent &theEvent);


	// Get the status of a command
	virtual CommandStatus				GetCommandStatus(const HICommandExtended &hiCmd);


	// Handle commands
	virtual OSStatus					CommandClose(void);
	virtual OSStatus					CommandSheet(UInt32 theCmd);
	virtual OSStatus					CommandToggleToolbar(void);


	// Handle sheet notifications
	//
	// Both the sheet parent, and the sheet itself, will be notified prior to the
	// sheet being opened or closed, allowing them to perform initialization/cleanup.
	virtual void						DoSheetOpening(NWindowController *theParent, NWindowController *theSheet);
	virtual void						DoSheetClosing(NWindowController *theParent, NWindowController *theSheet);


	// Find our views
	//
	// Views will be located automatically when the window is initialized, however
	// sub-classes whose contents change over time can use this method to re-find
	// their views.
	//
	// Any broadcasters amongst the views will automatically be listened to.
	void								FindViews(void);


private:
	NString								GetToggleText(void) const;

	bool								OwnsSheet(bool includeSelf=true);
	

protected:
	NWindowController					**mInstance;
	

private:
	NHIWindow							*mWindow;
	NUndoManager						mUndoManager;
	
	NStandardAlert						*mSheetAlert;
	NWindowController					*mSheetParent;
	NWindowController					*mSheetChild;
};





#endif // NWINDOWCONTROLLER_HDR



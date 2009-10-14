/*	NAME:
		NWindowController.cpp

	DESCRIPTION:
		Window controller.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFBundle.h"
#include "Nano Build.h"
#include "NHIToolbar.h"
#include "NHIToolbarItem.h"
#include "NMenuUtilities.h"
#include "NWindowController.h"





//============================================================================
//		NWindowController::NWindowController : Constructor.
//----------------------------------------------------------------------------
NWindowController::NWindowController(void)
{


	// Initialize ourselves
	mInstance = NULL;

	mWindow      = NULL;
	mSheetAlert  = NULL;
	mSheetParent = NULL;
	mSheetChild  = NULL;
}





//============================================================================
//		NWindowController::~NWindowController : Destructor.
//----------------------------------------------------------------------------
NWindowController::~NWindowController(void)
{


	// Close any sheet
	//
	// We only want to delete our sheet if we are the parent.
	if (OwnsSheet(false))
		EndSheet();



	// Remove our handler
	//
	// Since our window is deleted before us, we need to remove the handler
	// to ensure we don't receive any further events from the window.
	RemoveHandler();



	// Clean up
	delete mWindow;



	// Update our instance
	//
	// Singleton controllers have an instance pointer, which must be cleared
	// when the controller is destroyed to ensure it will be re-created when
	// the singleton is next accessed.
	if (mInstance != NULL)
		{
		NN_ASSERT(*mInstance == this);
		*mInstance = NULL;
		}
}





//============================================================================
//		NWindowController::Initialize : Initialize the controller.
//----------------------------------------------------------------------------
OSStatus NWindowController::Initialize(const NString &nibName, const NString &resName)
{	NHIWindow	*theWindow;



	// Validate our parameters
	NN_ASSERT(nibName.IsNotEmpty());
	NN_ASSERT(resName.IsNotEmpty());



	// Create the window
	theWindow = new NHIWindow(nibName, resName);
	if (theWindow == NULL)
		return(memFullErr);



	// Initialize the controller
	return(Initialize(theWindow));
}





//============================================================================
//		NWindowController::Initialize : Initialize the controller.
//----------------------------------------------------------------------------
OSStatus NWindowController::Initialize(NHIWindow *theWindow)
{	EventTypeSpec		theEvents[] = {	{ kEventClassCommand,	kEventCommandProcess      },
										{ kEventClassCommand,	kEventCommandUpdateStatus },
										{ kEventClassWindow,	kEventWindowClose         },
										{ kEventClassNone,		kEventKindNone            } };
	OSStatus			theErr;



	// Validate our parameters
	NN_ASSERT(theWindow != NULL);



	// Initialize ourselves
	mWindow = theWindow;



	// Register for events
	theErr = InstallHandler(mWindow->GetHIWindow(), theEvents);
	NN_ASSERT_NOERR(theErr);



	// Find our views
	if (theErr == noErr)
		FindViews();



	// Initialize the base class
	if (theErr == noErr)
		{
		theErr = InitializeSelf();
		NN_ASSERT_NOERR(theErr);
		}



	// Handle failure
	if (theErr != noErr)
		{
		delete mWindow;
		mWindow = NULL;
		}
	
	return(theErr);
}





//============================================================================
//		NWindowController::GetWindow : Get our window.
//----------------------------------------------------------------------------
NHIWindow *NWindowController::GetWindow(void) const
{


	// Get the window
	return(mWindow);
}





//============================================================================
//		NWindowController::Close : Close the window.
//----------------------------------------------------------------------------
void NWindowController::Close(void)
{	OSStatus	theErr;



	// Attempt to close
	theErr = CommandClose();
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NWindowController::DisplayAlert : Display an alert.
//----------------------------------------------------------------------------
void NWindowController::DisplayAlert(const NString		&textMajor,
									 const NString		&textMinor,
									 bool				stringsAreKeys,
									 const NString		&buttonDefault,
									 const NString		&buttonCancel,
									 const NString		&buttonOther,
									 AlertType			theType,
									 bool				hasHelp)
{	NString				valueMajor, valueMinor, valueDefault, valueCancel, valueOther;
	NStandardAlert		*theAlert;
	NCFBundle			appBundle;
	DialogItemIndex		theItem;
	UInt32				theCmd;



	// Prepare the strings
	valueMajor   = stringsAreKeys ? appBundle.GetString(textMajor,     textMajor)     : textMajor;
	valueMinor   = stringsAreKeys ? appBundle.GetString(textMinor,     textMinor)     : textMinor;
	valueDefault = stringsAreKeys ? appBundle.GetString(buttonDefault, buttonDefault) : buttonDefault;
	valueCancel  = stringsAreKeys ? appBundle.GetString(buttonCancel,  buttonCancel)  : buttonCancel;
	valueOther   = stringsAreKeys ? appBundle.GetString(buttonOther,   buttonOther)   : buttonOther;



	// Prepare the alert
	theAlert = new NStandardAlert;
	if (theAlert == NULL)
		{
		SysBeep(30);
		return;
		}
	
	theAlert->SetTextMajor(  valueMajor);
	theAlert->SetTextMinor(  valueMinor);
	theAlert->SetTextDefault(valueDefault);
	theAlert->SetTextCancel( valueCancel);
	theAlert->SetTextOther(  valueOther);
	theAlert->SetType(       theType);
	theAlert->SetHasHelp(    hasHelp);



	// Show the alert as a sheet
	//
	// If possible, we display the alert as a sheet. The sheet will send commands
	// to our window, which will pass them up to us to dismiss the sheet.
	if (!mWindow->HasSheet())
		BeginSheet(theAlert);



	// Or fall back to a dialog
	//
	// If we already have a sheet, rather than attempt to nest them, we fall back
	// to a modal dialog. In this situation the sheet is done when the dialog is
	// over, and so we need to process the command ourselves.
	else
		{
		// Do the dialog
		theItem = theAlert->Open();
		theCmd  = kHICommandNone;
		
		switch (theItem) {
			case kAlertStdAlertOKButton:
				theCmd = kHICommandOK;
				break;

			case kAlertStdAlertCancelButton:
				theCmd = kHICommandCancel;
				break;

			case kAlertStdAlertOtherButton:
				theCmd = kHICommandOther;
				break;

			case kAlertStdAlertHelpButton:
				theCmd = kHICommandAppHelp;
				break;
			
			default:
				NN_LOG("Unknown alert item: %d", theItem);
				break;
			}
		
		
		// Clean up
		if (theCmd != kHICommandNone)
			CommandSheet(theCmd);
		
		delete theAlert;
		}
}





//============================================================================
//		NWindowController::BeginSheet : Begin a sheet.
//----------------------------------------------------------------------------
OSStatus NWindowController::BeginSheet(NStandardAlert *theSheet)
{


	// Validate our state
	NN_ASSERT(mSheetAlert == NULL && mSheetParent == NULL && mSheetChild == NULL);
	NN_ASSERT(!mWindow->HasSheet());



	// Check our parameters
	if (theSheet == NULL)
		{
		SysBeep(30);
		return(paramErr);
		}



	// Begin the sheet
	mSheetAlert = theSheet;
	
	mSheetAlert->SetParent(mWindow);
	mSheetAlert->Open();

	return(noErr);
}





//============================================================================
//		NWindowController::BeginSheet : Begin a sheet.
//----------------------------------------------------------------------------
OSStatus NWindowController::BeginSheet(NWindowController *theSheet)
{


	// Validate our state
	NN_ASSERT(mSheetAlert == NULL && mSheetParent == NULL && mSheetChild == NULL);
	NN_ASSERT(!mWindow->HasSheet());



	// Check our parameters
	if (theSheet == NULL)
		{
		SysBeep(30);
		return(paramErr);
		}



	// Update our state
	mSheetChild               = theSheet;
	mSheetChild->mSheetParent = this;



	// Open the sheet
	mSheetChild->DoSheetOpening(this, mSheetChild);
	             DoSheetOpening(this, mSheetChild);

	mWindow->BeginSheet(mSheetChild->GetWindow());

	return(noErr);
}





//============================================================================
//		NWindowController::EndSheet : End a sheet.
//----------------------------------------------------------------------------
OSStatus NWindowController::EndSheet(void)
{


	// Validate our state
	NN_ASSERT(mSheetAlert != NULL || mSheetParent != NULL || mSheetChild != NULL);
	


	// We're the sheet
	//
	// As such, when closed we just update our parent and destroy ourselves.
	if (mSheetParent != NULL)
		{
		// Validate our state
		NN_ASSERT(mSheetParent->mSheetAlert  == NULL);
		NN_ASSERT(mSheetParent->mSheetParent == NULL);
		NN_ASSERT(mSheetParent->mSheetChild  == this);
		NN_ASSERT(mSheetParent->mWindow->HasSheet());


		// Close the sheet
		              DoSheetClosing(mSheetParent, this);
		mSheetParent->DoSheetClosing(mSheetParent, this);

		mSheetParent->mWindow->EndSheet();


		// Clean up
		mSheetParent->mSheetChild = NULL;

		delete this;
		}
	
	
	
	// We're the parent
	//
	// Our child sheet may be an alert or an arbitrary sheet.
	else
		{
		// Validate our state
		NN_ASSERT(mSheetParent == NULL);
		NN_ASSERT(mWindow->HasSheet());



		// Close the alert
		//
		// Alert sheets do not notify the parent when closing, since they are not
		// independent controllers - the parent can receive a notifications with
		// the final CommandSheet.
		if (mSheetAlert != NULL)
			{
			NN_ASSERT(mSheetAlert->GetParent() == GetWindow());
			NN_ASSERT(mSheetChild              == NULL);

			mSheetAlert->Close();
			delete mSheetAlert;
			
			mSheetAlert = NULL;
			}
		
		
		// Close the sheet
		else
			{
			NN_ASSERT(mSheetChild               != NULL);
			NN_ASSERT(mSheetChild->mSheetParent == this);
			
			mSheetChild->EndSheet();
			}
		}
	
	return(noErr);
}





//============================================================================
//		NWindowController::UpdateToolbarStatus : Update the toolbar status.
//----------------------------------------------------------------------------
void NWindowController::UpdateToolbarStatus(void)
{	NHIToolbar							*theToolbar;
	CommandStatus						theStatus;
	NHIToolbarItemList					theItems;
	NHIToolbarItem						*theItem;
	NHIToolbarItemListConstIterator		theIter;
	HICommandExtended					hiCmd;



	// Get the state we need
	memset(&hiCmd, 0x00, sizeof(hiCmd));

	theToolbar = mWindow->GetToolbar();
	if (theToolbar != NULL)
		theItems = theToolbar->GetToolbarItems();



	// Update the command status
	for (theIter = theItems.begin(); theIter != theItems.end(); theIter++)
		{
		theItem = *theIter;

		hiCmd.commandID = theItem->GetCommandID();
		theStatus       = GetCommandStatus(hiCmd);

		theItem->SetEnabled(theStatus != kCommandDisable);
		}
}





//============================================================================
//		NWindowController::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus NWindowController::InitializeSelf(void)
{


	// Initialize ourselves
	//
	// Sub-classes should override to perform their own initialisation.
	//
	// To allow controllers to fail during construction, CreateController
	// will construct the object and then call Initialize().
	//
	// This will initialize the base class, then initialize the sub-class; if
	// either fail, the controller will be destroyed.
	return(noErr);
}





//============================================================================
//		NWindowController::DoMessage : Handle messages.
//----------------------------------------------------------------------------
void NWindowController::DoMessage(BroadcastMsg /*theMsg*/, void */*msgData*/)
{


	// Ignore the message
	//
	// We derive from NMsgListener so that we can listen to any views in our
	// window, but since sub-classes may not use this feature (e.g., if they
	// don't have any broadasting views) we need to provide an implementation
	// of the otherwise pure virtual method.
	//
	// Sub-classes that override this method should call through with unknown
	// messages, in case we need to listen to our own messages in the future.
}





//============================================================================
//		NWindowController::DoEventCommandProcess : Handle kEventCommandProcess.
//----------------------------------------------------------------------------
OSStatus NWindowController::DoEventCommandProcess(NCarbonEvent &theEvent)
{	OSStatus			theErr;
	HICommandExtended	hiCmd;



	// Handle the command
	hiCmd = theEvent.GetHICommand();
	
	switch (hiCmd.commandID) {
		case kHICommandClose:
			theErr = CommandClose();
			break;
		
		case kHICommandNToggleToolbar:
			theErr = CommandToggleToolbar();
			break;

		default:
			if (OwnsSheet())
				theErr = CommandSheet(hiCmd.commandID);
			else
				theErr = eventNotHandledErr;
			break;
		}
	
	return(theErr);
}





//============================================================================
//		NWindowController::DoEventWindowClose : Handle kEventWindowClose.
//----------------------------------------------------------------------------
OSStatus NWindowController::DoEventWindowClose(NCarbonEvent &/*theEvent*/)
{	OSStatus	theErr;



	// Attempt to close
	theErr = CommandClose();
	
	return(theErr);
}





//============================================================================
//		NWindowController::GetCommandStatus : Get the status of a command.
//----------------------------------------------------------------------------
CommandStatus NWindowController::GetCommandStatus(const HICommandExtended &hiCmd)
{	CommandStatus	theStatus;
	bool			isModal;



	// Get the command status
	switch (hiCmd.commandID) {
		case kHICommandClose:
			isModal   = mWindow->GetFeatures() & kWindowIsModal;
			theStatus = EnableIf(!mWindow->HasSheet() && !isModal);
			break;
		
		case kHICommandNToggleToolbar:
			theStatus = EnableIf(mWindow->GetToolbar() != NULL) ;
			NMenuUtilities::SetItemText(hiCmd.source.menu.menuRef, hiCmd.commandID, GetToggleText());
			break;
		
		default:
			theStatus = NCarbonEventHandler::GetCommandStatus(hiCmd);
			break;
		}
	
	return(theStatus);
}





//============================================================================
//		NWindowController::CommandClose : Handle a close command.
//----------------------------------------------------------------------------
OSStatus NWindowController::CommandClose(void)
{


	// Delete ourselves
	//
	// Sub-classes may wish to override, e.g. to ask permission before closing.
	delete this;
	
	return(noErr);
}





//============================================================================
//		NWindowController::CommandSheet : Handle a sheet command.
//----------------------------------------------------------------------------
OSStatus NWindowController::CommandSheet(UInt32 theCmd)
{	OSStatus	theErr;



	// Process the command
	//
	// Sheets typically contain a mixture of command-generating controls, only
	// some of which should dismiss the sheet.
	//
	// When a sheet is active, unknown commands are send here for processing.
	//
	// Sheet controllers, or the controller of the sheet owner, can override
	// this method to take the appropriate action for the command.
	//
	//
	// The base class provides a default action for three standard commands,
	// automatically closing the sheet when these are received.
	//
	// Sub-classes can also call EndSheet() if they have additional commands
	// that should close their sheet, and should call through to the base class
	// after processing these three standard commands.
	//
	//
	// Note that EndSheet can be called both by a sheet controller, or by the
	// controller that owns the sheet controller.
	//
	// This allows sheets to be implemented with either a dedicated controller
	// class (which would typically handle its own commands) or with a generic
	// controller that is instantiated from a nib and attached to an existing
	// window (which would typically handle commands for the sheet).
	theErr = eventNotHandledErr;

	switch (theCmd) {
		case kHICommandOK:
		case kHICommandCancel:
		case kHICommandOther:
			// Close the sheet
			if (OwnsSheet())
				theErr = EndSheet();
			break;
		
		default:
			break;
		}
	
	return(theErr);
}





//============================================================================
//		NWindowController::CommandToggleToolbar : Handle a toogle toolbar command.
//----------------------------------------------------------------------------
OSStatus NWindowController::CommandToggleToolbar(void)
{


	// Validate our state
	NN_ASSERT(mWindow->GetToolbar() != NULL);



	// Toggle the toolbar visibility
	mWindow->SetToolbarVisible(!mWindow->IsToolbarVisible());
	
	return(noErr);
}





//============================================================================
//		NWindowController::DoSheetOpening : A sheet is opening.
//----------------------------------------------------------------------------
//		Note : Sub-classes may wish to override to perform initialization.
//----------------------------------------------------------------------------
void NWindowController::DoSheetOpening(NWindowController */*theParent*/, NWindowController */*theSheet*/)
{
}





//============================================================================
//		NWindowController::DoSheetClosing : A sheet is closing.
//----------------------------------------------------------------------------
//		Note : Sub-classes may wish to override to perform cleanup.
//----------------------------------------------------------------------------
void NWindowController::DoSheetClosing(NWindowController */*theParent*/, NWindowController */*theSheet*/)
{
}





//============================================================================
//		NWindowController::FindViews : Find our views.
//----------------------------------------------------------------------------
void NWindowController::FindViews(void)
{	NMsgBroadcaster					*theBroadcaster;
	const NIBViewList				*theViews;
	NIBViewListConstIterator		theIter;



	// Find our views
	NIBWindow::FindViews(GetWindow());



	// Listen to the broadcasters
	//
	// Windows that contain broadcasting views will typically want to listen
	// to the messages from those views, in the same way that they typically
	// want to process commands from their views.
	//
	// Therefore, we automatically register as a listener for any broadcasters
	// amongst our views.
	//
	// Sub-classes can stop listening to a view if they don't want the messages,
	// or just ignore them (message dispatch is very lightweight, so messages
	// can simply be ignored if they aren't needed).
	theViews = GetViews();
	
	for (theIter = theViews->begin(); theIter != theViews->end(); theIter++)
		{
		theBroadcaster = dynamic_cast<NMsgBroadcaster*>((*theIter)->GetView());
		if (theBroadcaster != NULL && !IsListeningTo(theBroadcaster))
			theBroadcaster->AddListener(this);
		}
}





//============================================================================
//		NWindowController::GetToggleText : Get the text for "toggle toolbar".
//----------------------------------------------------------------------------
#pragma mark -
NString NWindowController::GetToggleText(void) const
{	NString		theKey, theValue;
	NCFBundle	cfBundle;



	// Get the text
	theKey   = mWindow->IsToolbarVisible() ? kStringToolbarHide : kStringToolbarShow;
	theValue = cfBundle.GetString(theKey, "", kNanoStrings);
	
	NN_ASSERT(theValue.IsNotEmpty());
	
	return(theValue);
}





//============================================================================
//		NWindowController::OwnsSheet : Do we own a sheet?
//----------------------------------------------------------------------------
bool NWindowController::OwnsSheet(bool includeSelf)
{	bool	ownsSheet;



	// Check our state
	//
	// Note that we only check to see if we own the sheet - our window may still
	// have a system-owned sheet that we have no knowledge of.
	//
	// This method is used to determine when we need to take an action for one
	// of our own sheets, and our window must be queried directly if we need to
	// identify if there is any kind of sheet attached to our window.
	ownsSheet = (mSheetAlert != NULL || mSheetChild != NULL);
	
	if (includeSelf && mSheetParent != NULL)
		ownsSheet = true;
		
	return(ownsSheet);
}





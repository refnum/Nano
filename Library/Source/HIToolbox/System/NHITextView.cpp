/*	NAME:
		NHITextView.cpp

	DESCRIPTION:
		Text field view.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFileUtilities.h"
#include "NUndoManager.h"
#include "NMenuItem.h"
#include "NCFBundle.h"
#include "NHITextView.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Tags
static const OSType kTagTextFile									= 'file';
static const OSType kTagAutoScroll									= 'ascr';
static const OSType kTagMarginTop									= 'mtop';
static const OSType kTagMarginLeft									= 'mlef';
static const OSType kTagMarginBottom								= 'mbot';
static const OSType kTagMarginRight									= 'mrig';


// Misc
static const EventTime kTimeModified								= 0.8;
static const TXNCommandEventSupportOptions kCommandSpelling			= kTXNSupportSpellCheckCommandProcessing |
																	  kTXNSupportSpellCheckCommandUpdating;





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NHITextView, kHITextViewClassID, NULL);





//============================================================================
//		NHITextView::NHITextView : Constructor.
//----------------------------------------------------------------------------
NHITextView::NHITextView(HIWindowRef			theWindow,
							const HIRect		&theRect,
							const NString		&theValue,
							TXNFrameOptions		frameOptions)
{	HIViewRef	theView;
	OSStatus	theErr;



	// Create the view
	theErr = HITextViewCreate(NULL, 0, frameOptions, &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);



	// Configure the view
	SetTextValue(theValue);
}





//============================================================================
//		NHITextView::~NHITextView : Destructor.
//----------------------------------------------------------------------------
NHITextView::~NHITextView(void)
{
}





//============================================================================
//		NHITextView::IsModified : Has the content been modified?
//----------------------------------------------------------------------------
bool NHITextView::IsModified(void) const
{


	// Check our state
	return(CanUndo());
}





//============================================================================
//		NHITextView::ClearUndo : Clear the undo state.
//----------------------------------------------------------------------------
void NHITextView::ClearUndo(void)
{	OSStatus	theErr;


	// Clear the undo state
	theErr = TXNClearUndo(*this);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHITextView::CanUndo : Can the last action be undone?
//----------------------------------------------------------------------------
bool NHITextView::CanUndo(void) const
{	CFStringRef		cfString;
	bool			canUndo;



	// Check our state
	//
	// A non-NULL pointer is required on Mac OS X 10.4 (rdar://5254496).
	cfString = NULL;
	canUndo  = TXNCanUndoAction(*this, &cfString);
	
	return(canUndo);
}





//============================================================================
//		NHITextView::CanRedo : Can the last action be redone?
//----------------------------------------------------------------------------
bool NHITextView::CanRedo(void) const
{	CFStringRef		cfString;
	bool			canRedo;



	// Check our state
	//
	// A non-NULL pointer is required on Mac OS X 10.4 (rdar://5254496).
	cfString = NULL;
	canRedo  = TXNCanRedoAction(*this, &cfString);
	
	return(canRedo);
}





//============================================================================
//		NHITextView::GetBackgroundColor : Get the background color.
//----------------------------------------------------------------------------
NColor NHITextView::GetBackgroundColor(void) const
{	NColor			theValue;
	CGColorRef		cgColor;
	OSStatus		theErr;



	// Get the background color
	theErr = HITextViewCopyBackgroundColor(*this, &cgColor);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr == noErr && cgColor != NULL)
		theValue.SetColor(cgColor);
	
	return(theValue);
}





//============================================================================
//		NHITextView::SetBackgroundColor : Set the background color.
//----------------------------------------------------------------------------
void NHITextView::SetBackgroundColor(const NColor &theColor)
{	NCFObject		cgColor;
	OSStatus		theErr;



	// Set the background color
	cgColor.Set(theColor.CopyColor());

	theErr = HITextViewSetBackgroundColor(*this, cgColor);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHITextView::IsReadOnly : Get the read-only state.
//----------------------------------------------------------------------------
bool NHITextView::IsReadOnly(void) const
{	TXNControlData		txData;



	// Get our state
	GetTXNData(kTXNNoUserIOTag, txData);
	
	return((bool) txData.uValue);
}





//============================================================================
//		NHITextView::SetReadOnly : Set the read-only state.
//----------------------------------------------------------------------------
void NHITextView::SetReadOnly(bool readOnly)
{	TXNControlData		txData;



	// Set the state
	txData.uValue = (UInt32) readOnly;

	SetTXNData(kTXNNoUserIOTag, txData);
}





//============================================================================
//		NHITextView::IsSpellingEnabled : Get the spelling state.
//----------------------------------------------------------------------------
bool NHITextView::IsSpellingEnabled(void) const
{


	// Get the state
	return(GetCommandOptions() & kCommandSpelling);
}





//============================================================================
//		NHITextView::SetSpellingEnabled : Set the spelling state.
//----------------------------------------------------------------------------
void NHITextView::SetSpellingEnabled(bool theFlag)
{


	// Set the state
	if (theFlag)
		SetCommandOptions(kCommandSpelling);
	else
		SetCommandOptions(0, kCommandSpelling);
}





//============================================================================
//		NHITextView::GetSelection : Get the selection.
//----------------------------------------------------------------------------
CFRange NHITextView::GetSelection(void) const
{	TXNOffset		theStart, theEnd;
	CFRange			theSelection;



	// Get the selection
	TXNGetSelection(*this, &theStart, &theEnd);
	theSelection = CFRangeMake(theStart, theEnd - theStart);

	return(theSelection);
}





//============================================================================
//		NHITextView::SetSelection : Set the selection.
//----------------------------------------------------------------------------
void NHITextView::SetSelection(const CFRange &theSelection)
{	TXNOffset		theStart, theEnd;
	OSStatus		theErr;



	// Set the selection
	theStart = CFRangeGetFirst(theSelection);
	theEnd   = CFRangeGetLast( theSelection);
	
	theErr = TXNSetSelection(*this, theStart, theEnd);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHITextView::GetTextValue : Get the text value.
//----------------------------------------------------------------------------
//		Note :	We override NHIView's implementation, since HITextView only
//				supports kControlEditTextCFStringTag from Mac OS X 10.4.
//----------------------------------------------------------------------------
NString NHITextView::GetTextValue(void) const
{	NString			theValue;
	const UniChar	*textPtr;
	UInt32			textSize;
	NData			theData;



	// Get the value
	theData = GetTextData(kTXNUnicodeTextData);
	
	if (theData.IsValid())
		{
		textPtr  = (const UniChar *) theData.GetData();
		textSize = theData.GetSize() / sizeof(UniChar);
		
		theValue.Set(CFStringCreateWithCharacters(kCFAllocatorNano, textPtr, textSize));
		}

	return(theValue);
}





//============================================================================
//		NHITextView::SetTextValue : Set the text value.
//----------------------------------------------------------------------------
//		Note :	We override NHIView's implementation, since HITextView only
//				supports kControlEditTextCFStringTag from Mac OS X 10.4.
//----------------------------------------------------------------------------
void NHITextView::SetTextValue(const NString &theValue)
{	const UniChar	*textPtr;
	UInt32			textSize;
	NData			theData;



	// Prepare the data
	textPtr  = theValue.GetUTF16();
	textSize = theValue.GetSizeBytes(kCFStringEncodingUTF16);

	theData.AppendData(textSize, textPtr);



	// Set the value
	SetTextData(theData, kTXNUnicodeTextData);
}





//============================================================================
//		NHITextView::GetTextData : Get the text data.
//----------------------------------------------------------------------------
NData NHITextView::GetTextData(TXNDataType theFormat) const
{	NData			theData;
	CFDataRef		cfData;
	OSStatus		theErr;



	// Get the data
	//
	// TXNFlattenObjectToCFDataRef does not follow the CF model of Get/Copy/Create,
	// and we are assumed to take ownership of the returned string. 
	cfData = NULL;
	theErr = TXNFlattenObjectToCFDataRef(*this, theFormat, &cfData);
	NN_ASSERT_NOERR(theErr);
	
	if (cfData != NULL)
		theData.Set(cfData);
	
	return(theData);
}





//============================================================================
//		NHITextView::SetTextData : Set the text data.
//----------------------------------------------------------------------------
void NHITextView::SetTextData(const NData &theData, TXNDataType theFormat)
{	NFile		theFile;
	OSStatus	theErr;



	// Set RTF data
	//
	// TXNSetData does not support kTXNRichTextFormatData data (rdar://4512691),
	// and so for this format we have to write the data out to a temporary file.
	if (theFormat == kTXNRichTextFormatData)
		{
		theFile = NFileUtilities::GetTempFile("NHITextView.rtf");
		theErr  = NFileUtilities::SetFileData(theFile, theData);
		NN_ASSERT_NOERR(theErr);
			
		if (theErr == noErr)
			{
			theErr = Load(theFile);
			NN_ASSERT_NOERR(theErr);
			
			theFile.Delete();
			}
		}
	
	
	// Set other data
	else
		{
		theErr = TXNSetData(*this, theFormat, theData.GetData(), theData.GetSize(), kTXNStartOffset, kTXNEndOffset);
		NN_ASSERT_NOERR(theErr);
		}



	// Update our state
	ClearUndo();
}





//============================================================================
//		NHITextView::Load : Load a text file.
//----------------------------------------------------------------------------
OSStatus NHITextView::Load(const NFile &theFile)
{	OSStatus	theErr;



	// Validate our parameters
	NN_ASSERT(theFile.Exists());



	// Load the file
	theErr = TXNSetDataFromCFURLRef(*this, theFile.GetURL(), kTXNStartOffset, kTXNEndOffset);
	NN_ASSERT_NOERR(theErr);



	// Update our state
	ClearUndo();

	return(theErr);
}





//============================================================================
//		NHITextView::Save : Save a text file.
//----------------------------------------------------------------------------
OSStatus NHITextView::Save(const NFile &theFile)
{	FSSpec		theFSSpec;
	NFile		openFile;
	OSStatus	theErr;



	// Save an RTF file
	//
	// Although TXNWriteRangeToCFURL can write out RTF data, it requires 10.4
	// and so for now we write these files ourselves from their raw data.
	if (theFile.GetUTI() == kUTNTypeRTF)
		{
		theErr = NFileUtilities::SetFileData(theFile, GetTextData());
		NN_ASSERT_NOERR(theErr);
		}



	// Save a text file
	else
		{
		// Open the file
		openFile = theFile;
		theErr   = openFile.OpenFile(fsWrPerm, true);

		if (theErr != noErr)
			return(theErr);


		// Save the data
		theErr = TXNSave(*this, kTXNUnicodeTextFile,
							kTXNSingleStylePerTextDocumentResType,
							kTXNUnicodeEncoding, &theFSSpec,
							openFile.GetForkRef(), kInvalidForkRef);

		NN_ASSERT_NOERR(theErr);
		}



	// Update our state
	ClearUndo();

	return(theErr);
}





//============================================================================
//		NHITextView::GetActionCount : Get the action count.
//----------------------------------------------------------------------------
UInt32 NHITextView::GetActionCount(CFStringRef cfAction) const
{	ItemCount	theCount;
	OSStatus	theErr;



	// Get the action count
	theCount = 0;
	theErr   = TXNGetCountForActionType(*this, cfAction, &theCount);
	NN_ASSERT_NOERR(theErr);
	
	return(theCount);
}





//============================================================================
//		NHITextView::ClearActionCount : Clear the action count.
//----------------------------------------------------------------------------
void NHITextView::ClearActionCount(CFStringRef cfAction)
{	OSStatus	theErr;



	// Clear the action count
	theErr = TXNClearCountForActionType(*this, cfAction);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHITextView::GetCommandOptions : Get the command options.
//----------------------------------------------------------------------------
TXNCommandEventSupportOptions NHITextView::GetCommandOptions(void) const
{	TXNCommandEventSupportOptions		theFlags;
	OSStatus							theErr;



	// Get the options
	theFlags = 0;
	theErr   = TXNGetCommandEventSupport(*this, &theFlags);
	NN_ASSERT_NOERR(theErr);
	
	return(theFlags);
}





//============================================================================
//		NHITextView::SetCommandOptions : Set the command options.
//----------------------------------------------------------------------------
void NHITextView::SetCommandOptions(TXNCommandEventSupportOptions setThese, TXNCommandEventSupportOptions clearThese)
{	TXNCommandEventSupportOptions		theFlags;
	OSStatus							theErr;



	// Set the options
	theFlags = GetCommandOptions();
	
	theFlags |=    setThese;
	theFlags &= ~clearThese;
	
	theErr = TXNSetCommandEventSupport(*this, theFlags);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHITextView::GetAutoScroll : Get the auto-scroll mode.
//----------------------------------------------------------------------------
TXNAutoScrollBehavior NHITextView::GetAutoScroll(void) const
{	TXNControlData		txData;



	// Get our state
	GetTXNData(kTXNAutoScrollBehaviorTag, txData);
	
	return((TXNAutoScrollBehavior) txData.uValue);
}





//============================================================================
//		NHITextView::SetAutoScroll : Set the auto-scroll mode.
//----------------------------------------------------------------------------
void NHITextView::SetAutoScroll(TXNAutoScrollBehavior theMode)
{	TXNControlData		txData;



	// Set the state
	txData.uValue = (UInt32) theMode;

	SetTXNData(kTXNAutoScrollBehaviorTag, txData);
}





//============================================================================
//		NHITextView::GetMargin : Get a margin.
//----------------------------------------------------------------------------
float NHITextView::GetMargin(TextMargin theMargin) const
{	TXNMargins			txMargins;
	float				theValue;
	TXNControlData		txData;



	// Get the margin
	txData.marginsPtr = &txMargins;
	GetTXNData(kTXNMarginsTag, txData);
	
	switch (theMargin) {
		case kMarginTop:
			theValue = txMargins.topMargin;
			break;

		case kMarginLeft:
			theValue = txMargins.leftMargin;
			break;

		case kMarginBottom:
			theValue = txMargins.bottomMargin;
			break;

		case kMarginRight:
			theValue = txMargins.rightMargin;
			break;
		
		default:
			NN_LOG("Unknown margin: %d", theMargin);
			theValue = 0.0f;
			break;
		}
	
	return(theValue);
}





//============================================================================
//		NHITextView::SetMargin : Set a margin.
//----------------------------------------------------------------------------
void NHITextView::SetMargin(TextMargin theMargin, float theValue)
{	TXNMargins			txMargins;
	TXNControlData		txData;



	// Set the margins
	//
	// Note that although MLTE defines a bottom margin, setting it has no effect.
	txData.marginsPtr = &txMargins;
	GetTXNData(kTXNMarginsTag, txData);

	switch (theMargin) {
		case kMarginTop:
			txMargins.topMargin    = (SInt16) theValue;
			break;

		case kMarginLeft:
			txMargins.leftMargin   = (SInt16) theValue;
			break;

		case kMarginBottom:
			txMargins.bottomMargin = (SInt16) theValue;
			break;

		case kMarginRight:
			txMargins.rightMargin  = (SInt16) theValue;
			break;
		
		default:
			NN_LOG("Unknown margin: %d", theMargin);
			break;
		}

	SetTXNData(kTXNMarginsTag, txData);
}





//============================================================================
//		NHITextView::GetTXNData : Get a TXNControlData value.
//----------------------------------------------------------------------------
void NHITextView::GetTXNData(TXNControlTag theTag, TXNControlData &theData) const
{	OSStatus		theErr;



	// Get the data
	theErr = TXNGetTXNObjectControls(*this, 1, &theTag, &theData);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHITextView::SetTXNData : Set a TXNControlData value.
//----------------------------------------------------------------------------
void NHITextView::SetTXNData(TXNControlTag theTag, const TXNControlData &theData)
{	OSStatus	theErr;



	// Set the data
	theErr = TXNSetTXNObjectControls(*this, false, 1, &theTag, &theData);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHITextView::operator TXNObject : TXNObject-cast operator.
//----------------------------------------------------------------------------
NHITextView::operator TXNObject(void) const
{


	// Get the text object
	return(HITextViewGetTXNObject(*this));
}





//============================================================================
//		NHITextView::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NHITextView::InitializeView(void)
{	const EventTypeSpec	theEvents[] = { { kEventClassCommand, kEventCommandUpdateStatus },
										{ kEventClassNone,    kEventKindNone            } };



	// Initialize ourselves
	mModified = false;



	// Configure the view
	//
	// HITextView has most features disabled by default, so we enable useful options
	// to ensure that an NHITextView is more functional when dropped into a window.
	SetMargin(kMarginTop,    1.0f);
	SetMargin(kMarginBottom, 1.0f);
	SetMargin(kMarginLeft,   5.0f);
	SetMargin(kMarginRight,  5.0f);

	SetCommandOptions(kTXNSupportEditCommandProcessing       |
					  kTXNSupportEditCommandUpdating         |
					  kTXNSupportSpellCheckCommandProcessing |
					  kTXNSupportSpellCheckCommandUpdating   |
					  kTXNSupportFontCommandProcessing       |
					  kTXNSupportFontCommandUpdating);



	// Register for events
	AddEvents(theEvents);

	mTimer.AddTimer(BindSelf(NHITextView::UpdateModified), kTimeModified, kTimeModified);



	// Apply our properties
	if (HasProperty(                           kPropertyNano, kTagTextFile))
		Load(NBundleResource(GetPropertyString(kPropertyNano, kTagTextFile)));

	if (HasProperty(                                            kPropertyNano, kTagAutoScroll))
		SetAutoScroll((TXNAutoScrollBehavior) GetPropertyUInt32(kPropertyNano, kTagAutoScroll));

	if (HasProperty(                             kPropertyNano, kTagMarginTop))
		SetMargin(kMarginTop, GetPropertyFloat32(kPropertyNano, kTagMarginTop));

	if (HasProperty(                              kPropertyNano, kTagMarginLeft))
		SetMargin(kMarginLeft, GetPropertyFloat32(kPropertyNano, kTagMarginLeft));

	if (HasProperty(                               kPropertyNano, kTagMarginBottom))
		SetMargin(kMarginBottom, GetPropertyFloat32(kPropertyNano, kTagMarginBottom));

	if (HasProperty(                               kPropertyNano, kTagMarginRight))
		SetMargin(kMarginRight, GetPropertyFloat32(kPropertyNano, kTagMarginRight));



	// Update our state
	ClearUndo();
}





//============================================================================
//		NHITextView::GetCommandStatus : Get the status of a command.
//----------------------------------------------------------------------------
CommandStatus NHITextView::GetCommandStatus(const HICommandExtended &hiCmd)
{	CommandStatus	theStatus;
	NString			theValue;



	// Get the command status
	//
	// Work around rdar://5254459 - TXNSetActionNameMapper has no effect, and HITextView
	// updates the status of the undo/redo commands but will never change their text.
	//
	// This means if another window sets the undo item to "Can't Undo", and then
	// HITextView enables that item, the user will see an enabled "Can't Undo".
	//
	// To fix this, we override the MLTE handler for kEventCommandUpdateStatus and
	// update both the status and the text for the menu item.
	switch (hiCmd.commandID) {
		case kHICommandUndo:
			theStatus = EnableIf(CanUndo());
			theValue  = NUndoManager::GetCommandString(hiCmd.commandID, theStatus);
			break;

		case kHICommandRedo:
			theStatus = EnableIf(CanRedo());
			theValue  = NUndoManager::GetCommandString(hiCmd.commandID, theStatus);
			break;
		
		default:
			theStatus = NCarbonEventHandler::GetCommandStatus(hiCmd);
			break;
		}



	// Update the user interface
	//
	// NCarbonEventHandler will take care of enabling/disabling the menu
	// items for us, but we must update the text.
	if (theValue.IsNotEmpty())
		NMenuItem(hiCmd.commandID).SetText(theValue);

	return(theStatus);
}





//============================================================================
//		NHITextView::UpdateModified : Update the modified state.
//----------------------------------------------------------------------------
#pragma mark -
void NHITextView::UpdateModified(void)
{	bool	isModified;



	// Update the modified state
	//
	// Since HITextView does not provide an event when its state is changed,
	// we need to poll with a timer to be able to inform listeners when the
	// view's dirty state has been changed.
	//
	// As we do not use our window's NUndoManager, this message is often used
	// to update the is-modified state of the window containing the view.
	isModified = IsModified();
	
	if (isModified != mModified)
		{
		mModified = isModified;
		BroadcastMessage(kMsgTextViewChanged, this);
		}
}


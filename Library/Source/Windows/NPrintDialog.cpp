/*	NAME:
		NPrintDialog.cpp

	DESCRIPTION:
		Standard print dialog.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NGeometryUtilities.h"
#include "NArray.h"
#include "NData.h"
#include "NPrintDialog.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// state
static const NString kPrintDialogKey							= "NPrintDialog";
static const NString kPageFormatKey								= "PageFormat";

// Misc
static const EventTime kPrintDelay								= 0.5f;





//============================================================================
//		Static variables
//----------------------------------------------------------------------------
PMSheetDoneUPP NPrintDialog::mSheetDone = NewPMSheetDoneUPP(NPrintDialog::SheetDone);





//============================================================================
//		NPrintDialog::NPrintDialog : Constructor.
//----------------------------------------------------------------------------
NPrintDialog::NPrintDialog(void)
{


	// Initialize ourselves
	mParent      = NULL;
	mInsideSheet = false;

	mPageFormat    = NULL;
	mPrintSession  = NULL;
	mPrintSettings = NULL;
	
	mResult.printDialog = this;
	mResult.dialogKind  = kPrintDialogNone;
	mResult.didAccept   = false;
}





//============================================================================
//		NPrintDialog::~NPrintDialog : Destructor.
//----------------------------------------------------------------------------
NPrintDialog::~NPrintDialog(void)
{


	// Validate our state
	NN_ASSERT(!IsOpen());



	// Clean up
	DestroySession();
}





//============================================================================
//		NPrintDialog::IsOpen : Is the dialog open?
//----------------------------------------------------------------------------
bool NPrintDialog::IsOpen(void) const
{


	// Check our state
	return(mResult.dialogKind != kPrintDialogNone);
}





//============================================================================
//		NPrintDialog::Cancel : Cancel the dialog.
//----------------------------------------------------------------------------
void NPrintDialog::Cancel(void)
{


	// Validate our state
	NN_ASSERT(IsOpen());



	// Cancel the dialog
	DestroySession();
}





//============================================================================
//		NPrintDialog::GetParent : Get the parent.
//----------------------------------------------------------------------------
NHIWindow *NPrintDialog::GetParent(void) const
{


	// Get the parent
	return(mParent);
}





//============================================================================
//		NPrintDialog::SetParent : Set the parent.
//----------------------------------------------------------------------------
void NPrintDialog::SetParent(NHIWindow *theParent)
{


	// Validate our state
	NN_ASSERT(!IsOpen());



	// Set the parent
	mParent = theParent;
}





//============================================================================
//		NPrintDialog::CreateEventControlDraw : Create a print event.
//----------------------------------------------------------------------------
NCarbonEvent NPrintDialog::CreateEventControlDraw(void) const
{	NCarbonEvent		theEvent(kEventClassControl, kEventControlDraw);
	HIRect				paperRect, pageRect, contextRect;
	CGContextRef		cgContext;
	OSStatus			theErr;



	// Validate our state
	NN_ASSERT(mPrintSession != NULL);



	// Get the context
	theErr = PMSessionGetGraphicsContext(mPrintSession, kPMGraphicsContextCoreGraphics, (void **) &cgContext);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr != noErr)
		return(theEvent);



	// Prepare the context
	//
	// The context must be flipped to the normal HIView orientation,
	// then clipped to the printable area.
	paperRect   = GetPaperRect();
	pageRect    = GetPageRect();
	contextRect = CGRectMake(0, 0, pageRect.size.width, pageRect.size.height);
	
	CGContextTranslateCTM(cgContext, -paperRect.origin.x, -paperRect.origin.y);
	
	CGContextTranslateCTM(cgContext, 0.0f, pageRect.size.height);
	CGContextScaleCTM(    cgContext, 1.0f, -1.0f);
	
	CGContextClipToRect(cgContext, contextRect);



	// Prepare the event
	theEvent.SetCGContext(cgContext);
	
	theEvent.SetParameterBoolean(kEventNParamIsPrinting, true);
	theEvent.SetParameterHIRect( kEventNParamPageRect,   contextRect);

	return(theEvent);
}





//============================================================================
//		NPrintDialog::CreateSession : Create the print session.
//----------------------------------------------------------------------------
OSStatus NPrintDialog::CreateSession(void)
{	NArray			theArray;
	NPrintDialog	*thisPtr;
	NData			theData;
	OSStatus		theErr;



	// Initialize ourselves
	thisPtr = this;
	theErr  = noErr;



	// Create the print session
	if (theErr == noErr && mPrintSession == NULL)
		{
		theErr = PMCreateSession(&mPrintSession);
		NN_ASSERT_NOERR(theErr);
		
		if (theErr == noErr)
			{
			theArray.AppendValueString(kPMGraphicsContextCoreGraphics);
			
			theErr = PMSessionSetDocumentFormatGeneration(mPrintSession, kPMDocumentFormatDefault, theArray, NULL);
			NN_ASSERT_NOERR(theErr);
			}
		
		if (theErr == noErr)
			{
			theData.SetData(sizeof(thisPtr), &thisPtr);

			theErr = PMSessionSetDataInSession(mPrintSession, kPrintDialogKey, theData);
			NN_ASSERT_NOERR(theErr);
			}
		}



	// Create the page format
	if (theErr == noErr && mPageFormat == NULL)
		{
		// Restore the state
		if (mProperties.HasKey(kPageFormatKey))
			{
			theErr = PMUnflattenPageFormatWithCFData(mProperties.GetValueData(kPageFormatKey), &mPageFormat);
			NN_ASSERT_NOERR(theErr);
			}
		
		
		// Or use the defaults
		if (mPageFormat == NULL)
			{
			theErr = PMCreatePageFormat(&mPageFormat);
			NN_ASSERT_NOERR(theErr);
		
			if (theErr == noErr)
				{
				theErr = PMSessionDefaultPageFormat(mPrintSession, mPageFormat);
				NN_ASSERT_NOERR(theErr);
				}
			}
		}



	// Create the print settings
	if (theErr == noErr && mPrintSettings == NULL)
		{
		theErr = PMCreatePrintSettings(&mPrintSettings);
		NN_ASSERT_NOERR(theErr);
		
		if (theErr == noErr)
			{
			theErr = PMSessionDefaultPrintSettings(mPrintSession, mPrintSettings);
			NN_ASSERT_NOERR(theErr);
			}
		}



	// Validate the session
	if (theErr == noErr)
		ValidateSession();
	
	return(theErr);
}





//============================================================================
//		NPrintDialog::DestroySession : Destroy the print session.
//----------------------------------------------------------------------------
void NPrintDialog::DestroySession(void)
{	CFDataRef	cfData;
	OSStatus	theErr;



	// Save our state
	if (mPageFormat != NULL)
		{
		cfData = NULL;
		theErr = PMFlattenPageFormatToCFData(mPageFormat, &cfData);
		if (theErr == noErr && cfData != NULL)
			{
			mProperties.SetValueData(kPageFormatKey, cfData);
			CFSafeRelease(cfData);
			}
		}



	// Destroy the session
	if (mPrintSettings != NULL)
		{
		theErr = PMRelease(mPrintSettings);
		NN_ASSERT_NOERR(theErr);
		}

	if (mPageFormat != NULL)
		{
		theErr = PMRelease(mPageFormat);
		NN_ASSERT_NOERR(theErr);
		}

	if (mPrintSession != NULL)
		{
		theErr = PMRelease(mPrintSession);
		NN_ASSERT_NOERR(theErr);
		}



	// Reset our state
	mPageFormat    = NULL;
	mPrintSession  = NULL;
	mPrintSettings = NULL;
	
	mResult.printDialog = this;
	mResult.dialogKind  = kPrintDialogNone;
	mResult.didAccept   = false;
}





//============================================================================
//		NPrintDialog::GetPageFormat : Get the page format.
//----------------------------------------------------------------------------
PMPageFormat NPrintDialog::GetPageFormat(void) const
{


	// Get the page format
	return(mPageFormat);
}





//============================================================================
//		NPrintDialog::GetPrintSession : Get the print session.
//----------------------------------------------------------------------------
PMPrintSession NPrintDialog::GetPrintSession(void) const
{


	// Get the print session
	return(mPrintSession);
}





//============================================================================
//		NPrintDialog::GetPrintSettings : Get the print settings.
//----------------------------------------------------------------------------
PMPrintSettings NPrintDialog::GetPrintSettings(void) const
{


	// Get the print settings
	return(mPrintSettings);
}





//============================================================================
//		NPrintDialog::GetPaperRect : Get the paper rect.
//----------------------------------------------------------------------------
HIRect NPrintDialog::GetPaperRect(void) const
{	PMRect		printRect;
	HIRect		theRect;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(mPageFormat != NULL);



	// Get the paper rect
	theRect = kHIRectZero;
	theErr  = PMGetAdjustedPaperRect(mPageFormat, &printRect);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr == noErr)
		NGeometryUtilities::ConvertRect(printRect, theRect);
	
	return(theRect);
}





//============================================================================
//		NPrintDialog::GetPageRect : Get the page rect.
//----------------------------------------------------------------------------
HIRect NPrintDialog::GetPageRect(void) const
{	PMRect		printRect;
	HIRect		theRect;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(mPageFormat != NULL);



	// Get the page rect
	theRect = kHIRectZero;
	theErr  = PMGetAdjustedPageRect(mPageFormat, &printRect);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr == noErr)
		NGeometryUtilities::ConvertRect(printRect, theRect);
	
	return(theRect);
}





//============================================================================
//		NPrintDialog::GetOrientation : Get the orientation.
//----------------------------------------------------------------------------
PMOrientation NPrintDialog::GetOrientation(void) const
{	PMOrientation		theOrientation;
	OSStatus			theErr;



	// Validate our state
	NN_ASSERT(mPageFormat != NULL);



	// Get the orientation
	theErr = PMGetOrientation(mPageFormat, &theOrientation);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr != noErr)
		theOrientation = kPMPortrait;
	
	return(theOrientation);
}





//============================================================================
//		NPrintDialog::SetOrientation : Set the orientation.
//----------------------------------------------------------------------------
void NPrintDialog::SetOrientation(PMOrientation theOrientation)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(mPageFormat != NULL);



	// Set the orientation
	theErr = PMSetOrientation(mPageFormat, theOrientation, false);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NPrintDialog::GetResolution : Get the resolution.
//----------------------------------------------------------------------------
PMResolution NPrintDialog::GetResolution(void) const
{	PMResolution		theResolution;
	OSStatus			theErr;



	// Validate our state
	NN_ASSERT(mPageFormat != NULL);



	// Get the resolution
	theErr = PMGetResolution(mPageFormat, &theResolution);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr != noErr)
		memset(&theResolution, 0x00, sizeof(theResolution));
	
	return(theResolution);
}





//============================================================================
//		NPrintDialog::SetResolution : Set the resolution.
//----------------------------------------------------------------------------
void NPrintDialog::SetResolution(PMResolution theResolution)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(mPageFormat != NULL);



	// Set the resolution
	theErr = PMSetResolution(mPageFormat, &theResolution);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NPrintDialog::GetScale : Get the scale.
//----------------------------------------------------------------------------
double NPrintDialog::GetScale(void) const
{	double		theScale;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(mPageFormat != NULL);



	// Get the scale
	theErr = PMGetScale(mPageFormat, &theScale);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr != noErr)
		theScale = 0.0;
	
	return(theScale);
}





//============================================================================
//		NPrintDialog::SetScale : Set the Scale.
//----------------------------------------------------------------------------
void NPrintDialog::SetScale(double theScale)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(mPageFormat != NULL);



	// Set the scale
	theErr = PMSetScale(mPageFormat, theScale);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NPrintDialog::GetJobName : Get the job name.
//----------------------------------------------------------------------------
NString NPrintDialog::GetJobName(void) const
{	NString			theValue;
	CFStringRef		cfString;
	OSStatus		theErr;



	// Validate our state
	NN_ASSERT(mPrintSettings != NULL);



	// Get the job name
	//
	// Note that PMGetJobNameCFString does not follow the CF model of
	// Get/Copy/Create, and we take ownership of the returned string.
	cfString = NULL;
	theErr   = PMGetJobNameCFString(mPrintSettings, &cfString);
	NN_ASSERT_NOERR(theErr);
	
	if (cfString != NULL)
		theValue.Set(cfString);
	
	return(theValue);
}





//============================================================================
//		NPrintDialog::SetJobName : Set the job name.
//----------------------------------------------------------------------------
void NPrintDialog::SetJobName(const NString &theName)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(mPrintSettings != NULL);



	// Set the job name
	theErr = PMSetJobNameCFString(mPrintSettings, theName);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NPrintDialog::GetPageRage : Get the page range.
//----------------------------------------------------------------------------
void NPrintDialog::GetPageRange(UInt32 &firstPage, UInt32 &lastPage, bool activeRange) const
{	UInt32		rangeFirst, rangeLast;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(mPrintSettings != NULL);



	// Get the page range
	theErr = PMGetPageRange(mPrintSettings, &rangeFirst, &rangeLast);
	NN_ASSERT_NOERR(theErr);

	firstPage = rangeFirst;
	lastPage  = rangeLast;



	// Get the active range
	//
	// PMGetPageRange returns the maximum range of the job, as assigned by
	// an earlier call to SetPageRange.
	//
	// The user can change the active range to something outside this range by
	// editing the range fields, and we may need to clamp the results.
	if (activeRange)
		{
		theErr  = PMGetFirstPage(mPrintSettings, &firstPage);
		theErr |= PMGetLastPage( mPrintSettings,  &lastPage);
		NN_ASSERT_NOERR(theErr);
		
		firstPage = std::min(firstPage, rangeFirst);
		 lastPage = std::min( lastPage, rangeLast);
		}
}





//============================================================================
//		NPrintDialog::SetPageRange : Set the page range.
//----------------------------------------------------------------------------
void NPrintDialog::SetPageRange(UInt32 firstPage, UInt32 lastPage)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(mPrintSettings != NULL);



	// Set the page range
	theErr = PMSetPageRange(mPrintSettings, firstPage, lastPage);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NPrintDialog::GetProperties : Get the dialog properties.
//----------------------------------------------------------------------------
NDictionary NPrintDialog::GetProperties(void) const
{


	// Get the properties
	return(mProperties);
}





//============================================================================
//		NPrintDialog::SetProperties : Set the dialog properties.
//----------------------------------------------------------------------------
void NPrintDialog::SetProperties(const NDictionary &theProperties)
{


	// Set the properties
	mProperties = theProperties;
}





//============================================================================
//		NPrintDialog::DoPageSetup : Do the page setup dialog.
//----------------------------------------------------------------------------
OSStatus NPrintDialog::DoPageSetup(void)
{	Boolean		didAccept;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT_MSG(!mInsideSheet, "DoPageSetup can not be deferred");



	// Prepare the session
	theErr = PrepareSession(kPrintDialogPageSetup);
	NN_ASSERT_NOERR(theErr);



	// Open the dialog
	if (theErr == noErr)
		{
		theErr = PMSessionPageSetupDialog(mPrintSession, mPageFormat, &didAccept);
		NN_ASSERT_NOERR(theErr);
		
		if (theErr == noErr && mParent == NULL)
			theErr = FinishDialog(didAccept);
		}
	
	return(theErr);
}





//============================================================================
//		NPrintDialog::DoPrint : Do the print dialog.
//----------------------------------------------------------------------------
OSStatus NPrintDialog::DoPrint(void)
{	Boolean		didAccept;
	OSStatus	theErr;



	// Defer the dialog
	//
	// Starting with Mac OS X 10.4, a print dialog can no longer be invoked from
	// the SheetDone callback of a previous page setup dialog (rdar://4262579).
	//
	// From 10.4 onwards this will cause the print sheet to be attached to the
	// first non-sheeted window behind the sheet parent (i.e., some window that
	// was never passed to PMSessionUseSheets).
	//
	// To work around this, we defer the print dialog with a timer if necessary.
	if (mInsideSheet)
		{
		if (!mTimer.HasTimer())
			mTimer.AddTimer(BindSelf(NPrintDialog::DoPrint), kPrintDelay, kPrintDelay);

		return(noErr);
		}

	mTimer.RemoveTimer();



	// Prepare the session
	theErr = PrepareSession(kPrintDialogPrint);
	NN_ASSERT_NOERR(theErr);



	// Open the dialog
	if (theErr == noErr)
		{
		theErr = PMSessionPrintDialog(mPrintSession, mPrintSettings, mPageFormat, &didAccept);
		NN_ASSERT_NOERR(theErr);

		if (theErr == noErr && mParent == NULL)
			theErr = FinishDialog(didAccept);
		}
	
	return(theErr);
}





//============================================================================
//		NPrintDialog::PrepareSession : Prepare the session.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus NPrintDialog::PrepareSession(PrintDialogKind theKind)
{	OSStatus	theErr;



	// Create the session
	theErr = CreateSession();
	NN_ASSERT_NOERR(theErr);
	
	if (theErr != noErr)
		return(theErr);



	// Prepare the dialog
	mResult.dialogKind = theKind;

	if (mParent != NULL)
		{
		mParent->Select();
		
		theErr = PMSessionUseSheets(mPrintSession, *mParent, mSheetDone);
		NN_ASSERT_NOERR(theErr);
		}
	
	return(theErr);
}





//============================================================================
//		NPrintDialog::ValidateSession : Validate the session.
//----------------------------------------------------------------------------
void NPrintDialog::ValidateSession(void)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(mPageFormat   != NULL);
	NN_ASSERT(mPrintSession != NULL);



	// Validate the session
	theErr = PMSessionValidatePageFormat(mPrintSession, mPageFormat, NULL);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NPrintDialog::FinishDialog : Finish a modal dialog.
//----------------------------------------------------------------------------
OSStatus NPrintDialog::FinishDialog(bool didAccept)
{	OSStatus	theErr;



	// Prepare the result
	theErr = noErr;
	
	if (!didAccept)
		theErr = userCanceledErr;



	// Reset our state
	mResult.dialogKind = kPrintDialogNone;

	return(theErr);
}





//============================================================================
//		NPrintDialog::FinishSheet : Finish the sheet.
//----------------------------------------------------------------------------
void NPrintDialog::FinishSheet(bool didAccept)
{


	// Broadcast the result
	//
	// We maintain a flag indicating when we're finishing a sheet, in
	// case the message causes a further print dialog to be invoked.
	mResult.didAccept = didAccept;
	
	mInsideSheet = true;
	BroadcastMessage(kMsgPrintDialogResult, &mResult);
	mInsideSheet = false;



	// Reset our state
	mResult.dialogKind = kPrintDialogNone;
}





//============================================================================
//		NPrintDialog::SheetDone : Sheet completion callback.
//----------------------------------------------------------------------------
void NPrintDialog::SheetDone(PMPrintSession theSession, WindowRef /*theWindow*/, Boolean didAccept)
{	NPrintDialog	*thisPtr;
	CFDataRef		cfData;
	OSStatus		theErr;



	// Get the instance
	theErr = PMSessionGetDataFromSession(theSession, kPrintDialogKey, (CFTypeRef *) &cfData);
	NN_ASSERT_NOERR(theErr);
	
	NN_ASSERT(CFDataGetLength(cfData) == sizeof(thisPtr));
	memcpy(&thisPtr, CFDataGetBytePtr(cfData), sizeof(thisPtr));



	// Complete the dialog
	thisPtr->FinishSheet(didAccept);
}


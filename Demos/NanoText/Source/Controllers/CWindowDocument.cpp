/*	NAME:
		CWindowDocument.cpp

	DESCRIPTION:
		NanoText document window.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFBundle.h"

#include "NanoText Build.h"
#include "CApplication.h"
#include "CWindowDocument.h"





//============================================================================
//		CWindowDocument::CWindowDocument : Constructor.
//----------------------------------------------------------------------------
CWindowDocument::CWindowDocument(void)
{
}





//============================================================================
//		CWindowDocument::~CWindowDocument : Destructor.
//----------------------------------------------------------------------------
CWindowDocument::~CWindowDocument(void)
{
}





//============================================================================
//		CWindowDocument::InitializeProperties : Initialize the properties.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus CWindowDocument::InitializeProperties(const NDictionary *appProperties)
{	NDictionary		*theProperties;
	OSStatus		theErr;



	// Initialize the base class
	theErr = NDocument::InitializeProperties(appProperties);
	if (theErr != noErr)
		return(theErr);



	// Initialize the properties
	theProperties = GetProperties();

	theProperties->SetValueString(kDocFileUTIKey, kDocFileUTI);

	return(theErr);
}





//============================================================================
//		CWindowDocument::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
OSStatus CWindowDocument::InitializeSelf(void)
{	OSStatus		theErr;



	// Initialize the base class
	theErr = NDocument::InitializeSelf();
	if (theErr != noErr)
		return(theErr);



	// Install the toolbar
	mToolbar = new CToolbarDocument;
	mToolbar->Attach(GetWindow());

	return(theErr);
}





//============================================================================
//		CWindowDocument::DoMessage : Handle messages.
//----------------------------------------------------------------------------
void CWindowDocument::DoMessage(BroadcastMsg theMsg, void *msgData)
{


	// Handle the message
	switch (theMsg) {
		case kMsgTextViewChanged:
			SyncModified();
			break;
		
		default:
			NDocument::DoMessage(theMsg, msgData);
			break;
		}
}





//============================================================================
//		CWindowDocument::DoPrefChanged : Handle preference changes.
//----------------------------------------------------------------------------
void CWindowDocument::DoPrefChanged(NCFPreferences *thePrefs, const NString &theKey)
{	bool	changedAll;



	// Update our state
	changedAll = (theKey == kPrefChangedAllKey);
	
	if (changedAll || theKey == kPrefWindowBackgroundKey)
		mTextView->SetBackgroundColor(thePrefs->GetValueColor(kPrefWindowBackgroundKey));
}





//============================================================================
//		CWindowDocument::LoadSelf : Load ourselves.
//----------------------------------------------------------------------------
OSStatus CWindowDocument::LoadSelf(void)
{	OSStatus	theErr;



	// Load the file
	theErr = mTextView->Load(GetFile());
	NN_ASSERT_NOERR(theErr);

	return(theErr);
}





//============================================================================
//		CWindowDocument::SaveSelf : Save ourselves.
//----------------------------------------------------------------------------
OSStatus CWindowDocument::SaveSelf(void)
{	OSStatus	theErr;



	// Save the file
	theErr = mTextView->Save(GetFile());
	NN_ASSERT_NOERR(theErr);

	return(theErr);
}





//============================================================================
//		CWindowDocument::GetPrintView : Get the view to print.
//----------------------------------------------------------------------------
NHIView *CWindowDocument::GetPrintView(void)
{


	// Get the view to print
	return(mTextView);
}





//============================================================================
//		CWindowDocument::SyncModified : Sync the modified state.
//----------------------------------------------------------------------------
#pragma mark -
void CWindowDocument::SyncModified(void)
{


	// Sync the modified state
	//
	// Since NHITextView uses its own undo stack, we need to sync the document
	// state whenever the text view state is changed.
	SetModified(mTextView->IsModified());
}









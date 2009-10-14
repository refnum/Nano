/*	NAME:
		CSheetDocumentInfo.cpp

	DESCRIPTION:
		ÇPROJECTNAMEÈ document info sheet.
	
	COPYRIGHT:
		Copyright (c) ÇYEARÈ, ÇORGANIZATIONNAMEÈ

		All rights reserved.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "ÇPROJECTNAMEÈ Build.h"
#include "CSheetDocumentInfo.h"





//============================================================================
//		Controller
//----------------------------------------------------------------------------
DEFINE_WINDOW_CONTROLLER(CSheetDocumentInfo, kAppNibFile, "Document Info");





//============================================================================
//		CSheetDocumentInfo::CSheetDocumentInfo : Constructor.
//----------------------------------------------------------------------------
CSheetDocumentInfo::CSheetDocumentInfo(void)
{


	// Initialize ourselves
	mDocument = NULL;
}





//============================================================================
//		CSheetDocumentInfo::~CSheetDocumentInfo : Destructor.
//----------------------------------------------------------------------------
CSheetDocumentInfo::~CSheetDocumentInfo(void)
{
}





//============================================================================
//		CSheetDocumentInfo::CommandSheet : Handle a sheet command.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus CSheetDocumentInfo::CommandSheet(UInt32 theCmd)
{	DocumentInfo	theInfo;



	// Process the command
	if (theCmd == kHICommandOK)
		{
		theInfo.theAuthor      =      mAuthor->GetTextValue();
		theInfo.theDescription = mDescription->GetTextValue();
		
		if (theInfo != mDocument->GetInfo())
			mDocument->SetInfo(theInfo);
		}



	// Invoke the base class
	return(NWindowController::CommandSheet(theCmd));
}





//============================================================================
//		CSheetDocumentInfo::DoSheetOpening : A sheet is opening.
//----------------------------------------------------------------------------
void CSheetDocumentInfo::DoSheetOpening(NWindowController *theParent, NWindowController */*theSheet*/)
{	DocumentInfo	theInfo;



	// Obtain our document
	mDocument = dynamic_cast<CWindowDocument*>(theParent);
	NN_ASSERT(mDocument != NULL);



	// Initialize the sheet
	theInfo = mDocument->GetInfo();

	GetWindow()->SetKeyboardFocus(mAuthor);

	     mAuthor->SetTextValue(theInfo.theAuthor);
	mDescription->SetTextValue(theInfo.theDescription);
}

/*	NAME:
		CSheetDocumentInfo.h

	DESCRIPTION:
		ÇPROJECTNAMEÈ document info sheet.
	
	COPYRIGHT:
		Copyright (c) ÇYEARÈ, ÇORGANIZATIONNAMEÈ

		All rights reserved.
	__________________________________________________________________________
*/
#ifndef CSHEETDOCUMENTINFO_HDR
#define CSHEETDOCUMENTINFO_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NWindowController.h"
#include "NEditText.h"

#include "CWindowDocument.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class CSheetDocumentInfo : public NWindowController {
public:
										DECLARE_WINDOW_CONTROLLER(CSheetDocumentInfo);

										CSheetDocumentInfo( void);
	virtual								~CSheetDocumentInfo(void);


protected:
	// Handle commands
	OSStatus							CommandSheet(UInt32 theCmd);


	// Handle sheet notifications
	void								DoSheetOpening(NWindowController *theParent, NWindowController *theSheet);


private:
	NIB_VIEW('auth', NEditText,			Author);
	NIB_VIEW('desc', NEditText,			Description);
	
	CWindowDocument						*mDocument;
};





#endif // CSHEETDOCUMENTINFO_HDR



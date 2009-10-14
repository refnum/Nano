/*	NAME:
		CToolbarDocument.h

	DESCRIPTION:
		NanoWeb document toolbar.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef CTOOLBARDOCUMENT_HDR
#define CTOOLBARDOCUMENT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NChasingArrows.h"
#include "NEditText.h"
#include "NHIToolbar.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class CToolbarDocument : public NHIToolbar {
public:
										CToolbarDocument(void);
	virtual								~CToolbarDocument(void);


	// Get the address field
	NEditText							*GetAddress(void);


	// Get the progress arrows
	NChasingArrows						*GetProgress(void);


protected:
	// Get the toolbar contents
	HIToolbarItemInfoList				GetToolbarContents(void);


	// Create a toolbar item view
	NHIView								*CreateItemView(const NHIToolbarItem *theItem);


private:


};





#endif // CTOOLBARDOCUMENT_HDR



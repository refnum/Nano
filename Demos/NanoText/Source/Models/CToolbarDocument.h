/*	NAME:
		CToolbarDocument.h

	DESCRIPTION:
		NanoText document toolbar.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef CTOOLBARDOCUMENT_HDR
#define CTOOLBARDOCUMENT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIToolbar.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class CToolbarDocument : public NHIToolbar {
public:
										CToolbarDocument(void);
	virtual								~CToolbarDocument(void);


protected:
	// Get the toolbar contents
	HIToolbarItemInfoList				GetToolbarContents(void);


private:


};





#endif // CTOOLBARDOCUMENT_HDR



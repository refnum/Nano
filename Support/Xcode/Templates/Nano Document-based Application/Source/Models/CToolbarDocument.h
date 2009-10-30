/*	NAME:
		CToolbarDocument.h

	DESCRIPTION:
		ÇPROJECTNAMEÈ document toolbar.
	
	COPYRIGHT:
		Copyright (c) ÇYEARÈ, ÇORGANIZATIONNAMEÈ

		All rights reserved.
	__________________________________________________________________________
*/
#ifndef CTOOLBARDOCUMENT_HDR
#define CTOOLBARDOCUMENT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NPopupButton.h"
#include "NColorPicker.h"
#include "NHIToolbar.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class CToolbarDocument : public NHIToolbar {
public:
										CToolbarDocument(void);
	virtual								~CToolbarDocument(void);


	// Get the color picker
	NColorPicker						*GetColorPicker(void);


protected:
	// Get the toolbar contents
	HIToolbarItemInfoList				GetToolbarContents(void);


	// Create a toolbar item view
	NHIView								*CreateItemView(const NHIToolbarItem *theItem);


private:


};





#endif // CTOOLBARDOCUMENT_HDR



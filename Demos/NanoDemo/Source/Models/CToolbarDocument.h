/*	NAME:
		CToolbarDocument.h

	DESCRIPTION:
		NanoDemo document toolbar.
	
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


	// Get the shape menu
	NPopupButton						*GetShapeMenu(void);


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



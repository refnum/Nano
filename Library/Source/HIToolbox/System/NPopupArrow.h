/*	NAME:
		NPopupArrow.h

	DESCRIPTION:
		Popup arrow control.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NPOPUPARROW_HDR
#define NPOPUPARROW_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NPopupArrow : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NPopupArrow);

										NPopupArrow(HIWindowRef						theWindow,
													const HIRect					&theRect,
													ControlPopupArrowOrientation	theOrientation = kControlPopupArrowOrientationSouth,
													ControlPopupArrowSize			theSize        = kControlPopupArrowSizeNormal);
	virtual								~NPopupArrow(void);


private:


};





#endif // NPOPUPARROW_HDR



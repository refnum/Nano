/*	NAME:
		NDisclosureTriangle.h

	DESCRIPTION:
		Disclosure triangle control.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDISCLOSURETRIANGLE_HDR
#define NDISCLOSURETRIANGLE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDisclosureTriangle : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NDisclosureTriangle);

										NDisclosureTriangle(HIWindowRef								theWindow,
															const HIRect							&theRect,
															const NString							&theTitle      = "",
															ControlDisclosureTriangleOrientation	theOrientation = kControlDisclosureTrianglePointDefault,
															bool									isOpen         = false,
															bool									autoToggle     = true);

	virtual								~NDisclosureTriangle(void);


	// Get/set the open state
	bool								IsOpen(void) const;
	void								SetOpen(bool isOpen);


private:


};





#endif // NDISCLOSURETRIANGLE_HDR



/*	NAME:
		NIconView.h

	DESCRIPTION:
		Icon view.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NICONVIEW_HDR
#define NICONVIEW_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NIconView : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NIconView);

										NIconView(HIWindowRef		theWindow,
													const HIRect	&theRect,
													const NIcon		&theIcon,
													bool			trackMouse = false);

	virtual								~NIconView(void);


	// Get/set the icon
	NIcon								GetIcon(void) const;
	void								SetIcon(const NIcon &theIcon);


	// Get/set the transform
	IconTransformType					GetTransform(void) const;
	void								SetTransform(IconTransformType theTransform);


	// Get/set the alignment
	IconAlignmentType					GetAlignment(void) const;
	void								SetAlignment(IconAlignmentType theAlignment);


protected:
	// Initialize the view
	void								InitializeView(void);


private:


};





#endif // NIconView_HDR



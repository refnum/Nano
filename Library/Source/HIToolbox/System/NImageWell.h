/*	NAME:
		NImageWell.h

	DESCRIPTION:
		Image well control.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NIMAGEWELL_HDR
#define NIMAGEWELL_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NImageWell : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NImageWell);

										NImageWell(HIWindowRef		theWindow,
													const HIRect	&theRect,
													const NIcon		&theIcon);

										NImageWell(HIWindowRef		theWindow,
													const HIRect	&theRect,
													const NCGImage	&theImage);

	virtual								~NImageWell(void);


	// Get/set the icon
	NIcon								GetIcon(void) const;
	void								SetIcon(const NIcon &theIcon);


	// Get/set the image
	//
	// Image support requires Mac OS X 10.4 or later.
	NCGImage							GetImage(void) const;
	void								SetImage(const NCGImage &theImage);


	// Get/set the transform
	IconTransformType					GetTransform(void) const;
	void								SetTransform(IconTransformType theTransform);


protected:
	// Initialize the view
	void								InitializeView(void);


private:


};





#endif // NIMAGEWELL_HDR



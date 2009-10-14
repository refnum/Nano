/*	NAME:
		CExampleView.h

	DESCRIPTION:
		Example view.
	
	COPYRIGHT:
		Copyright (c) ÇYEARÈ, ÇORGANIZATIONNAMEÈ

		All rights reserved.
	__________________________________________________________________________
*/
#ifndef CEXAMPLEVIEW_HDR
#define CEXAMPLEVIEW_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NColor.h"
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class CExampleView : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(CExampleView);

										CExampleView(HIWindowRef	theWindow,
													const HIRect	&theRect,
													const NColor	&theColor = kColorRed);

	virtual								~CExampleView(void);


	// Get/set the color
	NColor								GetColor(void) const;
	void								SetColor(const NColor &theColor);


	// Encode/decode the properties
	OSStatus							EncodeProperties(      NDictionary &theProperties);
	OSStatus							DecodeProperties(const NDictionary &theProperties);


protected:
	// Initialize the view
	void								InitializeView(void);


	// Handle events
	OSStatus							DoEventControlDraw(NCarbonEvent &theEvent);


private:
	NColor								mColor;
};





#endif // CEXAMPLEVIEW_HDR



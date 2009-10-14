/*	NAME:
		NReflectionView.h

	DESCRIPTION:
		Reflection view.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
#ifndef NREFLECTIONVIEW_HDR
#define NREFLECTIONVIEW_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NReflectionView : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NReflectionView);

										NReflectionView(HIWindowRef			theWindow,
															const HIRect	&theRect,
															const HIViewID	&theID);

	virtual								~NReflectionView(void);


	// Get/set the alpha
	float								GetAlpha(void) const;
	void								SetAlpha(float theValue);
	

	// Get/set the source
	HIViewID							GetSource(void) const;
	void								SetSource(const HIViewID &theID);


protected:
	// Initialize the view
	void								InitializeView(void);


	// Handle events
	OSStatus							DoEventControlBoundsChanged(NCarbonEvent &theEvent);
	OSStatus							DoEventControlDraw(         NCarbonEvent &theEvent);


private:
	void								UpdateMask(void);


private:
	float								mAlpha;
	HIViewID							mSourceID;
	
	bool								mMaskValid;
	NCGImage							mMask;
};





#endif // NREFLECTIONVIEW_HDR



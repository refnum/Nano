/*	NAME:
		NRelevanceBar.h

	DESCRIPTION:
		Relevance bar control.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NRELEVANCEBAR_HDR
#define NRELEVANCEBAR_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NRelevanceBar : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NRelevanceBar);

										NRelevanceBar(HIWindowRef		theWindow,
														const HIRect	&theRect,
														SInt32			valueInitial    = 0,
														SInt32			valueMinimum    = 0,
														SInt32			valueMaximum    = 100);
	virtual								~NRelevanceBar(void);


private:


};





#endif // NRELEVANCEBAR_HDR



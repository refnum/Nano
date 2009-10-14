/*	NAME:
		NRadioGroup.h

	DESCRIPTION:
		Radio button group control.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NRADIOGROUP_HDR
#define NRADIOGROUP_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NRadioGroup : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NRadioGroup);

										NRadioGroup(HIWindowRef theWindow, const HIRect &theRect);

	virtual								~NRadioGroup(void);


private:


};





#endif // NRADIOGROUP_HDR



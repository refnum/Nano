/*	NAME:
		NPlacardView.h

	DESCRIPTION:
		Placard view.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NPLACARDVIEW_HDR
#define NPLACARDVIEW_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NPlacardView : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NPlacardView);

										NPlacardView(HIWindowRef theWindow, const HIRect &theRect);
	virtual								~NPlacardView(void);


private:


};





#endif // NPLACARDVIEW_HDR



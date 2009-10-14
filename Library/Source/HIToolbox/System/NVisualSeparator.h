/*	NAME:
		NVisualSeparator.h

	DESCRIPTION:
		Visual separator.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NVISUALSEPARATOR_HDR
#define NVISUALSEPARATOR_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NVisualSeparator : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NVisualSeparator);

										NVisualSeparator(HIWindowRef theWindow, const HIRect &theRect);
	virtual								~NVisualSeparator(void);


private:


};





#endif // NVISUALSEPARATOR_HDR



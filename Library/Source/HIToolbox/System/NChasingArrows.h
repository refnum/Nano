/*	NAME:
		NChasingArrows.h

	DESCRIPTION:
		Chasing arrows control.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCHASINGARROWS_HDR
#define NCHASINGARROWS_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NChasingArrows : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NChasingArrows);

										NChasingArrows(HIWindowRef theWindow, const HIRect &theRect, bool isAnimating=true);
	virtual								~NChasingArrows(void);


	// Get/set the animation state
	bool								IsAnimating(void) const;
	void								SetAnimating(bool isAnimating);


private:


};





#endif // NCHASINGARROWS_HDR



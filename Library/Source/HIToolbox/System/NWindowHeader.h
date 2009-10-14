/*	NAME:
		NWindowHeader.h

	DESCRIPTION:
		Window header view.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NWINDOWHEADER_HDR
#define NWINDOWHEADER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NWindowHeader : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NWindowHeader);

										NWindowHeader(HIWindowRef		theWindow,
														const HIRect	&theRect,
														bool			isList = false);
	virtual								~NWindowHeader(void);


	// Get/set the list header state
	bool								IsListHeader(void) const;
	void								SetListHeader(bool isList);


private:


};





#endif // NWINDOWHEADER_HDR



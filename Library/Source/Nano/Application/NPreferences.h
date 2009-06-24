/*	NAME:
		NPreferences.h

	DESCRIPTION:
		Preferences.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NPREFERENCES_HDR
#define NPREFERENCES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDictionary.h"
#include "NBroadcaster.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const BroadcastMsg kMsgPreferenceValueChanged				= 0x7076616C;

static const NString kPrefChangedAllKey								= "*";





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NPreferences : public NBroadcaster {
public:
										 NPreferences(void);
	virtual								~NPreferences(void);


	// Save the preferences
	void								Save(void);


	// Get the preferences
	static NPreferences					*Get(void);


private:


};





#endif // NPREFERENCES_HDR



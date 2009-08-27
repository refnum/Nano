/*	NAME:
		NLocale.h

	DESCRIPTION:
		System locale.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NLOCALE_HDR
#define NLOCALE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const NString kNLocaleUser									= "*user*";





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NLocale {
public:
										NLocale(const NString &theID=kNLocaleUser);
	virtual							   ~NLocale(void);


	// Is the measurement system metric?
	bool								IsMetric(void) const;


private:
	NString								mID;
};





#endif // NLOCALE_HDR



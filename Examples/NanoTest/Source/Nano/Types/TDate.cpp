/*	NAME:
		TDate.cpp

	DESCRIPTION:
		NDate tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDate.h"

#include "TDate.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NGregorianDate kValueDate1								= { 2010, 6, 1, 18, 21, 57 };





//============================================================================
//		TDate::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TDate::Execute(void)
{	NGregorianDate		gregDate;
	NDate				date1;



	// Get the state we need
	date1 = NDate(kValueDate1, kNTimeZoneUTC);



	// Conversion
	gregDate = date1.GetGregorianDate(kNTimeZoneUTC);
	NN_ASSERT(memcmp(&gregDate, &kValueDate1, sizeof(gregDate)) == 0);
}



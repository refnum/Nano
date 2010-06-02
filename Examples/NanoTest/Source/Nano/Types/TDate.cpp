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
static const NGregorianDate kValueDate1								= { 2010, 6, 1, 18, 21, 57, kNTimeZoneUTC  };
static const NGregorianDate kValueDate2								= { 2010, 6, 1, 11, 21, 57, kNTimeZonePDT  };
static const NGregorianDate kValueDate3								= { 2010, 6, 1, 20, 21, 57, kNTimeZoneCEST };





//============================================================================
//		TDate::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TDate::Execute(void)
{	NGregorianDate		gregDate;
	NDate				date1;



	// Get the state we need
	date1 = NDate(kValueDate1);



	// Conversion
	gregDate = date1.GetDate(kNTimeZoneUTC);
	NN_ASSERT(NDate(gregDate) == NDate(kValueDate1));

	gregDate = date1.GetDate(kNTimeZonePDT);
	NN_ASSERT(NDate(gregDate) == NDate(kValueDate2));

	gregDate = date1.GetDate(kNTimeZoneCEST);
	NN_ASSERT(NDate(gregDate) == NDate(kValueDate3));
}



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
static const NGregorianDate kTestDate1								= { 2010, 6, 1, 18, 21, 57, kNTimeZoneUTC  };
static const NGregorianDate kTestDate2								= { 2010, 6, 1, 11, 21, 57, kNTimeZonePDT  };
static const NGregorianDate kTestDate3								= { 2010, 6, 1, 20, 21, 57, kNTimeZoneCEST };





//============================================================================
//		TDate::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TDate::Execute(void)
{	NGregorianDate		gregDate;
	NDate				date1;
	NIndex				n, m;



	// Get the state we need
	date1 = NDate(kTestDate1);



	// Conversion
	gregDate = date1.GetDate(kNTimeZoneUTC);
	NN_ASSERT(NDate(gregDate) == NDate(kTestDate1));

	gregDate = date1.GetDate(kNTimeZonePDT);
	NN_ASSERT(NDate(gregDate) == NDate(kTestDate2));

	gregDate = date1.GetDate(kNTimeZoneCEST);
	NN_ASSERT(NDate(gregDate) == NDate(kTestDate3));



	// Indices
	n = date1.GetDayOfWeek();
	NN_ASSERT(n == 2);

	n = date1.GetDayOfYear();
	NN_ASSERT(n == 152);

	n = date1.GetWeekOfYear();
	NN_ASSERT(n == 22);

	gregDate       = kTestDate1;
	gregDate.month = 1;

	for (m = 1; m <= 10; m++)
		{
		gregDate.day = (SInt8) m;
		n            = NDate(gregDate).GetWeekOfYear();
		NN_ASSERT(n == 1);
		}

	gregDate.day = 11;
	n            = NDate(gregDate).GetWeekOfYear();
	NN_ASSERT(n == 2);
}


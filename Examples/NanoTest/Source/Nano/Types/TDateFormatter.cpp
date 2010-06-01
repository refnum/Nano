/*	NAME:
		TDateFormatter.cpp

	DESCRIPTION:
		NDateFormatter tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDateFormatter.h"

#include "TDateFormatter.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NGregorianDate kValueDate1								= { 2010, 6, 1, 18, 21, 57 };
static const NGregorianDate kValueDate2								= { -123, 6, 1, 18, 21, 57 };





//============================================================================
//		TDateFormatter::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TDateFormatter::Execute(void)
{	NDate		date1, date2;



	// Get the state we need
	date1 = NDate(kValueDate1, kNTimeZoneUTC);
	date2 = NDate(kValueDate2, kNTimeZoneUTC);



	// Quotes
	NN_ASSERT(date1.GetString("G''G",		kNTimeZoneUTC)	== "AD'AD");
	NN_ASSERT(date1.GetString("G'test'G",	kNTimeZoneUTC)	== "ADtestAD");
	NN_ASSERT(date1.GetString("G'testG",	kNTimeZoneUTC)	== "ADtestG");



	// Era
	NN_ASSERT(date1.GetString("G",		kNTimeZoneUTC) == "AD");
	NN_ASSERT(date1.GetString("GG",		kNTimeZoneUTC) == "AD");
	NN_ASSERT(date1.GetString("GGG",	kNTimeZoneUTC) == "AD");
	NN_ASSERT(date1.GetString("GGGG",	kNTimeZoneUTC) == "Anno Domini");
	NN_ASSERT(date1.GetString("GGGGG",	kNTimeZoneUTC) == "A");

	NN_ASSERT(date2.GetString("G",		kNTimeZoneUTC) == "BC");
	NN_ASSERT(date2.GetString("GG",		kNTimeZoneUTC) == "BC");
	NN_ASSERT(date2.GetString("GGG",	kNTimeZoneUTC) == "BC");
	NN_ASSERT(date2.GetString("GGGG",	kNTimeZoneUTC) == "Before Christ");
	NN_ASSERT(date2.GetString("GGGGG",	kNTimeZoneUTC) == "B");
}



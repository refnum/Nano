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
static const NGregorianDate kValueDate1								= { 2010, 6, 1, 18, 21, 57, kNTimeZoneUTC };
static const NGregorianDate kValueDate2								= { -123, 6, 1, 18, 21, 57, kNTimeZoneUTC };





//============================================================================
//		TDateFormatter::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TDateFormatter::Execute(void)
{	NDateFormatter		theFormatter;
	NString				theResult;



	// Quotes
	theResult = theFormatter.Format(kValueDate1, "G''G");
	NN_ASSERT(theResult == "AD'AD");

	theResult = theFormatter.Format(kValueDate1, "G'test'G");
	NN_ASSERT(theResult == "ADtestAD");
	
	theResult = theFormatter.Format(kValueDate1, "G'testG");
	NN_ASSERT(theResult == "ADtestG");



	// Era
	theResult = theFormatter.Format(kValueDate1, "G");
	NN_ASSERT(theResult == "AD");
	
	theResult = theFormatter.Format(kValueDate1, "GG");
	NN_ASSERT(theResult == "AD");
	
	theResult = theFormatter.Format(kValueDate1, "GGG");
	NN_ASSERT(theResult == "AD");
	
	theResult = theFormatter.Format(kValueDate1, "GGGG");
	NN_ASSERT(theResult == "Anno Domini");
	
	theResult = theFormatter.Format(kValueDate1, "GGGGG");
	NN_ASSERT(theResult == "A");

	
	theResult = theFormatter.Format(kValueDate2, "G");
	NN_ASSERT(theResult == "BC");
	
	theResult = theFormatter.Format(kValueDate2, "GG");
	NN_ASSERT(theResult == "BC");
	
	theResult = theFormatter.Format(kValueDate2, "GGG");
	NN_ASSERT(theResult == "BC");
	
	theResult = theFormatter.Format(kValueDate2, "GGGG");
	NN_ASSERT(theResult == "Before Christ");
	
	theResult = theFormatter.Format(kValueDate2, "GGGGG");
	NN_ASSERT(theResult == "B");



	// Year
	theResult = theFormatter.Format(kValueDate1, "y");
	NN_ASSERT(theResult == "2010");

	theResult = theFormatter.Format(kValueDate1, "yy");
	NN_ASSERT(theResult == "10");

	theResult = theFormatter.Format(kValueDate1, "yyy");
	NN_ASSERT(theResult == "2010");

	theResult = theFormatter.Format(kValueDate1, "yyyy");
	NN_ASSERT(theResult == "2010");

	theResult = theFormatter.Format(kValueDate1, "yyyyy");
	NN_ASSERT(theResult == "02010");


	theResult = theFormatter.Format(kValueDate1, "YYYYY");
	NN_ASSERT(theResult == "02010");

	theResult = theFormatter.Format(kValueDate1, "uuuuu");
	NN_ASSERT(theResult == "02010");
}



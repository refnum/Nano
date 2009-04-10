/*	NAME:
		TString.cpp

	DESCRIPTION:
		NString tests.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"

#include "TString.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const char   *kValueCharPtr									= "test string";
static const NString kValueString									= "another test string";





//============================================================================
//		TString::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TString::Execute(void)
{	NString		testString1, testString2;



	// Construction
	testString1 = kValueCharPtr;
	NN_ASSERT(testString1.GetSize() == (NIndex) strlen(kValueCharPtr));
	NN_ASSERT(strcmp(testString1.GetUTF8(), kValueCharPtr) == 0);

	testString1 = kValueString;
	NN_ASSERT(testString1.GetSize() == (NIndex) strlen(kValueString.GetUTF8()));
	NN_ASSERT(strcmp(testString1.GetUTF8(), kValueString.GetUTF8()) == 0);



	// Size
	

	
	// Assignment
	
	
	
	// Find
	
	
	
	
	// Replace
	
	
	
	// Contents
	
	
	
	// Comparisons
	testString1 = "test";
	testString2 = "test";
	NN_ASSERT(testString1 == testString2);

	testString1 = "test";
	testString2 = "texx";
	NN_ASSERT(testString1 <  testString2);
	NN_ASSERT(testString1 != testString2);
	NN_ASSERT(testString2 >  testString1);

	testString1 = "test";
	testString2 = "TeSt";
	NN_ASSERT(testString1.Compare(testString2, kNStringNone)   == kNCompareGreaterThan);
	NN_ASSERT(testString1.Compare(testString2, kNStringNoCase) == kNCompareEqualTo);

	NN_ASSERT(NString("foo2.txt"	).Compare("foo7.txt",	kNStringNumeric) == kNCompareLessThan);
	NN_ASSERT(NString("foo7.txt"	).Compare("foo2.txt",	kNStringNumeric) == kNCompareGreaterThan);
	NN_ASSERT(NString("foo002.txt"	).Compare("foo007.txt",	kNStringNumeric) == kNCompareLessThan);
	NN_ASSERT(NString("123"			).Compare("0123",		kNStringNumeric) == kNCompareLessThan);
	NN_ASSERT(NString("99"			).Compare("0123",		kNStringNumeric) == kNCompareLessThan);
	NN_ASSERT(NString("010"			).Compare("0002",		kNStringNumeric) == kNCompareGreaterThan);
	NN_ASSERT(NString("0100"		).Compare("20",			kNStringNumeric) == kNCompareGreaterThan);



	// Extraction

	
	
	// Manipulation
	
	
	
	// Formatting
	
	
	
	// Operators
	
	


}

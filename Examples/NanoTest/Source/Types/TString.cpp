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
static const char *kTestString1									= "test string";
static const char *kTestString2									= "another test string";





//============================================================================
//		TString::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TString::Execute(void)
{	NString		testString;



	// Execute the tests
	testString = kTestString1;
	NN_ASSERT(testString.GetSize() == strlen(kTestString1));
	NN_ASSERT(strcmp(testString.GetUTF8(), kTestString1) == 0);

	testString = kTestString2;
	NN_ASSERT(testString.GetSize() == strlen(kTestString2));
	NN_ASSERT(strcmp(testString.GetUTF8(), kTestString2) == 0);
}

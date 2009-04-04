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
{	NString		testString;



	// Execute the tests
	testString = kValueCharPtr;
	NN_ASSERT(testString.GetSize() == (NIndex) strlen(kValueCharPtr));
	NN_ASSERT(strcmp(testString.GetUTF8(), kValueCharPtr) == 0);

	testString = kValueString;
	NN_ASSERT(testString.GetSize() == (NIndex) strlen(kValueString.GetUTF8()));
	NN_ASSERT(strcmp(testString.GetUTF8(), kValueString.GetUTF8()) == 0);
}

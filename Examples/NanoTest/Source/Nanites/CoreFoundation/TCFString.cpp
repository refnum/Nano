/*	NAME:
		TCFString.cpp

	DESCRIPTION:
		NCFString tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFString.h"

#include "TCFString.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString     kTestNString								= "Hello World";
static const CFStringRef kTestCFString								= CFSTR("Hello World");





//============================================================================
//		TCFString::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TCFString::Execute(void)
{	NCFString		theString;
	NCFObject		cfString;



	// Execute the tests
	theString = kTestNString;
	cfString  = theString.GetObject();
	NN_ASSERT(CFStringCompare(cfString, kTestCFString, 0) == kCFCompareEqualTo);

	theString = NCFString(kTestCFString, false);
	NN_ASSERT(theString == kTestNString);

	theString.SetObject(kTestCFString, false);
	NN_ASSERT(theString == kTestNString);

	theString.SetObject(CFStringCreateCopy(kCFAllocatorDefault, kTestCFString), true);
	NN_ASSERT(theString == kTestNString);
}


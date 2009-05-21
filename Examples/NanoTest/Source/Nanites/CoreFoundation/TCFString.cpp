/*	NAME:
		TCFString.cpp

	DESCRIPTION:
		NCFString tests.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
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
static const NString     kNStringValue						= "Hello World";
static const CFStringRef kCFStringValue						= CFSTR("Hello World");





//============================================================================
//		TCFString::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TCFString::Execute(void)
{	NCFString		theString;
	CFStringRef		cfString;



	// Execute the tests
	theString = kNStringValue;
	cfString  = theString;
	NN_ASSERT(CFStringCompare(cfString, kCFStringValue, 0) == kCFCompareEqualTo);

	theString = NCFString(kCFStringValue, false);
	NN_ASSERT(theString == kNStringValue);

	theString.Set(kCFStringValue, false);
	NN_ASSERT(theString == kNStringValue);

	theString.Set(CFStringCreateCopy(kCFAllocatorDefault, kCFStringValue), true);
	NN_ASSERT(theString == kNStringValue);
}


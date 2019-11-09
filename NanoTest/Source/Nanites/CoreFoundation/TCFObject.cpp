/*	NAME:
		TCFObject.cpp

	DESCRIPTION:
		NCFObject tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTestFixture.h"
#include "NCFObject.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NCFOBJECT(...)											TEST_NANO(TCFObject, ##__VA_ARGS__)

FIXTURE_NANO(TCFObject)
{
	NCFObject		theArray;
	CFArrayRef		cfArray;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NCFOBJECT("Retain", "[cf]")
{


	// Perform the test
	cfArray = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);
	REQUIRE(CFGetRetainCount(cfArray) == 1);
	
	CFSafeRetain(cfArray);
	REQUIRE(CFGetRetainCount(cfArray) == 2);

	CFRelease(cfArray);
	REQUIRE(CFGetRetainCount(cfArray) == 1);

	CFSafeRelease(cfArray);
	REQUIRE(cfArray == NULL);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NCFOBJECT("NULL", "[cf]")
{


	// Perform the test
	cfArray = NULL;
	CFSafeRetain( cfArray);
	CFSafeRelease(cfArray);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NCFOBJECT("Assignment", "[cf]")
{


	// Perform the test
	REQUIRE(!theArray.IsValid());
	theArray.SetObject(CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks));

	REQUIRE(theArray.IsValid());
	REQUIRE(CFGetRetainCount(theArray) == 1);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NCFOBJECT("Copy", "[cf]")
{


	// Perform the test
	theArray.SetObject(CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks));
	REQUIRE(CFGetRetainCount(theArray) == 1);

	{	NCFObject	theArray2;
	
		theArray2 = theArray;
		REQUIRE(CFGetRetainCount(theArray)  == 2);
		REQUIRE(CFGetRetainCount(theArray2) == 2);
	}

	REQUIRE(CFGetRetainCount(theArray) == 1);
}





/*	NAME:
		TDictionary.cpp

	DESCRIPTION:
		NDictionary tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDictionary.h"

#include "TDictionary.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NPoint kTestPoint										= NPoint(10.0f, 10.0f);
static const SInt32 kTestSInt32										=  2000;
static const SInt64 kTestSInt64										= -4000;





//============================================================================
//		TDictionary::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TDictionary::Execute(void)
{	NDictionary		testDict, testDict2;



	// Execute the tests
	testDict.SetValue("point",   kTestPoint);
	testDict.SetValue("sint32",  kTestSInt32);
	testDict2.SetValue("sint64", kTestSInt64);
	NN_ASSERT(testDict.GetSize()  == 2);
	NN_ASSERT(testDict2.GetSize() == 1);

	testDict.Join(testDict2);
	NN_ASSERT(testDict.GetSize()        == 3);
	NN_ASSERT(testDict.GetKeys().size() == 3);

	NN_ASSERT(!testDict.HasKey("fail"));
	NN_ASSERT(testDict.GetValuePoint("point")   == kTestPoint);
	NN_ASSERT(testDict.GetValueSInt32("sint32") == kTestSInt32);
	NN_ASSERT(testDict.GetValueSInt64("sint64") == kTestSInt64);

	testDict.SetValue("remove", 3.1415);
	NN_ASSERT(testDict.HasKey("remove"));

	testDict.RemoveKey("remove");
	NN_ASSERT(!testDict.HasKey("remove"));
}


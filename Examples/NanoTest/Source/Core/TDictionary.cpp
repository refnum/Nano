/*	NAME:
		TDictionary.cpp

	DESCRIPTION:
		NDictionary tests.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
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
//		TDictionary::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TDictionary::Execute(void)
{	NDictionary		testDict, testDict2;
	NPoint			valuePoint(10, 10);
	SInt32			valueSInt32 =  2000;
	SInt64			valueSInt64 = -4000;



	// Execute the tests
	testDict.SetValue("point",   valuePoint);
	testDict.SetValue("sint32",  valueSInt32);
	testDict2.SetValue("sint64", valueSInt64);
	NN_ASSERT(testDict.GetSize()  == 2);
	NN_ASSERT(testDict2.GetSize() == 1);

	testDict.Join(testDict2);
	NN_ASSERT(testDict.GetSize()        == 3);
	NN_ASSERT(testDict.GetKeys().size() == 3);

	NN_ASSERT(!testDict.HasKey("fail"));
	NN_ASSERT(testDict.GetValuePoint("point")   == valuePoint);
	NN_ASSERT(testDict.GetValueSInt32("sint32") == valueSInt32);
	NN_ASSERT(testDict.GetValueSInt64("sint64") == valueSInt64);

	testDict.SetValue("remove", 3.1415);
	NN_ASSERT(testDict.HasKey("remove"));

	testDict.RemoveKey("remove");
	NN_ASSERT(!testDict.HasKey("remove"));
}

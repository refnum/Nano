/*	NAME:
		TCacheKey.cpp

	DESCRIPTION:
		NCacheKey tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCacheKey.h"

#include "TCacheKey.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kTestString1									= "first test string";
static const NString kTestString2									= "another test string";

static const NData kTestData1										= kTestString1.GetData();
static const NData kTestData2										= kTestString2.GetData();

static void *kTestPointer1											= NN_TARGET_ARCH_64 ? ((void *) 0xDEADBEEFDEADBEEFLL) : ((void *) 0xDEADBEEF);
static void *kTestPointer2											= NN_TARGET_ARCH_64 ? ((void *) 0xFEEDFACEFEEDFACELL) : ((void *) 0xFEEDFACE);

static NIndex kTestIndex1											= 12345;
static NIndex kTestIndex2											= 67890;





//============================================================================
//		TCacheKey::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TCacheKey::Execute(void)
{	NCacheKey		testKey1, testKey2;



	// Strings
	testKey1 = NCacheKey(kTestString1);
	testKey2 = NCacheKey(kTestString2);

	NN_ASSERT(testKey1 != testKey2);
	NN_ASSERT(testKey1.IsValid());
	NN_ASSERT(testKey2.IsValid());



	// Data
	testKey1 = NCacheKey(kTestData1);
	testKey2 = NCacheKey(kTestData2);

	NN_ASSERT(testKey1 != testKey2);
	NN_ASSERT(testKey1.IsValid());
	NN_ASSERT(testKey2.IsValid());



	// Pointers
	testKey1 = NCacheKey(kTestPointer1);
	testKey2 = NCacheKey(kTestPointer2);

	NN_ASSERT(testKey1 != testKey2);
	NN_ASSERT(testKey1.IsValid());
	NN_ASSERT(testKey2.IsValid());



	// Indices
	testKey1 = NCacheKey(kTestIndex1);
	testKey2 = NCacheKey(kTestIndex2);

	NN_ASSERT(testKey1 != testKey2);
	NN_ASSERT(testKey1.IsValid());
	NN_ASSERT(testKey2.IsValid());



	// Null
	testKey1 = NCacheKey();
	testKey2 = NCacheKey();
	
	NN_ASSERT(testKey1 == testKey2);
	NN_ASSERT(!testKey1.IsValid());
	NN_ASSERT(!testKey2.IsValid());



	// Clear
	testKey1 = NCacheKey(kTestIndex1);
	testKey2 = NCacheKey(kTestIndex2);

	NN_ASSERT(testKey1 != testKey2);
	NN_ASSERT(testKey1.IsValid());
	NN_ASSERT(testKey2.IsValid());

	testKey1.Clear();
	NN_ASSERT(testKey1 != testKey2);
	NN_ASSERT(!testKey1.IsValid());

	testKey2.Clear();
	NN_ASSERT(testKey1 == testKey2);
	NN_ASSERT(!testKey2.IsValid());



	// Set
	testKey1 = NCacheKey(kTestIndex1);
	testKey2 = NCacheKey(kTestIndex2);
	NN_ASSERT(testKey1 != testKey2);

	testKey1.SetValue(kTestIndex2);
	NN_ASSERT(testKey1 == testKey2);
}




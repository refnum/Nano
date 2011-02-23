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
static const NString kValueString1									= "first test string";
static const NString kValueString2									= "another test string";

static const NData kValueData1										= kValueString1.GetData();
static const NData kValueData2										= kValueString2.GetData();

static void *kValuePointer1											= NN_TARGET_ARCH_64 ? ((void *) 0xDEADBEEFDEADBEEFLL) : ((void *) 0xDEADBEEF);
static void *kValuePointer2											= NN_TARGET_ARCH_64 ? ((void *) 0xFEEDFACEFEEDFACELL) : ((void *) 0xFEEDFACE);

static NIndex kValueIndex1											= 12345;
static NIndex kValueIndex2											= 67890;





//============================================================================
//		TCacheKey::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TCacheKey::Execute(void)
{	NCacheKey		testKey1, testKey2;



	// Strings
	testKey1 = NCacheKey(kValueString1);
	testKey2 = NCacheKey(kValueString2);

	NN_ASSERT(testKey1 != testKey2);
	NN_ASSERT(testKey1.IsValid());
	NN_ASSERT(testKey2.IsValid());



	// Data
	testKey1 = NCacheKey(kValueData1);
	testKey2 = NCacheKey(kValueData2);

	NN_ASSERT(testKey1 != testKey2);
	NN_ASSERT(testKey1.IsValid());
	NN_ASSERT(testKey2.IsValid());



	// Pointers
	testKey1 = NCacheKey(kValuePointer1);
	testKey2 = NCacheKey(kValuePointer2);

	NN_ASSERT(testKey1 != testKey2);
	NN_ASSERT(testKey1.IsValid());
	NN_ASSERT(testKey2.IsValid());



	// Indices
	testKey1 = NCacheKey(kValueIndex1);
	testKey2 = NCacheKey(kValueIndex2);

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
	testKey1 = NCacheKey(kValueIndex1);
	testKey2 = NCacheKey(kValueIndex2);

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
	testKey1 = NCacheKey(kValueIndex1);
	testKey2 = NCacheKey(kValueIndex2);
	NN_ASSERT(testKey1 != testKey2);

	testKey1.SetValue(kValueIndex2);
	NN_ASSERT(testKey1 == testKey2);
}





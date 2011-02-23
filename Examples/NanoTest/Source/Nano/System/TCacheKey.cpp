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
#include "NDataDigest.h"
#include "NCacheKey.h"

#include "TCacheKey.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kValueString1									= "first test string";
static const NString kValueString2									= "another test string";





//============================================================================
//		TCacheKey::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TCacheKey::Execute(void)
{	NDigestSHA1			sha1_1, sha1_2;
	NIndex				index1, index2;
	NDigestMD5			md5_1, md5_2;
	void				*ptr1, *ptr2;
	NData				data1, data2;
	NCacheKey			key1, key2;
	NDataDigest			dataDigest;



	// Get the state we need
	data1 = kValueString1.GetData();
	data2 = kValueString2.GetData();



	// MD5
	md5_1 = dataDigest.GetMD5(data1);
	md5_2 = dataDigest.GetMD5(data2);

	key1 = NCacheKey(md5_1);
	key2 = NCacheKey(md5_2);

	NN_ASSERT(key1 != key2);



	// SHA1
	sha1_1 = dataDigest.GetSHA1(data1);
	sha1_2 = dataDigest.GetSHA1(data2);

	key1 = NCacheKey(sha1_1);
	key2 = NCacheKey(sha1_2);

	NN_ASSERT(key1 != key2);



	// Pointers
	ptr1 = NN_TARGET_ARCH_64 ? ((void *) 0xDEADBEEFDEADBEEFLL) : ((void *) 0xDEADBEEF);
	ptr2 = NN_TARGET_ARCH_64 ? ((void *) 0xFEEDFACEFEEDFACELL) : ((void *) 0xFEEDFACE);

	key1 = NCacheKey(ptr1);
	key2 = NCacheKey(ptr2);

	NN_ASSERT(key1 != key2);



	// Indices
	index1 = 123;
	index2 = 456;
	
	key1 = NCacheKey(index1);
	key2 = NCacheKey(index2);

	NN_ASSERT(key1 != key2);
	NN_ASSERT(key1.IsValid());
	NN_ASSERT(key2.IsValid());



	// Null
	key1 = NCacheKey();
	key2 = NCacheKey();
	
	NN_ASSERT(key1 == key2);
	NN_ASSERT(!key1.IsValid());
	NN_ASSERT(!key2.IsValid());



	// Clear
	key1 = NCacheKey(index1);
	key2 = NCacheKey(index2);

	NN_ASSERT(key1 != key2);
	NN_ASSERT(key1.IsValid());
	NN_ASSERT(key2.IsValid());

	key1.Clear();
	NN_ASSERT(key1 != key2);
	NN_ASSERT(!key1.IsValid());

	key2.Clear();
	NN_ASSERT(key1 == key2);
	NN_ASSERT(!key2.IsValid());



	// Set
	index1 = 123;
	index2 = 456;
	
	key1 = NCacheKey(index1);
	key2 = NCacheKey(index2);
	NN_ASSERT(key1 != key2);

	key1.SetValue(index2);
	NN_ASSERT(key1 == key2);
}





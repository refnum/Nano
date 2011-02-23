/*	NAME:
		NCacheKey.cpp

	DESCRIPTION:
		Cache key.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSTLUtilities.h"
#include "NByteSwap.h"
#include "NCacheKey.h"





//============================================================================
//		NCacheKey::NCacheKey : Constructor.
//----------------------------------------------------------------------------
NCacheKey::NCacheKey(const NDigestMD5 &theDigest)
{	const UInt32	*theKeys;



	// Get the state we need
	theKeys = (const UInt32 *) &theDigest;
	NN_ASSERT(sizeof(theDigest) == (sizeof(UInt32) * 4));



	// Initialise ourselves
	//
	// The digest is just a stream of bytes, but to ensure the cache key has
	// consistent ordering we interpret them as a set of big-endian values.
	SetValue(	NSwapUInt32_BtoN(theKeys[0]),
				NSwapUInt32_BtoN(theKeys[1]), 
				NSwapUInt32_BtoN(theKeys[2]), 
				NSwapUInt32_BtoN(theKeys[3])); 
}





//============================================================================
//		NCacheKey::NCacheKey : Constructor.
//----------------------------------------------------------------------------
NCacheKey::NCacheKey(const NDigestSHA1 &theDigest)
{	const UInt32	*theKeys;



	// Get the state we need
	theKeys = (const UInt32 *) &theDigest;
	NN_ASSERT(sizeof(theDigest) == (sizeof(UInt32) * 5));



	// Initialise ourselves
	//
	// The digest is just a stream of bytes, but to ensure the cache key has
	// consistent ordering we interpret them as a set of big-endian values.
	SetValue(	NSwapUInt32_BtoN(theKeys[0]),
				NSwapUInt32_BtoN(theKeys[1]), 
				NSwapUInt32_BtoN(theKeys[2]), 
				NSwapUInt32_BtoN(theKeys[3]), 
				NSwapUInt32_BtoN(theKeys[4])); 
}





//============================================================================
//		NCacheKey::NCacheKey : Constructor.
//----------------------------------------------------------------------------
NCacheKey::NCacheKey(NIndex key0, NIndex key1, NIndex key2, NIndex key3, NIndex key4, NIndex key5, NIndex key6, NIndex key7)
{


	// Initialise ourselves
	SetValue(key0, key1, key2, key3, key4, key5, key6, key7);
}





//============================================================================
//		NCacheKey::~NCacheKey : Destructor.
//----------------------------------------------------------------------------
NCacheKey::~NCacheKey(void)
{
}





//=============================================================================
//		NCacheKey::IsValid : Is the key valid?
//-----------------------------------------------------------------------------
bool NCacheKey::IsValid(void) const
{	NIndex		n;



	// Check the key
	for (n = 0; n < NN_ARRAY_SIZE(mKey); n++)
		{
		if (mKey[n] != kNCacheKeyInvalid)
			return(true);
		}
	
	return(false);
}





//=============================================================================
//		NCacheKey::Clear : Clear the key.
//-----------------------------------------------------------------------------
void NCacheKey::Clear(void)
{	NIndex		n;



	// Clear the key
	for (n = 0; n < NN_ARRAY_SIZE(mKey); n++)
		mKey[n] = kNCacheKeyInvalid;
}





//============================================================================
//		NCacheKey::SetValue : Set the value.
//----------------------------------------------------------------------------
void NCacheKey::SetValue(NIndex key0, NIndex key1, NIndex key2, NIndex key3, NIndex key4, NIndex key5, NIndex key6, NIndex key7)
{


	// Set the value
	mKey[0] = key0;
	mKey[1] = key1;
	mKey[2] = key2;
	mKey[3] = key3;
	mKey[4] = key4;
	mKey[5] = key5;
	mKey[6] = key6;
	mKey[7] = key7;
}





//============================================================================
//		NCacheKey::Compare : Compare the key.
//----------------------------------------------------------------------------
NComparison NCacheKey::Compare(const NCacheKey &theKey) const
{	NComparison		theResult;
	NIndex			n;



	// Compare the key
	theResult = kNCompareEqualTo;
	
	for (n = 0; n < NN_ARRAY_SIZE(mKey); n++)
		{
		theResult = GetComparison(mKey[n], theKey.mKey[n]);
		if (theResult != kNCompareEqualTo)
			return(theResult);
		}
	
	return(theResult);
}




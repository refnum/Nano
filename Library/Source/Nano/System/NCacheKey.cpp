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
#include "NDataDigest.h"
#include "NByteSwap.h"
#include "NCacheKey.h"





//============================================================================
//		NCacheKey::NCacheKey : Constructor.
//----------------------------------------------------------------------------
NCacheKey::NCacheKey(const NString &theValue)
{


	// Initialise ourselves
	SetValue(theValue);
}





//============================================================================
//		NCacheKey::NCacheKey : Constructor.
//----------------------------------------------------------------------------
NCacheKey::NCacheKey(const NData &theValue)
{


	// Initialise ourselves
	SetValue(theValue);
}





//============================================================================
//		NCacheKey::NCacheKey : Constructor.
//----------------------------------------------------------------------------
NCacheKey::NCacheKey(const void *key0, const void *key1, const void *key2, const void *key3)
{


	// Initialise ourselves
	SetValue(key0, key1, key2, key3);
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
//		NCacheKey::NCacheKey : Constructor.
//----------------------------------------------------------------------------
NCacheKey::NCacheKey(void)
{


	// Initialise ourselves
	SetValue(kNCacheKeyInvalid);
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
void NCacheKey::SetValue(const NString &theValue)
{


	// Validate our state
	NN_ASSERT(sizeof(NHashCode) == sizeof(NIndex));



	// Set the value
	SetValue((NIndex) theValue.GetHash());
}





//============================================================================
//		NCacheKey::SetValue : Set the value.
//----------------------------------------------------------------------------
void NCacheKey::SetValue(const NData &theValue)
{	NDataDigest		dataDigest;
	NDigestMD5		digestMD5;
	const UInt32	*theKeys;



	// Get the state we need
	digestMD5 = dataDigest.GetMD5(theValue);
	theKeys   = (const UInt32 *) &digestMD5;

	NN_ASSERT(sizeof(digestMD5) == (sizeof(UInt32) * 4));



	// Set the value
	//
	// The digest is just a stream of bytes, but to ensure the cache key has
	// consistent ordering we interpret them as a set of big-endian values.
	SetValue(	NSwapUInt32_BtoN(theKeys[0]),
				NSwapUInt32_BtoN(theKeys[1]), 
				NSwapUInt32_BtoN(theKeys[2]), 
				NSwapUInt32_BtoN(theKeys[3])); 
}





//============================================================================
//		NCacheKey::SetValue : Set the value.
//----------------------------------------------------------------------------
void NCacheKey::SetValue(const void *key0, const void *key1, const void *key2, const void *key3)
{


	// Set the value
	SetValue(	SplitHi(key0), SplitLo(key0),
				SplitHi(key1), SplitLo(key1),
				SplitHi(key2), SplitLo(key2),
				SplitHi(key3), SplitLo(key3));
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





//============================================================================
//		NCacheKey::SplitHi : Split a pointer.
//----------------------------------------------------------------------------
NIndex NCacheKey::SplitHi(const void *theValue)
{	NIndex		partPtr, shiftBy;
	UIntPtr		intPtr;



	// Split the pointer
	//
	// gcc emits a warning if you shift by the width of the type, so
	// we put the shift into a variable to suppress this warning.
	shiftBy = 32;
	intPtr  = (UIntPtr) theValue;
	partPtr = ((intPtr >> shiftBy) & 0xFFFFFFFF);
	
	return(partPtr);
}





//============================================================================
//		NCacheKey::SplitLo : Split a pointer.
//----------------------------------------------------------------------------
NIndex NCacheKey::SplitLo(const void *theValue)
{	NIndex		partPtr, shiftBy;
	UIntPtr		intPtr;



	// Split the pointer
	shiftBy = 0;
	intPtr  = (UIntPtr) theValue;
	partPtr = ((intPtr >> shiftBy) & 0xFFFFFFFF);
	
	return(partPtr);
}




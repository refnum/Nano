/*	NAME:
		NCacheKey.cpp

	DESCRIPTION:
		Cache key.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
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
NCacheKey::NCacheKey(NHashCode key0, NHashCode key1, NHashCode key2, NHashCode key3, NHashCode key4, NHashCode key5, NHashCode key6, NHashCode key7)
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


	// Set the value
	SetValue(theValue.GetHash());
}





//============================================================================
//		NCacheKey::SetValue : Set the value.
//----------------------------------------------------------------------------
void NCacheKey::SetValue(const NData &theValue)
{	NDataDigest		dataDigest;
	NDigestMD5		digestMD5;
	const uint32_t	*theKeys;



	// Get the state we need
	digestMD5 = dataDigest.GetMD5(theValue);
	theKeys   = (const uint32_t *) &digestMD5;

	NN_ASSERT(sizeof(digestMD5) == (sizeof(uint32_t) * 4));



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
	SetValue(	SplitLo(key0), SplitHi(key0),
				SplitLo(key1), SplitHi(key1),
				SplitLo(key2), SplitHi(key2),
				SplitLo(key3), SplitHi(key3));
}





//============================================================================
//		NCacheKey::SetValue : Set the value.
//----------------------------------------------------------------------------
void NCacheKey::SetValue(NHashCode key0, NHashCode key1, NHashCode key2, NHashCode key3, NHashCode key4, NHashCode key5, NHashCode key6, NHashCode key7)
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
NHashCode NCacheKey::SplitHi(const void *theValue)
{	NHashCode	thePart;
	uintptr_t	intPtr;



	// Split the pointer
	intPtr = (uintptr_t) theValue;

#if NN_ARCH_32
	(void) intPtr;
	thePart = 0;
#else
	thePart = (NHashCode) ((intPtr >> 32) & 0xFFFFFFFF);
#endif

	return(thePart);
}





//============================================================================
//		NCacheKey::SplitLo : Split a pointer.
//----------------------------------------------------------------------------
NHashCode NCacheKey::SplitLo(const void *theValue)
{	NHashCode	thePart;
	uintptr_t	intPtr;



	// Split the pointer
	intPtr  = (uintptr_t) theValue;
	thePart = (NHashCode) (((intPtr >> 0) & 0xFFFFFFFF));

	return(thePart);
}




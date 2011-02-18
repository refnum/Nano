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
#include "NCacheKey.h"





//============================================================================
//		NCacheKey::NCacheKey : Constructor.
//----------------------------------------------------------------------------
NCacheKey::NCacheKey(NIndex key0, NIndex key1, NIndex key2, NIndex key3, NIndex key4, NIndex key5, NIndex key6)
{


	// Initialise ourselves
	SetValue(key0, key1, key2, key3, key4, key5, key6);
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
	for (n = 0; n < NN_ARRAY_SIZE(mValue); n++)
		{
		if (mValue[n] != kNCacheKeyInvalid)
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
	for (n = 0; n < NN_ARRAY_SIZE(mValue); n++)
		mValue[n] = kNCacheKeyInvalid;
}





//============================================================================
//		NCacheKey::SetValue : Set the value.
//----------------------------------------------------------------------------
void NCacheKey::SetValue(NIndex key0, NIndex key1, NIndex key2, NIndex key3, NIndex key4, NIndex key5, NIndex key6)
{


	// Set the value
	mKey[0] = key0;
	mKey[1] = key1;
	mKey[2] = key2;
	mKey[3] = key3;
	mKey[4] = key4;
	mKey[5] = key5;
	mKey[6] = key6;
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
//		NCacheKeyString::NCacheKeyString : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
NCacheKeyString::NCacheKeyString(const NString &theKey)
{


	// Initialise ourselves
	mKey = theKey;
}





//============================================================================
//		NCacheKeyString::~NCacheKeyString : Destructor.
//----------------------------------------------------------------------------
NCacheKeyString::~NCacheKeyString(void)
{
}





//============================================================================
//		NCacheKeyString::Compare : Compare the key.
//----------------------------------------------------------------------------
NComparison NCacheKeyString::Compare(const NCacheKey &theKey) const
{	const NCacheKeyString		*otherKey = (const NCacheKeyString *) &theKey;



	// Compare the key
	return(NStringHashCompare().Compare(mKey, otherKey->mKey));
}




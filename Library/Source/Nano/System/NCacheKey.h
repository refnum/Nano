/*	NAME:
		NCacheKey.h

	DESCRIPTION:
		Cache key.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NNCACHEKEY_HDR
#define NNCACHEKEY_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NComparable.h"
#include "NString.h"
#include "NData.h"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
static const NHashCode kNCacheKeyInvalid							= (NHashCode) -1;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCacheKey {
public:
										NCacheKey(const NString &theValue);
										NCacheKey(const NData   &theValue);

										NCacheKey(const void *key0,
												  const void *key1=(const void *) kNCacheKeyInvalid,
												  const void *key2=(const void *) kNCacheKeyInvalid,
												  const void *key3=(const void *) kNCacheKeyInvalid);

										NCacheKey(NHashCode key0,
												  NHashCode key1=kNCacheKeyInvalid,
												  NHashCode key2=kNCacheKeyInvalid,
												  NHashCode key3=kNCacheKeyInvalid,
												  NHashCode key4=kNCacheKeyInvalid,
												  NHashCode key5=kNCacheKeyInvalid,
												  NHashCode key6=kNCacheKeyInvalid,
												  NHashCode key7=kNCacheKeyInvalid);

										NCacheKey(void);
	virtual							   ~NCacheKey(void);


	// Is the key valid?
	bool								IsValid(void) const;


	// Clear the key
	void								Clear(void);


	// Set the value
	void								SetValue(const NString &theValue);
	void								SetValue(const NData   &theValue);
	
	void								SetValue(	const void *key0,
													const void *key1=(const void *) kNCacheKeyInvalid,
													const void *key2=(const void *) kNCacheKeyInvalid,
													const void *key3=(const void *) kNCacheKeyInvalid);

	void								SetValue(	NHashCode key0,
													NHashCode key1=kNCacheKeyInvalid,
													NHashCode key2=kNCacheKeyInvalid,
													NHashCode key3=kNCacheKeyInvalid,
													NHashCode key4=kNCacheKeyInvalid,
													NHashCode key5=kNCacheKeyInvalid,
													NHashCode key6=kNCacheKeyInvalid,
													NHashCode key7=kNCacheKeyInvalid);


	// Compare the key
	NComparison							Compare(const NCacheKey &theKey) const;


	// Operators
	NCOMPARABLE_OPERATORS(NCacheKey)


	// Split a pointer
	//
	// Cache comparisons are performed by key order, so typically the low bits
	// of a pointer should be added before the high bits:
	//
	//		SetValue(123, 456, NCacheKey::SplitLo(thePtr), NCacheKey::SplitHi(thePtr));
	//
	// This ensures that the low bits, which are typically more unique on 64-bit (and
	// always more unique on 32-bit), are compared first.
	static NHashCode					SplitHi(const void *theValue);
	static NHashCode					SplitLo(const void *theValue);


private:
	NHashCode							mKey[8];
};







#endif // NNCACHEKEY_HDR



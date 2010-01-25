/*	NAME:
		NCacheKey.h

	DESCRIPTION:
		Cache key.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
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





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
static const NIndex kNCacheKeyInvalid								= (NIndex) -1;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCacheKey {
public:
										NCacheKey(NIndex key0=kNCacheKeyInvalid,
												  NIndex key1=kNCacheKeyInvalid,
												  NIndex key2=kNCacheKeyInvalid,
												  NIndex key3=kNCacheKeyInvalid,
												  NIndex key4=kNCacheKeyInvalid,
												  NIndex key5=kNCacheKeyInvalid,
												  NIndex key6=kNCacheKeyInvalid);
	virtual							   ~NCacheKey(void);


	// Set the value
	void								SetValue(NIndex key0,
												 NIndex key1=kNCacheKeyInvalid,
												 NIndex key2=kNCacheKeyInvalid,
												 NIndex key3=kNCacheKeyInvalid,
												 NIndex key4=kNCacheKeyInvalid,
												 NIndex key5=kNCacheKeyInvalid,
												 NIndex key6=kNCacheKeyInvalid);


	// Compare the key
	virtual NComparison					Compare(const NCacheKey &theKey) const;


	// Operators
	NCOMPARABLE_OPERATORS(NCacheKey);


private:
	NIndex								mKey[7];
};





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCacheKeyString : public NCacheKey {
public:
										NCacheKeyString(const NString &theKey);
	virtual							   ~NCacheKeyString(void);


	// Compare the key
	NComparison							Compare(const NCacheKey &theKey) const;


private:
	NString								mKey;
};








#endif // NNCACHEKEY_HDR



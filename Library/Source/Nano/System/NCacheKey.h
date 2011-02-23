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
#include "NDataDigest.h"
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
										NCacheKey(const NDigestMD5  &theDigest);
										NCacheKey(const NDigestSHA1 &theDigest);
										
										NCacheKey(NIndex key0=kNCacheKeyInvalid,
												  NIndex key1=kNCacheKeyInvalid,
												  NIndex key2=kNCacheKeyInvalid,
												  NIndex key3=kNCacheKeyInvalid,
												  NIndex key4=kNCacheKeyInvalid,
												  NIndex key5=kNCacheKeyInvalid,
												  NIndex key6=kNCacheKeyInvalid,
												  NIndex key7=kNCacheKeyInvalid);
	virtual							   ~NCacheKey(void);


	// Is the key valid?
	bool								IsValid(void) const;


	// Clear the key
	void								Clear(void);


	// Set the value
	void								SetValue(NIndex key0,
												 NIndex key1=kNCacheKeyInvalid,
												 NIndex key2=kNCacheKeyInvalid,
												 NIndex key3=kNCacheKeyInvalid,
												 NIndex key4=kNCacheKeyInvalid,
												 NIndex key5=kNCacheKeyInvalid,
												 NIndex key6=kNCacheKeyInvalid,
												 NIndex key7=kNCacheKeyInvalid);


	// Compare the key
	virtual NComparison					Compare(const NCacheKey &theKey) const;


	// Operators
	NCOMPARABLE_OPERATORS(NCacheKey);


private:
	NIndex								mKey[8];
};







#endif // NNCACHEKEY_HDR



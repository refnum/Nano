/*	NAME:
		NURL.h

	DESCRIPTION:
		Uniform Resource Locator.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NURL_HDR
#define NURL_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NComparable.h"
#include "NEncodable.h"
#include "NString.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NURL :	public NEncodable,
				public NComparable<NURL> {
public:
										NENCODABLE_DECLARE(NURL);

										NURL(const NString &theURL);
										NURL(const char    *theURL);

										NURL(void);
	virtual							   ~NURL(void);


	// Is the URL valid?
	bool								IsValid(void) const;


	// Clear the value
	void								Clear(void);


	// Compare the value
	NComparison							Compare(const NURL &theValue) const;


	// Get the components
	NString								GetScheme(void) const;
	NString								GetHost(  void) const;
	NIndex								GetPort(  void) const;
	NString								GetPath(  void) const;


	// Operators
										operator NString        (void) const;
										operator NFormatArgument(void) const;


protected:
	// Encode/decode the object
	void								EncodeSelf(      NEncoder &theEncoder) const;
	void								DecodeSelf(const NEncoder &theEncoder);


private:
	NString								GetToken(const NString &thePattern) const;


private:
	NString								mValue;
};





#endif // NURL_HDR

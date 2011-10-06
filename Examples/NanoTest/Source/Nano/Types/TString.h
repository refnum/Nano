/*	NAME:
		TString.h

	DESCRIPTION:
		NString tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef TSTRING_HDR
#define TSTRING_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class TString {
public:
	// Execute the tests
	static void							Execute(void);


private:
	static NString						Encode(const NString &theValue, NStringEncoding theEncoding);

	static void							TestTrim(NStringEncoding theEncoding);
};





#endif // TSTRING_HDR

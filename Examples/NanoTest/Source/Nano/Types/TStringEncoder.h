/*	NAME:
		TStringEncoder.h

	DESCRIPTION:
		NStringEncoder tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef TSTRINGENCODER_HDR
#define TSTRINGENCODER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class TStringEncoder {
public:
	// Execute the tests
	static void							Execute(void);


private:
	static void							TestTerminator(const NString &theString, NStringEncoding theEncoding);
};





#endif // TSTRINGENCODER_HDR

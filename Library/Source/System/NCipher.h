/*	NAME:
		NCipher.h

	DESCRIPTION:
		Nano encryption support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCIPHER_HDR
#define NCIPHER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCipher {
public:
	// Calculate a checksum
	static UInt16						GetChecksumInternet(NIndex theSize, const void *thePtr);
	static UInt32						GetChecksumAdler32( NIndex theSize, const void *thePtr, UInt32 prevAdler=0);
};





#endif // NCIPHER_HDR



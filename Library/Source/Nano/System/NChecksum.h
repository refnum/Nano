/*	NAME:
		NChecksum.h

	DESCRIPTION:
		Nano checksum support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCHECKSUM_HDR
#define NCHECKSUM_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NChecksum {
public:
										NChecksum( void);
	virtual								~NChecksum(void);


	// Get the internet checksum
	UInt16								GetInternet(NIndex theSize, const void *thePtr);


	// Get a DJB2 checksum
	UInt32								GetDJB2(NIndex theSize, const void *thePtr);


	// Get an Adler32 checksum
	UInt32								GetAdler32(NIndex theSize, const void *thePtr, UInt32 prevValue=0);


private:


};





#endif // NCHECKSUM_HDR



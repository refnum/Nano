/*	NAME:
		NDataDigest.cpp

	DESCRIPTION:
		Data digests.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "nano_zlib.h"
#include "fm_md5.h"
#include "sha2.h"

#include "NDataDigest.h"





//============================================================================
//		NDataDigest::NDataDigest : Constructor.
//----------------------------------------------------------------------------
NDataDigest::NDataDigest(void)
{
}





//============================================================================
//		NDataDigest::~NDataDigest : Destructor.
//----------------------------------------------------------------------------
NDataDigest::~NDataDigest(void)
{
}





//============================================================================
//		NDataDigest::GetString : Get an digest as a string.
//----------------------------------------------------------------------------
NString NDataDigest::GetString(UInt32 theValue) const
{	NString		theResult;



	// Get the string
	theResult.Format("%08lx", theValue);
	
	return(theResult);
}





//============================================================================
//		NDataDigest::GetString : Get an MD5 digest as a string.
//----------------------------------------------------------------------------
NString NDataDigest::GetString(const NDigestMD5 &theValue) const
{	NString		theResult;



	// Get the string
	theResult = GetQuad(&theValue.bytes[ 0]) + 
				GetQuad(&theValue.bytes[ 4]) + 
				GetQuad(&theValue.bytes[ 8]) + 
				GetQuad(&theValue.bytes[12]);

	return(theResult);
}





//============================================================================
//		NDataDigest::GetString : Get SHA1 digest as a string.
//----------------------------------------------------------------------------
NString NDataDigest::GetString(const NDigestSHA1 &theValue) const
{	NString		theResult;



	// Get the string
	theResult = GetQuad(&theValue.bytes[ 0]) + 
				GetQuad(&theValue.bytes[ 4]) + 
				GetQuad(&theValue.bytes[ 8]) + 
				GetQuad(&theValue.bytes[12]);
				GetQuad(&theValue.bytes[16]);

	return(theResult);
}





//============================================================================
//		NDataDigest::GetInternet : Get the internet digest.
//----------------------------------------------------------------------------
//		Note : From <http://www.rfc-editor.org/rfc/rfc1071.txt>.
//----------------------------------------------------------------------------
UInt16 NDataDigest::GetInternet(const NData &theData)
{	UInt32			theDigest, hi, lo;
	NIndex			dataSize;
	const UInt8		*dataPtr;



	// Check our parameters
	if (theData.IsEmpty())
		return(0);



	// Get the state we need
	dataPtr   = (const UInt8 *) theData.GetData();
	dataSize  =                 theData.GetSize();
	theDigest = 0;



	// Get the digest
	while (dataSize > 1)
		{
		hi = *dataPtr++;
		lo = *dataPtr++;

		theDigest += ((hi << 8) | lo);
		dataSize  -= 2;
		}

	if (dataSize > 0)
		theDigest += *dataPtr++;



	// Fold the 32-bit digest to 16 bits
	while (theDigest >> 16)
		theDigest = (theDigest & 0xffff) + (theDigest >> 16);

	theDigest  = ~theDigest;
	theDigest &= 0x0000FFFF;

	return((UInt16) theDigest);
}





//============================================================================
//		NDataDigest::GetDJB2 : Get a a DJB2 digest.
//----------------------------------------------------------------------------
//		Note : From <http://www.cse.yorku.ca/~oz/hash.html>.
//----------------------------------------------------------------------------
UInt32 NDataDigest::GetDJB2(const NData &theData)
{	NIndex			n, dataSize;
	UInt32			theDigest;
	const UInt8		*dataPtr;



	// Check our parameters
	if (theData.IsEmpty())
		return(0);



	// Get the state we need
	dataPtr   = (const UInt8 *) theData.GetData();
	dataSize  =                 theData.GetSize();
	theDigest = 5381;



	// Get the digest
	for (n = 0; n < dataSize; n++)
		theDigest = ((theDigest << 5) + theDigest) + dataPtr[n];
	
	return(theDigest);
}





//============================================================================
//		NDataDigest::GetAdler32 : Get an Adler32 digest.
//----------------------------------------------------------------------------
UInt32 NDataDigest::GetAdler32(const NData &theData, UInt32 prevValue)
{	UInt32		theDigest;



	// Check our parameters
	if (theData.IsEmpty())
		return(prevValue);



	// Get the digest
	theDigest = (UInt32) adler32(prevValue, theData.GetData(), theData.GetSize());
	
	return(theDigest);
}





//============================================================================
//		NDataDigest::GetMD5 : Get an MD5 digest.
//----------------------------------------------------------------------------
NDigestMD5 NDataDigest::GetMD5(const NData &theData)
{	NDigestMD5		theDigest;
	MD5Context		theState;



	// Validate our state
	NN_ASSERT(sizeof(theDigest.bytes) == 16);



	// Check our parameters
	memset(&theDigest, 0x00, sizeof(theDigest));

	if (theData.IsEmpty())
		return(theDigest);



	// Get the digest
	MD5Init(  &theState);
	MD5Update(&theState, (const unsigned char *) theData.GetData(), theData.GetSize());
	MD5Final(theDigest.bytes, &theState);
	
	return(theDigest);
}





//============================================================================
//		NDataDigest::GetSHA1 : Get an SHA1 digest.
//----------------------------------------------------------------------------
NDigestSHA1 NDataDigest::GetSHA1(const NData &theData)
{	NDigestSHA1		theDigest;



	// Validate our state
	NN_ASSERT(sizeof(theDigest.bytes) == SHA1_DIGEST_LENGTH);



	// Check our parameters
	memset(&theDigest, 0x00, sizeof(theDigest));

	if (theData.IsEmpty())
		return(theDigest);



	// Get the digest
	SHA1_Data(theData.GetData(), theData.GetSize(), (char *) theDigest.bytes);
	
	return(theDigest);
}





//============================================================================
//		NDataDigest::GetQuad : Get a byte quad string.
//----------------------------------------------------------------------------
#pragma mark -
NString NDataDigest::GetQuad(const UInt8 *theBytes) const
{	NString		theResult;



	// Get the string
	theResult.Format("%02x%02x%02x%02x", theBytes[0], theBytes[1], theBytes[2], theBytes[3]);

	return(theResult);
}




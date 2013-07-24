/*	NAME:
		NDataDigest.h

	DESCRIPTION:
		Data digests.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDATADIGEST_HDR
#define NDATADIGEST_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"
#include "NData.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Digests
typedef struct {
	uint8_t		bytes[16];
} NDigestMD5;

typedef struct NDigestSHA1 {
	uint8_t		bytes[20];
} NDigestSHA1;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDataDigest {
public:
										NDataDigest(void);
	virtual							   ~NDataDigest(void);


	// Get a digest as a string
	NString								GetString(      uint32_t     theValue) const;
	NString								GetString(const NDigestMD5  &theValue) const;
	NString								GetString(const NDigestSHA1 &theValue) const;


	// Get a digest
	//
	// The checksum of empty data is always 0.
	uint16_t							GetInternet(const NData &theData);
	uint32_t							GetDJB2(    const NData &theData);
	uint32_t							GetAdler32( const NData &theData, uint32_t prevValue=0);
	NDigestMD5							GetMD5(     const NData &theData);
	NDigestSHA1							GetSHA1(    const NData &theData);


private:
	NString								GetQuad(const uint8_t *theBytes) const;


private:


};





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
// Equality
inline bool operator ==(const NDigestMD5 &value1, const NDigestMD5 &value2)
{
	return(memcmp(&value1.bytes, &value2.bytes, sizeof(value1.bytes)) == 0);
}

inline bool operator ==(const NDigestSHA1 &value1, const NDigestSHA1 &value2)
{
	return(memcmp(&value1.bytes, &value2.bytes, sizeof(value1.bytes)) == 0);
}


// Inequality
inline bool operator !=(const NDigestMD5 &value1, const NDigestMD5 &value2)
{
	return(memcmp(&value1.bytes, &value2.bytes, sizeof(value1.bytes)) != 0);
}

inline bool operator !=(const NDigestSHA1 &value1, const NDigestSHA1 &value2)
{
	return(memcmp(&value1.bytes, &value2.bytes, sizeof(value1.bytes)) != 0);
}




#endif // NDATADIGEST_HDR



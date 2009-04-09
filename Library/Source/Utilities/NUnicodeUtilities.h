/*	NAME:
		NUnicodeUtilities.h

	DESCRIPTION:
		Unicode utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NUNICODEUTILITIES_HDR
#define NUNICODEUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NData.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NUnicodeUtilities {
public:
	// Convert encodings
	static NStatus						ConvertUTF8ToUTF16(const NData &srcData, NData &dstData);
	static NStatus						ConvertUTF8ToUTF32(const NData &srcData, NData &dstData);

	static NStatus						ConvertUTF16ToUTF8( const NData &srcData, NData &dstData);
	static NStatus						ConvertUTF16ToUTF32(const NData &srcData, NData &dstData);

	static NStatus						ConvertUTF32ToUTF8( const NData &srcData, NData &dstData);
	static NStatus						ConvertUTF32ToUTF16(const NData &srcData, NData &dstData);


private:
	static NStatus						ProcessUnicode(NData &theData, const void *dataEnd, UInt32 theResult);
};





#endif // NUNICODEUTILITIES_HDR



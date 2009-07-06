/*	NAME:
		NTextUtilities.h

	DESCRIPTION:
		Text utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTEXTUTILITIES_HDR
#define NTEXTUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTextUtilities {
public:
	// Get a hex dump
	static NString						GetHexDump(UInt32 theSize, const void *thePtr, char flowSign=0x00, bool hexOnly=false);


private:
	static NString						GetHexLine(UInt32 lineOffset, UInt32 lineSize, UInt32 linePad, const UInt8 *theData, char flowSign, bool hexOnly);
};





#endif // NTEXTUTILITIES_HDR



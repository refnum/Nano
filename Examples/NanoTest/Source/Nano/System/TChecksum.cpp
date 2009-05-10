/*	NAME:
		TChecksum.cpp

	DESCRIPTION:
		NChecksum tests.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"
#include "NChecksum.h"

#include "TChecksum.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const UInt8 kData1[]											= { 0xAA, 0xBB, 0xCC, 0xDD };
static const UInt8 kData2[]											= { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 };





//============================================================================
//		TChecksum::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TChecksum::Execute(void)
{	NChecksum	checkSum;



	// Execute the tests
	NN_ASSERT(checkSum.GetInternet(GET_ARRAY_SIZE(kData1), kData1) == 0x8866);
	NN_ASSERT(checkSum.GetInternet(GET_ARRAY_SIZE(kData2), kData2) == 0xF9F6);
	NN_ASSERT(checkSum.GetInternet(0, NULL)                        == 0xFFFF);

	NN_ASSERT(checkSum.GetDJB2(GET_ARRAY_SIZE(kData1), kData1) == 0x7CBD7E93);
	NN_ASSERT(checkSum.GetDJB2(GET_ARRAY_SIZE(kData2), kData2) == 0x07F24354);
	NN_ASSERT(checkSum.GetDJB2(0, NULL)                        == 0x00001505);

	NN_ASSERT(checkSum.GetAdler32(GET_ARRAY_SIZE(kData1), kData1) == 0x074E030E);
	NN_ASSERT(checkSum.GetAdler32(GET_ARRAY_SIZE(kData2), kData2) == 0x0023000F);
	NN_ASSERT(checkSum.GetAdler32(0, NULL)                        == 0x00000001);
}





/*	NAME:
		TSTLUtilities.cpp

	DESCRIPTION:
		NSTLUtilities tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSTLUtilities.h"

#include "TSTLUtilities.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const UInt32List kTestList1									= mkvector((UInt32) 11, (UInt32) 12, (UInt32) 13);
static const UInt32List kTestList2									= mkvector((UInt32) 24, (UInt32) 25, (UInt32) 26);





//============================================================================
//		TSTLUtilities::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TSTLUtilities::Execute(void)
{	UInt32List		theList;



	// Append
	theList = kTestList1;
	append(theList, kTestList2);
	
	NN_ASSERT(theList.size() == (kTestList1.size() + kTestList2.size()));
	NN_ASSERT(theList[0] == kTestList1[0]);
	NN_ASSERT(theList[1] == kTestList1[1]);
	NN_ASSERT(theList[2] == kTestList1[2]);
	NN_ASSERT(theList[3] == kTestList2[0]);
	NN_ASSERT(theList[4] == kTestList2[1]);
	NN_ASSERT(theList[5] == kTestList2[2]);



	// Prepend
	theList = kTestList1;
	prepend(theList, kTestList2);
	
	NN_ASSERT(theList.size() == (kTestList1.size() + kTestList2.size()));
	NN_ASSERT(theList[0] == kTestList2[0]);
	NN_ASSERT(theList[1] == kTestList2[1]);
	NN_ASSERT(theList[2] == kTestList2[2]);
	NN_ASSERT(theList[3] == kTestList1[0]);
	NN_ASSERT(theList[4] == kTestList1[1]);
	NN_ASSERT(theList[5] == kTestList1[2]);
}




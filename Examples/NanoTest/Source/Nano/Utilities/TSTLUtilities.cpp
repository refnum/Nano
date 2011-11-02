/*	NAME:
		TSTLUtilities.cpp

	DESCRIPTION:
		NSTLUtilities tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
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
static const UInt32List kValueList1									= vector((UInt32) 11, (UInt32) 12, (UInt32) 13);
static const UInt32List kValueList2									= vector((UInt32) 24, (UInt32) 25, (UInt32) 26);





//============================================================================
//		TSTLUtilities::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TSTLUtilities::Execute(void)
{	UInt32List		theList;



	// Append
	theList = kValueList1;
	append(theList, kValueList2);
	
	NN_ASSERT(theList.size() == (kValueList1.size() + kValueList2.size()));
	NN_ASSERT(theList[0] == kValueList1[0]);
	NN_ASSERT(theList[1] == kValueList1[1]);
	NN_ASSERT(theList[2] == kValueList1[2]);
	NN_ASSERT(theList[3] == kValueList2[0]);
	NN_ASSERT(theList[4] == kValueList2[1]);
	NN_ASSERT(theList[5] == kValueList2[2]);



	// Prepend
	theList = kValueList1;
	prepend(theList, kValueList2);
	
	NN_ASSERT(theList.size() == (kValueList1.size() + kValueList2.size()));
	NN_ASSERT(theList[0] == kValueList2[0]);
	NN_ASSERT(theList[1] == kValueList2[1]);
	NN_ASSERT(theList[2] == kValueList2[2]);
	NN_ASSERT(theList[3] == kValueList1[0]);
	NN_ASSERT(theList[4] == kValueList1[1]);
	NN_ASSERT(theList[5] == kValueList1[2]);
}





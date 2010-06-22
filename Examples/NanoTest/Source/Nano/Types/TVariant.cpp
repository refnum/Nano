/*	NAME:
		TVariant.cpp

	DESCRIPTION:
		NVariant tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NVariant.h"

#include "TVariant.h"





//============================================================================
//		TVariant::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TVariant::Execute(void)
{	NData			dataValue;
	const NData		*dataPtr;
	NVariant		theValue;
	bool			gotValue;



	// NData
	dataValue.AppendData(4, "test");
	theValue = dataValue;
	
	dataPtr = theValue.GetValue<NData>();
	NN_ASSERT(dataPtr != NULL);
	
	gotValue = theValue.GetValue(dataValue);
	NN_ASSERT(gotValue);
}



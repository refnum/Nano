/*	NAME:
		TStringEncoder.cpp

	DESCRIPTION:
		NStringEncoder tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"

#include "TStringEncoder.h"





//============================================================================
//		TStringEncoder::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TStringEncoder::Execute(void)
{


	// Terminators
	TestTerminator("Hello World", kNStringEncodingUTF8);
	TestTerminator("Hello World", kNStringEncodingUTF16);
	TestTerminator("Hello World", kNStringEncodingUTF32);
}





//============================================================================
//		TStringEncoder::TestTerminator : Test string terminators.
//----------------------------------------------------------------------------
void TStringEncoder::TestTerminator(const NString &theString, NStringEncoding theEncoding)
{	NData				data1, data2;
	NStringEncoder		testEncoder;



	// Add terminator
	data1 = theString.GetData(theEncoding, kNStringRenderNone);
	data2 = data1;

	testEncoder.AddTerminator(data2, theEncoding);
	NN_ASSERT(data1 != data2);

	testEncoder.RemoveTerminator(data2, theEncoding);
	NN_ASSERT(data1 == data2);



	// Remove terminator
	data1 = theString.GetData(theEncoding, kNStringNullTerminate);
	data2 = data1;

	testEncoder.RemoveTerminator(data2, theEncoding);
	NN_ASSERT(data1 != data2);

	testEncoder.AddTerminator(data2, theEncoding);
}


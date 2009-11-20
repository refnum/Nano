/*	NAME:
		TAttributes.cpp

	DESCRIPTION:
		NAttributes tests.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NAttributes.h"

#include "TAttributes.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NBitfield kTestAttribute1								= (1 << 0);
static const NBitfield kTestAttribute2								= (1 << 1);
static const NBitfield kTestAttribute3								= (1 << 30);
static const NBitfield kTestAttribute4								= (1 << 31);





//============================================================================
//		TAttributes::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TAttributes::Execute(void)
{	NAttributes		theAttributes;



	// Execute the tests
	NN_ASSERT(theAttributes.GetAttributes() == kNAttributesNone);
	NN_ASSERT(!theAttributes.HasAttribute(kTestAttribute1));
	NN_ASSERT(!theAttributes.HasAttribute(kTestAttribute2));
	NN_ASSERT(!theAttributes.HasAttribute(kTestAttribute3));
	NN_ASSERT(!theAttributes.HasAttribute(kTestAttribute4));

	theAttributes.SetAttributes(kTestAttribute1 | kTestAttribute3);
	NN_ASSERT( theAttributes.HasAttribute(kTestAttribute1));
	NN_ASSERT(!theAttributes.HasAttribute(kTestAttribute2));
	NN_ASSERT( theAttributes.HasAttribute(kTestAttribute3));
	NN_ASSERT(!theAttributes.HasAttribute(kTestAttribute4));

	theAttributes.SetAttributes(kTestAttribute2 | kTestAttribute4);
	NN_ASSERT( theAttributes.HasAttribute(kTestAttribute1));
	NN_ASSERT( theAttributes.HasAttribute(kTestAttribute2));
	NN_ASSERT( theAttributes.HasAttribute(kTestAttribute3));
	NN_ASSERT( theAttributes.HasAttribute(kTestAttribute4));

	theAttributes.SetAttributes(kNAttributesNone, kTestAttribute1 | kTestAttribute4);
	NN_ASSERT(!theAttributes.HasAttribute(kTestAttribute1));
	NN_ASSERT( theAttributes.HasAttribute(kTestAttribute2));
	NN_ASSERT( theAttributes.HasAttribute(kTestAttribute3));
	NN_ASSERT(!theAttributes.HasAttribute(kTestAttribute4));

	theAttributes.ClearAttributes(kTestAttribute2 | kTestAttribute3);
	NN_ASSERT(!theAttributes.HasAttribute(kTestAttribute1));
	NN_ASSERT(!theAttributes.HasAttribute(kTestAttribute2));
	NN_ASSERT(!theAttributes.HasAttribute(kTestAttribute3));
	NN_ASSERT(!theAttributes.HasAttribute(kTestAttribute4));

	theAttributes.SetAttributes(kTestAttribute1 | kTestAttribute2);
	theAttributes.SetAttributes(kTestAttribute3 | kTestAttribute4, kNAttributesAll);
	NN_ASSERT(!theAttributes.HasAttribute(kTestAttribute1));
	NN_ASSERT(!theAttributes.HasAttribute(kTestAttribute2));
	NN_ASSERT( theAttributes.HasAttribute(kTestAttribute3));
	NN_ASSERT( theAttributes.HasAttribute(kTestAttribute4));

	theAttributes.SetAttributes(kNAttributesAll);
	NN_ASSERT(theAttributes.GetAttributes() == kNAttributesAll);
	NN_ASSERT( theAttributes.HasAttribute(kTestAttribute1));
	NN_ASSERT( theAttributes.HasAttribute(kTestAttribute2));
	NN_ASSERT( theAttributes.HasAttribute(kTestAttribute3));
	NN_ASSERT( theAttributes.HasAttribute(kTestAttribute4));

	theAttributes.ClearAttributes();
	NN_ASSERT(theAttributes.GetAttributes() == kNAttributesNone);
	NN_ASSERT(!theAttributes.HasAttribute(kTestAttribute1));
	NN_ASSERT(!theAttributes.HasAttribute(kTestAttribute2));
	NN_ASSERT(!theAttributes.HasAttribute(kTestAttribute3));
	NN_ASSERT(!theAttributes.HasAttribute(kTestAttribute4));



	


}



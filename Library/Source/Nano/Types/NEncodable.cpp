/*	NAME:
		NEncodable.cpp

	DESCRIPTION:
		Mix-in class for encodable objects.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NEncodable.h"
#include "NVariant.h"
#include "NString.h"




//============================================================================
//		NEncodable::NEncodable : Constructor.
//----------------------------------------------------------------------------
NEncodable::NEncodable(void)
{
}





//============================================================================
//		NEncodable::~NEncodable : Destructor.
//----------------------------------------------------------------------------
NEncodable::~NEncodable(void)
{
}





//============================================================================
//		NEncodable::EncodeSelf : Encode the object.
//----------------------------------------------------------------------------
//		Note : Can't be pure virtual, as we need an NEncodable cast-operator.
//----------------------------------------------------------------------------
void NEncodable::EncodeSelf(NEncoder &/*theEncoder*/) const
{


	// Validate our state
	NN_LOG("NEncodable::EncodeSelf should have been overriden by sub-class");
}





//============================================================================
//		NEncodable::DecodeSelf : Decode the object.
//----------------------------------------------------------------------------
//		Note : Can't be pure virtual, as we need an NEncodable cast-operator.
//----------------------------------------------------------------------------
void NEncodable::DecodeSelf(const NEncoder &/*theEncoder*/)
{


	// Validate our state
	NN_LOG("NEncodable::EncodeSelf should have been overriden by sub-class");
}





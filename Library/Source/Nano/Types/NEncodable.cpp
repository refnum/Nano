/*	NAME:
		NEncodable.cpp

	DESCRIPTION:
		Mix-in class for encodable objects.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NEncodable.h"
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
//		NEncodable::EncodableGetClass : Get the encoder class name.
//----------------------------------------------------------------------------
NString NEncodable::EncodableGetClass(void) const
{


	// Should never be reached
	NN_LOG("NEncodable::EncodableGetClass should have been overriden by NENCODABLE_DECLARE!");

	return("NEncodable");
}





//============================================================================
//		NEncodable::EncodeSelf : Encode the object.
//----------------------------------------------------------------------------
void NEncodable::EncodeSelf(NEncoder &/*theEncoder*/) const
{
}





//============================================================================
//		NEncodable::DecodeSelf : Decode the object.
//----------------------------------------------------------------------------
void NEncodable::DecodeSelf(const NEncoder &/*theEncoder*/)
{
}





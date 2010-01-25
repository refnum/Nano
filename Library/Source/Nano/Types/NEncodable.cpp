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
//		NEncodable::EncodableGetDecoded : Get a decoded object.
//----------------------------------------------------------------------------
NVariant NEncodable::EncodableGetDecoded(const NEncoder &/*theEncoder*/)
{


	// Validate our state
	NN_LOG("NEncodable::EncodableGetDecoded should have been overriden by NENCODABLE_DEFINE!");
	return(NVariant());
}





//============================================================================
//		NEncodable::EncodableGetClass : Get the encoder class name.
//----------------------------------------------------------------------------
NString NEncodable::EncodableGetClass(void) const
{


	// Validate our state
	NN_LOG("NEncodable::EncodableGetClass should have been overriden by NENCODABLE_DEFINE!");
	return("NEncodable");
}





//============================================================================
//		NEncodable::EncodeSelf : Encode the object.
//----------------------------------------------------------------------------
void NEncodable::EncodeSelf(NEncoder &/*theEncoder*/) const
{


	// Validate our state
	NN_LOG("NEncodable::EncodeSelf should have been overriden by sub-class");
}





//============================================================================
//		NEncodable::DecodeSelf : Decode the object.
//----------------------------------------------------------------------------
void NEncodable::DecodeSelf(const NEncoder &/*theEncoder*/)
{


	// Validate our state
	NN_LOG("NEncodable::EncodeSelf should have been overriden by sub-class");
}





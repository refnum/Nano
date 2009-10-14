/*	NAME:
		NPropertyStore.cpp

	DESCRIPTION:
		Property store.

	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NPropertyStore.h"





//============================================================================
//		NPropertyStore::NPropertyStore : Constructor.
//----------------------------------------------------------------------------
NPropertyStore::NPropertyStore(void)
{
}





//============================================================================
//		NPropertyStore::~NPropertyStore : Destructor.
//----------------------------------------------------------------------------
NPropertyStore::~NPropertyStore(void)
{
}





//============================================================================
//		NPropertyStore::EncodeProperties : Encode the properties.
//----------------------------------------------------------------------------
OSStatus NPropertyStore::EncodeProperties(NDictionary &/*theProperties*/)
{


	// Encode our properties
	//
	// Override to encode object properties to the dictionary.
	return(noErr);
}





//============================================================================
//		NPropertyStore::DecodeProperties : Decode the properties.
//----------------------------------------------------------------------------
OSStatus NPropertyStore::DecodeProperties(const NDictionary &/*theProperties*/)
{


	// Decode our properties
	//
	// Override to decode object properties from the dictionary.
	return(noErr);
}



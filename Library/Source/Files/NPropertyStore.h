/*	NAME:
		NPropertyStore.h

	DESCRIPTION:
		Property store.

	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NPROPERTYSTORE_HDR
#define NPROPERTYSTORE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDictionary.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NPropertyStore {
public:
										NPropertyStore(void);
	virtual								~NPropertyStore(void);


	// Encode/decode the properties
	virtual OSStatus					EncodeProperties(      NDictionary &theProperties);
	virtual OSStatus					DecodeProperties(const NDictionary &theProperties);
};





#endif // NPROPERTYSTORE_HDR

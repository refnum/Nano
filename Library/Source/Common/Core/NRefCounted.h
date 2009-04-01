/*	NAME:
		NRefCounted.h

	DESCRIPTION:
		Reference-counted object.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NREFCOUNTED_HDR
#define NREFCOUNTED_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NAtomicInt.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NRefCounted {
public:
										 NRefCounted(void);
	virtual								~NRefCounted(void);


	// Retain/release the object
	void								Retain( void);
	void								Release(void);


	// Get the retain count
	UInt32								GetRetainCount(void) const;


private:
	NAtomicInt							mRefCount;
};





#endif // NREFCOUNTED_HDR



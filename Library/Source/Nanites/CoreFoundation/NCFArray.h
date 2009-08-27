/*	NAME:
		NCFArray.h

	DESCRIPTION:
		CFArrayRef wrapper.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCFARRAY_HDR
#define NCFARRAY_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NArray.h"

#include "NCFObject.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCFArray : public NArray {
public:
										NCFArray(const NArray &theArray);
										NCFArray(CFArrayRef cfObject, bool takeOwnership);
										
										NCFArray(void);
	virtual							   ~NCFArray(void);


	// Get/set the object
	NCFObject							GetObject(void) const;
	bool								SetObject(CFArrayRef cfObject, bool takeOwnership=true);


private:


};




#endif // NCFARRAY_HDR



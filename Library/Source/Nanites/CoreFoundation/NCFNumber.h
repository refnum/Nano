/*	NAME:
		NCFNumber.h

	DESCRIPTION:
		CFNumberRef wrapper.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCFNUMBER_HDR
#define NCFNUMBER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NNumber.h"

#include "NCFObject.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCFNumber : public NNumber {
public:
										NCFNumber(const NVariant &theValue);
										NCFNumber(const NNumber  &theNumber);
										NCFNumber(CFNumberRef cfObject, bool takeOwnership);
										
										NCFNumber(void);
	virtual							   ~NCFNumber(void);


	// Get/set the object
	NCFObject							GetObject(void) const;
	bool								SetObject(CFNumberRef cfObject, bool takeOwnership=true);


private:


};




#endif // NCFNUMBER_HDR



/*	NAME:
		NCFDate.h

	DESCRIPTION:
		CFDateRef wrapper.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCFDATE_HDR
#define NCFDATE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDate.h"

#include "NCFObject.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCFDate : public NDate {
public:
										NCFDate(const NDate &theDate);
										NCFDate(CFDateRef cfObject, bool takeOwnership);
										
										NCFDate(void);
	virtual							   ~NCFDate(void);


	// Get/set the object
	NCFObject							GetObject(void) const;
	bool								SetObject(CFDateRef cfObject, bool takeOwnership=true);


private:


};




#endif // NCFDATE_HDR



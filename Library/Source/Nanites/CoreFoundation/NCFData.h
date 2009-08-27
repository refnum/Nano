/*	NAME:
		NCFData.h

	DESCRIPTION:
		CFDataRef wrapper.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCFDATA_HDR
#define NCFDATA_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NData.h"

#include "NCFObject.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCFData : public NData {
public:
										NCFData(const NData &theData);
										NCFData(CFDataRef cfObject, bool takeOwnership);
										
										NCFData(void);
	virtual							   ~NCFData(void);


	// Get/set the object
	NCFObject							GetObject(void) const;
	bool								SetObject(CFDataRef cfObject, bool takeOwnership=true);


private:


};




#endif // NCFDATA_HDR



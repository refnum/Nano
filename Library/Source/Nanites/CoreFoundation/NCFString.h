/*	NAME:
		NCFString.h

	DESCRIPTION:
		CFStringRef wrapper.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCFSTRING_HDR
#define NCFSTRING_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"

#include "NCFObject.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCFString : public NString {
public:
										NCFString(const NString &theString);
										NCFString(CFStringRef  cfObject, bool takeOwnership);
										NCFString(NSString    *nsObject, bool takeOwnership);
										
										NCFString(void);
	virtual							   ~NCFString(void);


	// Get/set the object
	NCFObject							GetObject(void) const;
	bool								SetObject(CFStringRef cfObject, bool takeOwnership=true);


private:


};




#endif // NCFSTRING_HDR



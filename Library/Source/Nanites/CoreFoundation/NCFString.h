/*	NAME:
		NCFString.h

	DESCRIPTION:
		CFStringRef wrapper.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
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
										NCFString(CFStringRef cfString, bool takeOwnership);
										
										NCFString( void);
	virtual								~NCFString(void);


	// Assign an object
	bool								Set(CFStringRef cfString, bool takeOwnership=true);


	// Operators
										operator CFStringRef(void) const;


private:
	mutable NCFObjectList				mObjects;
};




#endif // NCFSTRING_HDR



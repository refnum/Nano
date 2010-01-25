/*	NAME:
		NCacheValue.h

	DESCRIPTION:
		Cache value.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NNCACHEVALUE_HDR
#define NNCACHEVALUE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NRefCounted.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCacheValue : public NRefCounted {
public:
										NCacheValue(NIndex theCost=0);
	virtual							   ~NCacheValue(void);


	// Is the value purgeable?
	virtual bool						IsPurgeable(void) const;


	// Get the cost
	NIndex								GetCost(void) const;


	// Get/set the access time
	NTime								GetAccessTime(void) const;
	void								SetAccessTime(void);


private:
	NIndex								mCost;
	NTime								mAccessTime;
};






#endif // NNCACHEVALUE_HDR



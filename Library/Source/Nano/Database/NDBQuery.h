/*	NAME:
		NDBQuery.h

	DESCRIPTION:
		Database query.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDBQUERY_HDR
#define NDBQUERY_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDictionary.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Misc
typedef SInt32  NDBStatus;
typedef void   *NDBHandleRef;
typedef void   *NDBQueryRef;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDBQuery {
public:
										NDBQuery(const NString &theValue);
										NDBQuery(const char    *theValue);

										NDBQuery(void);
	virtual							   ~NDBQuery(void);


	// Get/set the query
	NString								GetValue(void) const;
	void								SetValue(const NString &theValue);


	// Get/set the parameters
	NDictionary							GetParameters(void) const;
	void								SetParameters(const NDictionary &theValue);


private:
	NString								mValue;
	NDictionary							mParameters;
};




#endif // NDBQUERY_HDR



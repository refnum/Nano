/*	NAME:
		NURLRequest.h

	DESCRIPTION:
		URL request.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NURLREQUEST_HDR
#define NURLREQUEST_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NURL.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NURLRequest {
public:
										NURLRequest(void);
	virtual							   ~NURLRequest(void);


	// Get/set the URL
	NURL								GetURL(void) const;
	void								SetURL(const NURL &theURL);


private:
	NURL								mURL;
};




#endif // NURLREQUEST_HDR



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
#include "NPropertyStore.h"
#include "NURL.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Keys
static const NString kNURLRequestUserAgentKey						= "User-Agent";
static const NString kNURLRequestBodyKey							= "http*body";


// Commands
typedef enum {
	kURLGet,
	kURLPut,
	kURLHead
} NURLCommand;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NURLRequest : public NPropertyStore {
public:
										NURLRequest(const NURL &theURL="");
	virtual							   ~NURLRequest(void);


	// Is the request valid?
	bool								IsValid(void) const;


	// Get/set the URL
	NURL								GetURL(void) const;
	void								SetURL(const NURL &theURL);


	// Get/set the command
	NURLCommand							GetCommand(void) const;
	void								SetCommand(NURLCommand theCmd);


private:
	NURL								mURL;
	NURLCommand							mCmd;
};




#endif // NURLREQUEST_HDR



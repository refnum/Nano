/*	NAME:
		NMessageServer.h

	DESCRIPTION:
		Message server.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NMESSAGESERVER_HDR
#define NMESSAGESERVER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSocket.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NMessageServer : public NSocketDelegate {
public:
										NMessageServer(void);
	virtual							   ~NMessageServer(void);


	// Start/stop the server
	NStatus								Start(void);
	void								Stop( void);


private:
	NSocket								mSocket;
};




#endif // NMESSAGESERVER_HDR







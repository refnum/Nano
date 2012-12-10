/*	NAME:
		NMessageServer.cpp

	DESCRIPTION:
		Message server.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMessageServer.h"





//============================================================================
//		NMessageServer::NMessageServer : Constructor.
//----------------------------------------------------------------------------
NMessageServer::NMessageServer(void)
				: mSocket(this)
{
}





//============================================================================
//		NMessageServer::~NMessageServer : Destructor.
//----------------------------------------------------------------------------
NMessageServer::~NMessageServer(void)
{
}





//============================================================================
//		NMessageServer::Start : Start the server.
//----------------------------------------------------------------------------
NStatus NMessageServer::Start(void)
{


	// dair, to do
	return(kNErrNotSupported);
}





//============================================================================
//		NMessageServer::Stop : Stop the server.
//----------------------------------------------------------------------------
void NMessageServer::Stop(void)
{
}




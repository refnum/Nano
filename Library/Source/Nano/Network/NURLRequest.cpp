/*	NAME:
		NURLRequest.cpp

	DESCRIPTION:
		URL request.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NNetworkManager.h"
#include "NURLRequest.h"





//============================================================================
//		NURLRequest::NURLRequest : Constructor.
//----------------------------------------------------------------------------
NURLRequest::NURLRequest(const NURL &theURL)
{


	// Initialise ourselves
	mURL = theURL;
	mCmd = kURLGet;

	SetValue(kNURLRequestUserAgentKey, NNetworkManager::Get()->GetUserAgent());
}





//============================================================================
//		NURLRequest::~NURLRequest : Destructor.
//----------------------------------------------------------------------------
NURLRequest::~NURLRequest(void)
{
}





//============================================================================
//		NURLRequest::IsValid : Is the request valid?
//----------------------------------------------------------------------------
bool NURLRequest::IsValid(void) const
{


	// Check our state
	return(mURL.IsValid());
}





//============================================================================
//		NURLRequest::GetURL : Get the URL.
//----------------------------------------------------------------------------
NURL NURLRequest::GetURL(void) const
{


	// Get the URL
	return(mURL);
}





//============================================================================
//		NURLRequest::SetURL : Set the URL.
//----------------------------------------------------------------------------
void NURLRequest::SetURL(const NURL &theURL)
{


	// Set the URL
	mURL = theURL;
}





//============================================================================
//		NURLRequest::GetCommand : Get the command.
//----------------------------------------------------------------------------
NURLCommand NURLRequest::GetCommand(void) const
{


	// Get the command
	return(mCmd);
}





//============================================================================
//		NURLRequest::SetCommand : Set the command.
//----------------------------------------------------------------------------
void NURLRequest::SetCommand(NURLCommand theCmd)
{


	// Set the command
	mCmd = theCmd;
}




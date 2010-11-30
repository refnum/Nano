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
#include "NURLRequest.h"





//============================================================================
//		NURLRequest::NURLRequest : Constructor.
//----------------------------------------------------------------------------
NURLRequest::NURLRequest(void)
{
}





//============================================================================
//		NURLRequest::~NURLRequest : Destructor.
//----------------------------------------------------------------------------
NURLRequest::~NURLRequest(void)
{
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






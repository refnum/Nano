/*	NAME:
		TURL.cpp

	DESCRIPTION:
		NURL tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NURL.h"

#include "TURL.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NURL kValueURL1										= "http://www.apple.com/path/to/file.txt";
static const NURL kValueURL2										= "http://www.apple.com:80/path/to/file.txt";

static const NString kValueScheme									= "http";
static const NString kValueHost										= "www.apple.com";
static const NIndex  kValuePort										= 80;
static const NString kValuePath										= "/path/to/file.txt";





//============================================================================
//		TURL::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TURL::Execute(void)
{	


	// Execute the tests
	NN_ASSERT(kValueURL1.GetScheme() == kValueScheme);
	NN_ASSERT(kValueURL1.GetHost()   == kValueHost);
	NN_ASSERT(kValueURL1.GetPort()   == kValuePort);
	NN_ASSERT(kValueURL1.GetPath()   == kValuePath);
	
	NN_ASSERT(kValueURL2.GetScheme() == kValueScheme);
	NN_ASSERT(kValueURL2.GetHost()   == kValueHost);
	NN_ASSERT(kValueURL2.GetPort()   == kValuePort);
	NN_ASSERT(kValueURL2.GetPath()   == kValuePath);
}




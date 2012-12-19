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
static const NURL kTestURL1											= "http://www.apple.com/path/to/file.txt";
static const NURL kTestURL2											= "http://www.apple.com:80/path/to/file.txt";

static const NString kTestScheme									= "http";
static const NString kTestHost										= "www.apple.com";
static const NIndex  kTestPort										= 80;
static const NString kTestPath										= "/path/to/file.txt";





//============================================================================
//		TURL::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TURL::Execute(void)
{	


	// Execute the tests
	NN_ASSERT(kTestURL1.GetScheme() == kTestScheme);
	NN_ASSERT(kTestURL1.GetHost()   == kTestHost);
	NN_ASSERT(kTestURL1.GetPort()   == kTestPort);
	NN_ASSERT(kTestURL1.GetPath()   == kTestPath);
	
	NN_ASSERT(kTestURL2.GetScheme() == kTestScheme);
	NN_ASSERT(kTestURL2.GetHost()   == kTestHost);
	NN_ASSERT(kTestURL2.GetPort()   == kTestPort);
	NN_ASSERT(kTestURL2.GetPath()   == kTestPath);
}



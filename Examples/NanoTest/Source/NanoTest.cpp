/*	NAME:
		NanoTest.cpp

	DESCRIPTION:
		NanoTest app.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Build constants
//----------------------------------------------------------------------------
#define CATCH_CONFIG_RUNNER											1





//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCommandLine.h"
#include "NTestFixture.h"





//============================================================================
//		main : Program entry point.
//----------------------------------------------------------------------------
int main (int argc, const char **argv)
{


	// Initialise ourselves
	NCommandLine::Get()->SetArguments(argc, argv);



	// Execute the tests
    return(Catch::Session().run(argc, (char * const *) argv));
}





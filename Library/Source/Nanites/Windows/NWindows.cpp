/*	NAME:
		NWindows.cpp

	DESCRIPTION:
		Windows support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NWindows.h"





//============================================================================
//		StCOM::StCOM : Constructor.
//----------------------------------------------------------------------------
StCOM::StCOM(DWORD coInit)
{	HRESULT		winErr;



	// Initialise COM
	winErr = CoInitializeEx(NULL, coInit);
	NN_ASSERT_SUCCESS(winErr);
}





//============================================================================
//		StCOM::~StCOM : Destructor.
//----------------------------------------------------------------------------
StCOM::~StCOM(void)
{


	// Clean up
	CoUninitialize();
}



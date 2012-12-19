/*	NAME:
		TDebug.cpp

	DESCRIPTION:
		NDebug tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDebug.h"

#include "CTestUtilities.h"
#include "TDebug.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kTestMessage									= "Hello World";





//============================================================================
//		TDebug::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TDebug::Execute(void)
{	NString		theValue;
	NDebug		*nDebug;



	// Accessor
	nDebug = NDebug::Get();
	NN_ASSERT(nDebug != NULL);



	// Path logging
	NN_ASSERT(!nDebug->ShowPath());

	nDebug->SetShowPath(true);
	NN_ASSERT(nDebug->ShowPath());

	nDebug->SetShowPath(false);
	NN_ASSERT(!nDebug->ShowPath());



	// Log output
	CTestUtilities::SetDebugCapture(true);

	nDebug->LogMessage(__FILE__, __LINE__, kTestMessage);
	
	theValue = CTestUtilities::SetDebugCapture(false);
	theValue.Trim();

	NN_ASSERT(theValue.Contains("TDebug.cpp"));
	NN_ASSERT(theValue.EndsWith(kTestMessage));
}




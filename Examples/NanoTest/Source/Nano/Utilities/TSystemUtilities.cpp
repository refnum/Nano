/*	NAME:
		TSystemUtilities.cpp

	DESCRIPTION:
		NSystemUtilities tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"

#include "TSystemUtilities.h"





//============================================================================
//		TSystemUtilities::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TSystemUtilities::Execute(void)
{	OSVersion	theVers;



	// Version
	theVers = NSystemUtilities::GetOSVersion();
	NN_ASSERT(theVers != kOSUnknown);
}



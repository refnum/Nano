/*	NAME:
		TFileUtilities.cpp

	DESCRIPTION:
		NFileUtilities tests.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFileUtilities.h"

#include "TFileUtilities.h"





//============================================================================
//		TFileUtilities::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TFileUtilities::Execute(void)
{	NFile		theFile;



	// Get directories
	theFile = NFileUtilities::GetDirectory(kNLocationHome);
	theFile = NFileUtilities::GetDirectory(kNLocationDesktop);



	// Get files
	theFile = NFileUtilities::GetTemporaryFile();
}



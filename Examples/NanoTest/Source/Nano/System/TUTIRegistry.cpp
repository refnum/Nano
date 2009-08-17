/*	NAME:
		TUTIRegistry.cpp

	DESCRIPTION:
		NUTIRegistry tests.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NUTIRegistry.h"

#include "TUTIRegistry.h"





//============================================================================
//		TUTIRegistry::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TUTIRegistry::Execute(void)
{	NUTIRegistry		*theRegistry;
	NString				theUTI;



	// Get the state we need
	theRegistry = NUTIRegistry::Get();



	// Query
	theUTI = theRegistry->Get()->GetUTI(kNUTITagClassFileExtension, ".jpg");
	NN_ASSERT(theUTI == kNUTTypeJPEG);
}










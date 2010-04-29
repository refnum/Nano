/*	NAME:
		TUTIRegistry.cpp

	DESCRIPTION:
		NUTIRegistry tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
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
{	NUTIRegistry	*theRegistry;



	// Get the state we need
	theRegistry = NUTIRegistry::Get();



	// Query
	NN_ASSERT(theRegistry->GetUTI(kNUTITagClassFileExtension, "gif") == kNUTTypeGIF);
	NN_ASSERT(theRegistry->GetUTI(kNUTITagClassFileExtension, "png") == kNUTTypePNG);
	NN_ASSERT(theRegistry->GetUTI(kNUTITagClassFileExtension, "jpg") == kNUTTypeJPEG);
}










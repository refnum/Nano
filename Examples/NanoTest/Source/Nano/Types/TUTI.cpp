/*	NAME:
		TUTI.cpp

	DESCRIPTION:
		NUTI tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFile.h"
#include "NUTI.h"

#include "TUTI.h"





//============================================================================
//		TUTI::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TUTI::Execute(void)
{


	// Execute the tests
	NN_ASSERT(NFile("test.gif").GetUTI() == NUTI(kNUTTypeGIF));
	NN_ASSERT(NFile("test.png").GetUTI() == NUTI(kNUTTypePNG));
	NN_ASSERT(NFile("test.jpg").GetUTI() == NUTI(kNUTTypeJPEG));
}



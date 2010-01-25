/*	NAME:
		TFileIterator.cpp

	DESCRIPTION:
		NFileIterator tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFileIterator.h"

#include "TFileIterator.h"





//============================================================================
//		TFileIterator::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TFileIterator::Execute(void)
{	NFileIterator		fileIter;
	NFileList			theFiles;



	// Iterate
	theFiles = fileIter.GetFiles(NFile("/tmp"));
	NN_ASSERT(!theFiles.empty());
}





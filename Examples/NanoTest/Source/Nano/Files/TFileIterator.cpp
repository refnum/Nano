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
//		Constants
//----------------------------------------------------------------------------
// Paths
#if NN_TARGET_MAC
	static const NString kTestPath									= "/tmp";

#elif NN_TARGET_WINDOWS
	static const NString kTestPath									= "c:\\windows\\temp";

#else
	UNKNOWN TARGET
#endif

		
		

		
//============================================================================
//		TFileIterator::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TFileIterator::Execute(void)
{	NFileIterator		fileIter;
	NFileList			theFiles;



	// Iterate
	theFiles = fileIter.GetFiles(NFile(kTestPath));
	NN_ASSERT(!theFiles.empty());
}




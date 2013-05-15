/*	NAME:
		TFileMap.cpp

	DESCRIPTION:
		NFileMap tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFileMap.h"

#include "TFileMap.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Misc
static const UInt32 kBufferSize										= 9;
static const UInt8  kBufferData[kBufferSize]						= { 'T', 'e', 's', 't', ' ', 'd', 'a', 't', 'a' };





//============================================================================
//		TFileMap::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TFileMap::Execute(void)
{	NFile			theFile;
	NFileMap		fileMap;
	void			*thePtr;
	NStatus			theErr;



	// Prepare the file
	theFile = NFileUtilities::GetTemporaryFile();
	theErr  = NFileUtilities::SetFileData(theFile, NData(kBufferSize, kBufferData));
	NN_ASSERT_NOERR(theErr);



	// Open
	NN_ASSERT(!fileMap.IsOpen());
	
	theErr = fileMap.Open(theFile);
	NN_ASSERT_NOERR(theErr);

	NN_ASSERT(fileMap.IsOpen());



	// Access
	thePtr = fileMap.Map(0, kBufferSize);
	NN_ASSERT(memcmp(thePtr, kBufferData, kBufferSize) == 0);

	fileMap.Unmap(thePtr);
	
	
	
	// Close
	fileMap.Close();
	NN_ASSERT(!fileMap.IsOpen());

	theErr = theFile.Delete();
	NN_ASSERT_NOERR(theErr);
}





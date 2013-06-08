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
#include "NTestFixture.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Misc
static const UInt32 kBufferSize										= 9;
static const UInt8  kBufferData[kBufferSize]						= { 'T', 'e', 's', 't', ' ', 'd', 'a', 't', 'a' };





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NFILEMAP(_name, _desc)									NANO_TEST(TFileMap, _name, _desc)

NANO_FIXTURE(TFileMap)
{
	NFileMap	fileMap;
	NFile		theFile;
	NStatus		theErr;
	
	SETUP
	{
		theFile = NFileUtilities::GetTemporaryFile();
		theErr  = NFileUtilities::SetFileData(theFile, NData(kBufferSize, kBufferData));
		NN_ASSERT_NOERR(theErr);
	}
	
	TEARDOWN
	{
		if (theFile.Exists())
			{
			theErr = theFile.Delete();
			NN_ASSERT_NOERR(theErr);
			}
	}
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFILEMAP("Default", "Default state")
{


	// Perform the test
	NN_ASSERT(!fileMap.IsOpen());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFILEMAP("Open", "Open and close")
{


	// Perform the test
	theErr = fileMap.Open(theFile);
	NN_ASSERT_NOERR(theErr);

	NN_ASSERT(fileMap.IsOpen());

	fileMap.Close();
	NN_ASSERT(!fileMap.IsOpen());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFILEMAP("Map", "Map and unmap")
{	void	*thePtr;



	// Perform the test
	theErr = fileMap.Open(theFile);
	NN_ASSERT_NOERR(theErr);

	thePtr = fileMap.Map(0, kBufferSize);
	NN_ASSERT(memcmp(thePtr, kBufferData, kBufferSize) == 0);

	fileMap.Unmap(thePtr);
}






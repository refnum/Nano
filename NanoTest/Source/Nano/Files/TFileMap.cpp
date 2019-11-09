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
#include "NFileUtilities.h"
#include "NTestFixture.h"
#include "NFileMap.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Misc
static const NIndex  kBufferSize									= 9;
static const uint8_t kBufferData[kBufferSize]						= { 'T', 'e', 's', 't', ' ', 'd', 'a', 't', 'a' };





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NFILEMAP(...)											TEST_NANO(TFileMap, ##__VA_ARGS__)

FIXTURE_NANO(TFileMap)
{
	NFileMap	fileMap;
	NFile		theFile;
	NStatus		theErr;
	
	SETUP
	{
		theFile = NFileUtilities::GetTemporaryFile();
		theErr  = NFileUtilities::SetFileData(theFile, NData(kBufferSize, kBufferData));
		REQUIRE_NOERR(theErr);
	}
	
	TEARDOWN
	{
		if (theFile.Exists())
			{
			theErr = theFile.Delete();
			REQUIRE_NOERR(theErr);
			}
	}
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFILEMAP("Default")
{


	// Perform the test
	REQUIRE(!fileMap.IsOpen());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFILEMAP("Open")
{


	// Perform the test
	theErr = fileMap.Open(theFile);
	REQUIRE_NOERR(theErr);

	REQUIRE(fileMap.IsOpen());

	fileMap.Close();
	REQUIRE(!fileMap.IsOpen());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFILEMAP("Map")
{	void	*thePtr;



	// Perform the test
	theErr = fileMap.Open(theFile);
	REQUIRE_NOERR(theErr);

	thePtr = fileMap.Map(0, kBufferSize);
	REQUIRE(memcmp(thePtr, kBufferData, kBufferSize) == 0);

	fileMap.Unmap(thePtr);

	fileMap.Close();
	REQUIRE(!fileMap.IsOpen());
}






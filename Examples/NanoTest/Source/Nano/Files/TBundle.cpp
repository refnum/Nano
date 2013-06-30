/*	NAME:
		TBundle.cpp

	DESCRIPTION:
		NBundle tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTestFixture.h"
#include "NBundle.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Paths
#if NN_TARGET_MAC
	static const NString kPathBundle								= "/Applications/Safari.app";
#else
	static const NString kPathBundle								= "";

#endif





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NBUNDLE(...)											TEST_NANO(TBundle, ##__VA_ARGS__)

FIXTURE_NANO(TBundle)
{
	NBundle		theBundle;

	SETUP
	{
		// Mac-specific setup
		if (!kPathBundle.IsEmpty())
			theBundle = NBundle(NFile(kPathBundle));
	}
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NBUNDLE("Default")
{	NString		theString;



	// Perform the test
	REQUIRE(theBundle.IsValid());
	
	theString = theBundle.GetIdentifier();
	if (!kPathBundle.IsEmpty())
		REQUIRE(!theString.IsEmpty());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NBUNDLE("Files")
{	NFile		theFile;



	// Perform the test
	theFile = theBundle.GetFile();
	REQUIRE(theFile.IsDirectory());

	theFile = theBundle.GetResources();
	if (!kPathBundle.IsEmpty())
		REQUIRE(theFile.IsDirectory());

	theFile = theBundle.GetExecutable();
	if (!kPathBundle.IsEmpty())
		REQUIRE(theFile.IsFile());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NBUNDLE("Info.plist")
{	NDictionary		theInfo;



	// Perform the test
	theInfo = theBundle.GetInfoDictionary();
	if (!kPathBundle.IsEmpty())
		REQUIRE(!theInfo.IsEmpty());
}



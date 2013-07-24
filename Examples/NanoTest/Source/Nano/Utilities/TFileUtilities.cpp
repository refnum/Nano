/*	NAME:
		TFileUtilities.cpp

	DESCRIPTION:
		NFileUtilities tests.

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





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kTestText										= "test";
static const uint8_t kTestTextUTF8[]								= { 0xEF, 0xBB, 0xBF, 0x74, 0x65, 0x73, 0x74 };
static const uint8_t kTestTextUTF16BE[]								= { 0xFE, 0xFF, 0x00, 0x74, 0x00, 0x65, 0x00, 0x73, 0x00, 0x74 };
static const uint8_t kTestTextUTF16LE[]								= { 0xFF, 0xFE, 0x74, 0x00, 0x65, 0x00, 0x73, 0x00, 0x74, 0x00 };





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NFILEUTILITIES(...)									TEST_NANO(TFileUtilities, ##__VA_ARGS__)

FIXTURE_NANO(TFileUtilities)
{
	NFile	theFile;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFILEUTILITIES("Directories")
{


	// Perform the test
	theFile = NFileUtilities::GetDirectory(kNLocationHome);
	REQUIRE(theFile.IsDirectory());
	
	theFile = NFileUtilities::GetDirectory(kNLocationDesktop);
	REQUIRE(theFile.IsDirectory());
	
	theFile = NFileUtilities::GetDirectory(kNLocationPreferences);
	REQUIRE(theFile.IsDirectory());

	theFile = NFileUtilities::GetCWD();
	REQUIRE(theFile.IsDirectory());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFILEUTILITIES("DirectorySize")
{	uint64_t	theSize;



	// Perform the test
	theFile = NFileUtilities::GetDirectory(kNLocationLogs);
	theSize = NFileUtilities::GetDirectorySize(theFile);
	REQUIRE(theSize > (uint64_t) 0);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFILEUTILITIES("TemporaryFile")
{


	// Perform the test
	theFile = NFileUtilities::GetTemporaryFile();
	REQUIRE(!theFile.Exists());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFILEUTILITIES("ReadWriteData")
{	NData		theData1, theData2;
	NStatus		theErr;



	// Perform the test
	theFile  = NFileUtilities::GetTemporaryFile();
	theData1 = NData(NN_ARRAY_SIZE(kTestTextUTF16BE), kTestTextUTF16BE);
	
	theErr   = NFileUtilities::SetFileData(theFile, theData1);
	theData2 = NFileUtilities::GetFileData(theFile);
	REQUIRE_NOERR(theErr);
	REQUIRE(theData1.GetHash() == theData2.GetHash());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFILEUTILITIES("ReadWriteText")
{	NData		theData1, theData2;
	NString		theText;
	NStatus		theErr;



	// Perform the test
	theFile = NFileUtilities::GetTemporaryFile();
	REQUIRE(!theFile.Exists());

	theErr   = NFileUtilities::SetFileText(theFile, kTestText, kNStringEncodingUTF8, kNStringUnicodeBOM);
	theData1 = NData(NN_ARRAY_SIZE(kTestTextUTF8), kTestTextUTF8);
	theData2 = NFileUtilities::GetFileData(theFile);
	REQUIRE_NOERR(theErr);
	REQUIRE(theData1.GetHash() == theData2.GetHash());

	theErr   = NFileUtilities::SetFileText(theFile, kTestText, kNStringEncodingUTF16BE, kNStringUnicodeBOM);
	theData1 = NData(NN_ARRAY_SIZE(kTestTextUTF16BE), kTestTextUTF16BE);
	theData2 = NFileUtilities::GetFileData(theFile);
	REQUIRE_NOERR(theErr);
	REQUIRE(theData1.GetHash() == theData2.GetHash());

	theErr   = NFileUtilities::SetFileText(theFile, kTestText, kNStringEncodingUTF16LE, kNStringUnicodeBOM);
	theData1 = NData(NN_ARRAY_SIZE(kTestTextUTF16LE), kTestTextUTF16LE);
	theData2 = NFileUtilities::GetFileData(theFile);
	REQUIRE_NOERR(theErr);
	REQUIRE(theData1.GetHash() == theData2.GetHash());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFILEUTILITIES("TextEncoding")
{	NString		theText;
	NStatus		theErr;



	// Perform the test
	theFile = NFileUtilities::GetTemporaryFile();
	REQUIRE(!theFile.Exists());

	theErr   = NFileUtilities::SetFileText(theFile, kTestText, kNStringEncodingUTF16BE, kNStringUnicodeBOM);
	theText  = NFileUtilities::GetFileText(theFile);
	REQUIRE_NOERR(theErr);
	REQUIRE(theText                                        == kTestText);		// Compare as UTF16
	REQUIRE(strcmp(theText.GetUTF8(), kTestText.GetUTF8()) == 0);				// Compare as UTF8
}






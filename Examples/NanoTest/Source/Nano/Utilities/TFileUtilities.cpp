/*	NAME:
		TFileUtilities.cpp

	DESCRIPTION:
		NFileUtilities tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFileUtilities.h"

#include "TFileUtilities.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kValueText								= "test";
static const UInt8   kValueTextUTF8[]						= { 0xEF, 0xBB, 0xBF, 0x74, 0x65, 0x73, 0x74 };
static const UInt8   kValueTextUTF16BE[]					= { 0xFE, 0xFF, 0x00, 0x74, 0x00, 0x65, 0x00, 0x73, 0x00, 0x74 };
static const UInt8   kValueTextUTF16LE[]					= { 0xFF, 0xFE, 0x74, 0x00, 0x65, 0x00, 0x73, 0x00, 0x74, 0x00 };





//============================================================================
//		TFileUtilities::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TFileUtilities::Execute(void)
{	NData		theData1, theData2;
	NString		theText;
	NFile		theFile;
	NStatus		theErr;



	// Get directories
	theFile = NFileUtilities::GetDirectory(kNLocationHome);
	theFile = NFileUtilities::GetDirectory(kNLocationDesktop);



	// Get files
	theFile = NFileUtilities::GetTemporaryFile();



	// Data files
	theFile  = NFileUtilities::GetTemporaryFile();
	theData1 = NData(NN_ARRAY_SIZE(kValueTextUTF16BE), kValueTextUTF16BE);
	
	theErr   = NFileUtilities::SetFileData(theFile, theData1);
	theData2 = NFileUtilities::GetFileData(theFile);
	NN_ASSERT_NOERR(theErr);
	NN_ASSERT(theData1.GetHash() == theData2.GetHash());



	// Text file encodings
	theErr   = NFileUtilities::SetFileText(theFile, kValueText, kNStringEncodingUTF8, kNStringUnicodeBOM);
	theData1 = NData(NN_ARRAY_SIZE(kValueTextUTF8), kValueTextUTF8);
	theData2 = NFileUtilities::GetFileData(theFile);
	NN_ASSERT_NOERR(theErr);
	NN_ASSERT(theData1.GetHash() == theData2.GetHash());

	theErr   = NFileUtilities::SetFileText(theFile, kValueText, kNStringEncodingUTF16BE, kNStringUnicodeBOM);
	theData1 = NData(NN_ARRAY_SIZE(kValueTextUTF16BE), kValueTextUTF16BE);
	theData2 = NFileUtilities::GetFileData(theFile);
	NN_ASSERT_NOERR(theErr);
	NN_ASSERT(theData1.GetHash() == theData2.GetHash());

	theErr   = NFileUtilities::SetFileText(theFile, kValueText, kNStringEncodingUTF16LE, kNStringUnicodeBOM);
	theData1 = NData(NN_ARRAY_SIZE(kValueTextUTF16LE), kValueTextUTF16LE);
	theData2 = NFileUtilities::GetFileData(theFile);
	NN_ASSERT_NOERR(theErr);
	NN_ASSERT(theData1.GetHash() == theData2.GetHash());



	// Text file encoding detection
	theErr   = NFileUtilities::SetFileText(theFile, kValueText, kNStringEncodingUTF16BE, kNStringUnicodeBOM);
	theText  = NFileUtilities::GetFileText(theFile);
	NN_ASSERT(theText                                         == kValueText);		// Compare as UTF16
	NN_ASSERT(strcmp(theText.GetUTF8(), kValueText.GetUTF8()) == 0);				// Compare as UTF8
}




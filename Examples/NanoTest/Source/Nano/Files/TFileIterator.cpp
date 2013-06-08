/*	NAME:
		TFileIterator.cpp

	DESCRIPTION:
		NFileIterator tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFileIterator.h"
#include "NTestFixture.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Paths
#if NN_TARGET_MAC
	static const NString kTestParent								= "/tmp/TFileIterator";

#elif NN_TARGET_WINDOWS
	static const NString kTestParent								= "c:\\windows\\temp\\TFileIterator";

#elif NN_TARGET_LINUX
	static const NString kTestParent								= "/tmp/TFileIterator";

#else
	UNKNOWN TARGET
#endif

static const NString kTestChild										= kTestParent + NN_DIR + "Child1" + NN_DIR + "Child2" + NN_DIR + "Child3";





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NFILEITERATOR(_name, _desc)							NANO_TEST(TFileIterator, _name, _desc)

NANO_FIXTURE(TFileIterator)
{
	NFileIterator	fileIter;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFILEITERATOR("Iterate", "File iterator")
{	NFileList		theFiles;
	NFile			theFile;



	// Perform the test
	theFile = kTestChild;
	theFile.CreateFile();
	NN_ASSERT(theFile.Exists());

	theFile  = kTestParent;
	theFiles = fileIter.GetFiles(theFile);
	NN_ASSERT(theFiles.size() == 3);

	theFile = kTestParent;
	theFile.Delete();
}




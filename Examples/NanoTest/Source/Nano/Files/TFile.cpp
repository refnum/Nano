/*	NAME:
		TFile.cpp

	DESCRIPTION:
		NFile tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFile.h"
#include "NTestFixture.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Misc
static const UInt32 kBufferSize										= 9;
static const UInt8  kBufferData1[kBufferSize]						= { 'T', 'e', 's', 't', ' ', 'd', 'a', 't', 'a' };
static const UInt8  kBufferData2[kBufferSize]						= { 'A', 'l', 's', 'o', ' ', 't', 'h', 'i', 's' };

static const UInt32 kFileSizeTmp1									= 111;
static const UInt32 kFileSizeTmp2									= 222;


// Paths
static const NString kTestTmpName									= "TFile";
static const NString kTestTmpExtension								= "txt";
static const NString kTestTmpName2									= "TFileRename";
static const NString kTestTmpExtension2								= "txtrename";
static const NString kTestTmpName3									= "TFileMove";
static const NString kTestTmpPathParent								= "TFileParent";
static const NString kTestTmpPathChild								= kTestTmpPathParent + NN_DIR + "TFileChild1" + NN_DIR + "TFileChild2";
static const NString kTestTmpPathFile								= kTestTmpPathChild  + NN_DIR + "TFileFileInChildInChildInParent.txt";

#if NN_TARGET_MAC
	static const NString kPathTmp									= "/tmp";
	static const NString kPathFile									= "/bin/bash";
	static const NString kPathLinkTarget							= "/tmp/link.target";
	static const NString kPathLinkFile								= "/tmp/link.start";
	static const NString kPathDirectoryRoot							= "/Library";
	static const NString kPathDirectoryChildren						= "Application Support/Apple";
	static const NString kPathFileTmp								= kPathTmp + NN_DIR + kTestTmpName  + "." + kTestTmpExtension;
	static const NString kPathFileTmp2								= kPathTmp + NN_DIR + kTestTmpName2 + "." + kTestTmpExtension2;
	static const NString kPathDirTmp								= kPathTmp + NN_DIR + kTestTmpName3;

#elif NN_TARGET_WINDOWS
	static const NString kPathTmp									= "c:\\windows\\temp";
	static const NString kPathFile									= "c:\\windows\\WindowsShell.Manifest";
	static const NString kPathLinkTarget							= "c:\\windows\\temp\\link.target";
	static const NString kPathLinkFile								= "c:\\windows\\temp\\link.lnk";
	static const NString kPathDirectoryRoot							= "c:\\windows";
	static const NString kPathDirectoryChildren						= "boot\\fonts";
	static const NString kPathFileTmp								= kPathTmp + NN_DIR + kTestTmpName  + "." + kTestTmpExtension;
	static const NString kPathFileTmp2								= kPathTmp + NN_DIR + kTestTmpName2 + "." + kTestTmpExtension2;
	static const NString kPathDirTmp								= kPathTmp + NN_DIR + kTestTmpName3;

#elif NN_TARGET_LINUX
	static const NString kPathTmp									= "/tmp";
	static const NString kPathFile									= "/bin/bash";
	static const NString kPathLinkTarget							= "/tmp/link.target";
	static const NString kPathLinkFile								= "/tmp/link.start";
	static const NString kPathDirectoryRoot							= "/boot";
	static const NString kPathDirectoryChildren						= "grub";
	static const NString kPathFileTmp								= kPathTmp + NN_DIR + kTestTmpName  + "." + kTestTmpExtension;
	static const NString kPathFileTmp2								= kPathTmp + NN_DIR + kTestTmpName2 + "." + kTestTmpExtension2;
	static const NString kPathDirTmp								= kPathTmp + NN_DIR + kTestTmpName3;

#else
	UNKNOWN TARGET
#endif






//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NFILE(_name, _desc)									NANO_TEST(TFile, _name, _desc)

NANO_FIXTURE(TFile)
{
	NFile		tmpFile, tmpFile2;
	NFile		theFile;
	NFile		theDir;
	NStatus		theErr;
	
	SETUP
	{
		tmpFile  = kPathFileTmp;
		tmpFile2 = kPathFileTmp2;
		
		theFile = kPathFile;
		theDir  = NFile(kPathDirectoryRoot).GetChild(kPathDirectoryChildren);
		theErr  = kNoErr;
	}
	
	TEARDOWN
	{
		if (tmpFile.IsOpen())
			tmpFile.Close();

		if (tmpFile2.IsOpen())
			tmpFile2.Close();
			
		if (tmpFile.Exists())
			{
			theErr = tmpFile.Delete();
			NN_ASSERT_NOERR(theErr);
			}

		if (tmpFile2.Exists())
			{
			theErr = tmpFile2.Delete();
			NN_ASSERT_NOERR(theErr);
			}
	}
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFILE("Default", "Default state")
{	NFile	newFile;



	// Perform the test
	REQUIRE( theFile.IsValid());
	REQUIRE(  theDir.IsValid());
	REQUIRE(!newFile.IsValid());

	REQUIRE( theFile.IsFile());
	REQUIRE( !theDir.IsFile());
	REQUIRE(!newFile.IsFile());

	REQUIRE(!theFile.IsDirectory());
	REQUIRE(  theDir.IsDirectory());
	REQUIRE(!newFile.IsDirectory());

	REQUIRE(!theFile.IsWriteable());
	REQUIRE(  theDir.IsWriteable());
	REQUIRE(!newFile.IsWriteable());

	REQUIRE(!theFile.IsOpen());
	REQUIRE( !theDir.IsOpen());
	REQUIRE(!newFile.IsOpen());

	REQUIRE( theFile.Exists());
	REQUIRE(  theDir.Exists());
	REQUIRE(!newFile.Exists());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFILE("Comparisons", "Comparisons")
{	NFile		newFile;



	// Perform the test
	newFile = theFile;

	REQUIRE(newFile == theFile);
	REQUIRE(newFile != tmpFile);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFILE("Links", "Links")
{


	// Perform the test
	tmpFile = NFile(kPathLinkTarget);
	tmpFile.CreateFile();
	tmpFile.ResolveTarget();


	tmpFile2 = kPathLinkFile;
	theErr   = tmpFile2.CreateLink(tmpFile, kNLinkSoft);
	REQUIRE_NOERR(theErr);
	REQUIRE(tmpFile2.IsLink());
	REQUIRE(tmpFile2             != tmpFile);
	REQUIRE(tmpFile2.GetTarget() == tmpFile);

	theErr = tmpFile2.Delete();
	REQUIRE_NOERR(theErr);


	#if NN_TARGET_MAC || NN_TARGET_LINUX
	tmpFile2 = kPathLinkFile;
	theErr   = tmpFile2.CreateLink(tmpFile, kNLinkHard);
	REQUIRE_NOERR(theErr);
	REQUIRE(!tmpFile2.IsLink());
	REQUIRE(tmpFile2             != tmpFile);
	REQUIRE(tmpFile2.GetTarget() != tmpFile);

	theErr = tmpFile2.Delete();
	REQUIRE_NOERR(theErr);
	#endif


	#if NN_TARGET_MAC || NN_TARGET_WINDOWS
	tmpFile2 = kPathLinkFile;
	theErr   = tmpFile2.CreateLink(tmpFile, kNLinkUser);
	REQUIRE_NOERR(theErr);
	REQUIRE(tmpFile2.IsLink());
	REQUIRE(tmpFile2             != tmpFile);
	REQUIRE(tmpFile2.GetTarget() == tmpFile);

	theErr = tmpFile2.Delete();
	REQUIRE_NOERR(theErr);
	#endif
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFILE("Get components", "Get components")
{	NString		thePath, theName, nameDisplay, nameNoExt, theExtension;



	// Perform the test
	thePath      = tmpFile.GetPath();
	theName      = tmpFile.GetName();
	nameDisplay  = tmpFile.GetName(kNNameDisplay);
	nameNoExt    = tmpFile.GetName(kNNameNoExtension);
	theExtension = tmpFile.GetExtension();
	
	REQUIRE(thePath      == kPathFileTmp);
	REQUIRE(theName      == (kTestTmpName + "." + kTestTmpExtension));
	REQUIRE(nameDisplay  == (kTestTmpName + "." + kTestTmpExtension));
	REQUIRE(nameNoExt    == (kTestTmpName));
	REQUIRE(theExtension == kTestTmpExtension);
	REQUIRE(theName      == tmpFile.GetName(kNNameRaw));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFILE("Set components", "Set components")
{	NFile	newFile;



	// Perform the test
	newFile.SetPath(kPathFile);
	REQUIRE(newFile == theFile);

	newFile = tmpFile.GetParent();
	theErr  = tmpFile.CreateFile();
	REQUIRE_NOERR(theErr);
	REQUIRE(tmpFile.IsFile());
	
	tmpFile.SetName(kTestTmpName2 + "." + kTestTmpExtension, true);
	REQUIRE(tmpFile == newFile.GetChild(kTestTmpName2 + "." + kTestTmpExtension));
	
	tmpFile.SetExtension(kTestTmpExtension2, true);
	REQUIRE(tmpFile == newFile.GetChild(kTestTmpName2 + "." + kTestTmpExtension2));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFILE("Size", "File size")
{	UInt64		theSize;



	// Perform the test
	theSize = tmpFile.GetSize();
	REQUIRE(theSize == 0);
	
	theErr = tmpFile.SetSize(kBufferSize);
	REQUIRE_NOERR(theErr);

	REQUIRE(tmpFile.GetSize() == kBufferSize);
	REQUIRE(tmpFile.Exists());
	REQUIRE(tmpFile.IsFile());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFILE("Timestamps", "File timestamps")
{


	// Perform the test
	theErr = tmpFile.SetSize(kBufferSize);
	REQUIRE_NOERR(theErr);

	REQUIRE(tmpFile.GetCreationTime().IsValid());
	REQUIRE(tmpFile.GetAccessTime().IsValid());
	REQUIRE(tmpFile.GetModificationTime().IsValid());

	REQUIRE(tmpFile.GetCreationTime() <= tmpFile.GetAccessTime());
	REQUIRE(tmpFile.GetCreationTime() <= tmpFile.GetModificationTime());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFILE("Open and Close", "File opening/closing")
{


	// Perform the test
	theErr = tmpFile.Open(kNPermissionUpdate, true);
	REQUIRE_NOERR(theErr);
	REQUIRE(tmpFile.IsOpen());

	tmpFile2 = tmpFile;
	REQUIRE(!tmpFile2.IsOpen());

	tmpFile.Close();
	REQUIRE(!tmpFile.IsOpen());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFILE("Reading/Writing", "File reading/writing")
{	UInt8		tmpBuffer[kBufferSize];
	UInt64		n, theSize;



	// Write
	theErr = tmpFile.Open(kNPermissionUpdate, true);
	REQUIRE_NOERR(theErr);
	REQUIRE(tmpFile.IsOpen());

	theSize = tmpFile.GetPosition();
	REQUIRE(theSize == 0);
	
	theSize = 123456;
	theErr  = tmpFile.Write(kBufferSize, kBufferData1, theSize);
	REQUIRE_NOERR(theErr);
	REQUIRE(theSize == kBufferSize);



	// Read
	theSize = 123456;
	theErr  = tmpFile.Read(kBufferSize, tmpBuffer, theSize, 0, kNPositionFromStart);
	REQUIRE_NOERR(theErr);
	REQUIRE(theSize == kBufferSize);

	for (n = 0; n < theSize; n++)
		REQUIRE(tmpBuffer[n] == kBufferData1[n]);

	theSize = tmpFile.GetPosition();
	REQUIRE(theSize == kBufferSize);

	theSize = 123456;
	theErr  = tmpFile.Read(kBufferSize, tmpBuffer, theSize);
	REQUIRE(theErr  == kNErrExhaustedSrc);
	REQUIRE(theSize == 0);



	// Rewind
	theErr = tmpFile.SetPosition(0);
	REQUIRE_NOERR(theErr);

	theSize = tmpFile.GetPosition();
	REQUIRE(theSize == 0);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFILE("Append", "Opening a file with write permission should append, not overwrite.")
{	UInt8		tmpBuffer[kBufferSize];
	UInt64		n, theSize;



	// Perform the test
	theErr  = tmpFile.Open(kNPermissionWrite, true);
	REQUIRE_NOERR(theErr);

	theErr = tmpFile.Write(kBufferSize, kBufferData1, theSize);
	REQUIRE_NOERR(theErr);
	
	tmpFile.Close();


	theErr = tmpFile.Open(kNPermissionWrite);
	REQUIRE_NOERR(theErr);

	theErr = tmpFile.Write(kBufferSize, kBufferData2, theSize);
	REQUIRE_NOERR(theErr);

	tmpFile.Close();


	theErr = tmpFile.Open(kNPermissionRead);
	REQUIRE_NOERR(theErr);

	memset(tmpBuffer, 0x00, kBufferSize);
	theErr = tmpFile.Read(kBufferSize, tmpBuffer, theSize, 0, kNPositionFromStart);
	REQUIRE_NOERR(theErr);

	for (n = 0; n < theSize; n++)
		REQUIRE(tmpBuffer[n] == kBufferData1[n]);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFILE("Create and Delete", "Creating and deleting a file")
{


	// Perform the test
	REQUIRE(!tmpFile.Exists());

	tmpFile.CreateFile();
	REQUIRE( tmpFile.Exists());
	REQUIRE( tmpFile.IsFile());
	REQUIRE(!tmpFile.IsDirectory());
	
	tmpFile.Delete();
	REQUIRE(!tmpFile.Exists());
	REQUIRE(!tmpFile.IsFile());
	REQUIRE(!tmpFile.IsDirectory());

	tmpFile.CreateDirectory();
	REQUIRE( tmpFile.Exists());
	REQUIRE(!tmpFile.IsFile());
	REQUIRE( tmpFile.IsDirectory());

	tmpFile.Delete();
	REQUIRE(!tmpFile.Exists());
	REQUIRE(!tmpFile.IsFile());
	REQUIRE(!tmpFile.IsDirectory());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFILE("Open and Delete", "Deleting an open file")
{


	// Perform the test
	theErr = tmpFile.Open(kNPermissionUpdate, true);
	REQUIRE_NOERR(theErr);
	REQUIRE(tmpFile.IsOpen());

#if NN_TARGET_WINDOWS
	#pragma message("TFile: delete-while-open not implemented")
#else
	REQUIRE(tmpFile.Exists());
	tmpFile.Delete();
	REQUIRE(!tmpFile.Exists());
#endif
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFILE("File Parents", "Creating a file with parents")
{


	// Perform the test
	tmpFile  = kPathTmp + NN_DIR + kTestTmpPathFile;
	tmpFile2 = kPathTmp + NN_DIR + kTestTmpPathParent;

	tmpFile.CreateFile();
	REQUIRE(tmpFile.Exists());
	REQUIRE(tmpFile2.Exists());
	REQUIRE(tmpFile2.IsDirectory());

	tmpFile2.Delete();
	REQUIRE(!tmpFile.Exists());
	REQUIRE(!tmpFile2.Exists());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFILE("Dir Parents", "Creating a directory with parents")
{


	// Perform the test
	tmpFile  = kPathTmp + NN_DIR + kTestTmpPathChild;
	tmpFile2 = kPathTmp + NN_DIR + kTestTmpPathParent;

	tmpFile.CreateDirectory();
	REQUIRE(tmpFile.Exists());
	REQUIRE(tmpFile2.Exists());
	REQUIRE(tmpFile2.IsDirectory());

	tmpFile2.Delete();
	REQUIRE(!tmpFile.Exists());
	REQUIRE(!tmpFile2.Exists());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFILE("Delete Contents", "Delete the contents of a directory")
{	NFileIterator	fileIter;



	// Perform the test
	tmpFile  = kPathTmp + NN_DIR + kTestTmpPathFile;
	tmpFile2 = kPathTmp + NN_DIR + kTestTmpPathParent;
	
	tmpFile.CreateFile();
	REQUIRE(tmpFile.Exists());
	REQUIRE(fileIter.GetFiles(tmpFile2).size() == 3);

	tmpFile2.DeleteContents();
	REQUIRE(tmpFile2.IsDirectory());
	REQUIRE(fileIter.GetFiles(tmpFile2).empty());

	tmpFile2.Delete();
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFILE("Relatives", "Parent/child relationships")
{


	// Perform the test
	tmpFile2 = tmpFile.GetParent();
	REQUIRE(tmpFile2.GetPath() == kPathTmp);
	
	tmpFile2 = tmpFile2.GetChild(tmpFile.GetName());
	REQUIRE(tmpFile2 == tmpFile);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFILE("Exchange", "Exchange files")
{


	// Perform the test
	theErr  =  tmpFile.SetSize(kFileSizeTmp1);
	theErr |= tmpFile2.SetSize(kFileSizeTmp2);
	REQUIRE_NOERR(theErr);

	REQUIRE( tmpFile.GetSize() == kFileSizeTmp1);
	REQUIRE(tmpFile2.GetSize() == kFileSizeTmp2);

	theErr = tmpFile.ExchangeWith(tmpFile2);
	REQUIRE_NOERR(theErr);

	REQUIRE( tmpFile.GetSize() == kFileSizeTmp2);
	REQUIRE(tmpFile2.GetSize() == kFileSizeTmp1);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFILE("Move", "Move files")
{


	// Perform the test
	theDir = kPathDirTmp;
	
	tmpFile.CreateFile();
	theDir.CreateDirectory();
	
	REQUIRE(tmpFile.GetParent() == theDir.GetParent());
	REQUIRE(!theDir.GetChild(tmpFile.GetName()).Exists());

	tmpFile.MoveTo(theDir);
	REQUIRE(tmpFile.GetParent() == theDir);
	REQUIRE(theDir.GetChild(tmpFile.GetName()).Exists());

	tmpFile.MoveTo(theDir.GetParent());
	REQUIRE(tmpFile.GetParent() == theDir.GetParent());
	REQUIRE(!theDir.GetChild(tmpFile.GetName()).Exists());

	theDir.Delete();
}




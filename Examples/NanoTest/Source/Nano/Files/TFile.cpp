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

#include "TFile.h"





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
	static const NString kPathDirectoryRoot							= "/Library";
	static const NString kPathDirectoryChildren						= "Application Support/Apple";
	static const NString kPathFileTmp								= kPathTmp + NN_DIR + kTestTmpName  + "." + kTestTmpExtension;
	static const NString kPathFileTmp2								= kPathTmp + NN_DIR + kTestTmpName2 + "." + kTestTmpExtension2;
	static const NString kPathDirTmp								= kPathTmp + NN_DIR + kTestTmpName3;

#elif NN_TARGET_WINDOWS
	static const NString kPathTmp									= "c:\\windows\\temp";
	static const NString kPathFile									= "c:\\windows\\WindowsShell.Manifest";
	static const NString kPathDirectoryRoot							= "c:\\windows";
	static const NString kPathDirectoryChildren						= "system32\\boot";
	static const NString kPathFileTmp								= kPathTmp + NN_DIR + kTestTmpName  + "." + kTestTmpExtension;
	static const NString kPathFileTmp2								= kPathTmp + NN_DIR + kTestTmpName2 + "." + kTestTmpExtension2;
	static const NString kPathDirTmp								= kPathTmp + NN_DIR + kTestTmpName3;

#else
	UNKNOWN TARGET
#endif





//============================================================================
//		TFile::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TFile::Execute(void)
{	NString			thePath, theName, nameDisplay, nameNoExt, theExtension;
	NFile			theFile, theDir, tmpFile, tmpFile2;
	UInt8			tmpBuffer[kBufferSize];
	NFileIterator	fileIter;
	UInt64			theSize;
	NStatus			theErr;
	UInt32			n;



	// Initialize ourselves
	theFile = kPathFile;
	theDir  = NFile(kPathDirectoryRoot).GetChild(kPathDirectoryChildren);



	// State
	NN_ASSERT( theFile.IsValid());
	NN_ASSERT(  theDir.IsValid());
	NN_ASSERT(!tmpFile.IsValid());

	NN_ASSERT( theFile.IsFile());
	NN_ASSERT( !theDir.IsFile());
	NN_ASSERT(!tmpFile.IsFile());

	NN_ASSERT(!theFile.IsDirectory());
	NN_ASSERT(  theDir.IsDirectory());
	NN_ASSERT(!tmpFile.IsDirectory());

	NN_ASSERT(!theFile.IsWriteable());
	NN_ASSERT(  theDir.IsWriteable());
	NN_ASSERT(!tmpFile.IsWriteable());

	NN_ASSERT(!theFile.IsOpen());
	NN_ASSERT( !theDir.IsOpen());
	NN_ASSERT(!tmpFile.IsOpen());

	NN_ASSERT( theFile.Exists());
	NN_ASSERT(  theDir.Exists());
	NN_ASSERT(!tmpFile.Exists());



	// Comparisons
	tmpFile = theFile;
	NN_ASSERT(tmpFile == theFile);

	tmpFile = kPathFileTmp;
	NN_ASSERT(tmpFile != theFile);



	// Get components
	tmpFile = kPathFileTmp;

	thePath      = tmpFile.GetPath();
	theName      = tmpFile.GetName();
	nameDisplay  = tmpFile.GetName(kNNameDisplay);
	nameNoExt    = tmpFile.GetName(kNNameNoExtension);
	theExtension = tmpFile.GetExtension();
	
	NN_ASSERT(thePath      == kPathFileTmp);
	NN_ASSERT(theName      == (kTestTmpName + "." + kTestTmpExtension));
	NN_ASSERT(nameDisplay  == (kTestTmpName + "." + kTestTmpExtension));
	NN_ASSERT(nameNoExt    == (kTestTmpName));
	NN_ASSERT(theExtension == kTestTmpExtension);
	NN_ASSERT(theName      == tmpFile.GetName(kNNameRaw));



	// Set components
	tmpFile.SetPath(kPathFile);
	NN_ASSERT(tmpFile == theFile);

	tmpFile  = kPathFileTmp;
	tmpFile2 = tmpFile.GetParent();

	theErr = tmpFile.CreateFile();
	NN_ASSERT_NOERR(theErr);
	NN_ASSERT(tmpFile.IsFile());
	
	tmpFile.SetName(kTestTmpName2 + "." + kTestTmpExtension, true);
	NN_ASSERT(tmpFile == tmpFile2.GetChild(kTestTmpName2 + "." + kTestTmpExtension));
	
	tmpFile.SetExtension(kTestTmpExtension2, true);
	NN_ASSERT(tmpFile == tmpFile2.GetChild(kTestTmpName2 + "." + kTestTmpExtension2));

	tmpFile.Delete();



	// Size
	tmpFile = kPathFileTmp;
	theSize = tmpFile.GetSize();
	NN_ASSERT(theSize == 0);
	
	theErr = tmpFile.SetSize(kBufferSize);
	NN_ASSERT_NOERR(theErr);

	NN_ASSERT(tmpFile.GetSize() == kBufferSize);
	NN_ASSERT(tmpFile.Exists());
	NN_ASSERT(tmpFile.IsFile());



	// Timestamps
	NN_ASSERT(tmpFile.GetCreationTime().IsValid());
	NN_ASSERT(tmpFile.GetAccessTime().IsValid());
	NN_ASSERT(tmpFile.GetModificationTime().IsValid());

	NN_ASSERT(tmpFile.GetCreationTime() <= tmpFile.GetAccessTime());
	NN_ASSERT(tmpFile.GetCreationTime() <= tmpFile.GetModificationTime());



	// Open
	theErr = tmpFile.Open(kNPermissionUpdate);
	NN_ASSERT_NOERR(theErr);

	NN_ASSERT(tmpFile.IsOpen());
	tmpFile2 = tmpFile;
	NN_ASSERT(!tmpFile2.IsOpen());



	// Read
	theSize = tmpFile.GetPosition();
	NN_ASSERT(theSize == 0);

	theSize = 123456;
	theErr  = tmpFile.Read(kBufferSize, tmpBuffer, theSize);
	NN_ASSERT_NOERR(theErr);
	NN_ASSERT(theSize == kBufferSize);

	for (n = 0; n < theSize; n++)
		NN_ASSERT(tmpBuffer[n] == 0x00);

	theSize = tmpFile.GetPosition();
	NN_ASSERT(theSize == kBufferSize);

	theSize = 123456;
	theErr  = tmpFile.Read(kBufferSize, tmpBuffer, theSize);
	NN_ASSERT(theErr  == kNErrExhaustedSrc);
	NN_ASSERT(theSize == 0);



	// Position
	theErr = tmpFile.SetPosition(0);
	NN_ASSERT_NOERR(theErr);



	// Write
	theSize = tmpFile.GetPosition();
	NN_ASSERT(theSize == 0);
	
	theSize = 123456;
	theErr  = tmpFile.Write(kBufferSize, kBufferData1, theSize);
	NN_ASSERT_NOERR(theErr);
	NN_ASSERT(theSize == kBufferSize);



	// Verify
	theSize = 123456;
	theErr  = tmpFile.Read(kBufferSize, tmpBuffer, theSize, 0, kNPositionFromStart);
	NN_ASSERT_NOERR(theErr);
	NN_ASSERT(theSize == kBufferSize);

	for (n = 0; n < theSize; n++)
		NN_ASSERT(tmpBuffer[n] == kBufferData1[n]);	



	// Delete-while-open
#if NN_TARGET_WINDOWS
	#pragma message("TFile: delete-while-open not implemented")
#else
	NN_ASSERT(tmpFile.Exists());
	tmpFile.Delete();
	NN_ASSERT(!tmpFile.Exists());
#endif



	// Close
	tmpFile.Close();
	NN_ASSERT(!tmpFile.IsOpen());

#if NN_TARGET_WINDOWS
	tmpFile.Delete();	// Unable to delete-while-open
#endif



	// Creation/deletion
	NN_ASSERT(!tmpFile.Exists());

	tmpFile.CreateFile();
	NN_ASSERT( tmpFile.Exists());
	NN_ASSERT( tmpFile.IsFile());
	NN_ASSERT(!tmpFile.IsDirectory());
	
	tmpFile.Delete();
	NN_ASSERT(!tmpFile.Exists());
	NN_ASSERT(!tmpFile.IsFile());
	NN_ASSERT(!tmpFile.IsDirectory());

	tmpFile.CreateDirectory();
	NN_ASSERT( tmpFile.Exists());
	NN_ASSERT(!tmpFile.IsFile());
	NN_ASSERT( tmpFile.IsDirectory());

	tmpFile.Delete();
	NN_ASSERT(!tmpFile.Exists());
	NN_ASSERT(!tmpFile.IsFile());
	NN_ASSERT(!tmpFile.IsDirectory());



	// Creation of file with parents
	tmpFile  = kPathTmp + NN_DIR + kTestTmpPathFile;
	tmpFile2 = kPathTmp + NN_DIR + kTestTmpPathParent;

	tmpFile.CreateFile();
	NN_ASSERT(tmpFile.Exists());
	NN_ASSERT(tmpFile2.Exists());
	NN_ASSERT(tmpFile2.IsDirectory());

	tmpFile2.Delete();
	NN_ASSERT(!tmpFile.Exists());
	NN_ASSERT(!tmpFile2.Exists());



	// Creation of directory with parents
	tmpFile  = kPathTmp + NN_DIR + kTestTmpPathChild;
	tmpFile2 = kPathTmp + NN_DIR + kTestTmpPathParent;

	tmpFile.CreateDirectory();
	NN_ASSERT(tmpFile.Exists());
	NN_ASSERT(tmpFile2.Exists());
	NN_ASSERT(tmpFile2.IsDirectory());

	tmpFile2.Delete();
	NN_ASSERT(!tmpFile.Exists());
	NN_ASSERT(!tmpFile2.Exists());



	// Delete contents
	tmpFile  = kPathTmp + NN_DIR + kTestTmpPathFile;
	tmpFile2 = kPathTmp + NN_DIR + kTestTmpPathParent;
	
	tmpFile.CreateFile();
	NN_ASSERT(tmpFile.Exists());
	NN_ASSERT(fileIter.GetFiles(tmpFile2).size() == 3);

	tmpFile2.DeleteContents();
	NN_ASSERT(tmpFile2.IsDirectory());
	NN_ASSERT(fileIter.GetFiles(tmpFile2).empty());

	tmpFile2.Delete();



	// Relatives
	tmpFile  = kPathFileTmp;
	tmpFile2 = tmpFile.GetParent();
	NN_ASSERT(tmpFile2.GetPath() == kPathTmp);
	
	tmpFile2 = tmpFile2.GetChild(tmpFile.GetName());
	NN_ASSERT(tmpFile2 == tmpFile);



	// Resize/exchange
	tmpFile  = kPathFileTmp;
	tmpFile2 = kPathFileTmp2;
	
	theErr  =  tmpFile.SetSize(kFileSizeTmp1);
	theErr |= tmpFile2.SetSize(kFileSizeTmp2);
	NN_ASSERT_NOERR(theErr);

	NN_ASSERT( tmpFile.GetSize() == kFileSizeTmp1);
	NN_ASSERT(tmpFile2.GetSize() == kFileSizeTmp2);

	theErr = tmpFile.ExchangeWith(tmpFile2);
	NN_ASSERT_NOERR(theErr);

	NN_ASSERT( tmpFile.GetSize() == kFileSizeTmp2);
	NN_ASSERT(tmpFile2.GetSize() == kFileSizeTmp1);
	
	tmpFile.Delete();
	tmpFile2.Delete();



	// Move
	tmpFile = kPathFileTmp;
	theDir  = kPathDirTmp;
	
	tmpFile.CreateFile();
	theDir.CreateDirectory();
	
	NN_ASSERT(tmpFile.GetParent() == theDir.GetParent());
	NN_ASSERT(!theDir.GetChild(tmpFile.GetName()).Exists());

	tmpFile.MoveTo(theDir);
	NN_ASSERT(tmpFile.GetParent() == theDir);
	NN_ASSERT(theDir.GetChild(tmpFile.GetName()).Exists());

	tmpFile.MoveTo(theDir.GetParent());
	NN_ASSERT(tmpFile.GetParent() == theDir.GetParent());
	NN_ASSERT(!theDir.GetChild(tmpFile.GetName()).Exists());

	tmpFile.Delete();
	theDir.Delete();



	// Append-on-write
	//
	// Opening a file with write permission should append to the end, not overwrite.
	tmpFile = kPathFileTmp;
	theErr  = tmpFile.Open(kNPermissionWrite, true);
	NN_ASSERT_NOERR(theErr);

	theErr = tmpFile.Write(kBufferSize, kBufferData1, theSize);
	NN_ASSERT_NOERR(theErr);
	
	tmpFile.Close();


	theErr = tmpFile.Open(kNPermissionWrite);
	NN_ASSERT_NOERR(theErr);

	theErr = tmpFile.Write(kBufferSize, kBufferData2, theSize);
	NN_ASSERT_NOERR(theErr);

	tmpFile.Close();


	theErr = tmpFile.Open(kNPermissionRead);
	NN_ASSERT_NOERR(theErr);

	memset(tmpBuffer, 0x00, kBufferSize);
	theErr = tmpFile.Read(kBufferSize, tmpBuffer, theSize, 0, kNPositionFromStart);
	NN_ASSERT_NOERR(theErr);

	tmpFile.Close();
	tmpFile.Delete();

	for (n = 0; n < theSize; n++)
		NN_ASSERT(tmpBuffer[n] == kBufferData1[n]);
}




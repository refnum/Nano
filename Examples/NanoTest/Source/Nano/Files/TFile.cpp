/*	NAME:
		TFile.cpp

	DESCRIPTION:
		NFile tests.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
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
static const UInt8  kBufferData[kBufferSize]						= { 'T', 'e', 's', 't', ' ', 'd', 'a', 't', 'a' };

static const UInt32 kTestSize1										= 111;
static const UInt32 kTestSize2										= 222;


// Paths
#if NN_TARGET_MAC
	static const NString kTestTmpPath								= "/tmp";
	static const NString kTestTmpName								= "TFile";
	static const NString kTestTmpExtension							= "txt";
	static const NString kTestTmpName2								= "TFileRename";
	static const NString kTestTmpExtension2							= "txtrename";
	
	static const NString kPathFile									= "/bin/bash";
	static const NString kPathDirectoryRoot							= "/Library";
	static const NString kPathDirectoryChildren						= "Application Support/Apple";
	static const NString kPathFileTmp								= kTestTmpPath + "/" + kTestTmpName  + "." + kTestTmpExtension;
	static const NString kPathFileTmp2								= kTestTmpPath + "/" + kTestTmpName2 + "." + kTestTmpExtension2;
#else
	static const NString kTestTmpPath								= "c:\\windows\\temp";
	static const NString kTestTmpName								= "TFile";
	static const NString kTestTmpExtension							= "txt";
	static const NString kTestTmpName2								= "TFileRename";
	static const NString kTestTmpExtension2							= "txtrename";
	
	static const NString kPathFile									= "c:\\windows\\notepad.exe";
	static const NString kPathDirectoryRoot							= "c:\\windows";
	static const NString kPathDirectoryChildren						= "system32\\boot";
	static const NString kPathFileTmp								= kTestTmpPath + "\\" + kTestTmpName  + "." + kTestTmpExtension;
	static const NString kPathFileTmp2								= kTestTmpPath + "\\" + kTestTmpName2 + "." + kTestTmpExtension2;
#endif





//============================================================================
//		TFile::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TFile::Execute(void)
{	NString			thePath, theName, displayName, theExtension;
	NFile			theFile, theDir, tmpFile, tmpFile2;
	UInt8			tmpBuffer[kBufferSize];
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
	displayName  = tmpFile.GetName(true);
	theExtension = tmpFile.GetExtension();
	
	NN_ASSERT(thePath      == kPathFileTmp);
	NN_ASSERT(theName      == (kTestTmpName + "." + kTestTmpExtension));
	NN_ASSERT(displayName  == (kTestTmpName + "." + kTestTmpExtension));
	NN_ASSERT(theExtension == kTestTmpExtension);



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



	// Open
	theErr = tmpFile.Open(kNPermissionReadWrite);
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
	theErr  = tmpFile.Write(kBufferSize, kBufferData, theSize);
	NN_ASSERT_NOERR(theErr);
	NN_ASSERT(theSize == kBufferSize);



	// Verify
	theSize = 123456;
	theErr  = tmpFile.Read(kBufferSize, tmpBuffer, theSize, 0, kNPositionFromStart);
	NN_ASSERT_NOERR(theErr);
	NN_ASSERT(theSize == kBufferSize);

	for (n = 0; n < theSize; n++)
		NN_ASSERT(tmpBuffer[n] == kBufferData[n]);	



	// Delete-while-open
	NN_ASSERT(tmpFile.Exists());
	tmpFile.Delete();
	NN_ASSERT(!tmpFile.Exists());



	// Close
	tmpFile.Close();
	NN_ASSERT(!tmpFile.IsOpen());



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



	// Relatives
	tmpFile2 = tmpFile.GetParent();
	NN_ASSERT(tmpFile2.GetPath() == kTestTmpPath);
	
	tmpFile2 = tmpFile2.GetChild(kTestTmpName + "." + kTestTmpExtension);
	NN_ASSERT(tmpFile2 == tmpFile);



	// Exchange
	tmpFile  = kPathFileTmp;
	tmpFile2 = kPathFileTmp2;
	
	theErr  =  tmpFile.SetSize(kTestSize1);
	theErr |= tmpFile2.SetSize(kTestSize2);
	NN_ASSERT_NOERR(theErr);

	NN_ASSERT( tmpFile.GetSize() == kTestSize1);
	NN_ASSERT(tmpFile2.GetSize() == kTestSize2);

	theErr = tmpFile.ExchangeWith(tmpFile2);
	NN_ASSERT_NOERR(theErr);

	NN_ASSERT( tmpFile.GetSize() == kTestSize2);
	NN_ASSERT(tmpFile2.GetSize() == kTestSize1);
	
	tmpFile.Delete();
	tmpFile2.Delete();
}




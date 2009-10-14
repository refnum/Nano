/*	NAME:
		NFileUtilities.cpp

	DESCRIPTION:
		File utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFURL.h"
#include "NTask.h"
#include "NCFBundle.h"
#include "NPropertyFile.h"
#include "NSystemUtilities.h"
#include "NTextUtilities.h"
#include "NFileUtilities.h"





//============================================================================
//		NFileUtilities::GetAEFile : Get a file from an AEDesc.
//----------------------------------------------------------------------------
NFile NFileUtilities::GetAEFile(const AEDesc *theDesc)
{	AEDesc			urlDesc = { typeNull, NULL };
	NData			theData;
	NString			thePath;
	OSStatus		theErr;
	NCFURL			theURL;



	// Convert the AEDesc to a URL
	theErr = AECoerceDesc(theDesc, typeFileURL, &urlDesc);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr == noErr)
		{
		theErr = memFullErr;
		if (theData.SetSize(AEGetDescDataSize(&urlDesc)))
			{
			theErr = AEGetDescData(&urlDesc, theData.GetData(), theData.GetSize());
			NN_ASSERT_NOERR(theErr);
			}
		}
	
	AEDisposeDesc(&urlDesc);



	// Get the file
	if (theErr == noErr)
		theURL = theData;

	return(theURL.GetFile());
}





//============================================================================
//		NFileUtilities::GetAEFiles : Get files from an AEDescList.
//----------------------------------------------------------------------------
NFileList NFileUtilities::GetAEFiles(const AEDescList *theList)
{	long			numItems;
	NFileList		theFiles;
	AEDesc			urlDesc;
	NFile			theFile;
	OSStatus		theErr;
	SInt32			n;



	// Get the state we need
	theErr = AECountItems(theList, &numItems);
	NN_ASSERT_NOERR(theErr);



	// Get the files
	for (n = 1; n <= numItems; n++)
		{
		// Get the descriptor
		urlDesc.descriptorType = typeNull;
		urlDesc.dataHandle     = NULL;
		
		theErr = AEGetNthDesc(theList, n, typeFileURL, NULL, &urlDesc);
		NN_ASSERT_NOERR(theErr);


		// Extract the file
		if (theErr == noErr)
			{
			theFile = NFileUtilities::GetAEFile(&urlDesc);
			if (theFile.IsValid())
				theFiles.push_back(theFile);
			
			AEDisposeDesc(&urlDesc);
			}
		}
	
	return(theFiles);
}





//============================================================================
//		NFileUtilities::GetNavReplyFiles : Get files from an NavReplyRecord.
//----------------------------------------------------------------------------
NFileList NFileUtilities::GetNavReplyFiles(const NavReplyRecord &navReply)
{	NFileList	theFiles;
	FSRef		theFSRef;



	// Get the files
	theFiles = NFileUtilities::GetAEFiles(&navReply.selection);



	// Adjust the selection
	//
	// When saving, the selection contains a single file which refers to the
	// parent directory to save the file - the file name for the actual file
	// to save to is contained in a separate field.
	//
	// Since NFile can represent files which do not exist, we check for this
	// case and update the file to point to the result.
	if (theFiles.size() == 1 && navReply.version >= 1 && navReply.saveFileName != NULL)
		{
		theFSRef = theFiles[0].GetFSRef();
		
		theFiles[0].SetFSRef(theFSRef, navReply.saveFileName);
		}
	
	return(theFiles);
}





//============================================================================
//		NFileUtilities::GetFileText : Get a file's text.
//----------------------------------------------------------------------------
NString NFileUtilities::GetFileText(const NFile &theFile, CFStringEncoding theEncoding)
{	NData		theData;
	NString		theText;



	// Get the file text
	theData = GetFileData(theFile);
	if (theData.IsNotEmpty())
		theText = NString((const char *) theData.GetData(), theData.GetSize(), theEncoding);
		
	return(theText);
}





//============================================================================
//		NFileUtilities::GetFileData : Get a file's data.
//----------------------------------------------------------------------------
NData NFileUtilities::GetFileData(const NFile &theFile)
{	UInt64		theSize, numRead;
	NFile		mutableFile;
	NData		theData;
	OSStatus	theErr;



	// Get the state we need
	mutableFile = theFile;
	theSize     = mutableFile.GetFileSize();



	// Resize the buffer
	if (!theData.SetSize(theSize))
		return(theData);



	// Open the file
	theErr = mutableFile.OpenFile(fsRdPerm);
	if (theErr != noErr)
		{
		theData.Clear();
		return(theData);
		}



	// Read the data
	theErr = mutableFile.ReadFile(theSize, theData.GetData(), numRead);
	theData.SetSize(numRead);

	NN_ASSERT_NOERR(theErr);
	NN_ASSERT(numRead == theSize);



	// Clean up
	mutableFile.CloseFile();

	return(theData);
}





//============================================================================
//		NFileUtilities::SetFileText : Set a file's text.
//----------------------------------------------------------------------------
OSStatus NFileUtilities::SetFileText(const NFile &theFile, const NString &theText)
{	NData		theData;
	OSStatus	theErr;



	// Set the file text
	theErr = memFullErr;

	if (theData.SetData(theText.GetSize(), theText.GetUTF8()))
		theErr = SetFileData(theFile, theData);
	
	return(theErr);
}





//============================================================================
//		NFileUtilities::SetFileData : Set a file's data.
//----------------------------------------------------------------------------
OSStatus NFileUtilities::SetFileData(const NFile &theFile, const NData &theData)
{	UInt64		theSize, numWritten;
	NFile		mutableFile;
	OSStatus	theErr;



	// Get the state we need
	mutableFile = theFile;
	theSize     = theData.GetSize();



	// Open the file
	theErr = mutableFile.OpenFile(fsWrPerm, true);
	if (theErr != noErr)
		return(theErr);



	// Write the data
	theErr = mutableFile.WriteFile(theSize, theData.GetData(), numWritten);

	NN_ASSERT_NOERR(theErr);
	NN_ASSERT(numWritten == theSize);



	// Clean up
	mutableFile.CloseFile();
	
	return(theErr);
}





//============================================================================
//		NFileUtilities::GetPathSeparator : Get the path separator.
//----------------------------------------------------------------------------
NString NFileUtilities::GetPathSeparator(CFURLPathStyle theStyle)
{	NString		theResult;



	// Get the separator
	switch (theStyle) {
		case kCFURLPOSIXPathStyle:
			theResult = "/";
			break;
		
		case kCFURLHFSPathStyle:
			theResult = ":";
			break;
		
		case kCFURLWindowsPathStyle:
			theResult = "\\";
			break;
		
		default:
			NN_LOG("Unknown path style: %d", theStyle);
			theResult = "?";
			break;
		}
	
	return(theResult);
}





//============================================================================
//		NFileUtilities::FindFiles : Find files.
//----------------------------------------------------------------------------
NFileList NFileUtilities::FindFiles(const NFile &startAt, const NString &matchName, const NString &matchPath)
{	NFileIterator	theIter;



	// Find the files
	theIter.SetPatternName(matchName);
	theIter.SetPatternPath(matchPath);

	return(theIter.GetFiles(startAt));
}





//============================================================================
//		NFileUtilities::FindBundle : Find a bundle.
//----------------------------------------------------------------------------
NFile NFileUtilities::FindBundle(const NFile &startAt, const NString &bundleID, const NString &matchName)
{	NFileList		theFiles;
	NFileIterator	theIter;
	NFile			theFile;



	// Find the files
	theIter.SetPatternName(matchName);
	theIter.SetItemFilter(FilterBundleID, (void *) &bundleID);

	theFiles = theIter.GetFiles(startAt);
	if (!theFiles.empty())
		theFile = theFiles[0];
	
	return(theFile);
}





//============================================================================
//		NFileUtilities::FindFolder : Find a folder.
//----------------------------------------------------------------------------
NFile NFileUtilities::FindFolder(OSType theFolder, const NString &fileName, SInt16 theDomain, bool canCreate)
{	FSRef		theFSRef;
	NFile		theFile;
	OSStatus	theErr;



	// Find the folder
	theErr = FSFindFolder(theDomain, theFolder, canCreate, &theFSRef);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr == noErr)
		theFile.SetFSRef(theFSRef);



	// Find the file within the folder
	if (theErr == noErr && fileName.IsNotEmpty())
		theFile = theFile.GetChild(fileName);
	
	return(theFile);
}





//============================================================================
//		NFileUtilities::GetUniqueFile : Get a file with a unique name.
//----------------------------------------------------------------------------
NFile NFileUtilities::GetUniqueFile(const NFile &theDirectory, const NString &baseName)
{	NString		theName, fileName, fileExt;
	FSRef		theFSRef;
	CFRange		extBreak;
	NFile		theFile;
	UInt32		n;



	// Validate our parameters
	NN_ASSERT(theDirectory.IsDirectory());



	// Get the state we need
	theFSRef = theDirectory.GetFSRef();
	
	if (baseName.IsEmpty())
		{
		fileName = "Temp";
		fileExt  = ".tmp";
		}
	else
		{
		extBreak = baseName.Find(".", kCFCompareBackwards);
		if (extBreak.length == 1)
			{
			fileName = baseName.GetLeft(  extBreak.location);
			fileExt  = baseName.GetString(extBreak.location);
			}
		else
			{
			fileName = baseName;
			fileExt  = "";
			}
		}



	// Generate a unique name
	n = 0;
	
	while (n < 10000)
		{
		// Build the name
		if (n == 0)
			theName = fileName;
		else
			theName.Format("%s %ld", fileName.GetUTF8(), n);

		if (fileExt.IsNotEmpty())
			theName += fileExt;



		// Check for the file
		theFile.SetFSRef(theFSRef, theName);
		if (!theFile.Exists())
			return(theFile);

		n++;
		}



	// Handle failure
	NN_LOG("Unable to create a unique name");
	
	theFile.SetFSRef(kFSRefZero);
	
	return(theFile);
}





//============================================================================
//		NFileUtilities::GetTempFile : Get a temporary file.
//----------------------------------------------------------------------------
NFile NFileUtilities::GetTempFile(const NString &baseName)
{


	// Get the file
	return(GetUniqueFile(FindFolder(kChewableItemsFolderType), baseName));
}





//============================================================================
//		NFileUtilities::RedirectStderr : Redirect stderr.
//----------------------------------------------------------------------------
SInt32 NFileUtilities::RedirectStderr(SInt32 errorFD)
{


	// Close stderr
	if (errorFD == -1)
		{
		errorFD = dup(fileno(stderr));
		freopen("/dev/null", "w", stderr);
		}
	
	
	// Or restore it
	else
		{
		fflush(stderr);
		dup2(errorFD, fileno(stderr));

		close(errorFD);
		clearerr(stderr);
		
		errorFD = -1;
		}
	
	return(errorFD);
}





//============================================================================
//		NFileUtilities::MountDMG : Mount a disk image.
//----------------------------------------------------------------------------
NFile NFileUtilities::MountDMG(const NFile &theDMG, const char *arg1, ...)
{	UInt32				n, numItems;
	NArray				theArray;
	NTask				theTask;
	NStringList			theArgs;
	va_list				argList;
	NDictionary			theDict;
	NFile				theFile;
	OSStatus			theErr;
	NPropertyFile		pList;
	


	// Get the arguments
	//
	// For programmatic mounting, we suppress any "internet disk image" behaviour.
	va_start(argList, arg1);
		theArgs = NTextUtilities::CollectArguments("hdiutil", "attach", "-noidme", "-noidmereveal", "-noidmetrash", "-plist", NULL);
		container_join(theArgs, NTextUtilities::CollectArguments(argList, arg1));
		theArgs.push_back(theDMG.GetPath());
	va_end(argList);



	// Prepare the task
	theTask.SetCommand("/usr/bin/env");
	theTask.SetArguments(theArgs);



	// Mount the disk image
	theErr = theTask.Launch();
	NN_ASSERT_NOERR(theErr);
	
	if (theErr == noErr)
		theTask.WaitForTask();



	// Find the mount point
	if (theErr == noErr)
		{
		theErr = pList.Load(theTask.ReadOutput());
		if (theErr == noErr)
			{
			theDict  = pList.GetProperties();
			theArray = theDict.GetValueArray("system-entities");
			numItems = theArray.GetSize();

			for (n = 0; n < numItems; n++)
				{
				theDict = theArray.GetValueDictionary(n);
				if (theDict.HasKey("mount-point"))
					{
					theFile = theDict.GetValueString("mount-point");
					return(theFile);
					}
				}
			}
		}
	
	return(theFile);
}





//============================================================================
//		NFileUtilities::UnmountDMG : Unmount a disk image.
//----------------------------------------------------------------------------
void NFileUtilities::UnmountDMG(const NFile &mountPoint)
{	NTask		theTask;
	OSStatus	theErr;



	// Prepare the task
	theTask.SetCommand("/usr/bin/env");
	theTask.SetArguments("hdiutil", "detach", "-quiet", mountPoint.GetPath().GetUTF8(), NULL);



	// Unmount the disk image
	theErr = theTask.Launch();
	NN_ASSERT_NOERR(theErr);
	
	if (theErr == noErr)
		theTask.WaitForTask();
}





//============================================================================
//		NFileUtilities::MoveToTrash : Move a file to the trash.
//----------------------------------------------------------------------------
OSStatus NFileUtilities::MoveToTrash(const NFile &theFile)
{	AppleEvent		theEvent, theReply;
	FSRef			theFSRef;
	AliasHandle		aliasHnd;
	OSStatus		theErr;



	// Initialize ourselves
	AEInitializeDesc(&theEvent);
	AEInitializeDesc(&theReply);

	aliasHnd = NULL;



	// Create the Apple Event
	theFSRef = theFile.GetFSRef();
	theErr   = FSNewAliasMinimal(&theFSRef, &aliasHnd);

	if (theErr == noErr)
		theErr = AEBuildAppleEvent(kAECoreSuite, kAEDelete,
									typeApplSignature, &kFileCreatorFinder, sizeof(OSType),
									kAutoGenerateReturnID, kAnyTransactionID, &theEvent, NULL,
									"'----':alis(@@)", aliasHnd);



	// Ask the Finder to move the file
	if (theErr == noErr)
		theErr = AESendMessage(&theEvent, &theReply, kAEWaitReply, kAEDefaultTimeout);



	// Clean up
	AEDisposeDesc(&theEvent);
	AEDisposeDesc(&theReply);
	
	if (aliasHnd != NULL)
		DisposeHandle((Handle) aliasHnd);
	
	return(theErr);
}





//============================================================================
//		NFileUtilities::FilterBundleID : Filter by bundle ID.
//----------------------------------------------------------------------------
#pragma mark -
bool NFileUtilities::FilterBundleID(bool &stopNow, const IteratorFilterInfo &theInfo)
{	NString			*bundleID = (NString *) theInfo.userData;
	NCFBundle		theBundle(theInfo.theFile);



	// Check the bundle identifier
	if (theBundle.IsValid() && bundleID->EqualTo(theBundle.GetIdentifier(), true))
		{
		stopNow = true;
		return(true);
		}
	
	return(false);
}


/*	NAME:
		NFileUtilities.cpp

	DESCRIPTION:
		File utilities.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFURL.h"
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
	if (theData.GetSize() != 0)
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
		extBreak = baseName.Find(".");
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
	n = 1;
	
	while (n != 0)
		{
		// Build the name
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


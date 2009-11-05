/*  NAME:
        NFileUtilities.cpp

    DESCRIPTION:
        File utilities.
    
    COPYRIGHT:
        Copyright (c) 2006-2009, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
    __________________________________________________________________________
*/
//============================================================================
//      Include files
//----------------------------------------------------------------------------
#include "NTargetPOSIX.h"
#include "NTargetFile.h"
#include "NFileUtilities.h"





//============================================================================
//		NFileUtilities::GetFileText : Get a file as text.
//----------------------------------------------------------------------------
NString NFileUtilities::GetFileText(const NFile &theFile, NStringEncoding theEncoding)
{	NData		theData;
	NString		theText;



	// Get the file text
	theData = GetFileData(theFile);
	if (!theData.IsEmpty())
		theText = NString(theData, theEncoding);
		
	return(theText);
}





//============================================================================
//		NFileUtilities::GetFileData : Get a file as data.
//----------------------------------------------------------------------------
NData NFileUtilities::GetFileData(const NFile &theFile)
{	UInt64			theSize, numRead;
	NFile			mutableFile;
	NData			theData;
	NStatus			theErr;



	// Get the state we need
	mutableFile = theFile;
	theSize     = mutableFile.GetSize();
	NN_ASSERT( ((SInt64) theSize) <= kSInt32Max);



	// Resize the buffer
	if (!theData.SetSize((NIndex) theSize))
		return(theData);



	// Open the file
	theErr = mutableFile.Open();
	if (theErr != kNoErr)
		{
		theData.Clear();
		return(theData);
		}



	// Read the data
	theErr = mutableFile.Read(theSize, theData.GetData(), numRead);
	theData.SetSize((NIndex) numRead);

	NN_ASSERT_NOERR(theErr);
	NN_ASSERT(numRead == theSize);
	NN_ASSERT(((SInt64) numRead) <= kSInt32Max);



	// Clean up
	mutableFile.Close();

	return(theData);
}





//============================================================================
//		NFileUtilities::SetFileText : Set a file to text.
//----------------------------------------------------------------------------
NStatus NFileUtilities::SetFileText(const NFile &theFile, const NString &theText, NStringEncoding theEncoding)
{


	// Set the file text
	return(SetFileData(theFile, theText.GetData(theEncoding, kNStringUnicodeBOM)));
}





//============================================================================
//		NFileUtilities::SetFileData : Set a file to data.
//----------------------------------------------------------------------------
NStatus NFileUtilities::SetFileData(const NFile &theFile, const NData &theData)
{	UInt64			theSize, numWritten;
	NFile			mutableFile;
	NStatus			theErr;



	// Get the state we need
	mutableFile = theFile;
	theSize     = theData.GetSize();



	// Open the file
	theErr = mutableFile.Open(kNPermissionWrite, true);
	if (theErr != kNoErr)
		return(theErr);



	// Write the data
	theErr = mutableFile.SetSize(0);
	theErr = mutableFile.Write(theSize, theData.GetData(), numWritten);

	NN_ASSERT_NOERR(theErr);
	NN_ASSERT(numWritten == theSize);



	// Clean up
	mutableFile.Close();
	
	return(theErr);
}





//============================================================================
//      NFileUtilities::GetCWD : Get the current working directory.
//----------------------------------------------------------------------------
NFile NFileUtilities::GetCWD(void)
{	char		*thePath;
	NFile		theFile;



	// Get the directory
	thePath = NTargetPOSIX::getcwd(NULL, 0);
	if (thePath != NULL)
		{
		theFile = NFile(NString(thePath));
		free(thePath);
		}

	return(theFile);
}





//============================================================================
//		NFileUtilities::GetDirectory : Get a standard directory.
//----------------------------------------------------------------------------
NFile NFileUtilities::GetDirectory(NDirectoryLocation theLocation, const NString &fileName, NDirectoryDomain theDomain)
{	NFile		theFile;



	// Get the directory
	theFile = NTargetFile::GetDirectory(theDomain, theLocation);

	if (!fileName.IsEmpty() && theFile.IsValid())
		theFile = theFile.GetChild(fileName);

	return(theFile);
}





//============================================================================
//		NFileUtilities::GetUniqueFile : Get a uniquely-named file.
//----------------------------------------------------------------------------
NFile NFileUtilities::GetUniqueFile(const NFile &theDirectory, const NString &fileName)
{	NString		nameChild, nameFile, nameExt;
	NRange		extBreak;
	NFile		theFile;
	UInt32		n;



	// Validate our parameters
	NN_ASSERT(theDirectory.IsDirectory());



	// Get the state we need
	if (fileName.IsEmpty())
		{
		nameFile = "Temp";
		nameExt  = ".tmp";
		}
	else
		{
		extBreak = fileName.Find(".", kNStringBackwards);
		if (extBreak.GetSize() == 1)
			{
			nameFile = fileName.GetLeft(  extBreak.GetLocation());
			nameExt  = fileName.GetString(extBreak.GetLocation());
			}
		else
			{
			nameFile = fileName;
			nameExt  = "";
			}
		}



	// Generate a unique name
	n = 0;
	
	while (n < 10000)
		{
		// Build the name
		if (n == 0)
			nameChild = nameFile;
		else
			nameChild.Format("%@ %ld", nameFile, n);

		if (!nameExt.IsEmpty())
			nameChild += nameExt;



		// Check for the file
		theFile = theDirectory.GetChild(nameChild);
		if (!theFile.Exists())
			return(theFile);

		n++;
		}



	// Handle failure
	NN_LOG("Unable to create a unique name");
	theFile.Clear();
	
	return(theFile);
}





//============================================================================
//		NFileUtilities::GetTemporaryFile : Get a temporary file.
//----------------------------------------------------------------------------
NFile NFileUtilities::GetTemporaryFile(const NString &fileName)
{


	// Get the file
	return(GetUniqueFile(GetDirectory(kNLocationTemporaryData), fileName));
}



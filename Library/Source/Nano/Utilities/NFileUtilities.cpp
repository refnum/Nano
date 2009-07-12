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
#include "NFileUtilities.h"





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
//		NFileUtilities::GetFileText : Get a file as text.
//----------------------------------------------------------------------------
NString NFileUtilities::GetFileText(const NFile &theFile, NStringEncoding theEncoding)
{	NData		theData;
	NString		theText;



	// Get the file text
	theData = GetFileData(theFile);
	if (theData.IsNotEmpty())
		theText = NString((const char *) theData.GetData(), theData.GetSize(), theEncoding);
		
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
	return(SetFileData(theFile, theText.GetData(theEncoding)));
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
	theErr = mutableFile.Open(kFilePermissionWrite, true);
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



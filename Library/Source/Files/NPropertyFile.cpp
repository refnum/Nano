/*	NAME:
		NPropertyFile.cpp

	DESCRIPTION:
		Property file.

	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NPropertyFile.h"





//============================================================================
//		NPropertyFile::NPropertyFile : Constructor.
//----------------------------------------------------------------------------
NPropertyFile::NPropertyFile(void)
{


	// Initialize ourselves
	mFormat = kCFPropertyListBinaryFormat_v1_0;
}





//============================================================================
//		NPropertyFile::~NPropertyFile : Destructor.
//----------------------------------------------------------------------------
NPropertyFile::~NPropertyFile(void)
{
}





//============================================================================
//		NPropertyFile::GetFormat : Get the format.
//----------------------------------------------------------------------------
CFPropertyListFormat NPropertyFile::GetFormat(void) const
{


	// Get the format
	return(mFormat);
}





//============================================================================
//		NPropertyFile::SetFormat : Set the format.
//----------------------------------------------------------------------------
void NPropertyFile::SetFormat(CFPropertyListFormat theFormat)
{


	// Set the format
	mFormat = theFormat;
}





//============================================================================
//		NPropertyFile::Load : Load the file.
//----------------------------------------------------------------------------
OSStatus NPropertyFile::Load(const NFile &theFile)
{	NCFObject		cfStream;
	CFStringRef		cfError;
	OSStatus		theErr;



	// Reset our state
	mProperties.Clear();



	// Open the stream
	if (!cfStream.Set(CFReadStreamCreateWithFile(NULL, theFile.GetURL())))
		return(memFullErr);

	if (!CFReadStreamOpen(cfStream))
		return(memFullErr);



	// Load the file
	cfError = NULL;
	theErr  = noErr;
	
	mProperties.Set(CFPropertyListCreateFromStream(NULL, cfStream, 0, kCFPropertyListMutableContainersAndLeaves, &mFormat, &cfError));

	if (cfError != NULL)
		{
		NN_LOG("Unable to load property list: %s", NString(cfError).GetUTF8());
		theErr = badFormat;
		}



	// Clean up
	CFReadStreamClose(cfStream);
	CFSafeRelease(cfError);
	
	return(theErr);
}





//============================================================================
//		NPropertyFile::Save : Save the file.
//----------------------------------------------------------------------------
OSStatus NPropertyFile::Save(const NFile &theFile)
{	NCFObject		cfStream;
	CFStringRef		cfError;
	OSStatus		theErr;



	// Open the stream
	if (!cfStream.Set(CFWriteStreamCreateWithFile(NULL, theFile.GetURL())))
		return(memFullErr);

	if (!CFWriteStreamOpen(cfStream))
		return(memFullErr);



	// Save the file
	cfError = NULL;
	theErr  = noErr;
	
	(void) CFPropertyListWriteToStream(mProperties, cfStream, mFormat, &cfError);
	
	if (cfError != NULL)
		{
		NN_LOG("Unable to save property list: %s", NString(cfError).GetUTF8());
		theErr = dskFulErr;
		}



	// Clean up
	CFReadStreamClose(cfStream);
	CFSafeRelease(cfError);
	
	return(theErr);
}





//============================================================================
//		NPropertyFile::GetProperties : Get the properties.
//----------------------------------------------------------------------------
NDictionary &NPropertyFile::GetProperties(void)
{


	// Get the properties
	return(mProperties);
}

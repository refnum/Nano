/*	NAME:
		NPropertyFile.cpp

	DESCRIPTION:
		Property file.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
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
//		NPropertyFile::Load : Load the property list.
//----------------------------------------------------------------------------
OSStatus NPropertyFile::Load(const NFile &theFile)
{	NCFStream		theStream(theFile);



	// Load the property list
	return(Load(theStream));
}





//============================================================================
//		NPropertyFile::Load : Load the property list.
//----------------------------------------------------------------------------
OSStatus NPropertyFile::Load(const NString &theText)
{	NCFStream		theStream(theText.GetSize(), theText.GetUTF8());



	// Load the property list
	return(Load(theStream));
}





//============================================================================
//		NPropertyFile::Load : Load the property list.
//----------------------------------------------------------------------------
OSStatus NPropertyFile::Load(NCFStream &theStream)
{	CFStringRef		cfError;
	OSStatus		theErr;



	// Validate our parameters
	NN_ASSERT(theStream.IsRead());
	


	// Reset our state
	mProperties.Clear();



	// Open the stream
	if (!theStream.Open())
		return(paramErr);



	// Load the property list
	cfError = NULL;
	theErr  = noErr;

	mProperties.Set(CFPropertyListCreateFromStream(kCFAllocatorNano, theStream, 0, kCFPropertyListMutableContainersAndLeaves, &mFormat, &cfError));

	if (cfError != NULL)
		{
		NN_LOG("Unable to load property list: %s", NString(cfError).GetUTF8());
		theErr = badFormat;
		}



	// Clean up
	theStream.Close();
	CFSafeRelease(cfError);
	
	return(theErr);
}





//============================================================================
//		NPropertyFile::Save : Save the property list.
//----------------------------------------------------------------------------
OSStatus NPropertyFile::Save(const NFile &theFile)
{	NCFStream		theStream(theFile, kCFStreamWrite);



	// Save the property list
	return(Save(theStream));
}





//============================================================================
//		NPropertyFile::Save : Save the property list.
//----------------------------------------------------------------------------
OSStatus NPropertyFile::Save(NString &theText)
{	NCFStream		theStream(0, NULL, kCFStreamWrite);
	NData			theData;
	OSStatus		theErr;



	// Save the property list
	theText.Clear();
	theErr = Save(theStream);

	if (theErr == noErr)
		{
		theData = theStream.GetPropertyData(kCFStreamPropertyDataWritten);
		if (theData.IsNotEmpty())
			theText = NString((char *) theData.GetData(), theData.GetSize());
		}
	
	return(theErr);
}





//============================================================================
//		NPropertyFile::Save : Save the property list.
//----------------------------------------------------------------------------
OSStatus NPropertyFile::Save(NCFStream &theStream)
{	CFStringRef		cfError;
	OSStatus		theErr;



	// Validate our parameters
	NN_ASSERT(theStream.IsWrite());



	// Open the stream
	if (!theStream.Open())
		return(paramErr);



	// Save the property list
	cfError = NULL;
	theErr  = noErr;
	
	(void) CFPropertyListWriteToStream(mProperties, theStream, mFormat, &cfError);
	
	if (cfError != NULL)
		{
		NN_LOG("Unable to save property list: %s", NString(cfError).GetUTF8());
		theErr = dskFulErr;
		}



	// Clean up
	theStream.Close();

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

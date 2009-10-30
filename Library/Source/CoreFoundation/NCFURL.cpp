/*	NAME:
		NCFURL.cpp

	DESCRIPTION:
		CoreFoundation URL object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTextUtilities.h"
#include "NFile.h"
#include "NCFURL.h"





//============================================================================
//		NCFURL::NCFURL : Constructor.
//----------------------------------------------------------------------------
NCFURL::NCFURL(const NString &theString)
{


	// Initialize ourselves
	Set(CFURLCreateWithString(kCFAllocatorNano, NTextUtilities::EncodeURL(theString), NULL));
	NN_ASSERT(IsValid());
}





//============================================================================
//		NCFURL::NCFURL : Constructor.
//----------------------------------------------------------------------------
NCFURL::NCFURL(const NData &theData, CFStringEncoding theEncoding)
{


	// Initialize ourselves
	Set(CFURLCreateWithBytes(kCFAllocatorNano, theData.GetData(), theData.GetSize(), theEncoding, NULL));
	NN_ASSERT(IsValid());
}





//============================================================================
//		NCFURL::NCFURL : Constructor.
//----------------------------------------------------------------------------
NCFURL::NCFURL(const NFile &theFile)
{


	// Initialize ourselves
	Set(CFURLCreateWithFileSystemPath(kCFAllocatorNano, theFile.GetPath(), kCFURLPOSIXPathStyle, false));
	NN_ASSERT(IsValid());
}





//============================================================================
//		NCFURL::NCFURL : Constructor.
//----------------------------------------------------------------------------
NCFURL::NCFURL(CFURLRef cfURL)
{


	// Initialize ourselves
	Set(cfURL, false);
	
	if (cfURL != NULL)
		NN_ASSERT(IsValid());
}





//============================================================================
//		NCFURL::~NCFURL : Destructor.
//----------------------------------------------------------------------------
NCFURL::~NCFURL(void)
{
}





//============================================================================
//		NCFURL::GetString : Get the URL string.
//----------------------------------------------------------------------------
NString NCFURL::GetString(void) const
{


	// Get the string
	return(CFURLGetString(*this));
}





//============================================================================
//		NCFURL::GetFile : Get the URL file.
//----------------------------------------------------------------------------
NFile NCFURL::GetFile(void) const
{	NString		thePath;
	NFile		theFile;



	// Get the file
	if (thePath.Set(CFURLCopyFileSystemPath(*this, kCFURLPOSIXPathStyle)))
		theFile.SetPath(thePath);
	
	return(theFile);
}





//============================================================================
//		NCFURL::IsDecomposable : Can the URL be decomposed?
//----------------------------------------------------------------------------
bool NCFURL::IsDecomposable(void) const
{


	// Get our state
	return(CFURLCanBeDecomposed(*this));
}





//============================================================================
//		NCFURL::GetScheme : Get the scheme.
//----------------------------------------------------------------------------
NString NCFURL::GetScheme(void) const
{	NString		theValue;



	// Get the scheme
	theValue.Set(CFURLCopyScheme(*this));
	
	return(theValue);
}





//============================================================================
//		NCFURL::GetUserName : Get the user name.
//----------------------------------------------------------------------------
NString NCFURL::GetUserName(void) const
{	NString		theValue;



	// Get the user name
	theValue.Set(CFURLCopyUserName(*this));
	
	return(theValue);
}





//============================================================================
//		NCFURL::GetPassword : Get the password.
//----------------------------------------------------------------------------
NString NCFURL::GetPassword(void) const
{	NString		theValue;



	// Get the password
	theValue.Set(CFURLCopyPassword(*this));
	
	return(theValue);
}





//============================================================================
//		NCFURL::GetHost : Get the host.
//----------------------------------------------------------------------------
NString NCFURL::GetHost(void) const
{	NString		theValue;



	// Get the host
	theValue.Set(CFURLCopyHostName(*this));
	
	return(theValue);
}





//============================================================================
//		NCFURL::GetPort : Get the port.
//----------------------------------------------------------------------------
UInt32 NCFURL::GetPort(void) const
{


	// Get the port
	return((UInt32) CFURLGetPortNumber(*this));
}





//============================================================================
//		NCFURL::GetPath : Get the path.
//----------------------------------------------------------------------------
NString NCFURL::GetPath(CFURLPathStyle pathStyle) const
{	Boolean		isAbsolute;
	NString		theValue;



	// Get the path
	//
	// The logical path component of a URL does not include a leading '/'.
	if (pathStyle == kCFURLLogicalPathStyle)
		theValue.Set(CFURLCopyStrictPath(    *this, &isAbsolute));
	else
		theValue.Set(CFURLCopyFileSystemPath(*this, pathStyle));

	return(theValue);
}





//============================================================================
//		NCFURL::GetResource : Get the resource.
//----------------------------------------------------------------------------
NString NCFURL::GetResource(void) const
{	NString		theValue;



	// Get the resource
	theValue.Set(CFURLCopyResourceSpecifier(*this));
	
	return(theValue);
}





//============================================================================
//		NCFURL::GetBaseURL : Get the base URL.
//----------------------------------------------------------------------------
NCFURL NCFURL::GetBaseURL(void) const
{


	// Get the base URL
	return(CFURLGetBaseURL(*this));
}





//============================================================================
//		NCFURL::GetPathFile : Get the path filename.
//----------------------------------------------------------------------------
NString NCFURL::GetPathFile(void) const
{	NString		theValue;



	// Get the file from the path
	theValue.Set(CFURLCopyLastPathComponent(*this));
	
	return(theValue);
}





//============================================================================
//		NCFURL::GetPathExtension : Get the path extension.
//----------------------------------------------------------------------------
NString NCFURL::GetPathExtension(void) const
{	NString		theValue;



	// Get the file extension from the path
	theValue.Set(CFURLCopyPathExtension(*this));
	
	return(theValue);
}





//============================================================================
//		NCFURL::GetNullValue : Get the null value.
//----------------------------------------------------------------------------
#pragma mark -
CFTypeRef NCFURL::GetNullValue(void)
{	static CFTypeRef	sNullValue = CFURLCreateWithString(kCFAllocatorNano, CFSTR("file://localhost/"), NULL);



	// Get the value
	//
    // Unfortunately empty CFURLs are not allowed, so we have to use a non-empty default.
	return(sNullValue);
}



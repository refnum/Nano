/*	NAME:
		NCFURL.h

	DESCRIPTION:
		CoreFoundation URL object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCFURL_HDR
#define NCFURL_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFObject.h"
#include "NString.h"
#include "NData.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const UInt32 kCFURLInvalidPort							= (UInt32) -1;

static const CFURLPathStyle kCFURLLogicalPathStyle				= (CFURLPathStyle) -1;





//============================================================================
//		Types
//----------------------------------------------------------------------------
class NFile;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCFURL : public NCFObject {
public:
										NCFURL(CFURLRef        cfURL);
										NCFURL(const NString &theString);
										NCFURL(const NData   &theData, CFStringEncoding theEncoding=kCFStringEncodingUTF8);
										NCFURL(const NFile   &theFile);

										NCFURL(void);
	virtual								~NCFURL(void);


	// Get the URL string
	NString								GetString(void) const;


	// Get the URL file
	NFile								GetFile(void) const;


	// Can we decomposed?
	bool								IsDecomposable(void) const;


	// Decompose the URL
	//
	// A URL can be broken down into:
	//
	//	scheme://user:password@host:port/path#resource
	//
	// If the URL does not contain a port number, kCFURLInvalidPort is returned.
	//
	// GetPath typically returns a filesystem path (which includes the leading '/',
	// that strictly speaking is not part of the path component). To obtain the
	// exact path component of the URL use kCFURLLogicalPathStyle.
	NString								GetScheme(  void) const;
	NString								GetUserName(void) const;
	NString								GetPassword(void) const;
	NString								GetHost(    void) const;
	UInt32								GetPort(    void) const;
	NString								GetPath(CFURLPathStyle pathStyle=kCFURLPOSIXPathStyle) const;
	NString								GetResource(void) const;


	// Get the base URL
	NCFURL								GetBaseURL(void) const;


	// Get the file path details
	NString								GetPathFile(     void) const;
	NString								GetPathExtension(void) const;


protected:
	// Get the null value
	CFTypeRef							GetNullValue(void);


private:
	void								InitializeSelf(CFURLRef cfURL);


private:


};





#endif // NCFURL_HDR



/*  NAME:
        NFileUtilities.h

    DESCRIPTION:
        File utilities.
    
    COPYRIGHT:
        Copyright (c) 2006-2009, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
    __________________________________________________________________________
*/
#ifndef NFILEUTILITIES_HDR
#define NFILEUTILITIES_HDR
//============================================================================
//      Include files
//----------------------------------------------------------------------------
#include "NFile.h"
#include "NData.h"





//============================================================================
//      Constants
//----------------------------------------------------------------------------
// Directory domains
typedef enum {
	kNDomainUser,						// Applies to current user
	kNDomainUsers,						// Applies to all users
	kNDomainSystem						// Applies to the system
} NDirectoryDomain;


// Directory locations
typedef enum {
	kNLocationHome,						// User's "home"
	kNLocationDesktop,					// User's desktop
	kNLocationCachedData,				// For persistent cache files
	kNLocationTemporaryData,			// For deleted-at-startup files
	kNLocationApplicationSupport,		// For application support files
} NDirectoryLocation;





//============================================================================
//      Class declaration
//----------------------------------------------------------------------------
class NFileUtilities {
public:
	// Get/set the contents of a file
	static NString						GetFileText(const NFile &theFile, NStringEncoding theEncoding=kNStringEncodingUTF8);
	static NData						GetFileData(const NFile &theFile);

	static NStatus						SetFileText(const NFile &theFile, const NString &theText, NStringEncoding theEncoding=kNStringEncodingUTF8);
	static NStatus						SetFileData(const NFile &theFile, const NData   &theData);


	// Get the current working directory
	static NFile						GetCWD(void);


	// Get a standard directory
	//
	// Returns the appropriate directory within the specified domain, creating it if necessary.
	//
	// If fileName is non-empty, the result will be to a file of that name within the directory.
	static NFile						GetDirectory(	NDirectoryLocation		theLocation,
														const NString			&fileName = "",
														NDirectoryDomain		theDomain = kNDomainUser);


	// Get a uniquely-named file
	//
	// If fileName is non-empty, it will be used as the base name.
	static NFile						GetUniqueFile(const NFile &theDirectory, const NString &fileName="");


	// Get a temporary file
	//
	// If fileName is non-empty, it will be used as the base name.
	static NFile						GetTemporaryFile(const NString &fileName="");
};



#endif // NFILEUTILITIES_HDR



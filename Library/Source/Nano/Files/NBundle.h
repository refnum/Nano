/*	NAME:
		NBundle.h

	DESCRIPTION:
		Resource bundle.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NBUNDLE_HDR
#define NBUNDLE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDictionary.h"
#include "NString.h"
#include "NArray.h"
#include "NFile.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const NString kNBundleExecutableKey						= "CFBundleExecutable";
static const NString kNBundleIdentifierKey						= "CFBundleIdentifier";
static const NString kNBundleVersionKey							= "CFBundleVersion";
static const NString kNBundleNameKey							= "CFBundleName";





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NBundle {
public:
										NBundle(const NFile   &theFile);
										NBundle(const NString &bundleID="");
	virtual								~NBundle(void);
	

	// Get the bundle identifier
	NString								GetIdentifier(void) const;


	// Get the bundle directory
	NFile								GetFile(void) const;


	// Get the resources directory
	NFile								GetResources(void) const;


	// Get an Info.plist value
	//
	// GetInfoDictionary returns the entire Info.plist dictionary if theKey is empty.
    bool								GetInfoBoolean(   const NString &theKey)    const;
	NString								GetInfoString(    const NString &theKey)    const;
	NArray								GetInfoArray(     const NString &theKey)    const;
	NDictionary							GetInfoDictionary(const NString &theKey="") const;


	// Get a resource
	NFile								GetResource(const NString &theName,
													const NString &theType = "",
													const NString &subDir  = "") const;


	// Get a string
	NString								GetString(const NString &theKey,
												  const NString &defaultValue = "",
												  const NString &tableName    = "") const;


private:
	void								LoadBundle(void)                     const;
	void								LoadStrings(const NString &theTable) const;


private:
	NFile								mFile;
	mutable NDictionary					mInfo;
	mutable NDictionary					mStrings;
};





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
inline NFile NBundleResource(const NString &theName, const NString &theType="", const NString &subDir="")
{	NBundle		appBundle;

	return(appBundle.GetResource(theName, theType, subDir));
}

inline NString NBundleString(const NString &theKey, const NString &defaultValue="", const NString &tableName="")
{	NBundle		appBundle;

	return(appBundle.GetString(theKey, defaultValue, tableName));
}





#endif // NBUNDLE_HDR



/*	NAME:
		NCFBundle.h

	DESCRIPTION:
		CoreFoundation bundle object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCFBUNDLE_HDR
#define NCFBUNDLE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDictionary.h"
#include "NCFObject.h"
#include "NString.h"
#include "NArray.h"
#include "NFile.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Info.plist keys
//
// Standard Info.plist keys that are not defined by CFBundle.h.
static const NString kCFBundleDisplayNameKey					= "CFBundleDisplayName";
static const NString kCFBundleSignatureKey						= "CFBundleSignature";
static const NString kCFBundleDocumentTypesKey					= "CFBundleDocumentTypes";
static const NString kCFBundleGetInfoStringKey					= "CFBundleGetInfoString";
static const NString kCFBundleHelpBookFolderKey					= "CFBundleHelpBookFolder";
static const NString kCFBundleHelpBookNameKey					= "CFBundleHelpBookName";
static const NString kCFBundleIconFileKey						= "CFBundleIconFile";
static const NString kCFBundleShortVersionStringKey				= "CFBundleShortVersionString";
static const NString kCFBundleTypeRoleKey						= "CFBundleTypeRole";
static const NString kCFBundleURLSchemesKey						= "CFBundleURLSchemes";
static const NString kCFBundleURLTypesKey						= "CFBundleURLTypes";
static const NString kLSItemContentTypesKey						= "LSItemContentTypes";
static const NString kSUFeedURLKey								= "SUFeedURL";


// Bundle identifiers
//
// Bundle identifiers for standard system applications.
static const NString kBundleIDSoftwareUpdate					= "com.apple.SoftwareUpdate";
static const NString kBundleIDAddressBook						= "com.apple.AddressBook";





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCFBundle : public NCFObject {
public:
										NCFBundle(const NFile   &theFile);
										NCFBundle(const NString &bundleID = "");
										NCFBundle(CFBundleRef  cfBundle);

	virtual								~NCFBundle(void);
	

	// Get the bundle identifier
	NString								GetIdentifier(void) const;


	// Get the bundle directory
	NFile								GetFile(void) const;


	// Get an Info.plist value
	//
	// GetInfoDictionary returns the root Info.plist dictionary if passed an empty key.
    bool								GetInfoBoolean(   const NString &theKey)    const;
	NString								GetInfoString(    const NString &theKey) const;
	NArray								GetInfoArray(     const NString &theKey) const;
	NDictionary							GetInfoDictionary(const NString &theKey="") const;


	// Get the Info.plist UTI values
	NArray								GetInfoUTIs(LSRolesMask theRoles=kLSRolesAll);


	// Get the resources directory
	NFile								GetResources(void) const;


	// Get a resource from the bundle
	NFile								GetResource(const NString &theName,
													const NString &theType = "",
													const NString &subDir  = "") const;


	// Get a string from the bundle
	NString								GetString(const NString &theKey,
												  const NString &defaultValue = "",
												  const NString &tableName    = "") const;
};





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
inline NFile NBundleResource(const NString &theName, const NString &theType="", const NString &subDir="")
{	NCFBundle	appBundle;

	return(appBundle.GetResource(theName, theType, subDir));
}

inline NString NBundleString(const NString &theKey, const NString &defaultValue="", const NString &tableName="")
{	NCFBundle	appBundle;

	return(appBundle.GetString(theKey, defaultValue, tableName));
}









#endif // NCFBUNDLE_HDR



/*	NAME:
		NCFBundle.h

	DESCRIPTION:
		CoreFoundation bundle object.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
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
static const NString kCFBundleGetInfoStringKey					= "CFBundleGetInfoString";
static const NString kCFBundleIconFileKey						= "CFBundleIconFile";
static const NString kCFBundleDocumentTypesKey					= "CFBundleDocumentTypes";
static const NString kCFBundleTypeRoleKey						= "CFBundleTypeRole";
static const NString kLSItemContentTypesKey						= "LSItemContentTypes";





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCFBundle : public NCFObject {
public:
										NCFBundle(const NString &bundleID="");
	virtual								~NCFBundle(void);
	

	// Get the bundle identifier
	NString								GetIdentifier(void) const;


	// Get the bundle directory
	NFile								GetFile(void) const;


	// Get an Info.plist value
	//
	// GetInfoDictionary returns the root Info.plist dictionary if passed an empty key.
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


private:


};





#endif // NCFBUNDLE_HDR



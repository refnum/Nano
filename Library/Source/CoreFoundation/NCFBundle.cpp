/*	NAME:
		NCFBundle.cpp

	DESCRIPTION:
		CoreFoundation bundle object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFBundle.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kRoleNone										= "none";
static const NString kRoleEditor									= "editor";
static const NString kRoleViewer									= "viewer";
static const NString kRoleShell										= "shell";





//============================================================================
//		NCFBundle::NCFBundle : Constructor.
//----------------------------------------------------------------------------
NCFBundle::NCFBundle(const NFile &theFile)
{


	// Initialize ourselves
	Set(CFBundleCreate(kCFAllocatorNano, theFile.GetURL()));
}





//============================================================================
//		NCFBundle::NCFBundle : Constructor.
//----------------------------------------------------------------------------
NCFBundle::NCFBundle(const NString &bundleID)
{	bool			takeOwnership;
	CFBundleRef     cfBundle;
	OSStatus		theErr;
	CFURLRef		cfURL;



	// Find the bundle
	//
	// If a name is supplied, we first check the currently open bundles
	// before falling back to a more expensive search using LaunchServices.
	//
	// Note that LSFindApplicationForInfo, despite not being a CreateXXX
	// API, expects us to take ownership of the returned URL.
	takeOwnership = false;

	if (bundleID.IsEmpty())
		cfBundle = CFBundleGetMainBundle();
	else
		{
		cfBundle = CFBundleGetBundleWithIdentifier(bundleID);
		if (cfBundle == NULL)
			{
			theErr = LSFindApplicationForInfo(kLSUnknownCreator, bundleID, NULL, NULL, &cfURL);
			if (theErr == noErr && cfURL != NULL)
				{
				cfBundle      = CFBundleCreate(kCFAllocatorNano, cfURL);
				takeOwnership = (cfBundle != NULL);

				CFSafeRelease(cfURL);
				}
			}
		}



	// Initialize ourselves
    Set(cfBundle, takeOwnership);
	NN_ASSERT(IsValid());
}





//============================================================================
//		NCFBundle::NCFBundle : Constructor.
//----------------------------------------------------------------------------
NCFBundle::NCFBundle(CFBundleRef cfBundle)
{


	// Initialize ourselves
	Set(cfBundle, false);
	
	if (cfBundle != NULL)
		NN_ASSERT(IsValid());
}





//============================================================================
//		NCFBundle::~NCFBundle : Destructor.
//----------------------------------------------------------------------------
NCFBundle::~NCFBundle(void)
{
}





//============================================================================
//		NCFBundle::GetIdentifier : Get the identifier.
//----------------------------------------------------------------------------
NString NCFBundle::GetIdentifier(void) const
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Get the identifier
	return(CFBundleGetIdentifier(*this));
}





//============================================================================
//		NCFBundle::GetFile : Get the file.
//----------------------------------------------------------------------------
NFile NCFBundle::GetFile(void) const
{	NCFURL	theURL;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the file
	theURL.Set(CFBundleCopyBundleURL(*this));
	
	return(theURL.GetFile());
}





//============================================================================
//      NCFBundle::GetInfoBoolean : Get an Info.plist boolean.
//----------------------------------------------------------------------------
bool NCFBundle::GetInfoBoolean(const NString &theKey) const
{ 


    // Validate our state
    NN_ASSERT(IsValid());



    // Get the value
    return(CFBooleanGetValue((CFBooleanRef) CFBundleGetValueForInfoDictionaryKey(*this, theKey)));
}





//============================================================================
//		NCFBundle::GetInfoString : Get an Info.plist string.
//----------------------------------------------------------------------------
NString NCFBundle::GetInfoString(const NString &theKey) const
{	NString		theValue;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the value
	theValue.Set(CFBundleGetValueForInfoDictionaryKey(*this, theKey), false);
	
	return(theValue);
}





//============================================================================
//		NCFBundle::GetInfoArray : Get an Info.plist array.
//----------------------------------------------------------------------------
NArray NCFBundle::GetInfoArray(const NString &theKey) const
{	NArray	theValue;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the value
	theValue.Set(CFBundleGetValueForInfoDictionaryKey(*this, theKey), false);
	
	return(theValue);
}





//============================================================================
//		NCFBundle::GetInfoDictionary : Get an Info.plist dictionary.
//----------------------------------------------------------------------------
NDictionary NCFBundle::GetInfoDictionary(const NString &theKey) const
{	NDictionary		theValue;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the value
	if (theKey.IsEmpty())
		theValue.Set(CFBundleGetLocalInfoDictionary(*this), false);
	else
		theValue.Set(CFBundleGetValueForInfoDictionaryKey(*this, theKey), false);
	
	return(theValue);
}





//============================================================================
//		NCFBundle::GetInfoUTIs : Get the Info.plist UTI values.
//----------------------------------------------------------------------------
NArray NCFBundle::GetInfoUTIs(LSRolesMask theRoles)
{	bool			isMatch, isNone, isEditor, isViewer, isShell;
	NArray			theTypes, theUTIs, theResult;
	UInt32			n, numItems;
	NDictionary		typeDict;
	NString			theRole;



	// Validate our state
	NN_ASSERT(IsValid());



	// Extract the UTIs
	theTypes = GetInfoArray(kCFBundleDocumentTypesKey);
	numItems = theTypes.GetSize();
	
	for (n = 0; n < numItems; n++)
		{
		// Get the document type info
		typeDict = theTypes.GetValueDictionary(n);
		theRole  = typeDict.GetValueString(kCFBundleTypeRoleKey);

		isNone   = theRole.EqualTo(kRoleNone);
		isEditor = theRole.EqualTo(kRoleEditor);
		isViewer = theRole.EqualTo(kRoleViewer) || isEditor;
		isShell  = theRole.EqualTo(kRoleShell);



		// Add the UTIs that match
		isMatch = ( (isNone   && (theRoles & kLSRolesNone))   ||
					(isEditor && (theRoles & kLSRolesEditor)) ||
					(isViewer && (theRoles & kLSRolesViewer)) ||
					(isShell  && (theRoles & kLSRolesShell)) );

		if (isMatch)
			{
			theUTIs = typeDict.GetValueArray(kLSItemContentTypesKey);
			theResult.Join(theUTIs);
			}
		}
	
	return(theResult);
}





//============================================================================
//		NCFBundle::GetResources : Get the resources directory.
//----------------------------------------------------------------------------
NFile NCFBundle::GetResources(void) const
{	NCFURL		baseURL, resURL, theURL;
	NString		thePath;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the URL
	//
	// Since CFBundleCopyResourcesDirectoryURL returns a relative URL,
	// we need to combine it with our path to obtain the final file.
	baseURL.Set(CFBundleCopyBundleURL(            *this));
	resURL.Set( CFBundleCopyResourcesDirectoryURL(*this));

	thePath = resURL.GetString();
	theURL.Set(CFURLCreateAbsoluteURLWithBytes(kCFAllocatorNano, (const UInt8 *) thePath.GetUTF8(), thePath.GetSize(), kCFStringEncodingUTF8, baseURL, false));
	
	return(theURL.GetFile());
}





//============================================================================
//		NCFBundle::GetResource : Get a resource from the bundle.
//----------------------------------------------------------------------------
NFile NCFBundle::GetResource(const NString &theName, const NString &theType, const NString &subDir) const
{	CFStringRef		cfType, cfSubDir;
	NFile			theFile;
	NCFURL			theURL;



	// Validate our state
	NN_ASSERT(IsValid());



	// Check our parameters
	if (theName.IsEmpty())
		return(theFile);



	// Get the state we need
	cfType   = theType.IsEmpty() ? NULL : (CFStringRef) theType;
	cfSubDir =  subDir.IsEmpty() ? NULL : (CFStringRef) subDir;



	// Get the file
	//
	// We allow bundle resources to be specified with an absolute path, since
	// this lets us use absolute paths in .nib files and treat those items as
	// if they were part of the bundle.
	if (theName.StartsWith("/"))
		theFile = NFile(theName);
	else
		{
		if (theURL.Set(CFBundleCopyResourceURL(*this, theName, cfType, cfSubDir)))
			theFile = theURL.GetFile();
		}
	
	return(theFile);
}





//============================================================================
//		NCFBundle::GetString : Get a string from the bundle.
//----------------------------------------------------------------------------
NString NCFBundle::GetString(const NString &theKey, const NString &defaultValue, const NString &tableName) const
{	CFStringRef		cfDefaultValue, cfTableName;
	NString			theValue;



	// Validate our state
	NN_ASSERT(IsValid());



	// Check our parameters
	if (theKey.IsEmpty())
		return(theValue);



	// Get the state we need
	cfDefaultValue = defaultValue.IsEmpty() ? NULL : ((CFStringRef) defaultValue);
	cfTableName    =    tableName.IsEmpty() ? NULL : ((CFStringRef) tableName);



	// Get the string
	theValue.Set(CFBundleCopyLocalizedString(*this, theKey, cfDefaultValue, cfTableName));
	
	return(theValue);
}


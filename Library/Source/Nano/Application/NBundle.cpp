/*	NAME:
		NBundle.cpp

	DESCRIPTION:
		Resource bundle.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NBundle.h"





//============================================================================
//		NBundle::NBundle : Constructor.
//----------------------------------------------------------------------------
NBundle::NBundle(const NFile &theFile)
{


	// Initialize ourselves
	mFile = theFile;
}





//============================================================================
//		NBundle::NBundle : Constructor.
//----------------------------------------------------------------------------
NBundle::NBundle(const NString &bundleID)
{
// dair
/*
	bool			takeOwnership;
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
*/
}





//============================================================================
//		NBundle::~NBundle : Destructor.
//----------------------------------------------------------------------------
NBundle::~NBundle(void)
{
}





//============================================================================
//		NBundle::GetIdentifier : Get the identifier.
//----------------------------------------------------------------------------
NString NBundle::GetIdentifier(void) const
{


	// Get the identifier
	return(GetInfoString(kNBundleIdentifierKey));
}





//============================================================================
//		NBundle::GetFile : Get the bundle directory.
//----------------------------------------------------------------------------
NFile NBundle::GetFile(void) const
{


	// Get the bundle
	return(mFile);
}





//============================================================================
//		NBundle::GetResources : Get the resources directory.
//----------------------------------------------------------------------------
NFile NBundle::GetResources(void) const
{


	// Get the resources
	return(mFile.GetChild("Contents/Resources"));
}





//============================================================================
//      NBundle::GetInfoBoolean : Get an Info.plist boolean.
//----------------------------------------------------------------------------
bool NBundle::GetInfoBoolean(const NString &theKey) const
{


	// Get the value
	return(GetInfoDictionary().GetValueBoolean(theKey));
}





//============================================================================
//		NBundle::GetInfoString : Get an Info.plist string.
//----------------------------------------------------------------------------
NString NBundle::GetInfoString(const NString &theKey) const
{


	// Get the value
	return(GetInfoDictionary().GetValueString(theKey));
}





//============================================================================
//		NBundle::GetInfoArray : Get an Info.plist array.
//----------------------------------------------------------------------------
NArray NBundle::GetInfoArray(const NString &theKey) const
{


	// Get the value
	return(GetInfoDictionary().GetValueArray(theKey));
}





//============================================================================
//		NBundle::GetInfoDictionary : Get an Info.plist dictionary.
//----------------------------------------------------------------------------
NDictionary NBundle::GetInfoDictionary(const NString &theKey) const
{	NDictionary		theValue;



	// Load the dictionary
	if (mInfo.IsEmpty())
		LoadDictionary();



	// Get the value
	if (theKey.IsEmpty())
		theValue = mInfo;
	else
		theValue = mInfo.GetValueDictionary(theKey);
	
	return(theValue);
}





//============================================================================
//		NBundle::GetResource : Get a resource from the bundle.
//----------------------------------------------------------------------------
NFile NBundle::GetResource(const NString &theName, const NString &theType, const NString &subDir) const
{
// dair
/*
	CFStringRef		cfType, cfSubDir;
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
*/
}





//============================================================================
//		NBundle::GetString : Get a string from the bundle.
//----------------------------------------------------------------------------
NString NBundle::GetString(const NString &theKey, const NString &defaultValue, const NString &tableName) const
{
// dair
/*
	CFStringRef		cfDefaultValue, cfTableName;
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
*/
}





//============================================================================
//		NBundle::LoadDictionary : Load the dictionary.
//----------------------------------------------------------------------------
#pragma mark -
void NBundle::LoadDictionary(void) const
{	NFile		thePList;



	// Validate our state
	NN_ASSERT(mInfo.IsEmpty());



	// Load the dictionary
	thePList = mFile.GetChild("Contents/Info.plist");



		// dair, to do
}





/*	NAME:
		NBundle.cpp

	DESCRIPTION:
		Resource bundle.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFileUtilities.h"
#include "NPropertyList.h"
#include "NTargetSystem.h"
#include "NBundle.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kPathBundlePList						= "Contents/Info.plist";
static const NString kPathBundleResources					= "Contents/Resources";

static const NString kStringsExtension						= ".strings";
static const NString kStringsDefaultLanguage				= "en.lproj";
static const NString kStringsDefaultTable					= "Localizable";





//============================================================================
//		NBundle::NBundle : Constructor.
//----------------------------------------------------------------------------
NBundle::NBundle(const NFile &theFile)
{


	// Validate our parameters
	NN_ASSERT(theFile.IsDirectory());



	// Initialize ourselves
	mFile = theFile;
}





//============================================================================
//		NBundle::NBundle : Constructor.
//----------------------------------------------------------------------------
NBundle::NBundle(const NString &bundleID)
{


	// Initialize ourselves
	mFile = NTargetSystem::FindBundle(bundleID);
	NN_ASSERT(mFile.IsDirectory());
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
	return(mFile.GetChild(kPathBundleResources));
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



	// Get the value
	theValue = GetBundleInfo();
	
	if (!theKey.IsEmpty())
		theValue = theValue.GetValueDictionary(theKey);
	
	return(theValue);
}





//============================================================================
//		NBundle::GetResource : Get a resource from the bundle.
//----------------------------------------------------------------------------
NFile NBundle::GetResource(const NString &theName, const NString &theType, const NString &subDir) const
{	NFile		theFile;
	NString		thePath;



	// Check our parameters
	if (theName.IsEmpty())
		return(theFile);



	// Get the resource
	//
	// We allow bundle resources to be specified with an absolute path,
	// since this allows us to reference 'resources' outside the bundle.
	if (theName.StartsWith("/"))
		thePath = theName;
	else
		{
		if (subDir.IsEmpty())
			thePath.Format("%@/%@",    kPathBundleResources,         theName);
		else
			thePath.Format("%@/%@/%@", kPathBundleResources, subDir, theName);

		if (!theType.IsEmpty())
			{
			if (!theType.StartsWith("."))
				thePath += ".";

			thePath += theType;
			}
		}
	
	theFile = NFile(thePath);

	return(theFile);
}





//============================================================================
//		NBundle::GetString : Get a string from the bundle.
//----------------------------------------------------------------------------
NString NBundle::GetString(const NString &theKey, const NString &defaultValue, const NString &tableName) const
{	NString			theTable, theValue;
	NDictionary		theStrings;



	// Get the state we need
	theTable = tableName.IsEmpty() ? kStringsDefaultTable : tableName;
	theTable.Replace(kStringsExtension, "");
	theStrings = GetBundleStrings(theTable);



	// Get the value
	theValue = theStrings.GetValueString(theKey);
	if (theValue.IsEmpty())
		theValue = defaultValue;
	
	return(theValue);
}





//============================================================================
//		NBundle::GetBundleInfo : Get a bundle's info dictionary.
//----------------------------------------------------------------------------
#pragma mark -
NDictionary NBundle::GetBundleInfo(void) const
{	NBundleInfo			*bundleInfo;
	NDictionary			theResult;
	NString				thePath;
	NFile				theFile;
	NPropertyList		pList;



	// Get the state we need
	bundleInfo = AcquireInfo(mFile);
	theResult  = bundleInfo->theInfo;



	// Load the info
	if (theResult.IsEmpty())
		{
		theFile   = mFile.GetChild(kPathBundlePList);
		theResult = pList.Load(theFile);

		bundleInfo->theInfo = theResult;
		}
	
	
	
	// Clean up
	ReleaseInfo();
	
	return(theResult);
}





//============================================================================
//		NBundle::GetBundleStrings : Get a bundle's string table.
//----------------------------------------------------------------------------
NDictionary NBundle::GetBundleStrings(const NString &theTable) const
{	NString							theText, theLine, theKey, theValue;
	NBundleInfo						*bundleInfo;
	NDictionary						theResult;
	NRangeList						theRanges;
	NStringList						theLines;
	NStringListConstIterator		theIter;
	NFile							theFile;



	// Validate our parameters
	NN_ASSERT(!theTable.IsEmpty());



	// Get the state we need
	bundleInfo = AcquireInfo(mFile);
	theResult  = bundleInfo->theStrings.GetValueDictionary(theTable);



	// Load the strings
	if (theResult.IsEmpty())
		{
		// Get the state we need
		theFile  = GetResource(theTable, kStringsExtension, kStringsDefaultLanguage);
		theText  = NFileUtilities::GetFileText(theFile);
		theLines = theText.Split(kNStringNewline);



		// Parse the strings
		for (theIter = theLines.begin(); theIter != theLines.end(); theIter++)
			{
			theLine   = *theIter;
			theRanges = theLine.FindAll("(.*?)\\s*=\\s*\"(.*)\";", kNStringPattern);

			if (theRanges.size() == 3)
				{
				theKey   = theLine.GetString(theRanges[1]);
				theValue = theLine.GetString(theRanges[2]);

				theResult.SetValue(theKey, theValue);
				}
			}
		
		
		
		// Update the info
		bundleInfo->theStrings.SetValue(theTable, theResult);
		}
	
	
	
	// Clean up
	ReleaseInfo();
	
	return(theResult);
}





//============================================================================
//		NBundle::AcquireInfo : Acquire the info for a bundle.
//----------------------------------------------------------------------------
NBundleInfo *NBundle::AcquireInfo(const NFile &theFile)
{	NBundleInfo					theInfo;
	NBundleInfoMapIterator		theIter;
	NString						thePath;



	// Validate our parameters
	NN_ASSERT(theFile.IsDirectory());



	// Acquire the lock
	mLock.Lock();



	// Get the state we need
	thePath = theFile.GetPath();
	theIter = mBundles.find(theFile.GetPath());



	// Populate the info
	if (theIter == mBundles.end())
		{
		mBundles[thePath] = theInfo;
		theIter           = mBundles.find(theFile.GetPath());
		}
	
	
	
	// Get the info
	return(&theIter->second);
}





//============================================================================
//		NBundle::ReleaseInfo : Release the info for a bundle.
//----------------------------------------------------------------------------
void NBundle::ReleaseInfo(void)
{


	// Release the lock
	mLock.Unlock();
}



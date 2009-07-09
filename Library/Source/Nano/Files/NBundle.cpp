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


	// Initialize ourselves
	mFile = theFile;
}





//============================================================================
//		NBundle::NBundle : Constructor.
//----------------------------------------------------------------------------
NBundle::NBundle(const NString &bundleID)
{


	// Validate our parameters
	NN_ASSERT(bundleID.IsEmpty());
	
	
	
	// Initialize ourselves
		// mFile = app bundle
		NN_LOG("NBundle - unable to locate app bundle");
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



	// Load the bundle
	if (mInfo.IsEmpty())
		LoadBundle();



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

		if (theType.IsNotEmpty())
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
{	NString		theTable, theValue;



	// Load the strings
	theTable = tableName.IsEmpty() ? kStringsDefaultTable : tableName;
	theTable.Replace(kStringsExtension, "");

	if (!mStrings.HasKey(theTable))
		LoadStrings(theTable);



	// Get the value
	theValue = mStrings.GetValueDictionary(theTable).GetValueString(theKey);
	if (theValue.IsEmpty())
		theValue = defaultValue;
	
	return(theValue);
}





//============================================================================
//		NBundle::LoadBundle : Load the bundle.
//----------------------------------------------------------------------------
#pragma mark -
void NBundle::LoadBundle(void) const
{	NFile				theFile;
	NPropertyList		pList;



	// Validate our state
	NN_ASSERT(mInfo.IsEmpty());



	// Load the dictionary
	theFile = mFile.GetChild(kPathBundlePList);
	mInfo   = pList.Load(theFile);
}





//============================================================================
//		NBundle::LoadStrings : Load the strings.
//----------------------------------------------------------------------------
void NBundle::LoadStrings(const NString &theTable) const
{	NString							theText, theLine, theKey, theValue;
	NDictionary						theStrings;
	NRangeList						theRanges;
	NStringList						theLines;
	NStringListConstIterator		theIter;
	NFile							theFile;



	// Validate our parameters and state
	NN_ASSERT(theTable.IsNotEmpty());
	NN_ASSERT(!mStrings.HasKey(theTable));



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
			
			theStrings.SetValue(theKey, theValue);
			}
		}



	// Update our state
	mStrings.SetValue(theTable, theStrings);
}





/*	NAME:
		NUTIRegistryRegistry.cpp

	DESCRIPTION:
		UTI Registry.
		
		For now we implement a simple hard-coded list of UTI types; this should
		be expanded over time, and ideally mapped to the native target equivalent
		(LaunchServices on Mac, Registry on Windows).

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSTLUtilities.h"
#include "NUTIRegistry.h"





//============================================================================
//		NUTIRegistry::NUTIRegistry : Constructor.
//----------------------------------------------------------------------------
NUTIRegistry::NUTIRegistry(void)
{


	// Initialise the registry
	InitializeRegistry();
}





//============================================================================
//		NUTIRegistry::~NUTIRegistry : Destructor.
//----------------------------------------------------------------------------
NUTIRegistry::~NUTIRegistry(void)
{
}





//============================================================================
//		NUTIRegistry::GetUTI : Get a UTI.
//----------------------------------------------------------------------------
NString NUTIRegistry::GetUTI(NUTITagClass theClass, const NString &theTag)
{	StLock							acquireLock(mLock);
	NString							theUTI, lowerTag;
	NUTIInfoMapConstIterator		iterInfo;
	NUTITagMapConstIterator			iterTag;
	NUTIInfo						theInfo;



	// Get the state we need
	lowerTag = theTag.GetLower();



	// Get the UTI
	//
	// This should perform a deepest-first search, to ensure we find
	// the most-specific UTI that matches the class/tag combination.
	//
	// For now we just return the first match.
	for (iterInfo = mInfo.begin(); iterInfo != mInfo.end(); iterInfo++)
		{
		theInfo = iterInfo->second;
		iterTag = theInfo.theTags.find(theClass);
			
		if (iterTag != theInfo.theTags.end())
			{
			if (iterTag->second == lowerTag)
				{
				theUTI = iterInfo->first;
				break;
				}
			}
		}
	
	return(theUTI);
}





//============================================================================
//		NUTIRegistry::GetDescription : Get a UTI's description.
//----------------------------------------------------------------------------
NString NUTIRegistry::GetDescription(const NString &theUTI)
{	StLock							acquireLock(mLock);
	NString							theResult;
	NUTIInfoMapConstIterator		theIter;



	// Validate our parameters
	NN_ASSERT(!theUTI.IsEmpty());



	// Get the description
	theIter = mInfo.find(theUTI);
	if (theIter != mInfo.end())
		theResult = theIter->second.theDescription;

	return(theResult);
}





//============================================================================
//		NUTIRegistry::GetConformsTo : Get a UTI's conforms-to list.
//----------------------------------------------------------------------------
NStringList NUTIRegistry::GetConformsTo(const NString &theUTI)
{	StLock							acquireLock(mLock);
	NStringList						theResult;
	NUTIInfoMapConstIterator		theIter;



	// Validate our parameters
	NN_ASSERT(!theUTI.IsEmpty());



	// Get the tags
	theIter = mInfo.find(theUTI);
	if (theIter != mInfo.end())
		theResult = theIter->second.conformsTo;
	
	return(theResult);
}





//============================================================================
//		NUTIRegistry::GetTags : Get a UTI's tags.
//----------------------------------------------------------------------------
NUTITagMap NUTIRegistry::GetTags(const NString &theUTI)
{	StLock							acquireLock(mLock);
	NUTITagMap						theResult;
	NUTIInfoMapConstIterator		theIter;



	// Validate our parameters
	NN_ASSERT(!theUTI.IsEmpty());



	// Get the tags
	theIter = mInfo.find(theUTI);
	if (theIter != mInfo.end())
		theResult = theIter->second.theTags;
	
	return(theResult);
}





//============================================================================
//		NUTIRegistry::GetTag : Get a UTI tag.
//----------------------------------------------------------------------------
NString NUTIRegistry::GetTag(const NString &theUTI, NUTITagClass theClass)
{	StLock							acquireLock(mLock);
	NUTITagMapConstIterator			iterTags;
	NUTIInfoMapConstIterator		iterInfo;
	NUTITagMap						theTags;
	NString							theTag;



	// Validate our parameters
	NN_ASSERT(!theUTI.IsEmpty());



	// Get the tag value
	iterInfo = mInfo.find(theUTI);
	if (iterInfo != mInfo.end())
		{
		iterTags = iterInfo->second.theTags.find(theClass);
		if (iterTags != iterInfo->second.theTags.end())
			theTag = iterTags->second;
		}
	
	return(theTag);
}





//============================================================================
//		NUTIRegistry::AddUTI : Add a UTI.
//----------------------------------------------------------------------------
void NUTIRegistry::AddUTI(	const NString		&theUTI,
							const NString		&theDescription,
							const NString		&conformsTo,
								  NUTITagClass	 theClass,
							const NString		&theTag)
{	StLock			acquireLock(mLock);
	NUTIInfo		theInfo;



	// Add the UTI
	theInfo.theDescription    = theDescription;
	theInfo.conformsTo        = vector(conformsTo);
	theInfo.theTags[theClass] = theTag.GetLower();

	NN_ASSERT(mInfo.find(theUTI) == mInfo.end());
	mInfo[theUTI] = theInfo;
}





//============================================================================
//		NUTIRegistry::AddTag : Add a tag.
//----------------------------------------------------------------------------
void NUTIRegistry::AddTag(const NString &theUTI, NUTITagClass theClass, const NString &theTag)
{	StLock					acquireLock(mLock);
	NUTIInfoMapIterator		theIter;



	// Add the tag
	theIter = mInfo.find(theUTI);
	NN_ASSERT(theIter != mInfo.end());
		
	if (theIter != mInfo.end())
		theIter->second.theTags[theClass] = theTag.GetLower();
}





//============================================================================
//		NUTIRegistry::AddConformsTo : Add a conformance association.
//----------------------------------------------------------------------------
void NUTIRegistry::AddConformsTo(const NString &theUTI, const NString &conformsTo)
{	StLock					acquireLock(mLock);
	NUTIInfoMapIterator		theIter;



	// Add the tag
	theIter = mInfo.find(theUTI);
	NN_ASSERT(theIter != mInfo.end());
		
	if (theIter != mInfo.end())
		theIter->second.conformsTo.push_back(conformsTo);
}





//============================================================================
//		NUTIRegistry::Get : Get the registry.
//----------------------------------------------------------------------------
NUTIRegistry *NUTIRegistry::Get(void)
{	static NUTIRegistry		sRegistry;



	// Get the registry
	//
	// We return a local static to ensure our state is constructed before
	// any other static initialisers.
	return(&sRegistry);
}





//============================================================================
//		NUTIRegistryRegistry::InitializeRegistry : Initialize the registry.
//----------------------------------------------------------------------------
void NUTIRegistry::InitializeRegistry(void)
{


	// Validate our state
	NN_ASSERT(mInfo.empty());



	// Initialize the registry
	AddUTI(kNUTTypeItem,    "", kNUTTypeNone, kNUTITagClassNone, "");
	AddUTI(kNUTTypeContent, "", kNUTTypeNone, kNUTITagClassNone, "");

	AddUTI(kNUTTypeData, "", kNUTTypeItem, kNUTITagClassNone, "");
	AddUTI(kNUTTypeText, "", kNUTTypeItem, kNUTITagClassNone, "");

	AddUTI(       kNUTTypeImage, "Image", kNUTTypeData, kNUTITagClassNone, "");
	AddConformsTo(kNUTTypeImage,          kNUTTypeContent);

	AddUTI(       kNUTTypeXML, "XML", kNUTTypeText, kNUTITagClassNone, "");
	AddConformsTo(kNUTTypeXML,        kNUTTypeContent);

	AddUTI(kNUTTypeGIF, "GIF Image",	kNUTTypeImage,	kNUTITagClassMIMEType,      "image/gif");
	AddTag(kNUTTypeGIF,									kNUTITagClassFileExtension, "gif");

	AddUTI(kNUTTypePNG, "PNG Image",	kNUTTypeImage,	kNUTITagClassMIMEType,      "image/png");
	AddTag(kNUTTypePNG,									kNUTITagClassFileExtension, "png");

	AddUTI(kNUTTypeJPEG, "JPEG Image",	kNUTTypeImage,	kNUTITagClassMIMEType,      "image/jpeg");
	AddTag(kNUTTypeJPEG,								kNUTITagClassFileExtension, "jpg");

	AddUTI(kNUTTypeKML, "Google KML",	kNUTTypeXML,	kNUTITagClassMIMEType,      "application/vnd.google-earth.kml+xml");
	AddTag(kNUTTypeKML,									kNUTITagClassFileExtension, "kml");

	AddUTI(kNUTTypeKMZ, "Google KMZ",	kNUTTypeData,	kNUTITagClassMIMEType,      "application/vnd.google-earth.kmz");
	AddTag(kNUTTypeKMZ,									kNUTITagClassFileExtension, "kmz");

	AddUTI(kNUTTypeGPX, "GPX",			kNUTTypeXML,	kNUTITagClassMIMEType,      "application/gpx+xml");
	AddTag(kNUTTypeGPX,									kNUTITagClassFileExtension, "gpx");

	AddUTI(kNUTTypeCSV, "CSV",			kNUTTypeText,	kNUTITagClassMIMEType,      "text/csv");
	AddTag(kNUTTypeCSV,									kNUTITagClassFileExtension, "csv");
}




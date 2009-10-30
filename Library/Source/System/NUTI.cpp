/*	NAME:
		NUTI.cpp

	DESCRIPTION:
		Universal Type Identifier object.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NUTI.h"





//============================================================================
//		Public constants
//----------------------------------------------------------------------------
// UTIs
//
// See NUTI.h - to support older systems, we redeclare UTCoreTypes.h values.
//
// In addition, we provide named constants for UTIs that are assigned a dynamic
// type at runtime (e.g., there is no UTI for colors (rdar://4520770), and the
// closest match is to use the dynamic UTI used by the Cocoa NSPasteboard).
const NUTI kUTNTypeColor									= UTTypeCreatePreferredIdentifierForTag(kUTTagClassNSPboardType, (CFStringRef) NSColorPboardType, NULL);
const NUTI kUTNTypeFileURL									= CFSTR("public.file-url");
const NUTI kUTNTypeImage									= CFSTR("public.image");
const NUTI kUTNTypeItem										= CFSTR("public.item");
const NUTI kUTNTypeJPEG										= CFSTR("public.jpeg");
const NUTI kUTNTypePICT										= CFSTR("com.apple.pict");
const NUTI kUTNTypePNG										= CFSTR("public.png");
const NUTI kUTNTypeRTF										= CFSTR("public.rtf");
const NUTI kUTNTypeUTF8PlainText							= CFSTR("public.utf8-plain-text");
const NUTI kUTNTypeUTF16PlainText							= CFSTR("public.utf16-plain-text");





//============================================================================
//		NUTI::NUTI : Constructor.
//----------------------------------------------------------------------------
NUTI::NUTI(const NString &theUTI)
{


	// Initialize ourselves
	mValue = theUTI;
}





//============================================================================
//		NUTI::NUTI : Constructor.
//----------------------------------------------------------------------------
NUTI::NUTI(CFStringRef theUTI)
{


	// Initialize ourselves
	mValue.Set(theUTI, false);
}





//============================================================================
//		NUTI::NUTI : Constructor.
//----------------------------------------------------------------------------
NUTI::NUTI(const NString &tagClass, const NString &theTag, const NString &conformsTo)
{	CFStringRef		cfString;



	// Initialize ourselves
	cfString = conformsTo.IsEmpty() ? NULL : (CFStringRef) conformsTo;

	mValue.Set(UTTypeCreatePreferredIdentifierForTag(tagClass, theTag, cfString));
}





//============================================================================
//		NUTI::NUTI : Constructor.
//----------------------------------------------------------------------------
NUTI::NUTI(void)
{
}





//============================================================================
//		NUTI::~NUTI : Destructor.
//----------------------------------------------------------------------------
NUTI::~NUTI(void)
{
}





//============================================================================
//		NUTI::IsValid : Is the UTI valid?
//----------------------------------------------------------------------------
bool NUTI::IsValid(void) const
{


	// Check our state
	return(mValue.IsNotEmpty());
}





//============================================================================
//		NUTI::Clear : Clear the UTI.
//----------------------------------------------------------------------------
void NUTI::Clear(void)
{


	// Reset our state
	mValue.Clear();
}





//============================================================================
//		NUTI::ConformsTo : Does this UTI conform to another?
//----------------------------------------------------------------------------
bool NUTI::ConformsTo(const NUTI &theUTI) const
{


	// Check our state
	return(UTTypeConformsTo(mValue, theUTI.mValue));
}





//============================================================================
//		NUTI::GetFileType : Get the file type tag.
//----------------------------------------------------------------------------
OSType NUTI::GetFileType(void) const
{	NString		theResult;



	// Get the tag
	theResult.Set(UTTypeCopyPreferredTagWithClass(mValue, kUTTagClassOSType));
	
	return(GetOSTypeFromString(theResult));
}





//============================================================================
//		NUTI::GetFileExtension : Get the file extension tag.
//----------------------------------------------------------------------------
NString NUTI::GetFileExtension(void) const
{	NString		theResult;



	// Get the tag
	theResult.Set(UTTypeCopyPreferredTagWithClass(mValue, kUTTagClassFilenameExtension));
	
	return(theResult);
}





//============================================================================
//		NUTI::GetNSPasteboard : Get the NSPasteboard tag.
//----------------------------------------------------------------------------
NString NUTI::GetNSPasteboard(void) const
{	NString		theResult;



	// Get the tag
	theResult.Set(UTTypeCopyPreferredTagWithClass(mValue, kUTTagClassNSPboardType));
	
	return(theResult);
}





//============================================================================
//		NUTI::GetMIMEType : Get the MIME type tag.
//----------------------------------------------------------------------------
NString NUTI::GetMIMEType(void) const
{	NString		theResult;



	// Get the tag
	theResult.Set(UTTypeCopyPreferredTagWithClass(mValue, kUTTagClassMIMEType));
	
	return(theResult);
}





//============================================================================
//		NUTI::GetDescription : Get the description.
//----------------------------------------------------------------------------
NString NUTI::GetDescription(void) const
{	NString		theResult;



	// Get the description
	theResult.Set(UTTypeCopyDescription(mValue));
	
	return(theResult);
}





//============================================================================
//		NUTI::GetDeclaration : Get the declaration.
//----------------------------------------------------------------------------
NDictionary NUTI::GetDeclaration(void) const
{	NDictionary		theResult;



	// Get the description
	theResult.Set(UTTypeCopyDeclaration(mValue));
	
	return(theResult);
}





//============================================================================
//		NUTI::GetDeclarationURL : Get the declaration URL.
//----------------------------------------------------------------------------
NCFURL NUTI::GetDeclarationURL(void) const
{	NCFURL	theResult;



	// Get the description
	theResult.Set(UTTypeCopyDeclaringBundleURL(mValue));
	
	return(theResult);
}





//============================================================================
//		NUTI::GetValue : Get the value.
//----------------------------------------------------------------------------
CFStringRef NUTI::GetValue(void) const
{


	// Get the value
	return((CFStringRef) mValue);
}





//============================================================================
//		NUTI::operator NString : NString-cast operator.
//----------------------------------------------------------------------------
NUTI::operator NString(void) const
{


	// Get the value
	return(mValue);
}






//============================================================================
//		NUTI::GetTagIdentifiers : Get the UTIs for a tag.
//----------------------------------------------------------------------------
NArray NUTI::GetTagIdentifiers(const NString &tagClass, const NString &theTag, const NString &conformsTo)
{	NArray	theResult;



	// Get the identifiers
	theResult.Set(UTTypeCreateAllIdentifiersForTag(tagClass, theTag, conformsTo));
	
	return(theResult);
}





//============================================================================
//		NUTI::GetOSTypeFromString : Get an OSType from a string.
//----------------------------------------------------------------------------
OSType NUTI::GetOSTypeFromString(const NString &theString)
{	OSType		theResult;



	// Get the value
	theResult = UTGetOSTypeFromString(theString);
	
	return(theResult);
}





//============================================================================
//		NUTI::GetStringFromOSType : Get a string from an OSType.
//----------------------------------------------------------------------------
NString NUTI::GetStringFromOSType(OSType theType)
{	NString		theResult;



	// Get the value
	theResult.Set(UTCreateStringForOSType(theType));
	
	return(theResult);
}





//============================================================================
//		NUTI::Compare : Compare two objects.
//----------------------------------------------------------------------------
#pragma mark -
CFComparisonResult NUTI::Compare(const NComparable &theObject) const
{	const NUTI		*theUTI = dynamic_cast<const NUTI*>(&theObject);



	// Validate our parameters
	NN_ASSERT(theUTI != NULL);



	// Compare the UTIs
	if (UTTypeEqual(mValue, theUTI->mValue))
		return(kCFCompareEqualTo);
	else
		return(mValue.CompareTo(theUTI->mValue));
}







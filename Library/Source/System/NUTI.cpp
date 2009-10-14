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
extern const CFStringRef kUTNTypeColor				= UTTypeCreatePreferredIdentifierForTag(kUTTagClassNSPboardType, (CFStringRef) NSColorPboardType, NULL);
extern const CFStringRef kUTNTypeFileURL			= CFSTR("public.file-url");
extern const CFStringRef kUTNTypeImage				= CFSTR("public.image");
extern const CFStringRef kUTNTypeItem				= CFSTR("public.item");
extern const CFStringRef kUTNTypeJPEG				= CFSTR("public.jpeg");
extern const CFStringRef kUTNTypePICT				= CFSTR("com.apple.pict");
extern const CFStringRef kUTNTypePNG				= CFSTR("public.png");
extern const CFStringRef kUTNTypeRTF				= CFSTR("public.rtf");
extern const CFStringRef kUTNTypeUTF8PlainText		= CFSTR("public.utf8-plain-text");
extern const CFStringRef kUTNTypeUTF16PlainText		= CFSTR("public.utf16-plain-text");





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
	mValue = theUTI;
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
//		NUTI::EqualTo : Is this UTI equal to another?
//----------------------------------------------------------------------------
bool NUTI::EqualTo(const NUTI &theUTI) const
{


	// Check our state
	return(UTTypeEqual(mValue, theUTI.mValue));
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
//		NUTI::== : Are two UTIs equal?
//----------------------------------------------------------------------------
const bool NUTI::operator == (const NUTI &theUTI) const
{


	// Compare the UTIs
	//
	// Note - since we provide an NString operator, overloading == is ambiguous.
	//
	// This method is provided simply to catch these cases, and ensure that any
	// UTI comparisons using == are changed to use EqualTo.
	return(UTTypeEqual(mValue, theUTI.mValue));
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
//		NUTI::operator CFStringRef : CFStringRef-cast operator.
//----------------------------------------------------------------------------
NUTI::operator CFStringRef(void) const
{


	// Get the value
	return((CFStringRef) mValue);
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












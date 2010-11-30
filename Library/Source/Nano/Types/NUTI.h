/*	NAME:
		NUTI.h

	DESCRIPTION:
		Uniform Type Identifier.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NUTI_HDR
#define NUTI_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NStringFormatter.h"
#include "NComparable.h"
#include "NEncodable.h"
#include "NString.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Tag classes
typedef enum {
	kNUTITagClassNone,
	kNUTITagClassMIMEType,
	kNUTITagClassFileExtension,
} NUTITagClass;


// UTIs
//
// Abstract base types
static const NString kNUTTypeNone									= "";
static const NString kNUTTypeItem									= "public.item";
static const NString kNUTTypeContent								= "public.content";


// Concrete base types
static const NString kNUTTypeData									= "public.data";
static const NString kNUTTypeText									= "public.text";


// Image content types
static const NString kNUTTypeImage									= "public.image";
static const NString kNUTTypeGIF									= "public.gif";
static const NString kNUTTypeJPEG									= "public.jpeg";
static const NString kNUTTypePNG									= "public.png";
static const NString kNUTTypeTIFF									= "public.tiff";
static const NString kNUTTypeSVG									= "public.svg";


// XML content types
static const NString kNUTTypeXML									= "public.xml";
static const NString kNUTTypeGPX									= "public.gpx";
static const NString kNUTTypeKML									= "public.kml";


// Misc content types
static const NString kNUTTypeKMZ									= "public.kmz";
static const NString kNUTTypeCSV									= "public.csv";





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Lists
typedef std::map<NUTITagClass, NString>								NUTITagMap;
typedef NUTITagMap::iterator										NUTITagMapIterator;
typedef NUTITagMap::const_iterator									NUTITagMapConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NUTI :	public NEncodable,
				public NComparable<NUTI> {
public:
										NENCODABLE_DECLARE(NUTI);

										NUTI(NUTITagClass theClass, const NString &theTag);
										NUTI(const NString &theUTI=kNUTTypeNone);
	virtual							   ~NUTI(void);


	// Is the UTI valid?
	bool								IsValid(void) const;


	// Does the UTI conform to another?
	bool								ConformsTo(const NUTI &theUTI) const;
	

	// Clear the value
	void								Clear(void);


	// Compare the value
	NComparison							Compare(const NUTI &theValue) const;


	// Get the UTI
	NString								GetUTI(void) const;


	// Get the description
	NString								GetDescription(void) const;


	// Get a tag
	NString								GetTag(NUTITagClass theClass) const;

	NString								GetMIMEType(     void) const;
	NString								GetFileExtension(void) const;


	// Operators
										operator NFormatArgument(void) const;


protected:
	// Encode/decode the object
	void								EncodeSelf(      NEncoder &theEncoder) const;
	void								DecodeSelf(const NEncoder &theEncoder);


private:
	NString								mValue;
};





#endif // NUTI_HDR

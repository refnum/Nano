/*	NAME:
		NUTI.h

	DESCRIPTION:
		Uniform Type Identifier.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NUTI_HDR
#define NUTI_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NComparable.h"
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
static const NString kNUTTypeNone									= kNStringEmpty;
static const NString kNUTTypeItem									= "public.item";
static const NString kNUTTypeContent								= "public.content";


// Concrete base types
static const NString kNUTTypeData									= "public.data";


// Image content types
static const NString kNUTTypeImage									= "public.image";
static const NString kNUTTypeJPEG									= "public.jpeg";





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
class NUTI : public NComparable<NUTI> {
public:
										 NUTI(NUTITagClass theClass, const NString &theTag);
										 NUTI(const NString &theUTI=kNUTTypeNone);
	virtual								~NUTI(void);


	// Is the UTI valid?
	bool								IsValid(void) const;


	// Does the UTI conform to another?
	bool								ConformsTo(const NString &theUTI) const;
	

	// Clear the value
	void								Clear(void);


	// Compare the value
	NComparison							Compare(const NUTI &theValue) const;


	// Get a tag
	NString								GetMIMEType(     void) const;
	NString								GetFileExtension(void) const;


private:
	NString								mUTI;
};





#endif // NUTI_HDR

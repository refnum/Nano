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





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NUTI : public NComparable<NUTI> {
public:
										NUTI(NUTITagClass theClass, const NString &theTag);

										 NUTI(void);
	virtual								~NUTI(void);


	// Is the UTI valid?
	bool								IsValid(void) const;


	// Clear the value
	void								Clear(void);


	// Compare the value
	NComparison							Compare(const NUTI &theValue) const;


	// Get a tag
	NString								GetMIMEType(     void) const;
	NString								GetFileExtension(void) const;


private:
	NUTITagClass						mClass;
	NString								mTag;
};





#endif // NUTI_HDR

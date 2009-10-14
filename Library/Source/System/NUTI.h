/*	NAME:
		NUTI.h

	DESCRIPTION:
		Universal Type Identifier object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NUTI_HDR
#define NUTI_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDictionary.h"
#include "NComparable.h"
#include "NString.h"
#include "NArray.h"
#include "NCFURL.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// UTIs
//
// System UTIs are typically declared in UTCoreTypes.h, however most types
// were only introduced in 10.4.
//
// Unfortunately UTIs are typically declared as global variables rather than
// constants (despite having fixed values for Info.plist usage), and so apps
// that wish to run on older systems are unable to reference these constants.
//
// To address this, we provide our own declarations of common UTIs.
class NUTI;

extern const NUTI kUTNTypeColor;
extern const NUTI kUTNTypeFileURL;
extern const NUTI kUTNTypeImage;
extern const NUTI kUTNTypeItem;
extern const NUTI kUTNTypeJPEG;
extern const NUTI kUTNTypePICT;
extern const NUTI kUTNTypePNG;
extern const NUTI kUTNTypeRTF;
extern const NUTI kUTNTypeUTF8PlainText;
extern const NUTI kUTNTypeUTF16PlainText;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NUTI : public NComparable {
public:
										NUTI(const NString &theUTI);
										NUTI(CFStringRef    theUTI);

										NUTI(const NString &tagClass,
											 const NString &theTag,
											 const NString &conformsTo = "");
										
										NUTI( void);
	virtual								~NUTI(void);


	// Is the UTI valid?
	bool								IsValid(void) const;
	
	
	// Clear the UTI
	void								Clear(void);


	// Does a UTI conform to another?
	bool								ConformsTo(const NUTI &theUTI) const;
	

	// Get a tag
	OSType								GetFileType(     void) const;
	NString								GetFileExtension(void) const;
	NString								GetNSPasteboard(void) const;
	NString								GetMIMEType(    void) const;


	// Get the description
	NString								GetDescription(void) const;


	// Get the declaration
	NDictionary							GetDeclaration(void) const;
	

	// Get the declaration URL
	NCFURL								GetDeclarationURL(void) const;


	// Get the value
	//
	// Returns a CFStringRef for use when a POD is required. Not an operator, to
	// avoid comparisons with system types using a simple pointer comparison.
	CFStringRef							GetValue(void) const;


	// Operators
	operator							NString(void) const;


	// Get the UTIs for a tag
	static NArray						GetTagIdentifiers(const NString &tagClass,
														  const NString &theTag,
														  const NString &confirmsTo = "");


	// Convert a UTI to/from an OSType
	static OSType						GetOSTypeFromString(const NString &theString);
	static NString						GetStringFromOSType(OSType theType);


protected:
	// Compare two objects
	CFComparisonResult					Compare(const NComparable &theObject) const;


private:
	NString								mValue;
};





#endif // NUTI_HDR



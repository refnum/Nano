/*	NAME:
		NUTI.h

	DESCRIPTION:
		Universal Type Identifier object.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
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
// To address this, we provide some identical declarations of common UTIs.
extern const CFStringRef kUTNTypeColor;
extern const CFStringRef kUTNTypeFileURL;
extern const CFStringRef kUTNTypeImage;
extern const CFStringRef kUTNTypeItem;
extern const CFStringRef kUTNTypeJPEG;
extern const CFStringRef kUTNTypePICT;
extern const CFStringRef kUTNTypePNG;
extern const CFStringRef kUTNTypeRTF;
extern const CFStringRef kUTNTypeUTF8PlainText;
extern const CFStringRef kUTNTypeUTF16PlainText;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NUTI {
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


	// Is a UTI equal to another?
	bool								EqualTo(const NUTI &theUTI) const;


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


	// Operators
	const bool							operator == (const NUTI &theUTI) const;

	operator							NString(    void) const;
	operator							CFStringRef(void) const;


	// Get the UTIs for a tag
	static NArray						GetTagIdentifiers(const NString &tagClass,
														  const NString &theTag,
														  const NString &confirmsTo = "");


	// Convert a UTI to/from an OSType
	static OSType						GetOSTypeFromString(const NString &theString);
	static NString						GetStringFromOSType(OSType theType);


private:
	NString								mValue;
};





#endif // NUTI_HDR



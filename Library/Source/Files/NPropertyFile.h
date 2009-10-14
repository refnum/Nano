/*	NAME:
		NPropertyFile.h

	DESCRIPTION:
		Property file.

	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NPROPERTYFILE_HDR
#define NPROPERTYFILE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFile.h"
#include "NDictionary.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NPropertyFile {
public:
										NPropertyFile(void);
	virtual								~NPropertyFile(void);


	// Get/set the format
	CFPropertyListFormat				GetFormat(void) const;
	void								SetFormat(CFPropertyListFormat theFormat);
	

	// Load/save the file
	OSStatus							Load(const NFile &theFile);
	OSStatus							Save(const NFile &theFile);


	// Get the properties
	NDictionary							&GetProperties(void);


private:
	CFPropertyListFormat				mFormat;
	NDictionary							mProperties;
};





#endif // NPROPERTYFILE_HDR

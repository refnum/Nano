/*	NAME:
		NPropertyFile.h

	DESCRIPTION:
		Property file.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
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
#include "NData.h"
#include "NCFStream.h"
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
	

	// Load/save the property list
	OSStatus							Load(const NFile     &theFile);
	OSStatus							Load(const NString   &theText);
	OSStatus							Load(const NData     &theData);
	OSStatus							Load(      NCFStream &theStream);

	OSStatus							Save(const NFile     &theFile);
	OSStatus							Save(      NString   &theText);
	OSStatus							Save(      NData     &theData);
	OSStatus							Save(      NCFStream &theStream);


	// Get/set the properties
	NDictionary							GetProperties(void) const;
	void								SetProperties(const NDictionary &theProperties);


private:
	CFPropertyListFormat				mFormat;
	NDictionary							mProperties;
};





#endif // NPROPERTYFILE_HDR

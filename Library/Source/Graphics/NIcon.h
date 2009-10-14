/*	NAME:
		NIcon.h

	DESCRIPTION:
		IconRef object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NICON_HDR
#define NICON_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFile.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NIcon {
public:
										NIcon(OSType iconCreator, OSType iconType);
										NIcon(IconRef        theIcon);
										NIcon(const NFile   &theFile, OSType iconCreator=0, OSType iconType=0);
										NIcon(const NString &theName, const NString &subDir="", CFBundleRef theBundle=NULL);
										NIcon(const NIcon   &theIcon);
										
										NIcon(void);
	virtual								~NIcon(void);


	// Is the icon valid?
	bool								IsValid(void) const;


	// Clear the icon
	void								Clear(void);
	

	// Operators
	NIcon								&operator = (const NIcon &otherIcon);
	operator							IconRef(void) const;


private:
	void								AcquireIcon(IconRef theIcon);
	void								ReleaseIcon(void);

	OSType								GetNextType(void);


private:
	IconRef								mIcon;
	OSType								mIconCreator;
	OSType								mIconType;
};





#endif // NICON_HDR



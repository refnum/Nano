/*	NAME:
		NAppleScript.h

	DESCRIPTION:
		AppleScript object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NAPPLESCRIPT_HDR
#define NAPPLESCRIPT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NAppleScript {
public:
										NAppleScript(const NString &theText);

										NAppleScript(void);
	virtual								~NAppleScript(void);


	// Is the script valid?
	bool								IsValid(void) const;


	// Load an AppleScript
	OSStatus							Load(const NString &theText);


	// Execute the script
	OSStatus							Execute(void);


private:
	void								InitializeSelf();
	void								Unload(void);


private:
	OSAID								mScript;
	ComponentInstance					mInstance;
};





#endif // NAPPLESCRIPT_HDR



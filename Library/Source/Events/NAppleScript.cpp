/*	NAME:
		NAppleScript.cpp

	DESCRIPTION:
		AppleScript object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NAppleScript.h"





//============================================================================
//		NAppleScript::NAppleScript : Constructor.
//----------------------------------------------------------------------------
NAppleScript::NAppleScript(const NString &theText)
{	OSStatus	theErr;



	// Initialize ourselves
	InitializeSelf();



	// Load the script
	theErr = Load(theText);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NAppleScript::NAppleScript : Constructor.
//----------------------------------------------------------------------------
NAppleScript::NAppleScript(void)
{


	// Initialize ourselves
	InitializeSelf();
}





//============================================================================
//		NAppleScript::~NAppleScript : Destructor.
//----------------------------------------------------------------------------
NAppleScript::~NAppleScript(void)
{


	// Clean up
	Unload();
}





//============================================================================
//		NAppleScript::IsValid : Is the script valid?
//----------------------------------------------------------------------------
bool NAppleScript::IsValid(void) const
{


	// Check our state
	return(mScript != kOSANullScript);
}





//============================================================================
//		NAppleScript::Load : Load a script.
//----------------------------------------------------------------------------
OSStatus NAppleScript::Load(const NString &theText)
{	AEDesc		textDesc;
	OSStatus	theErr;



	// Validate our parameters
	NN_ASSERT(theText.IsNotEmpty());



	// Unload any previous script
	Unload();



	// Initialize ourselves
	mInstance = OpenDefaultComponent(kOSAComponentType, typeAppleScript);
	if (mInstance == NULL)
		return(memFullErr);
	
	theErr = AECreateDesc(typeUTF8Text, theText.GetUTF8(), theText.GetSizeBytes(), &textDesc);
	if (theErr != noErr)
		return(theErr);



	// Compile the script
	theErr = OSACompile(mInstance, &textDesc, kOSAModeNull, &mScript);



	// Clean up
    AEDisposeDesc(&textDesc);
	
	return(theErr);
}





//============================================================================
//		NAppleScript::Execute : Execute the script.
//----------------------------------------------------------------------------
OSStatus NAppleScript::Execute(void) const
{	OSAID		theResult;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(mScript   != kOSANullScript);
	NN_ASSERT(mInstance != NULL);



	// Execute the script
	theResult = kOSANullScript;
	theErr    = OSAExecute(mInstance, mScript, kOSANullScript, kOSAModeNull, &theResult);



	// Clean up
	if (theResult != kOSANullScript)
		OSADispose(mInstance, theResult);
	
	return(theErr);
}





//============================================================================
//		NAppleScript::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
void NAppleScript::InitializeSelf(void)
{


	// Initialize ourselves
	mScript   = kOSANullScript;
	mInstance = NULL;
}





//============================================================================
//		NAppleScript::Unload : Unload the script.
//----------------------------------------------------------------------------
void NAppleScript::Unload(void)
{


	// Clean up
	if (mScript != kOSANullScript)
		{
		NN_ASSERT(mInstance != NULL);
		OSADispose(mInstance, mScript);
		}

	if (mInstance != NULL)
		CloseComponent(mInstance);



	// Reset our state
	mScript   = kOSANullScript;
	mInstance = NULL;
}




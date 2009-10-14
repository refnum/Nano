/*  NAME:
        NKeyboardUtilities.h

    DESCRIPTION:
        Keyboard utilities.
    
    COPYRIGHT:
        Copyright (c) 2006-2007, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
    __________________________________________________________________________
*/
#ifndef NKEYBOARDUTILITIES_HDR
#define NKEYBOARDUTILITIES_HDR
//============================================================================
//      Include files
//----------------------------------------------------------------------------





//============================================================================
//      Constants
//----------------------------------------------------------------------------
static const UInt32 kKeyCodeBackscape                               = 0x33;
static const UInt32 kKeyCodeDelete                                  = 0x75;
static const UInt32 kKeyCodeEnter                                   = 0x34;
static const UInt32 kKeyCodeNumPad_Enter                            = 0x4C;
static const UInt32 kKeyCodeReturn                                  = 0x24;





//============================================================================
//      Class declaration
//----------------------------------------------------------------------------
class NKeyboardUtilities {
public:
	// Get the modifiers
	//
	// Polling the hardware is expensive, and so should only be performed when
	// the modifier state of the current event loop is not sufficient (e.g., if
	// the app is currently in the background).
	static UInt32						GetModifiers(bool pollHardware=false);


	// Check the keyboard state
	static bool							IsCapsLockDown(bool pollHardware=false);
	static bool							IsCommandDown( bool pollHardware=false);
	static bool							IsControlDown( bool pollHardware=false);
	static bool							IsOptionDown(  bool pollHardware=false);
	static bool							IsShiftDown(   bool pollHardware=false);
};

    
    


#endif // NKEYBOARDUTILITIES_HDR




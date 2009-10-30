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
// Key codes
//
// These codes are physical scan codes for a Mac extended keyboard.
static const UInt32 kKeyCodeA										= 0x00;
static const UInt32 kKeyCodeS										= 0x01;
static const UInt32 kKeyCodeD										= 0x02;
static const UInt32 kKeyCodeF										= 0x03;
static const UInt32 kKeyCodeH										= 0x04;
static const UInt32 kKeyCodeG										= 0x05;
static const UInt32 kKeyCodeZ										= 0x06;
static const UInt32 kKeyCodeX										= 0x07;
static const UInt32 kKeyCodeC										= 0x08;
static const UInt32 kKeyCodeV										= 0x09;
static const UInt32 kKeyCodePlusMinus								= 0x0A;
static const UInt32 kKeyCodeB										= 0x0B;
static const UInt32 kKeyCodeQ										= 0x0C;
static const UInt32 kKeyCodeW										= 0x0D;
static const UInt32 kKeyCodeE										= 0x0E;
static const UInt32 kKeyCodeR										= 0x0F;
static const UInt32 kKeyCodeY										= 0x10;
static const UInt32 kKeyCodeT										= 0x11;
static const UInt32 kKeyCode1										= 0x12;
static const UInt32 kKeyCode2										= 0x13;
static const UInt32 kKeyCode3										= 0x14;
static const UInt32 kKeyCode4										= 0x15;
static const UInt32 kKeyCode6										= 0x16;
static const UInt32 kKeyCode5										= 0x17;
static const UInt32 kKeyCodeEqual									= 0x18;
static const UInt32 kKeyCode9										= 0x19;
static const UInt32 kKeyCode7										= 0x1A;
static const UInt32 kKeyCodeMinus									= 0x1B;
static const UInt32 kKeyCode8										= 0x1C;
static const UInt32 kKeyCode0										= 0x1D;
static const UInt32 kKeyCodeRightSquareBracket						= 0x1E;
static const UInt32 kKeyCodeO										= 0x1F;
static const UInt32 kKeyCodeU										= 0x20;
static const UInt32 kKeyCodeLeftSquareBracket						= 0x21;
static const UInt32 kKeyCodeI										= 0x22;
static const UInt32 kKeyCodeP										= 0x23;
static const UInt32 kKeyCodeReturn									= 0x24;
static const UInt32 kKeyCodeL										= 0x25;
static const UInt32 kKeyCodeJ										= 0x26;
static const UInt32 kKeyCodeApostrophe								= 0x27;
static const UInt32 kKeyCodeK										= 0x28;
static const UInt32 kKeyCodeSemiColon								= 0x29;
static const UInt32 kKeyCodeBackslash								= 0x2A;
static const UInt32 kKeyCodeComma									= 0x2B;
static const UInt32 kKeyCodeSlash									= 0x2C;
static const UInt32 kKeyCodeN										= 0x2D;
static const UInt32 kKeyCodeM										= 0x2E;
static const UInt32 kKeyCodePeriod									= 0x2F;
static const UInt32 kKeyCodeTab										= 0x30;
static const UInt32 kKeyCodeSpace									= 0x31;
static const UInt32 kKeyCodeGrave									= 0x32;   
static const UInt32 kKeyCodeBackspace								= 0x33;
static const UInt32 kKeyCodeEnter									= 0x34;
static const UInt32 kKeyCodeEscape									= 0x35;
static const UInt32 kKeyCodeInvalid_36								= 0x36;
static const UInt32 kKeyCodeCommand									= 0x37;   
static const UInt32 kKeyCodeLeftShift								= 0x38;
static const UInt32 kKeyCodeCapsLock								= 0x39;
static const UInt32 kKeyCodeLeftOption								= 0x3A;
static const UInt32 kKeyCodeLeftControl								= 0x3B;
static const UInt32 kKeyCodeRightShift								= 0x3C;
static const UInt32 kKeyCodeRightOption								= 0x3D;
static const UInt32 kKeyCodeRightControl							= 0x3E;
static const UInt32 kKeyCodeInvalid_3F								= 0x3F;
static const UInt32 kKeyCodeInvalid_40								= 0x40;
static const UInt32 kKeyCodeNumPadPeriod							= 0x41;
static const UInt32 kKeyCodeInvalid_42								= 0x42;
static const UInt32 kKeyCodeNumPadMultiply							= 0x43;
static const UInt32 kKeyCodeInvalid_44								= 0x44;
static const UInt32 kKeyCodeNumPadPlus								= 0x45;
static const UInt32 kKeyCodeInvalid_46								= 0x46;
static const UInt32 kKeyCodeNumLock									= 0x47;
static const UInt32 kKeyCodeInvalid_48								= 0x48;
static const UInt32 kKeyCodeInvalid_49								= 0x49;
static const UInt32 kKeyCodeInvalid_4A								= 0x4A;
static const UInt32 kKeyCodeNumPadDivide							= 0x4B;
static const UInt32 kKeyCodeNumPadEnter								= 0x4C;
static const UInt32 kKeyCodeInvalid_4D								= 0x4D;
static const UInt32 kKeyCodeNumPadMinus								= 0x4E;
static const UInt32 kKeyCodeInvalid_4F								= 0x4F;
static const UInt32 kKeyCodeInvalid_50								= 0x50;
static const UInt32 kKeyCodeNumPadEqual								= 0x51;
static const UInt32 kKeyCodeNumPad0									= 0x52;
static const UInt32 kKeyCodeNumPad1									= 0x53;
static const UInt32 kKeyCodeNumPad2									= 0x54;
static const UInt32 kKeyCodeNumPad3									= 0x55;
static const UInt32 kKeyCodeNumPad4									= 0x56;
static const UInt32 kKeyCodeNumPad5									= 0x57;
static const UInt32 kKeyCodeNumPad6									= 0x58;
static const UInt32 kKeyCodeNumPad7									= 0x59;
static const UInt32 kKeyCodeInvalid_5A								= 0x5A;
static const UInt32 kKeyCodeNumPad8									= 0x5B;
static const UInt32 kKeyCodeNumPad9									= 0x5C;
static const UInt32 kKeyCodeInvalid_5D								= 0x5D;
static const UInt32 kKeyCodeInvalid_5E								= 0x5E;
static const UInt32 kKeyCodeInvalid_5F								= 0x5F;
static const UInt32 kKeyCodeF5										= 0x60;
static const UInt32 kKeyCodeF6										= 0x61;
static const UInt32 kKeyCodeF7										= 0x62;
static const UInt32 kKeyCodeF3										= 0x63;
static const UInt32 kKeyCodeF8										= 0x64;
static const UInt32 kKeyCodeF9										= 0x65;
static const UInt32 kKeyCodeInvalid_66								= 0x66;
static const UInt32 kKeyCodeF11										= 0x67;
static const UInt32 kKeyCodeInvalid_68								= 0x68;
static const UInt32 kKeyCodeF13										= 0x69;
static const UInt32 kKeyCodeF16										= 0x6A;
static const UInt32 kKeyCodeF14										= 0x6B;
static const UInt32 kKeyCodeInvalid_6C								= 0x6C;
static const UInt32 kKeyCodeF10										= 0x6D;
static const UInt32 kKeyCodeInvalid_6E								= 0x6E;
static const UInt32 kKeyCodeF12										= 0x6F;
static const UInt32 kKeyCodeInvalid_70								= 0x70;
static const UInt32 kKeyCodeF15										= 0x71;
static const UInt32 kKeyCodeHelp									= 0x72;
static const UInt32 kKeyCodeHome									= 0x73;
static const UInt32 kKeyCodePageUp									= 0x74;
static const UInt32 kKeyCodeDelete                                  = 0x75;
static const UInt32 kKeyCodeF4										= 0x76;
static const UInt32 kKeyCodeEnd										= 0x77;
static const UInt32 kKeyCodeF2										= 0x78;
static const UInt32 kKeyCodePageDown								= 0x79;
static const UInt32 kKeyCodeF1										= 0x7A;
static const UInt32 kKeyCodeArrowLeft								= 0x7B;
static const UInt32 kKeyCodeArrowRight								= 0x7C;
static const UInt32 kKeyCodeArrowDown								= 0x7D;
static const UInt32 kKeyCodeArrowUp									= 0x7E;





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




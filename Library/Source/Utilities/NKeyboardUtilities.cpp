/*  NAME:
        NKeyboardUtilities.cpp

    DESCRIPTION:
        Keyboard utilities.
    
    COPYRIGHT:
        Copyright (c) 2006-2007, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
    __________________________________________________________________________
*/
//============================================================================
//      Include files
//----------------------------------------------------------------------------
#include "NKeyboardUtilities.h"





//============================================================================
//      NKeyboardUtilities::GetModifiers : Get the modifiers.
//----------------------------------------------------------------------------
UInt32 NKeyboardUtilities::GetModifiers(bool pollHardware)
{


	// Get the modifiers
	if (pollHardware)
		return(GetCurrentKeyModifiers());
	else
		return(GetCurrentEventKeyModifiers());
}





//============================================================================
//      NKeyboardUtilities::IsCapsLockDown : Is the caps-lock key down?
//----------------------------------------------------------------------------
bool NKeyboardUtilities::IsCapsLockDown(bool pollHardware)
{


	// Check the modifiers
	return((GetModifiers(pollHardware) & alphaLock) != 0);
}





//============================================================================
//      NKeyboardUtilities::IsCommandDown : Is a command key key down?
//----------------------------------------------------------------------------
bool NKeyboardUtilities::IsCommandDown(bool pollHardware)
{


	// Check the modifiers
	return((GetModifiers(pollHardware) & cmdKey) != 0);
}





//============================================================================
//      NKeyboardUtilities::IsControlDown : Is a control key key down?
//----------------------------------------------------------------------------
bool NKeyboardUtilities::IsControlDown(bool pollHardware)
{


	// Check the modifiers
	return((GetModifiers(pollHardware) & (controlKey | rightControlKey)) != 0);
}





//============================================================================
//      NKeyboardUtilities::IsOptionDown : Is an option key key down?
//----------------------------------------------------------------------------
bool NKeyboardUtilities::IsOptionDown(bool pollHardware)
{


	// Check the modifiers
	return((GetModifiers(pollHardware) & (optionKey | rightOptionKey)) != 0);
}





//============================================================================
//      NKeyboardUtilities::IsShiftDown : Is a shift key key down?
//----------------------------------------------------------------------------
bool NKeyboardUtilities::IsShiftDown(bool pollHardware)
{


	// Check the modifiers
	return((GetModifiers(pollHardware) & (shiftKey | rightShiftKey)) != 0);
}





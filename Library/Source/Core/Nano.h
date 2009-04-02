/*	NAME:
		Nano.h

	DESCRIPTION:
		Nano Library.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NANO_HDR
#define NANO_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
// Bootstrap
#include "NanoTarget.h"
#include "NanoNotes.h"


// Mac
#if NN_TARGET_MAC
	#include <Carbon/Carbon.h>
	#if defined(__OBJC__)
		#include <Cocoa/Cocoa.h>
	#endif
#endif


// Windows
#if NN_TARGET_WIN
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#include <Windows.h>
#endif


// Nano
#if defined(__cplusplus)
	#include "NanoTypes.h"
	#include "NDebug.h"
#endif




#endif // NANO_HDR



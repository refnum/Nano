/*	NAME:
		Nano.h

	DESCRIPTION:
		Nano header.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
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
#include "Nano 64.h"
#include "Nano Notes.h"


// Apple
#include <Carbon/Carbon.h>
#include <QuickTime/QuickTime.h>

#ifdef __OBJC__
	#include <Cocoa/Cocoa.h>
#endif


// Nano
#ifdef __cplusplus
	#include "NDebug.h"
#endif





//============================================================================
//		Build constants
//----------------------------------------------------------------------------
// SDK constants
#define NN_SDK_VERSION_10_4											0x00001040
#define NN_SDK_VERSION_10_5											0x00001050


// SDK version
//
// The Mac OS X SDK system does not provide a single constant that can be used
// to conditionalise SDK-specific behaviour (e.g., providing declarations that
// are supported on older systems but never documented).
//
// By examining some of the macros defined by the umbrella frameworks, we can
// determine the current SDK version to conditionalise our behaviour.
#ifdef __HIACCESSIBILITY__
	#define NN_SDK_VERSION											NN_SDK_VERSION_10_5
#else
	#define NN_SDK_VERSION											NN_SDK_VERSION_10_4
#endif




#endif // NANO_HDR



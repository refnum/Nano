/*	NAME:
		Nano.h

	DESCRIPTION:
		Nano Library.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
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
#if NN_TARGET_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	
	#ifndef UNICODE
	#define UNICODE
	#endif
	
	#ifndef _UNICODE
	#define _UNICODE
	#endif

	#include <Windows.h>
#endif


// iOS
#if NN_TARGET_IOS
	#include <CoreFoundation/CoreFoundation.h>
	#include <CoreGraphics/CoreGraphics.h>
	#include <QuartzCore/QuartzCore.h>

	#if defined(__OBJC__)
		#include <Foundation/Foundation.h>
		#include <UIKit/UIKit.h>
	#endif
#endif


// Nano
#include "NanoTypes.h"
#include "NanoMacros.h"
#include "NDebug.h"




#endif // NANO_HDR



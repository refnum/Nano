/*	NAME:
		Nano 64.h

	DESCRIPTION:
		Nano 64-bit compatibility header.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NANO_64_HDR
#define NANO_64_HDR
//============================================================================
//		Compatibility
//----------------------------------------------------------------------------
// Although Xcode 2.4 supports 64-bit x86 builds, these can not be deployed
// prior to the official 10.5/Xcode 3.0 release.
//
// Since 64-bit builds emit useful additional warnings to help prepare code
// for the transition, this header defines some temporary workarounds that
// allow 64-bit builds to be performed with the 10.4u SDK.
//
// If you wish to perform preliminary 64-bit builds of your code, you should
// include this header prior to including Carbon/Carbon.h.
//
// Once Xcode 3.0 is available, this header will be removed.
#ifdef __x86_64__
	#define SIGDIGLEN 36  

	typedef struct ExceptionInformation {
		int foo;
	} ExceptionInformation;
#endif





#endif // NANO_64_HDR



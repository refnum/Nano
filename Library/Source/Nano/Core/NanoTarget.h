/*	NAME:
		NanoTarget.h

	DESCRIPTION:
		Nano build targets.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NANO_TARGET_HDR
#define NANO_TARGET_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Target
//----------------------------------------------------------------------------
// Baseline
#define NN_TARGET_MAC												0
#define NN_TARGET_WINDOWS											0
#define NN_TARGET_IOS											0


// Mac
#if (defined(__APPLE_CPP__) || defined(__APPLE_CC__)) && !defined(__arm__) && !defined(__IPHONE_OS_VERSION_MIN_REQUIRED)
	#undef  NN_TARGET_MAC
	#define NN_TARGET_MAC											1
#endif


// Windows
#if defined(_MSC_VER)
	#undef  NN_TARGET_WINDOWS
	#define NN_TARGET_WINDOWS										1
#endif


// iOS
#if (defined(__APPLE_CPP__) || defined(__APPLE_CC__)) && (defined(__arm__) || defined(__IPHONE_OS_VERSION_MIN_REQUIRED))
	#undef  NN_TARGET_IOS
	#define NN_TARGET_IOS										1
#endif


// Validate
#if !NN_TARGET_MAC && !NN_TARGET_WINDOWS && !NN_TARGET_IOS
	ERROR - Unable to determine target platform
#endif





//============================================================================
//		Endian-ness
//----------------------------------------------------------------------------
// Baseline
#define NN_TARGET_ENDIAN_BIG										0
#define NN_TARGET_ENDIAN_LITTLE										0


// Mac
#if NN_TARGET_MAC
	#if defined(__ppc__) || defined(__ppc64__)
		#undef  NN_TARGET_ENDIAN_BIG
		#define NN_TARGET_ENDIAN_BIG								1
	#else
		#undef  NN_TARGET_ENDIAN_LITTLE
		#define NN_TARGET_ENDIAN_LITTLE								1
	#endif
#endif


// Windows
#if NN_TARGET_WINDOWS
	#undef  NN_TARGET_ENDIAN_LITTLE
	#define NN_TARGET_ENDIAN_LITTLE									1
#endif


// Windows
#if NN_TARGET_IOS
	#undef  NN_TARGET_ENDIAN_LITTLE
	#define NN_TARGET_ENDIAN_LITTLE									1
#endif


// Validate
#if NN_TARGET_ENDIAN_BIG == NN_TARGET_ENDIAN_LITTLE
	ERROR - Unable to determine target endian-ness
#endif





//============================================================================
//		Architecture size
//----------------------------------------------------------------------------
// Baseline
#define NN_TARGET_ARCH_64											0
#define NN_TARGET_ARCH_32											0


// Mac
#if NN_TARGET_MAC
	#if defined(__x86_64__) || defined(__ppc64__)
		#undef  NN_TARGET_ARCH_64
		#define NN_TARGET_ARCH_64									1
	#else
		#undef  NN_TARGET_ARCH_32
		#define NN_TARGET_ARCH_32									1
	#endif
#endif


// Windows
#if NN_TARGET_WINDOWS
	#if defined(_WIN64)
		#undef  NN_TARGET_ARCH_64
		#define NN_TARGET_ARCH_64									1
	#else
		#undef  NN_TARGET_ARCH_32
		#define NN_TARGET_ARCH_32									1
	#endif
#endif


// iOS
#if NN_TARGET_IOS
	#undef  NN_TARGET_ARCH_32
	#define NN_TARGET_ARCH_32										1
#endif


// Validate
#if NN_TARGET_ARCH_64 == NN_TARGET_ARCH_32
	ERROR - Unable to determine target architecture size
#endif




#endif // NANO_TARGET_HDR



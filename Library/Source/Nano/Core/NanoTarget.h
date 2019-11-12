/*	NAME:
		NanoTarget.h

	DESCRIPTION:
		Nano build targets.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
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
#define NN_TARGET_IOS										0
#define NN_TARGET_LINUX										0
#define NN_TARGET_MACOS										0
#define NN_TARGET_TVOS										0
#define NN_TARGET_WINDOWS									0

// iOS
#if (defined(__APPLE_CPP__) || defined(__APPLE_CC__)) && (defined(__arm__) || defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__))
	#undef  NN_TARGET_IOS
	#define NN_TARGET_IOS									1
#endif


// Linux
#if defined(__linux__)
	#undef  NN_TARGET_LINUX
	#define NN_TARGET_LINUX									1
#endif


// macOS
#if (defined(__APPLE_CPP__) || defined(__APPLE_CC__)) && !defined(__arm__) && !defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__)
	#undef  NN_TARGET_MACOS
	#define NN_TARGET_MACOS									1
#endif


// tvOS
#if (defined(__APPLE_CPP__) || defined(__APPLE_CC__)) && defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__) && !defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__)
	#undef  NN_TARGET_TVOS
	#define NN_TARGET_TVOS									1
#endif


// Windows
#if defined(_MSC_FULL_VER)
	#undef  NN_TARGET_WINDOWS
	#define NN_TARGET_WINDOWS								1
#endif


// Validate
#if ((NN_TARGET_IOS + NN_TARGET_LINUX + NN_TARGET_MACOS + NN_TARGET_TVOS + NN_TARGET_WINDOWS) != 1)
	#error Unable to identify platform
#endif





//============================================================================
//		Endian-ness
//----------------------------------------------------------------------------
// Baseline
#define NN_ENDIAN_BIG										0
#define NN_ENDIAN_LITTLE									0


// iOS
#if NN_TARGET_IOS
	#undef  NN_ENDIAN_LITTLE
	#define NN_ENDIAN_LITTLE								1
#endif


// Linux
#if NN_TARGET_LINUX
	#if defined(__ppc__) || defined(__ppc64__)
		#undef  NN_ENDIAN_BIG
		#define NN_ENDIAN_BIG								1
	#else
		#undef  NN_ENDIAN_LITTLE
		#define NN_ENDIAN_LITTLE							1
	#endif
#endif


// macOS
#if NN_TARGET_MACOS
	#if defined(__ppc__) || defined(__ppc64__)
		#undef  NN_ENDIAN_BIG
		#define NN_ENDIAN_BIG								1
	#else
		#undef  NN_ENDIAN_LITTLE
		#define NN_ENDIAN_LITTLE							1
	#endif
#endif


// tvOS
#if NN_TARGET_IOS
	#undef  NN_ENDIAN_LITTLE
	#define NN_ENDIAN_LITTLE								1
#endif


// Windows
#if NN_TARGET_WINDOWS
	#undef  NN_ENDIAN_LITTLE
	#define NN_ENDIAN_LITTLE								1
#endif


// Validate
#if ((NN_ENDIAN_BIG + NN_ENDIAN_LITTLE) != 1)
	#error Unable to identify endian-ness
#endif





//============================================================================
//		Architecture size
//----------------------------------------------------------------------------
// Baseline
#define NN_ARCH_64											0
#define NN_ARCH_32											0


// iOS
#if NN_TARGET_IOS
	#if defined(__x86_64__) || defined(__aarch64__)
		#undef  NN_ARCH_64
		#define NN_ARCH_64									1
	#else
		#undef  NN_ARCH_32
		#define NN_ARCH_32									1
	#endif
#endif



// Linux
#if NN_TARGET_LINUX
	#if defined(__x86_64__) || defined(__amd64__)
		#undef  NN_ARCH_64
		#define NN_ARCH_64									1
	#else
		#undef  NN_ARCH_32
		#define NN_ARCH_32									1
	#endif
#endif


// macOS
#if NN_TARGET_MACOS
	#if defined(__x86_64__) || defined(__ppc64__)
		#undef  NN_ARCH_64
		#define NN_ARCH_64									1
	#else
		#undef  NN_ARCH_32
		#define NN_ARCH_32									1
	#endif
#endif


// tvOS
#if NN_TARGET_TVOS
	#if defined(__x86_64__) || defined(__aarch64__)
		#undef  NN_ARCH_64
		#define NN_ARCH_64									1
	#else
		#undef  NN_ARCH_32
		#define NN_ARCH_32									1
	#endif
#endif


// Windows
#if NN_TARGET_WINDOWS
	#if defined(_WIN64)
		#undef  NN_ARCH_64
		#define NN_ARCH_64									1
	#else
		#undef  NN_ARCH_32
		#define NN_ARCH_32									1
	#endif
#endif


// Validate
#if ((NN_ARCH_32 + NN_ARCH_64) != 1)
	#error Unable to identify architecture size
#endif





//============================================================================
//		Compiler
//----------------------------------------------------------------------------
// Baseline
#define NN_COMPILER_CLANG							0
#define NN_COMPILER_GCC								0
#define NN_COMPILER_MSC								0


// Clang
#if defined(__clang__)
	#undef  NN_COMPILER_CLANG
	#define NN_COMPILER_CLANG						((__clang_major__      * 10000 ) + \
													 (__clang_minor__      * 100   ) + \
													 (__clang_patchlevel__ * 1))
#endif


// GCC
//
// Clang also defines __GNUC__
#if defined(__GNUC__) && !defined(__clang__)
	#undef  NN_COMPILER_GCC
	#define NN_COMPILER_GCC							((__GNUC__            * 10000) + \
													 (__GNUC_MINOR__      * 100)   + \
													 (__GNUC_PATCHLEVEL__ * 1))
#endif


// Visual Studio
#if defined(_MSC_FULL_VER)
	#undef  NN_COMPILER_MSC
	#define NN_COMPILER_MSC							_MSC_FULL_VER
#endif


// Validate
#if ( ((NN_COMPILER_CLANG != 0) + (NN_COMPILER_GCC != 0) + (NN_COMPILER_MSC != 0)) != 1)
	#error Unable to identify compiler
#endif





#endif // NANO_TARGET_HDR



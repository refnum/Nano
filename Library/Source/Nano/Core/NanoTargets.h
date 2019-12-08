/*	NAME:
		NanoTargets.h

	DESCRIPTION:
		Nano build targets.

	COPYRIGHT:
		Copyright (c) 2006-2019, refNum Software
		All rights reserved.

		Redistribution and use in source and binary forms, with or without
		modification, are permitted provided that the following conditions
		are met:
		
		1. Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.
		
		2. Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.
		
		3. Neither the name of the copyright holder nor the names of its
		contributors may be used to endorse or promote products derived from
		this software without specific prior written permission.
		
		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
		"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
		LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
		A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
		HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
		DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
		THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
		(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	___________________________________________________________________________
*/
#ifndef NANO_TARGETS_H
#define NANO_TARGETS_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------





//=============================================================================
//		Target
//-----------------------------------------------------------------------------
// Baseline
#define NN_TARGET_ANDROID                                   0
#define NN_TARGET_IOS                                       0
#define NN_TARGET_LINUX                                     0
#define NN_TARGET_MACOS                                     0
#define NN_TARGET_TVOS                                      0
#define NN_TARGET_WINDOWS                                   0


// Android
#if defined(__ANDROID__)
	#undef  NN_TARGET_ANDROID
	#define NN_TARGET_ANDROID                               1
#endif


// iOS
#if (defined(__APPLE_CPP__) || defined(__APPLE_CC__)) &&    \
	(defined(__arm__) || defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__))
	#undef  NN_TARGET_IOS
	#define NN_TARGET_IOS                                   1
#endif


// Linux
#if defined(__linux__) && !defined(__ANDROID__)
	#undef  NN_TARGET_LINUX
	#define NN_TARGET_LINUX                                 1
#endif


// macOS
#if (defined(__APPLE_CPP__) || defined(__APPLE_CC__)) && !defined(__arm__) &&   \
	!defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__) &&                     \
	!defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__)
	#undef  NN_TARGET_MACOS
	#define NN_TARGET_MACOS                                 1
#endif


// tvOS
#if (defined(__APPLE_CPP__) || defined(__APPLE_CC__)) &&    \
	defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__) &&  \
	!defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__)
	#undef  NN_TARGET_TVOS
	#define NN_TARGET_TVOS                                  1
#endif


// Windows
#if defined(_MSC_FULL_VER)
	#undef  NN_TARGET_WINDOWS
	#define NN_TARGET_WINDOWS                               1
#endif


// Validate
#if ((NN_TARGET_ANDROID + NN_TARGET_IOS + NN_TARGET_LINUX + NN_TARGET_MACOS + NN_TARGET_TVOS +  \
	  NN_TARGET_WINDOWS) != 1)
	#error Unable to identify platform
#endif





//=============================================================================
//		Architecture endian-ness
//-----------------------------------------------------------------------------
// Baseline
#define NN_ENDIAN_BIG                                       0
#define NN_ENDIAN_LITTLE                                    0


// Android
#if NN_TARGET_ANDROID
	#undef  NN_ENDIAN_LITTLE
	#define NN_ENDIAN_LITTLE                                1
#endif


// iOS
#if NN_TARGET_IOS
	#undef  NN_ENDIAN_LITTLE
	#define NN_ENDIAN_LITTLE                                1
#endif


// Linux
#if NN_TARGET_LINUX
	#if defined(__ppc__) || defined(__ppc64__)
		#undef  NN_ENDIAN_BIG
		#define NN_ENDIAN_BIG                               1
	#else
		#undef  NN_ENDIAN_LITTLE
		#define NN_ENDIAN_LITTLE                            1
	#endif
#endif


// macOS
#if NN_TARGET_MACOS
	#if defined(__ppc__) || defined(__ppc64__)
		#undef  NN_ENDIAN_BIG
		#define NN_ENDIAN_BIG                               1
	#else
		#undef  NN_ENDIAN_LITTLE
		#define NN_ENDIAN_LITTLE                            1
	#endif
#endif


// tvOS
#if NN_TARGET_TVOS
	#undef  NN_ENDIAN_LITTLE
	#define NN_ENDIAN_LITTLE                                1
#endif


// Windows
#if NN_TARGET_WINDOWS
	#undef  NN_ENDIAN_LITTLE
	#define NN_ENDIAN_LITTLE                                1
#endif


// Validate
#if ((NN_ENDIAN_BIG + NN_ENDIAN_LITTLE) != 1)
	#error Unable to identify endian-ness
#endif





//=============================================================================
//		Architecture type
//-----------------------------------------------------------------------------
// Baseline
#define NN_ARCH_ARM                                         0
#define NN_ARCH_X86                                         0


// Android
#if NN_TARGET_ANDROID
	#if defined(__arm__) || defined(__aarch64__)
		#undef  NN_ARCH_ARM
		#define NN_ARCH_ARM                                 1

	#elif defined(__i386__) || defined(__x86_64__)
		#undef  NN_ARCH_X86
		#define NN_ARCH_X86                                 1
	#endif
#endif


// iOS
#if NN_TARGET_IOS
	#if defined(__arm__) || defined(__arm64__)
		#undef  NN_ARCH_ARM
		#define NN_ARCH_ARM                                 1
	#elif defined(__i386__) || defined(__x86_64__)
		#undef  NN_ARCH_X86
		#define NN_ARCH_X86                                 1
	#endif
#endif


// Linux
#if NN_TARGET_LINUX
	#if defined(__arm__) || defined(__aarch64__)
		#undef  NN_ARCH_ARM
		#define NN_ARCH_ARM                                 1

	#elif defined(__i386__) || defined(__x86_64__)
		#undef  NN_ARCH_X86
		#define NN_ARCH_X86                                 1
	#endif
#endif


// macOS
#if NN_TARGET_MACOS
	#undef  NN_ARCH_X86
	#define NN_ARCH_X86                                     1
#endif


// tvOS
#if NN_TARGET_TVOS
	#if defined(__arm__) || defined(__arm64__)
		#undef  NN_ARCH_ARM
		#define NN_ARCH_ARM                                 1
	#elif defined(__i386__) || defined(__x86_64__)
		#undef  NN_ARCH_X86
		#define NN_ARCH_X86                                 1
	#endif
#endif


// Windows
#if NN_TARGET_WINDOWS
	#undef  NN_ARCH_X86
	#define NN_ARCH_X86                                     1
#endif


// Validate
#if ((NN_ARCH_ARM + NN_ARCH_X86) != 1)
	#error Unable to identify architecture type
#endif





//=============================================================================
//		Architecture size
//-----------------------------------------------------------------------------
// Baseline
#define NN_ARCH_64                                          0
#define NN_ARCH_32                                          0


// Android
#if NN_TARGET_ANDROID
	#if defined(__aarch64__) || defined(__x86_64__)
		#undef  NN_ARCH_64
		#define NN_ARCH_64                                  1
	#else
		#undef  NN_ARCH_32
		#define NN_ARCH_32                                  1
	#endif
#endif



// iOS
#if NN_TARGET_IOS
	#if defined(__arm64__) || defined(__x86_64__)
		#undef  NN_ARCH_64
		#define NN_ARCH_64                                  1
	#else
		#undef  NN_ARCH_32
		#define NN_ARCH_32                                  1
	#endif
#endif



// Linux
#if NN_TARGET_LINUX
	#if defined(__aarch64__) || defined(__amd64__) || defined(__x86_64__)
		#undef  NN_ARCH_64
		#define NN_ARCH_64                                  1
	#else
		#undef  NN_ARCH_32
		#define NN_ARCH_32                                  1
	#endif
#endif


// macOS
#if NN_TARGET_MACOS
	#if defined(__x86_64__) || defined(__ppc64__)
		#undef  NN_ARCH_64
		#define NN_ARCH_64                                  1
	#else
		#undef  NN_ARCH_32
		#define NN_ARCH_32                                  1
	#endif
#endif


// tvOS
#if NN_TARGET_TVOS
	#if defined(__arm64__) || defined(__x86_64__)
		#undef  NN_ARCH_64
		#define NN_ARCH_64                                  1
	#else
		#undef  NN_ARCH_32
		#define NN_ARCH_32                                  1
	#endif
#endif


// Windows
#if NN_TARGET_WINDOWS
	#if defined(_WIN64)
		#undef  NN_ARCH_64
		#define NN_ARCH_64                                  1
	#else
		#undef  NN_ARCH_32
		#define NN_ARCH_32                                  1
	#endif
#endif


// Validate
#if ((NN_ARCH_32 + NN_ARCH_64) != 1)
	#error Unable to identify architecture size
#endif





//=============================================================================
//		Compiler
//-----------------------------------------------------------------------------
// Baseline
#define NN_COMPILER_CLANG                                   0
#define NN_COMPILER_GCC                                     0
#define NN_COMPILER_MSVC                                    0


// Clang
#if defined(__clang__)
	#undef  NN_COMPILER_CLANG
	#define NN_COMPILER_CLANG                               \
		((__clang_major__ * 10000) + (__clang_minor__ * 100) + (__clang_patchlevel__ * 1))
#endif


// GCC
//
// Clang also defines __GNUC__
#if defined(__GNUC__) && !defined(__clang__)
	#undef  NN_COMPILER_GCC
	#define NN_COMPILER_GCC                                 \
		((__GNUC__ * 10000) + (__GNUC_MINOR__ * 100) + (__GNUC_PATCHLEVEL__ * 1))
#endif


// Visual Studio
#if defined(_MSC_FULL_VER)
	#undef  NN_COMPILER_MSVC
	#define NN_COMPILER_MSVC                                _MSC_FULL_VER
#endif


// Validate
#if (((NN_COMPILER_CLANG != 0) + (NN_COMPILER_GCC != 0) + (NN_COMPILER_MSVC != 0)) != 1)
	#error Unable to identify compiler
#endif





//=============================================================================
//		Platform
//-----------------------------------------------------------------------------
// User Experience
#define NN_PLATFORM_CONSOLE                                 (NN_TARGET_TVOS)
#define NN_PLATFORM_DESKTOP                                 (NN_TARGET_LINUX || NN_TARGET_MACOS || NN_TARGET_WINDOWS)
#define NN_PLATFORM_MOBILE                                  (NN_TARGET_ANDROID | NN_TARGET_IOS)


// Platform Family
#define NN_PLATFORM_DARWIN                                  (NN_TARGET_MACOS || NN_TARGET_IOS || NN_TARGET_TVOS)



#endif // NANO_TARGETS_H

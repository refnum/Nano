/*	NAME:
		NSystem.h

	DESCRIPTION:
		System support.

	COPYRIGHT:
		Copyright (c) 2006-2021, refNum Software
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
#ifndef NSYSTEM_H
#define NSYSTEM_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NMixinComparable.h"
#include "NString.h"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// OS version
//
// A NOSVersion represents the major.minor.patch release of an OS.
//
// These constants may be serialised - their values are fixed.
using NOSVersion                                            = uint32_t;

inline constexpr NOSVersion kNOSUnknown                     = 0x00000000;

inline constexpr NOSVersion kNOSAndroid                     = 0x10000000;
inline constexpr NOSVersion kNOSAndroid_8                   = 0x10080000;
inline constexpr NOSVersion kNOSAndroid_9                   = 0x10090000;
inline constexpr NOSVersion kNOSAndroid_10                  = 0x100a0000;
inline constexpr NOSVersion kNOSAndroid_11                  = 0x100b0000;

inline constexpr NOSVersion kNOSLinux                       = 0x20000000;
inline constexpr NOSVersion kNOSLinux_4                     = 0x20040000;
inline constexpr NOSVersion kNOSLinux_5                     = 0x20050000;

inline constexpr NOSVersion kNOSiOS                         = 0x30000000;
inline constexpr NOSVersion kNOSiOS_13                      = 0x300d0000;
inline constexpr NOSVersion kNOSiOS_14                      = 0x300e0000;
inline constexpr NOSVersion kNOSiOS_14_0_1                  = 0x300e0001;
inline constexpr NOSVersion kNOSiOS_14_1                    = 0x300e0100;
inline constexpr NOSVersion kNOSiOS_14_2                    = 0x300e0200;
inline constexpr NOSVersion kNOSiOS_14_2_1                  = 0x300e0201;
inline constexpr NOSVersion kNOSiOS_14_3                    = 0x300e0300;
inline constexpr NOSVersion kNOSiOS_14_4                    = 0x300e0400;

inline constexpr NOSVersion kNOSmacOS                       = 0x40000000;
inline constexpr NOSVersion kNOSmacOS_10_14                 = 0x400a0e00;
inline constexpr NOSVersion kNOSmacOS_10_15                 = 0x400a0f00;
inline constexpr NOSVersion kNOSmacOS_11                    = 0x400b0000;
inline constexpr NOSVersion kNOSmacOS_11_0_1                = 0x400b0001;
inline constexpr NOSVersion kNOSmacOS_11_1                  = 0x400b0100;
inline constexpr NOSVersion kNOSmacOS_11_2                  = 0x400b0200;

inline constexpr NOSVersion kNOStvOS                        = 0x50000000;
inline constexpr NOSVersion kNOStvOS_13                     = 0x500d0000;
inline constexpr NOSVersion kNOStvOS_14                     = 0x500e0000;
inline constexpr NOSVersion kNOStvOS_14_0_1                 = 0x500e0001;
inline constexpr NOSVersion kNOStvOS_14_0_2                 = 0x500e0002;
inline constexpr NOSVersion kNOStvOS_14_2                   = 0x500e0200;
inline constexpr NOSVersion kNOStvOS_14_3                   = 0x500e0300;
inline constexpr NOSVersion kNOStvOS_14_4                   = 0x500e0400;

inline constexpr NOSVersion kNOSWindows                     = 0x60000000;
inline constexpr NOSVersion kNOSWindows_XP                  = 0x60050000;
inline constexpr NOSVersion kNOSWindows_Vista               = 0x60060000;
inline constexpr NOSVersion kNOSWindows_7                   = 0x60070000;
inline constexpr NOSVersion kNOSWindows_8                   = 0x60080000;
inline constexpr NOSVersion kNOSWindows_10                  = 0x600a0000;


// OS name
//
// An NOSName represents a formatted name for an OS.
//
// The structure of OS names may vary per platform, and over time.
enum class NOSName
{
	Platform,    // "macOS"
	Version,     // "11.1
	Build,       // "20C69"

	Short,       // "macOS 11.1"
	Detailed,    // "macOS 11.1 (20C69)"
	Maximum,     // "macOS 11.1 (20C69) [Darwin Kernel Version...]"
};





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declaration
enum class NVersionPart;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NSystem
{
public:
	// Get an environment variable
	static NString                      GetEnv(const NString& theName);


	// Get the page size
	static size_t                       GetPageSize();


	// Get the version
	static NOSVersion                   GetVersion();


	// Get the name
	static NString                      GetName(NOSName theName = NOSName::Short);


	// Compare two versions
	static NComparison                  CompareVersions(const NString& versionA, const NString& versionB);


private:
	static NString                      SystemName(NOSName theName);

	static NVectorString                GetVersionParts(const NString& theVersion);
	static NVersionPart                 GetPartType(    const NString& thePart);
};



#endif // NSYSTEM_H

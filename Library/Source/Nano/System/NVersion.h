/*	NAME:
		NVersion.h

	DESCRIPTION:
		Version number.

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
#ifndef NSY_H
#define NSY_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NMixinComparable.h"
#include "NString.h"
#include "NanoMacros.h"
#include "NanoTypes.h"

// System
#include <array>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declaration
enum class NVersionPart;

// Tags
using NVersionTag                                           = std::array<utf8_t, 12>;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NN_EMPTY_BASE NVersion final : public NMixinComparable<NVersion>
{
public:
										NVersion(const NString& theVersion);

										NVersion(uint8_t        theProduct,
												 uint8_t        verMajor,
												 uint8_t        verMinor,
												 uint8_t        verPatch,
												 const NString& theTag);

public:
	constexpr                           NVersion(uint8_t theProduct,
												 uint8_t verMajor,
												 uint8_t verMinor = 0,
												 uint8_t verPatch = 0);

	constexpr                           NVersion();


	// Is the version valid?
	constexpr bool                      IsValid() const;


	// Clear the version
	void                                Clear();


	// Get/set the product
	//
	// The product value is meta-data used to group versions.
	constexpr uint8_t                   GetProduct() const;
	constexpr void                      SetProduct(uint8_t theValue);


	// Get/set the version components
	//
	// A version is composed of three integers:
	//
	//		major.minor.patch
	constexpr uint8_t                   GetMajor() const;
	constexpr uint8_t                   GetMinor() const;
	constexpr uint8_t                   GetPatch() const;

	constexpr void                      SetMajor(uint8_t theValue);
	constexpr void                      SetMinor(uint8_t theValue);
	constexpr void                      SetPatch(uint8_t theValue);


	// Get/set the tag
	//
	// The tag is an arbitrary suffix used to identify pre-releases:
	//
	//		"a1"		"b3"		"RC5"
	NString                             GetTag() const;
	void                                SetTag(  const NString& theTag);


	// Get / set the version as a string
	//
	// A version string is structured as "major.minor.patch<tag>":
	//
	//		"1.0"		"1.0.1"		"1.0.1b2"
	//
	// The product is never included.
	//
	// The major and minor components are always included, and the
	// patch component is included if it is not zero.
	//
	// The tag component is included if it is not empty.
	NString                             GetString() const;
	void                                SetString(  const NString& theVersion);


	// Compare two version strings
	//
	// Strings may contain sequences of integers, strings, and periods:
	//
	//		"1.0a1" < "1.0a2" < "1.0b1" < "1.0" < "1.0.1"
	//
	// String components are compared alphanumerically.
	static NComparison                  Compare(const NString& versionA, const NString& versionB);


public:
	// NMixinComparable
	bool                                CompareEqual(const NVersion& theVersion) const;
	NComparison                         CompareOrder(const NVersion& theVersion) const;


private:
	static NVectorString                GetParts(   const NString& theVersion);
	static NVersionPart                 GetPartType(const NString& thePart);


private:
	uint8_t                             mProduct;
	uint8_t                             mMajor;
	uint8_t                             mMinor;
	uint8_t                             mPatch;
	NVersionTag                         mTag;
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NVersion.inl"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// OS versions
//
// Versions to represent the major.minor.patch release of an OS.
//
// These constants may be serialised - their values are fixed.
inline constexpr uint8_t kNOSAndroid = 0x01;
inline constexpr uint8_t kNOSiOS     = 0x02;
inline constexpr uint8_t kNOSLinux   = 0x03;
inline constexpr uint8_t kNOSmacOS   = 0x04;
inline constexpr uint8_t kNOStvOS    = 0x05;
inline constexpr uint8_t kNOSWindows = 0x06;

inline constexpr NVersion kNOSAndroid_8  = NVersion(kNOSAndroid, 8, 0, 0);
inline constexpr NVersion kNOSAndroid_9  = NVersion(kNOSAndroid, 9);
inline constexpr NVersion kNOSAndroid_10 = NVersion(kNOSAndroid, 10);
inline constexpr NVersion kNOSAndroid_11 = NVersion(kNOSAndroid, 11);

inline constexpr NVersion kNOSLinux_4 = NVersion(kNOSLinux, 4);
inline constexpr NVersion kNOSLinux_5 = NVersion(kNOSLinux, 5);

inline constexpr NVersion kNOSiOS_13     = NVersion(kNOSiOS, 13);
inline constexpr NVersion kNOSiOS_14     = NVersion(kNOSiOS, 14);
inline constexpr NVersion kNOSiOS_14_0_1 = NVersion(kNOSiOS, 14, 0, 1);
inline constexpr NVersion kNOSiOS_14_1   = NVersion(kNOSiOS, 14,    1);
inline constexpr NVersion kNOSiOS_14_2   = NVersion(kNOSiOS, 14, 2);
inline constexpr NVersion kNOSiOS_14_2_1 = NVersion(kNOSiOS, 14, 2, 1);
inline constexpr NVersion kNOSiOS_14_3   = NVersion(kNOSiOS, 14, 3);
inline constexpr NVersion kNOSiOS_14_4   = NVersion(kNOSiOS, 14, 4);

inline constexpr NVersion kNOSmacOS_10_14  = NVersion(kNOSmacOS, 10, 14);
inline constexpr NVersion kNOSmacOS_10_15  = NVersion(kNOSmacOS, 10, 15);
inline constexpr NVersion kNOSmacOS_11     = NVersion(kNOSmacOS, 11);
inline constexpr NVersion kNOSmacOS_11_0_1 = NVersion(kNOSmacOS, 11, 0, 1);
inline constexpr NVersion kNOSmacOS_11_1   = NVersion(kNOSmacOS, 11,    1);
inline constexpr NVersion kNOSmacOS_11_2   = NVersion(kNOSmacOS, 11, 2);

inline constexpr NVersion kNOStvOS_13     = NVersion(kNOStvOS, 13);
inline constexpr NVersion kNOStvOS_14     = NVersion(kNOStvOS, 14);
inline constexpr NVersion kNOStvOS_14_0_1 = NVersion(kNOStvOS, 14, 0, 1);
inline constexpr NVersion kNOStvOS_14_0_2 = NVersion(kNOStvOS, 14, 0, 2);
inline constexpr NVersion kNOStvOS_14_2   = NVersion(kNOStvOS, 14,    2);
inline constexpr NVersion kNOStvOS_14_3   = NVersion(kNOStvOS, 14, 3);
inline constexpr NVersion kNOStvOS_14_4   = NVersion(kNOStvOS, 14, 4);

inline constexpr NVersion kNOSWindows_XP    = NVersion(kNOSWindows, 5);
inline constexpr NVersion kNOSWindows_Vista = NVersion(kNOSWindows, 6);
inline constexpr NVersion kNOSWindows_7     = NVersion(kNOSWindows, 7);
inline constexpr NVersion kNOSWindows_8     = NVersion(kNOSWindows, 8);
inline constexpr NVersion kNOSWindows_10    = NVersion(kNOSWindows, 9);



#endif // NSYSTEM_H

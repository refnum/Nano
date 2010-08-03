/*	NAME:
		NSystemUtilities.h

	DESCRIPTION:
		System utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSYSTEMUTILITIES_HDR
#define NSYSTEMUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NArray.h"
#include "NColor.h"
#include "NData.h"
#include "NDate.h"
#include "NDictionary.h"
#include "NPoint.h"
#include "NRectangle.h"
#include "NVector.h"
#include "NSize.h"
#include "NString.h"
#include "NVariant.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Operating system version
//
// Later releases are always numerically higher than older releases, and any
// named version is always higher than the generic release for that platform.
typedef enum {
	kOSUnknown														= 0,

	kOSMac															= 0x6d000000,
	kOSMacTenFive													= 0x6d0a0500,
	kOSMacTenFiveOne												= 0x6d0a0501,
	kOSMacTenFiveTwo												= 0x6d0a0502,
	kOSMacTenFiveThree												= 0x6d0a0503,
	kOSMacTenFiveFour												= 0x6d0a0504,
	kOSMacTenFiveFive												= 0x6d0a0505,
	kOSMacTenFiveSix												= 0x6d0a0506,
	kOSMacTenFiveSeven												= 0x6d0a0507,
	kOSMacTenFiveEight												= 0x6d0a0508,
	kOSMacTenFiveNine												= 0x6d0a0509,
	kOSMacTenSix													= 0x6d0a0600,
	kOSMacTenSixOne													= 0x6d0a0601,
	kOSMacTenSixTwo													= 0x6d0a0602,
	kOSMacTenSixThree												= 0x6d0a0603,
	kOSMacTenSixFour												= 0x6d0a0604,

	kOSWinXP														= 0x78000000,
	kOSWinXP_SP2													= 0x78000200,
	kOSWinXP_SP3													= 0x78000300,
	kOSWinVista														= 0x76000000,
	kOSWinSeven														= 0x37000000,

	kOSIPhone														= 0x69000000,
	kOSIPhoneTwo													= 0x69020000,
	kOSIPhoneTwoOne													= 0x69020100,
	kOSIPhoneTwoTwo													= 0x69020200,
	kOSIPhoneThree													= 0x69030000,
	kOSIPhoneThreeOne												= 0x69030100,
	kOSIPhoneThreeTwo												= 0x69030200,
	kOSIPhoneFour													= 0x69040000
} OSVersion;





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// Memory pool
//
// Allows portable code to insert a Mac auto-release pool within some scope,
// without needing to create a platform-specific wrapper around that scope.
#if NN_TARGET_MAC || NN_TARGET_IPHONE
	#include "NNSAutoReleasePool.h"
	
	#define NN_MEMORY_POOL							StAutoReleasePool
#else
	#define NN_MEMORY_POOL							NRange
#endif





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NSystemUtilities {
public:
	// Get a type'd value
	//
	// If the value can not be returned as the specified type, 0/empty is returned.
	static bool							GetBoolean(   const NVariant &theValue, const NString &debugID="");
	static SInt32						GetSInt32(    const NVariant &theValue, const NString &debugID="");
	static SInt64						GetSInt64(    const NVariant &theValue, const NString &debugID="");
	static Float32						GetFloat32(   const NVariant &theValue, const NString &debugID="");
	static Float64						GetFloat64(   const NVariant &theValue, const NString &debugID="");
	static NString						GetString(    const NVariant &theValue, const NString &debugID="");
	static NData						GetData(      const NVariant &theValue, const NString &debugID="");
	static NDate						GetDate(      const NVariant &theValue, const NString &debugID="");
	static NColor						GetColor(     const NVariant &theValue, const NString &debugID="");
	static NPoint						GetPoint(     const NVariant &theValue, const NString &debugID="");
	static NSize						GetSize(      const NVariant &theValue, const NString &debugID="");
	static NRectangle					GetRectangle( const NVariant &theValue, const NString &debugID="");
	static NVector						GetVector(    const NVariant &theValue, const NString &debugID="");
	static NArray						GetArray(     const NVariant &theValue, const NString &debugID="");
	static NDictionary					GetDictionary(const NVariant &theValue, const NString &debugID="");


	// Get the system version
	static OSVersion					GetOSVersion(void);


	// Compare two version strings
	static NComparison					CompareVersions(const NString &version1, const NString &version2);


private:
	static NStringList					GetVersionParts(const NString &theVersion);
	static NIndex						GetPartType(    const NString &thePart);
};






#endif // NSYSTEMUTILITIES_HDR




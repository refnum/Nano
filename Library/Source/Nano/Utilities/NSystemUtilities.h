/*	NAME:
		NSystemUtilities.h

	DESCRIPTION:
		System utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
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
#include "NURL.h"
#include "NVariant.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Operating system version
//
// OS version identifiers are arbitrary, and may change in future releases.
//
// However, later releases will always be numerically higher than older
// releases. In addition the specific numbered versions will always be
// higher than the generic platform version.
typedef enum {
	kOSUnknown														= 0,

	kOSMac															= 0x10000000,
	kOSMacTenFive													= 0x100a0500,
	kOSMacTenFiveOne												= 0x100a0501,
	kOSMacTenFiveTwo												= 0x100a0502,
	kOSMacTenFiveThree												= 0x100a0503,
	kOSMacTenFiveFour												= 0x100a0504,
	kOSMacTenFiveFive												= 0x100a0505,
	kOSMacTenFiveSix												= 0x100a0506,
	kOSMacTenFiveSeven												= 0x100a0507,
	kOSMacTenFiveEight												= 0x100a0508,
	kOSMacTenFiveNine												= 0x100a0509,
	kOSMacTenSix													= 0x100a0600,
	kOSMacTenSixOne													= 0x100a0601,
	kOSMacTenSixTwo													= 0x100a0602,
	kOSMacTenSixThree												= 0x100a0603,
	kOSMacTenSixFour												= 0x100a0604,
	kOSMacTenSixFive												= 0x100a0605,
	kOSMacTenSixSix													= 0x100a0606,
	kOSMacTenSixSeven												= 0x100a0607,
	kOSMacTenSixEight												= 0x100a0608,
	kOSMacTenSeven													= 0x100a0700,
	kOSMacTenSevenOne												= 0x100a0701,
	kOSMacTenSevenTwo												= 0x100a0702,
	kOSMacTenSevenThree												= 0x100a0703,
	kOSMacTenSevenFour												= 0x100a0704,
	kOSMacTenSevenFive												= 0x100a0705,
	kOSMacTenEight													= 0x100a0800,
	kOSMacTenEightOne												= 0x100a0801,
	kOSMacTenEightTwo												= 0x100a0802,
	kOSMacTenEightThree												= 0x100a0803,

	kOSWindows														= 0x20050000,
	kOSWindowsXP													= 0x20050000,
	kOSWindowsXP_SP1												= 0x20050100,
	kOSWindowsXP_SP2												= 0x20050200,
	kOSWindowsXP_SP3												= 0x20050300,
	kOSWindowsVista													= 0x20060000,
	kOSWindowsSeven													= 0x20070000,
	kOSWindowsEight													= 0x20080000,

	kOSiOS															= 0x30000000,
	kOSiOSTwo														= 0x30020000,
	kOSiOSTwoOne													= 0x30020100,
	kOSiOSTwoTwo													= 0x30020200,
	kOSiOSThree														= 0x30030000,
	kOSiOSThreeOne													= 0x30030100,
	kOSiOSThreeTwo													= 0x30030200,
	kOSiOSFour														= 0x30040000,
	kOSiOSFourOne													= 0x30040100,
	kOSiOSFourTwo													= 0x30040200,
	kOSiOSFourThree													= 0x30040300,
	kOSiOSFive														= 0x30050000,
	kOSiOSFiveOne													= 0x30050100,
	kOSiOSSix														= 0x30060000,
	kOSiOSSixOne													= 0x30060100,
	
	kOSLinux														= 0x40000000,
	kOSLinuxTwo														= 0x40020000,
	kOSLinuxTwoTwo													= 0x40020200,
	kOSLinuxTwoFour													= 0x40020400,
	kOSLinuxTwoSix													= 0x40020600,
	kOSLinuxThree													= 0x40030000,
	kOSLinuxThreeTwo												= 0x40030200,
	kOSLinuxThreeFour												= 0x40030400
} OSVersion;





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// Memory pool
//
// Allows portable code to insert a Mac auto-release pool within some scope,
// without needing to create a platform-specific wrapper around that scope.
#if NN_TARGET_MAC || NN_TARGET_IOS
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
	static int32_t						GetInt32(     const NVariant &theValue, const NString &debugID="");
	static int64_t						GetInt64(     const NVariant &theValue, const NString &debugID="");
	static float32_t					GetFloat32(   const NVariant &theValue, const NString &debugID="");
	static float64_t					GetFloat64(   const NVariant &theValue, const NString &debugID="");
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
	static NURL							GetURL(       const NVariant &theValue, const NString &debugID="");


	// Get the OS version
	static OSVersion					GetOSVersion(void);


	// Get the OS name
	static NString						GetOSName(void);


	// Get the process name
	static NString						GetProcessName(void);


	// Get the clock speed
	//
	// The result is in hertz.
	static uint64_t						GetSystemCPU(void);


	// Get the physical memory
	//
	// The result is in bytes.
	static uint64_t						GetSystemRAM(void);


	// Get the architecture name
	static NString						GetSystemArch(void);


	// Get an environment variable
	static NString						GetEnvironment(const NString &theKey);


	// Compare two version strings
	static NComparison					CompareVersions(const NString &version1, const NString &version2);


private:
	static NStringList					GetVersionParts(const NString &theVersion);
	static NIndex						GetPartType(    const NString &thePart);
};






#endif // NSYSTEMUTILITIES_HDR




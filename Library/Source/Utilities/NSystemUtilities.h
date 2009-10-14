/*	NAME:
		NSystemUtilities.h

	DESCRIPTION:
		System utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSYSTEMUTILITIES_HDR
#define NSYSTEMUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <vector>

#include "NTimer.h"
#include "NString.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// System version
typedef enum {
	kSystemUnknown												= 0,
	
	kSystemTenThree												= 0x000A0300,
	kSystemTenThreeOne											= 0x000A0301,
	kSystemTenThreeTwo											= 0x000A0302,
	kSystemTenThreeThree										= 0x000A0303,
	kSystemTenThreeFour											= 0x000A0304,
	kSystemTenThreeFive											= 0x000A0305,
	kSystemTenThreeSix											= 0x000A0306,
	kSystemTenThreeSeven										= 0x000A0307,
	kSystemTenThreeEight										= 0x000A0308,
	kSystemTenThreeNine											= 0x000A0309,

	kSystemTenFour												= 0x000A0400,
	kSystemTenFourOne											= 0x000A0401,
	kSystemTenFourTwo											= 0x000A0402,
	kSystemTenFourThree											= 0x000A0403,
	kSystemTenFourFour											= 0x000A0404,
	kSystemTenFourFive											= 0x000A0405,
	kSystemTenFourSix											= 0x000A0406,
	kSystemTenFourSeven											= 0x000A0407,
	kSystemTenFourEight											= 0x000A0408,
	kSystemTenFourNine											= 0x000A0409,

	kSystemTenFive												= 0x000A0500
} SystemVersion;


// Misc
static const UInt32 kKilobyte									= 1024;
static const UInt32 kMegabyte									= 1024 * 1024;

static const EventTime kEventDurationNever						= 0.0;
static const EventTime kEventDurationWeek						= kEventDurationDay * 7;
static const EventTime kEventDurationMonth						= kEventDurationDay * 30;





//============================================================================
//      Types
//----------------------------------------------------------------------------
typedef std::vector<UInt32>										UInt32List;
typedef UInt32List::iterator									UInt32ListIterator;
typedef UInt32List::const_iterator								UInt32ListConstIterator;

typedef std::vector<SInt32>										SInt32List;
typedef SInt32List::iterator									SInt32ListIterator;
typedef SInt32List::const_iterator								SInt32ListConstIterator;

typedef std::vector<Float32>									Float32List;
typedef Float32List::iterator									Float32ListIterator;
typedef Float32List::const_iterator								Float32ListConstIterator;


typedef std::vector<UInt64>										UInt64List;
typedef UInt64List::iterator									UInt64ListIterator;
typedef UInt64List::const_iterator								UInt64ListConstIterator;

typedef std::vector<SInt64>										SInt64List;
typedef SInt64List::iterator									SInt64ListIterator;
typedef SInt64List::const_iterator								SInt64ListConstIterator;

typedef std::vector<Float64>									Float64List;
typedef Float64List::iterator									Float64ListIterator;
typedef Float64List::const_iterator								Float64ListConstIterator;





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// Compare two values
//
// Returns a CFComparisonResult for two types that define the < and > operators.
#define GET_CF_COMPARE(_a, _b)						((_a) < (_b) ? kCFCompareLessThan : ((_a) > (_b) ? kCFCompareGreaterThan : kCFCompareEqualTo))


// Get the size of an array
#define GET_ARRAY_SIZE(_a)							(sizeof((_a)) / sizeof((_a)[0]))


// Clamp a value
#define GET_CLAMPED_VALUE(_v, _min, _max)			std::min((_max), std::max((_min), (_v)))





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NSystemUtilities {
public:
	// Get a Gestalt value
	static UInt32						GetGestaltValue(OSType theSelector);


	// Get the system version
	static SystemVersion				GetSystemVersion(void);


	// Is the system version the specified value or higher?
	static bool							SystemIsAtLeast(SystemVersion theVersion);


	// Convert an EventTime to a Duration
	static Duration						EventTimeToDuration(EventTime theTime);


	// Get the Finder label names
	static NStringList					GetLabelNames(void);


	// Get the main thread's run loop
	static CFRunLoopRef					GetMainRunLoop(void);


	// Compare two version strings
	static CFComparisonResult			CompareVersions(const NString &version1, const NString &version2);


	// Search for text
	static void							SearchDictionary(const NString &theText);
	static void							SearchSpotlight( const NString &theText);
	static void							SearchGoogle(    const NString &theText);


	// Execute an AppleScript
	static OSStatus						ExecuteScript(const NString &theText);
	

	// Delay a functor
	static void							DelayFunctor(const NFunctor &theFunctor, EventTime theDelay);


private:
	static NStringList					GetVersionParts(const NString &theVersion);
	static OSType						GetPartType(    const NString &thePart);
	
	static void							DelayedFunctor(const NFunctor &theFunctor, NTimer *theTimer);
};






#endif // NSYSTEMUTILITIES_HDR




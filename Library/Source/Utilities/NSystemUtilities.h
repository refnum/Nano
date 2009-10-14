/*	NAME:
		NSystemUtilities.h

	DESCRIPTION:
		System utilities.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSYSTEMUTILITIES_HDR
#define NSYSTEMUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMouseUtilities.h"
#include "NString.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// System version
typedef enum {
	kSystemUnknown								= 0,
	
	kSystemTenThree								= 0x000A0300,
	kSystemTenThreeOne							= 0x000A0301,
	kSystemTenThreeTwo							= 0x000A0302,
	kSystemTenThreeThree						= 0x000A0303,
	kSystemTenThreeFour							= 0x000A0304,
	kSystemTenThreeFive							= 0x000A0305,
	kSystemTenThreeSix							= 0x000A0306,
	kSystemTenThreeSeven						= 0x000A0307,
	kSystemTenThreeEight						= 0x000A0308,
	kSystemTenThreeNine							= 0x000A0309,

	kSystemTenFour								= 0x000A0400,
	kSystemTenFourOne							= 0x000A0401,
	kSystemTenFourTwo							= 0x000A0402,
	kSystemTenFourThree							= 0x000A0403,
	kSystemTenFourFour							= 0x000A0404,
	kSystemTenFourFive							= 0x000A0405,
	kSystemTenFourSix							= 0x000A0406,
	kSystemTenFourSeven							= 0x000A0407,
	kSystemTenFourEight							= 0x000A0408,
	kSystemTenFourNine							= 0x000A0409
} SystemVersion;


// Animation effects
typedef enum {
	kAnimationDisappearingItem
} NAnimationEffect;





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


// Construct a std::vector from an array
//
//		typedef std::vector<float> FloatList;
//
//		static const float kSomeArray[] = { 1, 2, 3, 4, 5 };
//
//		UInt32 SomeFunction(void)
//		{	FloatList	theArray(CREATE_VECTOR(kSomeArray));
//
//			return(theArray.size());
//		}
template <typename T> std::vector<T> _create_vector(UInt32 numValues, const T *theValues) 
{
	return(std::vector<T>(theValues, theValues + numValues));
} 

#define CREATE_VECTOR(_a)							_create_vector(GET_ARRAY_SIZE(_a), (_a))





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


	// Show an animation effect
	static void							ShowEffect(NAnimationEffect theEffect,
													const HIPoint	&thePoint = NMouseUtilities::GetPosition(),
													const HISize	&theSize  = CGSizeZero);


	// Get the Finder label names
	static NStringList					GetLabelNames(void);
};

	
	


#endif // NSYSTEMUTILITIES_HDR



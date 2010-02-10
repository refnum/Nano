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
#include "NTimer.h"
#include "NVariant.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const NTime kNFunctorDelayTime								=  250 * kNTimeMillisecond;





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


	// Delay a functor
	//
	// The functor can be invoked on the main thread, or on a new one-shot thread.
	static void							DelayFunctor(const NFunctor &theFunctor, NTime theDelay=kNFunctorDelayTime, bool onMainThread=true);


	// Compare two version strings
	static NComparison					CompareVersions(const NString &version1, const NString &version2);


private:
	static void							DelayedFunctor(NTimer *theTimer, const NFunctor &theFunctor, bool onMainThread);

	static NStringList					GetVersionParts(const NString &theVersion);
	static NIndex						GetPartType(    const NString &thePart);
};






#endif // NSYSTEMUTILITIES_HDR




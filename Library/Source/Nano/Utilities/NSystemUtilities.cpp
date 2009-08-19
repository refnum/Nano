/*	NAME:
		NSystemUtilities.cpp

	DESCRIPTION:
		System utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"
#include "NMathUtilities.h"
#include "NTargetThread.h"
#include "NThread.h"





//============================================================================
//		NSystemUtilities::GetBoolean : Get a boolean value.
//----------------------------------------------------------------------------
bool NSystemUtilities::GetBoolean(const NVariant &theValue, const NString &debugID)
{	bool	theResult;



	// Get the value
	theResult = false;

	if (!theValue.GetValue(theResult))
		{
		if (NN_DEBUG && !debugID.IsEmpty())
			NN_LOG("Unable to convert value [%@] to bool", debugID);
		}

	return(theResult);
}





//============================================================================
//		NSystemUtilities::GetSInt32 : Get a SInt32 value.
//----------------------------------------------------------------------------
SInt32 NSystemUtilities::GetSInt32(const NVariant &theValue, const NString &debugID)
{	NNumber		theNumber;
	SInt32		theResult;



	// Get the value
	theResult = 0;

	if (!theNumber.SetValue(theValue) || !theNumber.GetSInt32(theResult))
		{
		if (NN_DEBUG && !debugID.IsEmpty())
			NN_LOG("Unable to convert value [%@] to SInt32", debugID);
		}

	return(theResult);
}





//============================================================================
//		NSystemUtilities::GetSInt64 : Get a SInt64 value.
//----------------------------------------------------------------------------
SInt64 NSystemUtilities::GetSInt64(const NVariant &theValue, const NString &debugID)
{	NNumber		theNumber;
	SInt64		theResult;



	// Get the value
	theResult = 0;

	if (!theNumber.SetValue(theValue) || !theNumber.GetSInt64(theResult))
		{
		if (NN_DEBUG && !debugID.IsEmpty())
			NN_LOG("Unable to convert value [%@] to SInt64", debugID);
		}

	return(theResult);
}





//============================================================================
//		NSystemUtilities::GetFloat32 : Get a Float32 value.
//----------------------------------------------------------------------------
Float32 NSystemUtilities::GetFloat32(const NVariant &theValue, const NString &debugID)
{	NNumber		theNumber;
	Float32		theResult;



	// Get the value
	theResult = 0.0f;

	if (!theNumber.SetValue(theValue) || !theNumber.GetFloat32(theResult))
		{
		if (NN_DEBUG && !debugID.IsEmpty())
			NN_LOG("Unable to convert value [%@] to Float32", debugID);
		}

	return(theResult);
}





//============================================================================
//		NSystemUtilities::GetFloat64 : Get a Float64 value.
//----------------------------------------------------------------------------
Float64 NSystemUtilities::GetFloat64(const NVariant &theValue, const NString &debugID)
{	NNumber		theNumber;
	Float64		theResult;



	// Get the value
	theResult = 0.0;

	if (!theNumber.SetValue(theValue) || !theNumber.GetFloat64(theResult))
		{
		if (NN_DEBUG && !debugID.IsEmpty())
			NN_LOG("Unable to convert value [%@] to Float64", debugID);
		}

	return(theResult);
}





//============================================================================
//		NSystemUtilities::GetString : Get a string value.
//----------------------------------------------------------------------------
NString NSystemUtilities::GetString(const NVariant &theValue, const NString &debugID)
{	NString		theResult;



	// Get the value
	if (!theValue.GetValue(theResult))
		{
		if (NN_DEBUG && !debugID.IsEmpty())
			NN_LOG("Unable to convert value [%@] to NString", debugID);
		}

	return(theResult);
}





//============================================================================
//		NSystemUtilities::GetData : Get a data value.
//----------------------------------------------------------------------------
NData NSystemUtilities::GetData(const NVariant &theValue, const NString &debugID)
{	NData		theResult;



	// Get the value
	if (!theValue.GetValue(theResult))
		{
		if (NN_DEBUG && !debugID.IsEmpty())
			NN_LOG("Unable to convert value [%@] to NData", debugID);
		}

	return(theResult);
}





//============================================================================
//		NSystemUtilities::GetDate : Get a date value.
//----------------------------------------------------------------------------
NDate NSystemUtilities::GetDate(const NVariant &theValue, const NString &debugID)
{	NDate		theResult;



	// Get the value
	if (!theValue.GetValue(theResult))
		{
		if (NN_DEBUG && !debugID.IsEmpty())
			NN_LOG("Unable to convert value [%@] to NDate", debugID);
		}

	return(theResult);
}





//============================================================================
//		NSystemUtilities::GetColor : Get a color value.
//----------------------------------------------------------------------------
NColor NSystemUtilities::GetColor(const NVariant &theValue, const NString &debugID)
{	NColor		theResult;



	// Get the value
	if (!theValue.GetValue(theResult))
		{
		if (NN_DEBUG && !debugID.IsEmpty())
			NN_LOG("Unable to convert value [%@] to NColor", debugID);
		}

	return(theResult);
}





//============================================================================
//		NSystemUtilities::GetPoint : Get a point value.
//----------------------------------------------------------------------------
NPoint NSystemUtilities::GetPoint(const NVariant &theValue, const NString &debugID)
{	NPoint		theResult;



	// Get the value
	if (!theValue.GetValue(theResult))
		{
		if (NN_DEBUG && !debugID.IsEmpty())
			NN_LOG("Unable to convert value [%@] to NPoint", debugID);
		}

	return(theResult);
}





//============================================================================
//		NSystemUtilities::GetSize : Get a size value.
//----------------------------------------------------------------------------
NSize NSystemUtilities::GetSize(const NVariant &theValue, const NString &debugID)
{	NSize		theResult;



	// Get the value
	if (!theValue.GetValue(theResult))
		{
		if (NN_DEBUG && !debugID.IsEmpty())
			NN_LOG("Unable to convert value [%@] to NSize", debugID);
		}

	return(theResult);
}





//============================================================================
//		NSystemUtilities::GetRectangle : Get a rectangle value.
//----------------------------------------------------------------------------
NRectangle NSystemUtilities::GetRectangle(const NVariant &theValue, const NString &debugID)
{	NRectangle	theResult;



	// Get the value
	if (!theValue.GetValue(theResult))
		{
		if (NN_DEBUG && !debugID.IsEmpty())
			NN_LOG("Unable to convert value [%@] to NRectangle", debugID);
		}

	return(theResult);
}





//============================================================================
//		NSystemUtilities::GetVector : Get a vector value.
//----------------------------------------------------------------------------
NVector NSystemUtilities::GetVector(const NVariant &theValue, const NString &debugID)
{	NVector		theResult;



	// Get the value
	if (!theValue.GetValue(theResult))
		{
		if (NN_DEBUG && !debugID.IsEmpty())
			NN_LOG("Unable to convert value [%@] to NVector", debugID);
		}

	return(theResult);
}





//============================================================================
//		NSystemUtilities::GetArray : Get an array value.
//----------------------------------------------------------------------------
NArray NSystemUtilities::GetArray(const NVariant &theValue, const NString &debugID)
{	NArray		theResult;



	// Get the value
	if (!theValue.GetValue(theResult))
		{
		if (NN_DEBUG && !debugID.IsEmpty())
			NN_LOG("Unable to convert value [%@] to NArray", debugID);
		}

	return(theResult);
}





//============================================================================
//		NSystemUtilities::GetDictionary : Get a dictionary value.
//----------------------------------------------------------------------------
NDictionary NSystemUtilities::GetDictionary(const NVariant &theValue, const NString &debugID)
{	NDictionary		theResult;



	// Get the value
	if (!theValue.GetValue(theResult))
		{
		if (NN_DEBUG && !debugID.IsEmpty())
			NN_LOG("Unable to convert value [%@] to NDictionary", debugID);
		}

	return(theResult);
}





//============================================================================
//		NSystemUtilities::DelayFunctor : Delay a functor.
//----------------------------------------------------------------------------
void NSystemUtilities::DelayFunctor(const NFunctor &theFunctor, NTime theDelay, bool mainThread)
{	NTimer		*theTimer;



	// Invoke immediately
	//
	// If we're to invoke on a new thread, or are already on the main
	// thread, we can invoke the functor directly without any delay.
	//
	// If we can't (we have a delay or we're not the main thread but
	// the functor must execute on the main thread), we fall through
	// to the timer case.
	if (NMathUtilities::IsZero(theDelay))
		{
		if (!mainThread)
			{
			NTargetThread::ThreadCreate(theFunctor);
			return;
			}
		
		else if (NThread::IsMain())
			{
			theFunctor();
			return;
			}
		}



	// Invoke with a delay
	theTimer = new NTimer;
	if (theTimer != NULL)
		theTimer->AddTimer(BindFunction(NSystemUtilities::DelayedFunctor, theTimer, theFunctor, mainThread), theDelay);
}





//============================================================================
//		NSystemUtilities::DelayedFunctor : Execute a delayed functor.
//----------------------------------------------------------------------------
#pragma mark -
void NSystemUtilities::DelayedFunctor(NTimer *theTimer, const NFunctor &theFunctor, bool mainThread)
{


	// Invoke the functor
	if (mainThread)
		theFunctor();
	else
		NTargetThread::ThreadCreate(theFunctor);



	// Clean up
	delete theTimer;
}





	
	



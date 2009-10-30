/*	NAME:
		NCGShading.cpp

	DESCRIPTION:
		CoreGraphics shading object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCoreGraphicsUtilities.h"
#include "NGeometryUtilities.h"
#include "NCGShading.h"





//============================================================================
//		Public constants
//----------------------------------------------------------------------------
// Shadings
const NShadingSample kShadingBlackWhite[] =					{ { 0.0f, kColorBlack },
															  { 1.0f, kColorWhite } };

const NShadingSample kShadingWhiteBlack[] =					{ { 0.0f, kColorWhite },
															  { 1.0f, kColorBlack } };

const NShadingSample kShadingFrontRow[] =					{ { 0.0f, kColorBlack },
															  { 0.5f, kColorBlack },
															  { 1.0f, NColor(0.298f, 0.298f, 0.298f, 1.0f) } };





//============================================================================
//		NCGShading::NCGShading : Constructor.
//----------------------------------------------------------------------------
NCGShading::NCGShading(const NCGShading &theShading)
{


	// Initialize ourselves
	CopyShading(theShading);
}





//============================================================================
//		NCGShading::NCGShading : Constructor.
//----------------------------------------------------------------------------
NCGShading::NCGShading(NShadingMode theMode)
{


	// Initialize ourselves
	InitializeSelf();
	SetMode(theMode);
}





//============================================================================
//		NCGShading::~NCGShading : Destructor.
//----------------------------------------------------------------------------
NCGShading::~NCGShading(void)
{
}





//============================================================================
//		NCGShading::GetMode : Get the shading mode.
//----------------------------------------------------------------------------
NShadingMode NCGShading::GetMode(void) const
{


	// Get the mode
	return(mMode);
}





//============================================================================
//		NCGShading::SetMode : Set the shading mode.
//----------------------------------------------------------------------------
void NCGShading::SetMode(NShadingMode theMode)
{


	// Set the mode
	if (theMode != mMode)
		{
		mMode = theMode;
		ResetShading();
		}
}





//============================================================================
//		NCGShading::GetSamples : Get the shading samples.
//----------------------------------------------------------------------------
NShadingSampleList NCGShading::GetSamples(void) const
{


	// Get the samples
	return(mSamples);
}





//============================================================================
//		NCGShading::SetSamples : Set the shading samples.
//----------------------------------------------------------------------------
void NCGShading::SetSamples(const NShadingSampleList &theSamples)
{	UInt32		n, numItems;
	bool		didChange;



	// Get the state we need
	didChange = (mSamples.size() != theSamples.size());
	
	if (!didChange)
		{
		numItems = mSamples.size();
		for (n = 0; n < numItems && !didChange; n++)
			didChange = (mSamples[n] != theSamples[n]);
		}



	// Set the samples
	if (didChange)
		{
		mSamples = theSamples;
		ResetShading();
		}
}





//============================================================================
//		NCGShading::SetSamples : Set the shading samples.
//----------------------------------------------------------------------------
void NCGShading::SetSamples(const NShadingSample *theSamples)
{	NShadingSampleList		theList;



	// Validate our parameters
	NN_ASSERT(theSamples != NULL);



	// Set the samples
	while (true)
		{
		theList.push_back(*theSamples);
		if (theSamples->theValue >= 1.0f)
			break;

		theSamples++;
		}
	
	SetSamples(theList);
}





//============================================================================
//		NCGShading::GetStartPoint : Get the start point.
//----------------------------------------------------------------------------
CGPoint NCGShading::GetStartPoint(void) const
{


	// Get the start point
	return(mStartPoint);
}





//============================================================================
//		NCGShading::SetStartPoint : Set the start point.
//----------------------------------------------------------------------------
void NCGShading::SetStartPoint(const CGPoint &thePoint)
{


	// Set the start point
	if (thePoint != mStartPoint)
		{
		mStartPoint = thePoint;
		ResetShading();
		}
}





//============================================================================
//		NCGShading::GetStartExtend : Get the start extend.
//----------------------------------------------------------------------------
bool NCGShading::GetStartExtend(void) const
{


	// Get the start extend
	return(mStartExtend);
}





//============================================================================
//		NCGShading::SetStartExtend : Set the start extend.
//----------------------------------------------------------------------------
void NCGShading::SetStartExtend(bool theValue)
{


	// Set the start extend
	if (theValue != mStartExtend)
		{
		mStartExtend = theValue;
		ResetShading();
		}
}





//============================================================================
//		NCGShading::GetStartRadius : Get the start radius.
//----------------------------------------------------------------------------
float NCGShading::GetStartRadius(void) const
{


	// Get the start radius
	return(mStartRadius);
}





//============================================================================
//		NCGShading::SetStartRadius : Set the start radius.
//----------------------------------------------------------------------------
void NCGShading::SetStartRadius(float theValue)
{


	// Validate our state
	NN_ASSERT(mMode == kShadingRadial);
	


	// Set the start radius
	if (NMathUtilities::NotEqual(theValue, mStartRadius))
		{
		mStartRadius = theValue;
		ResetShading();
		}
}





//============================================================================
//		NCGShading::GetEndPoint : Get the end point.
//----------------------------------------------------------------------------
CGPoint NCGShading::GetEndPoint(void) const
{


	// Get the end point
	return(mEndPoint);
}





//============================================================================
//		NCGShading::SetEndPoint : Set the end point.
//----------------------------------------------------------------------------
void NCGShading::SetEndPoint(const CGPoint &thePoint)
{


	// Set the end point
	if (thePoint != mEndPoint)
		{
		mEndPoint = thePoint;
		ResetShading();
		}
}





//============================================================================
//		NCGShading::GetEndExtend : Get the end extend.
//----------------------------------------------------------------------------
bool NCGShading::GetEndExtend(void) const
{


	// Get the end extend
	return(mEndExtend);
}





//============================================================================
//		NCGShading::SetEndExtend : Set the end extend.
//----------------------------------------------------------------------------
void NCGShading::SetEndExtend(bool theValue)
{


	// Set the end extend
	if (theValue != mEndExtend)
		{
		mEndExtend = theValue;
		ResetShading();
		}
}





//============================================================================
//		NCGShading::GetEndRadius : Get the end radius.
//----------------------------------------------------------------------------
float NCGShading::GetEndRadius(void) const
{


	// Get the end radius
	return(mEndRadius);
}





//============================================================================
//		NCGShading::SetEndRadius : Set the end radius.
//----------------------------------------------------------------------------
void NCGShading::SetEndRadius(float theValue)
{


	// Validate our state
	NN_ASSERT(mMode == kShadingRadial);
	


	// Set the end radius
	if (NMathUtilities::NotEqual(theValue, mEndRadius))
		{
		mEndRadius = theValue;
		ResetShading();
		}
}





//============================================================================
//		NCGShading::ApplyLinearShading : Apply a linear shading.
//----------------------------------------------------------------------------
void NCGShading::ApplyLinearShading(NLinearOrientation theOrientation, float a, float b)
{


	// Apply the shading
	switch (theOrientation) {
		case kLinearTopBottom:
			SetStartPoint(CGPointMake(0.0f, a));
			SetEndPoint(  CGPointMake(0.0f, b));
			break;

		case kLinearBottomTop:
			SetStartPoint(CGPointMake(0.0f, b));
			SetEndPoint(  CGPointMake(0.0f, a));
			break;

		case kLinearLeftRight:
			SetStartPoint(CGPointMake(a, 0.0f));
			SetEndPoint(  CGPointMake(b, 0.0f));
			break;

		case kLinearRightLeft:
			SetStartPoint(CGPointMake(0.0f, b));
			SetEndPoint(  CGPointMake(0.0f, a));
			break;
		
		default:
			NN_LOG("Unknown orientation: %d", theOrientation);
			break;
		}
}





//============================================================================
//		NCGShading::operator CGShadingRef : CGShadingRef-cast operator.
//----------------------------------------------------------------------------
NCGShading::operator CGShadingRef(void) const
{


	// Update the shading
	//
	// Shadings are lazily-evaluated, since multiple attributes of the shading
	// may need to be modified before a shading is ready for rendering.
	if (!mShading.IsValid())
		UpdateShading();



	// Get the shading
	return(mShading);
}





//============================================================================
//		NCGShading::= : Assignment operator.
//----------------------------------------------------------------------------
const NCGShading& NCGShading::operator = (const NCGShading &theShading)
{


	// Assign the shading
	if (this != &theShading)
		CopyShading(theShading);

	return(*this);
}





//============================================================================
//		NCGShading::Evaluate : Evaluate the shading.
//----------------------------------------------------------------------------
#pragma mark -
NColor NCGShading::Evaluate(float theSample)
{	NColor		theResult, colorOne, colorTwo;
	float		theRange, theWeight;
	SInt32		n, numSamples;



	// Evaluate the shading
	//
	// Provides a linear interpolation through the color sample list, by
	// identifying the relevant pair of colors for this sample and using
	// the color values to determine where each color starts/stops.
	//
	// We special case shadings of 1 or 2 colors, and ignore the sample
	// values to perform a straight no/pair shading.
	switch (mSamples.size()) {
		case 0:
			theResult = kColorBlack;
			break;

		case 1:
			theResult = mSamples[0].theColor;
			break;
		
		case 2:
			theResult = InterpolateColor(theSample, mSamples[0].theColor, mSamples[1].theColor);
			break;
		
		default:
			// Find the appropriate pair
			numSamples = mSamples.size();
			theResult  = mSamples[0].theColor;
	
			for (n = numSamples-1; n >= 0; n--)
				{
				if (theSample >= mSamples[n].theValue)
					{
					// Use the last color
					if (n == (numSamples -1))
						theResult = mSamples[n].theColor;
				
				
					// Use the pair of colors
					else
						{
						colorOne = mSamples[n + 0].theColor;
						colorTwo = mSamples[n + 1].theColor;
					
						theRange  = mSamples[n+1].theValue - mSamples[n].theValue;
						theWeight = (theSample - mSamples[n].theValue) / theRange;
					
						theResult = InterpolateColor(theWeight, colorOne, colorTwo);
						}
				
					break;
					}
				}
			break;
		}
	
	return(theResult);
}





//============================================================================
//		NCGShading::InterpolateColor : Interpolate between two colors.
//----------------------------------------------------------------------------
NColor NCGShading::InterpolateColor(float theWeight, const NColor &firstColor, const NColor &secondColor)
{	NColor		theResult;



	// Interpolate between the colors
	theResult.SetRed(  (firstColor.GetRed()   * (1.0f - theWeight)) + (secondColor.GetRed()   * theWeight));
	theResult.SetGreen((firstColor.GetGreen() * (1.0f - theWeight)) + (secondColor.GetGreen() * theWeight));
	theResult.SetBlue( (firstColor.GetBlue()  * (1.0f - theWeight)) + (secondColor.GetBlue()  * theWeight));
	theResult.SetAlpha((firstColor.GetAlpha() * (1.0f - theWeight)) + (secondColor.GetAlpha() * theWeight));
	
	return(theResult);
}





//============================================================================
//		NCGShading::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
void NCGShading::InitializeSelf(void)
{


	// Initialize ourselves
	mMode = kShadingNone;
	
	ResetShading();
	
	mStartPoint  = CGPointZero;
	mStartExtend = true;
	mStartRadius = 0.0f;

	mEndPoint  = CGPointZero;
	mEndExtend = true;
	mEndRadius = 1.0f;
}





//============================================================================
//		NCGShading::ResetShading : Reset the shading.
//----------------------------------------------------------------------------
void NCGShading::ResetShading(void) const
{


	// Work around rdar://5604593
	//
	// 10.5 incorrectly caches the results of shading evaluation callbacks:
	//
	//		<http://lists.apple.com/archives/quartz-dev/2007/Nov/msg00052.html>
	//
	// To fully reset the shading object, we must also re-create the evaluate
	// callback to ensure the cached results will be discarded.
	mShading.Clear();
	mEvaluate.Set(CreateEvaluateCallback());
}





//============================================================================
//		NCGShading::UpdateShading : Update the shading.
//----------------------------------------------------------------------------
void NCGShading::UpdateShading(void) const
{	CGColorSpaceRef		cgColorSpace;



	// Validate our state
	NN_ASSERT(!mShading.IsValid());



	// Create the new shading
	cgColorSpace = NCoreGraphicsUtilities::GetColorSpaceRGB();

	switch (mMode) {
		case kShadingNone:
			ResetShading();
			break;

		case kShadingLinear:
			mShading.Set(CGShadingCreateAxial( cgColorSpace, mStartPoint, mEndPoint, mEvaluate, mStartExtend, mEndExtend));
			break;

		case kShadingRadial:
			mShading.Set(CGShadingCreateRadial(cgColorSpace, mStartPoint, mStartRadius, mEndPoint, mEndRadius, mEvaluate, mStartExtend, mEndExtend));
			break;

		default:
			NN_LOG("Unknown shading mode: %d", mMode);
			break;
		}
}





//============================================================================
//		NCGShading::CopyShading : Copy a shading.
//----------------------------------------------------------------------------
void NCGShading::CopyShading(const NCGShading &theShading)
{


	// Reset our state
	//
	// The shading object and evaluate callback are instance-specific, since
	// the evaluate callback's user data is our this pointer.
	//
	// Those objects are never copied, and we initialize ourselves prior to
	// copying to allow CopyShading to be used from our copy constructor.
	InitializeSelf();



	// Copy the shading
	mMode = theShading.mMode;
	mSamples = theShading.mSamples;
	
	mStartPoint  = theShading.mStartPoint;
	mStartExtend = theShading.mStartExtend;
	mStartRadius = theShading.mStartRadius;

	mEndPoint  = theShading.mEndPoint;
	mEndExtend = theShading.mEndExtend;
	mEndRadius = theShading.mEndRadius;
}





//============================================================================
//		NCGShading::CreateEvaluateCallback : Create the evaluate callback.
//----------------------------------------------------------------------------
CGFunctionRef NCGShading::CreateEvaluateCallback(void) const
{	static const CGFunctionCallbacks	kCallbackInfo     = { 0, NCGShading::EvaluateCallback, NULL };
	static const float					kCallbackRange[]  = { 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f };
	static const float					kCallbackDomain[] = { 0.0f, 1.0f };



	// Create the callback
	return(CGFunctionCreate((void *) this, 1, kCallbackDomain, 4, kCallbackRange, &kCallbackInfo));
}





//============================================================================
//		NCGShading::EvaluateCallback : Evaluate callback.
//----------------------------------------------------------------------------
void NCGShading::EvaluateCallback(void *info, const float *in, float *out)
{	NCGShading		*thisPtr = (NCGShading *) info;
	NColor			theResult;



	// Validate our parameters
	NN_ASSERT(in[0] >= 0.0f && in[0] <= 1.0f);



	// Evaluate the shading
	theResult = thisPtr->Evaluate(in[0]);

	theResult.GetColor(out[0], out[1], out[2], out[3]);
}


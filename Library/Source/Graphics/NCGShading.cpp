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
//		Internal constants
//----------------------------------------------------------------------------
static const float kDomainRGBA[] = { 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f };





//============================================================================
//		NCGShading::NCGShading : Constructor.
//----------------------------------------------------------------------------
NCGShading::NCGShading(NShadingMode theMode)
{	CGFunctionCallbacks		theInfo = { 0, EvaluateCallback, NULL };



	// Initialize ourselves
	mIsValid = false;
	
	mMode     = kShadingNone;
	mEvaluate = CGFunctionCreate(this, 1, NULL, 4, kDomainRGBA, &theInfo);
	
	mStartPoint  = CGPointZero;
	mStartExtend = true;
	mStartRadius = 0.0f;

	mEndPoint  = CGPointZero;
	mEndExtend = true;
	mEndRadius = 1.0f;



	// Create the shading
	SetMode(theMode);
}





//============================================================================
//		NCGShading::~NCGShading : Destructor.
//----------------------------------------------------------------------------
NCGShading::~NCGShading(void)
{


	// Clean up
	CFSafeRelease(mEvaluate);
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
		mMode    = theMode;
		mIsValid = false;
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
		mIsValid = false;
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
		mIsValid    = false;
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
		mIsValid     = false;
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
		mIsValid     = false;
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
		mIsValid  = false;
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
		mIsValid   = false;
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
		mIsValid   = false;
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
	//
	// Note that we have to cast away our const in order to update the shading,
	// since we can't declare methods as mutable.
	if (!mIsValid)
		((NCGShading *) this)->UpdateShading();



	// Get the shading
	return((CGShadingRef) ((CFTypeRef) *this));
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
	theResult.SetRed(  (firstColor.GetRed()   * (1.0 - theWeight)) + (secondColor.GetRed()   * theWeight));
	theResult.SetGreen((firstColor.GetGreen() * (1.0 - theWeight)) + (secondColor.GetGreen() * theWeight));
	theResult.SetBlue( (firstColor.GetBlue()  * (1.0 - theWeight)) + (secondColor.GetBlue()  * theWeight));
	theResult.SetAlpha((firstColor.GetAlpha() * (1.0 - theWeight)) + (secondColor.GetAlpha() * theWeight));
	
	return(theResult);
}





//============================================================================
//		NCGShading::UpdateShading : Update the shading.
//----------------------------------------------------------------------------
#pragma mark -
void NCGShading::UpdateShading(void)
{	CGColorSpaceRef		cgColorSpace;
	CGShadingRef		cgShading;



	// Validate our state
	NN_ASSERT(!mIsValid);



	// Create the new shading
	cgColorSpace = NCoreGraphicsUtilities::GetColorSpaceRGB();

	switch (mMode) {
		case kShadingNone:
			cgShading = NULL;
			break;

		case kShadingLinear:
			cgShading = CGShadingCreateAxial( cgColorSpace, mStartPoint, mEndPoint, mEvaluate, mStartExtend, mEndExtend);
			break;

		case kShadingRadial:
			cgShading = CGShadingCreateRadial(cgColorSpace, mStartPoint, mStartRadius, mEndPoint, mEndRadius, mEvaluate, mStartExtend, mEndExtend);
			break;

		default:
			cgShading = NULL;
			NN_LOG("Unknown shading mode: %d", mMode);
			break;
		}



	// Update our state
	mIsValid = true;

	Set(cgShading);
}





//============================================================================
//		NCGShading::EvaluateCallback : Evaluate callback.
//----------------------------------------------------------------------------
void NCGShading::EvaluateCallback(void *info, const float *in, float *out)
{	NCGShading		*thisPtr = (NCGShading *) info;
	NColor			theResult;



	// Evaluate the shading
	theResult = thisPtr->Evaluate(in[0]);

	theResult.GetColor(out[0], out[1], out[2], out[3]);
}


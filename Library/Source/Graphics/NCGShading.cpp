/*	NAME:
		NCGShading.cpp

	DESCRIPTION:
		CoreGraphics shading object.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCoreGraphicsUtilities.h"
#include "NCGShading.h"





//============================================================================
//		Public constants
//----------------------------------------------------------------------------
// Shadings
const ColorSample kShadingBlackWhite[] =					{ { 0.0f, kColorBlack },
															  { 1.0f, kColorWhite } };

const ColorSample kShadingWhiteBlack[] =					{ { 0.0f, kColorWhite },
															  { 1.0f, kColorBlack } };

const ColorSample kShadingFrontRow[] =						{ { 0.0f, kColorBlack },
															  { 0.5f, kColorBlack },
															  { 1.0f, NColor(0.298f, 0.298f, 0.298f, 1.0f) } };





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const float kDomainRGBA[] = { 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f };





//============================================================================
//		NCGShading::NCGShading : Constructor.
//----------------------------------------------------------------------------
NCGShading::NCGShading(CGShadingMode theMode)
{	CGFunctionCallbacks		theInfo = { 0, EvaluateCallback, NULL };



	// Initialize ourselves
	mMode     = kShadingNone;
	mEvaluate = CGFunctionCreate(this, 1, NULL, 4, kDomainRGBA, &theInfo);
	
	mStartPoint  = CGPointZero;
	mStartExtend = false;
	mStartRadius = 1.0f;

	mEndPoint  = CGPointZero;
	mEndExtend = false;
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
CGShadingMode NCGShading::GetMode(void) const
{


	// Get the mode
	return(mMode);
}





//============================================================================
//		NCGShading::SetMode : Set the shading mode.
//----------------------------------------------------------------------------
void NCGShading::SetMode(CGShadingMode theMode)
{


	// Set the mode
	mMode = theMode;
	
	CreateShading();
}





//============================================================================
//		NCGShading::GetColorSamples : Get the color samples.
//----------------------------------------------------------------------------
ColorSampleList NCGShading::GetColorSamples(void) const
{


	// Get the samples
	return(mSamples);
}





//============================================================================
//		NCGShading::SetColorSamples : Set the color samples.
//----------------------------------------------------------------------------
void NCGShading::SetColorSamples(const ColorSampleList &theSamples)
{


	// Set the samples
	mSamples = theSamples;
}





//============================================================================
//		NCGShading::SetColorSamples : Set the color samples.
//----------------------------------------------------------------------------
void NCGShading::SetColorSamples(const ColorSample *theSamples)
{	ColorSampleList		theList;



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
	
	SetColorSamples(theList);
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
	mStartPoint = thePoint;
	
	CreateShading();
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
	mStartExtend = theValue;
	
	CreateShading();
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
	mStartRadius = theValue;
	
	CreateShading();
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
	mEndPoint = thePoint;
	
	CreateShading();
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
	mEndExtend = theValue;
	
	CreateShading();
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
	mEndRadius = theValue;
	
	CreateShading();
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
//		NCGShading::CreateShading : Create the shading.
//----------------------------------------------------------------------------
#pragma mark -
void NCGShading::CreateShading(void)
{	CGColorSpaceRef		cgColorSpace;
	CGShadingRef		cgShading;



	// Get the state we need
	cgColorSpace = NCoreGraphicsUtilities::GetColorSpaceRGB();



	// Create the shading
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



	// Update our object
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





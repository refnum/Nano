/*	NAME:
		NCGShading.cpp

	DESCRIPTION:
		CoreGraphics shading object.

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
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NCGShading.h"

// Nano
#include "NCGColor.h"
#include "NCoreGraphics.h"





//=============================================================================
//		NCGShading::NCGShading : Constructor.
//-----------------------------------------------------------------------------
NCGShading::NCGShading()
	: mShading()
	, mEvaluate()
	, mMode(NShadingMode::None)
	, mSamples()
	, mStartPoint()
	, mStartExtend(true)
	, mStartRadius(0.0)
	, mEndPoint()
	, mEndExtend(true)
	, mEndRadius(1.0)
{
}





//=============================================================================
//		NCGShading::NCGShading : Constructor.
//-----------------------------------------------------------------------------
inline NCGShading::NCGShading(const NCGShading& otherShading)
	: mShading()
	, mEvaluate()
	, mMode(otherShading.mMode)
	, mSamples(otherShading.mSamples)
	, mStartPoint(otherShading.mStartPoint)
	, mStartExtend(otherShading.mStartExtend)
	, mStartRadius(otherShading.mStartRadius)
	, mEndPoint(otherShading.mEndPoint)
	, mEndExtend(otherShading.mEndExtend)
	, mEndRadius(otherShading.mEndRadius)
{
}





//=============================================================================
//		NCGShading::operator= : Assignment operator.
//-----------------------------------------------------------------------------
inline NCGShading& NCGShading::operator=(const NCGShading& otherShading)
{


	// Assign the Shading
	if (this != &otherShading)
	{
		mShading.Set(nullptr);
		mEvaluate.Set(nullptr);

		mMode        = otherShading.mMode;
		mSamples     = otherShading.mSamples;
		mStartPoint  = otherShading.mStartPoint;
		mStartExtend = otherShading.mStartExtend;
		mStartRadius = otherShading.mStartRadius;
		mEndPoint    = otherShading.mEndPoint;
		mEndExtend   = otherShading.mEndExtend;
		mEndRadius   = otherShading.mEndRadius;
	}

	return *this;
}





//=============================================================================
//		NCGShading::NCGShading : Constructor.
//-----------------------------------------------------------------------------
inline NCGShading::NCGShading(NCGShading&& otherShading)
	: mShading()
	, mEvaluate()
	, mMode(std::exchange(otherShading.mMode, NShadingMode::None))
	, mSamples(std::exchange(otherShading.mSamples, {}))
	, mStartPoint(std::exchange(otherShading.mStartPoint, {}))
	, mStartExtend(std::exchange(otherShading.mStartExtend, false))
	, mStartRadius(std::exchange(otherShading.mStartRadius, 0.0))
	, mEndPoint(std::exchange(otherShading.mEndPoint, {}))
	, mEndExtend(std::exchange(otherShading.mEndExtend, false))
	, mEndRadius(std::exchange(otherShading.mEndRadius, 0.0))
{


	// Reset the other object
	otherShading.mShading.Set(nullptr);
	otherShading.mEvaluate.Set(nullptr);
}





//=============================================================================
//		NCGShading::operator= : Assignment operator.
//-----------------------------------------------------------------------------
inline NCGShading& NCGShading::operator=(NCGShading&& otherShading)
{


	// Move the shading
	if (this != &otherShading)
	{
		mShading.Set(nullptr);
		mEvaluate.Set(nullptr);

		mMode        = std::exchange(otherShading.mMode, NShadingMode::None);
		mSamples     = std::exchange(otherShading.mSamples, {});
		mStartPoint  = std::exchange(otherShading.mStartPoint, {});
		mStartExtend = std::exchange(otherShading.mStartExtend, false);
		mStartRadius = std::exchange(otherShading.mStartRadius, 0.0);
		mEndPoint    = std::exchange(otherShading.mEndPoint, {});
		mEndExtend   = std::exchange(otherShading.mEndExtend, false);
		mEndRadius   = std::exchange(otherShading.mEndRadius, 0.0);

		otherShading.mShading.Set(nullptr);
		otherShading.mEvaluate.Set(nullptr);
	}

	return *this;
}





//=============================================================================
//		NCGShading::IsValid : Is the obejct valid?
//-----------------------------------------------------------------------------
bool NCGShading::IsValid() const
{


	// Validate our state
	NN_REQUIRE((mShading.IsValid() && mEvaluate.IsValid()) ||
			   (!mShading.IsValid() && !mEvaluate.IsValid()));



	// Check our state
	return mShading.IsValid() && mEvaluate.IsValid();
}





//=============================================================================
//		NCGShading::SetMode : Set the shading mode.
//-----------------------------------------------------------------------------
void NCGShading::SetMode(NShadingMode theMode)
{


	// Set the mode
	if (theMode != mMode)
	{
		mMode = theMode;
		SetDirty();
	}
}





//=============================================================================
//		NCGShading::SetSamples : Set the shading samples.
//-----------------------------------------------------------------------------
void NCGShading::SetSamples(const NVectorShadingSample& theSamples)
{


	// Check for a change
	bool didChange = (mSamples.size() != theSamples.size());

	if (!didChange)
	{
		size_t numSamples = mSamples.size();

		for (size_t n = 0; n < numSamples && !didChange; n++)
		{
			didChange = (mSamples[n] != theSamples[n]);
		}
	}



	// Set the samples
	if (didChange)
	{
		mSamples = theSamples;
		SetDirty();
	}
}





//=============================================================================
//		NCGShading::SetStartPoint : Set the start point.
//-----------------------------------------------------------------------------
void NCGShading::SetStartPoint(const NPoint& thePoint)
{


	// Set the start point
	if (thePoint != mStartPoint)
	{
		mStartPoint = thePoint;
		SetDirty();
	}
}





//=============================================================================
//		NCGShading::SetStartExtend : Set the start extend.
//-----------------------------------------------------------------------------
void NCGShading::SetStartExtend(bool theValue)
{


	// Set the start extend
	if (theValue != mStartExtend)
	{
		mStartExtend = theValue;
		SetDirty();
	}
}





//=============================================================================
//		NCGShading::SetStartRadius : Set the start radius.
//-----------------------------------------------------------------------------
void NCGShading::SetStartRadius(CGFloat theValue)
{


	// Validate our state
	NN_REQUIRE(mMode == NShadingMode::Radial);



	// Set the start radius
	if (theValue != mStartRadius)
	{
		mStartRadius = theValue;
		SetDirty();
	}
}





//=============================================================================
//		NCGShading::SetEndPoint : Set the end point.
//-----------------------------------------------------------------------------
void NCGShading::SetEndPoint(const NPoint& thePoint)
{


	// Set the end point
	if (thePoint != mEndPoint)
	{
		mEndPoint = thePoint;
		SetDirty();
	}
}





//=============================================================================
//		NCGShading::SetEndExtend : Set the end extend.
//-----------------------------------------------------------------------------
void NCGShading::SetEndExtend(bool theValue)
{


	// Set the end extend
	if (theValue != mEndExtend)
	{
		mEndExtend = theValue;
		SetDirty();
	}
}





//=============================================================================
//		NCGShading::SetEndRadius : Set the end radius.
//-----------------------------------------------------------------------------
void NCGShading::SetEndRadius(CGFloat theValue)
{


	// Validate our state
	NN_REQUIRE(mMode == NShadingMode::Radial);



	// Set the end radius
	if (theValue != mStartRadius)
	{
		mEndRadius = theValue;
		SetDirty();
	}
}





//=============================================================================
//		NCGShading::SetStartEndPoints : Set the start/end points.
//-----------------------------------------------------------------------------
void NCGShading::SetStartEndPoints(const NRectangle& theRect, NPosition startPos, NPosition endPos)
{


	// Update our state
	SetStartPoint(theRect.GetPoint(startPos));
	SetEndPoint(theRect.GetPoint(endPos));
}





//=============================================================================
//		NCGShading::operator CGShadingRef : Cast operator.
//-----------------------------------------------------------------------------
NCGShading::operator CGShadingRef() const
{


	// Get the shading
	if (!mShading.IsValid())
	{
		CreateShading();
	}

	return mShading;
}





//=============================================================================
//		NCGShading::Evaluate : Evaluate the shading.
//-----------------------------------------------------------------------------
#pragma mark -
NColor NCGShading::Evaluate(CGFloat theSample)
{


	// Evaluate the shading
	//
	// Provides a linear interpolation through the color sample list, by
	// identifying the relevant pair of colors for this sample and using
	// the color values to determine where each color starts/stops.
	//
	// We special case shadings of 1 or 2 colors, and ignore the sample
	// values to perform a straight no/pair shading.
	NColor theResult;

	if (mSamples.size() == 1)
	{
		theResult = mSamples[0].theColor;
	}
	else if (mSamples.size() == 2)
	{
		theResult =
			NColor::Interpolate(mSamples[0].theColor, mSamples[1].theColor, float32_t(theSample));
	}
	else
	{
		// Find the appropriate pair
		theResult = mSamples[0].theColor;
		size_t n  = mSamples.size();

		do
		{
			n--;

			if (theSample >= mSamples[n].theValue)
			{
				// Use the last color
				if (n == (mSamples.size() - 1))
				{
					theResult = mSamples[n].theColor;
				}


				// Use the pair of colors
				else
				{
					NColor colorOne = mSamples[n + 0].theColor;
					NColor colorTwo = mSamples[n + 1].theColor;

					CGFloat theRange  = mSamples[n + 1].theValue - mSamples[n].theValue;
					CGFloat theWeight = (theSample - mSamples[n].theValue) / theRange;

					theResult = NColor::Interpolate(colorOne, colorTwo, float32_t(theWeight));
				}

				break;
			}
		} while (n != 0);
	}

	return theResult;
}





#pragma mark private
//=============================================================================
//		NCGShading::CreateShading : Create the shading.
//-----------------------------------------------------------------------------
void NCGShading::CreateShading() const
{


	// Validate our state
	NN_REQUIRE(!mShading.IsValid() && !mEvaluate.IsValid());



	// Create the evaluate callback
	static const CGFunctionCallbacks kNCallbackInfo    = {0, NCGShading::EvaluateCallback, nullptr};
	static const CGFloat             kNCallbackRange[] = {0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0};
	static const CGFloat             kNCallbackDomain[] = {0.0, 1.0};

	void* userData = const_cast<void*>(reinterpret_cast<const void*>(this));
	bool  wasOK    = mEvaluate.Set(
		CGFunctionCreate(userData, 1, kNCallbackDomain, 4, kNCallbackRange, &kNCallbackInfo));
	NN_EXPECT(wasOK);



	// Create the shading
	if (wasOK)
	{
		NCGColorSpace cgColorSpace = NCGColor::GetDeviceRGB();

		switch (mMode)
		{
			case NShadingMode::None:
				// Invalid shading
				NN_LOG_ERROR("Unable to create NShadingMode::None shading!");
				break;

			case NShadingMode::Linear:
				wasOK = mShading.Set(CGShadingCreateAxial(cgColorSpace,
														  ToCG(mStartPoint),
														  ToCG(mEndPoint),
														  mEvaluate,
														  mStartExtend,
														  mEndExtend));
				NN_EXPECT(wasOK);
				break;

			case NShadingMode::Radial:
				wasOK = mShading.Set(CGShadingCreateRadial(cgColorSpace,
														   ToCG(mStartPoint),
														   mStartRadius,
														   ToCG(mEndPoint),
														   mEndRadius,
														   mEvaluate,
														   mStartExtend,
														   mEndExtend));
				NN_EXPECT(wasOK);
				break;
		}
	}
}





//=============================================================================
//		NCGShading::SetDirty : Invalidate the shading.
//-----------------------------------------------------------------------------
void NCGShading::SetDirty()
{


	// Invalidate the shading
	mShading.Set(nullptr);
	mEvaluate.Set(nullptr);
}





//=============================================================================
//		NCGShading::EvaluateCallback : Evaluate callback.
//-----------------------------------------------------------------------------
void NCGShading::EvaluateCallback(void* userData, const CGFloat* theSamples, CGFloat* theOutput)
{


	// Validate our parameters
	NN_REQUIRE(userData != nullptr);
	NN_REQUIRE(theSamples != nullptr);
	NN_REQUIRE(theOutput != nullptr);
	NN_REQUIRE(theSamples[0] >= 0.0 && theSamples[0] <= 1.0);



	// Evaluate the shading
	NCGShading* thisPtr  = reinterpret_cast<NCGShading*>(userData);
	NColor      theColor = thisPtr->Evaluate(theSamples[0]);

	theOutput[0] = CGFloat(theColor.GetRed());
	theOutput[1] = CGFloat(theColor.GetGreen());
	theOutput[2] = CGFloat(theColor.GetBlue());
	theOutput[3] = CGFloat(theColor.GetAlpha());
}

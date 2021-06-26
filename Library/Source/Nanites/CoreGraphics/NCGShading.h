/*	NAME:
		NCGShading.h

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
#ifndef NCGSHADING_H
#define NCGSHADING_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NCFObject.h"
#include "NColor.h"
#include "NPoint.h"
#include "NRectangle.h"

// System
#include <CoreGraphics/CGShading.h>
#include <vector>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Shading sample
struct NShadingSample
{
	CGFloat theValue;
	NColor  theColor;
};


// Containers
using NVectorShadingSample = std::vector<NShadingSample>;





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Shading mode
enum class NShadingMode
{
	None,
	Linear,
	Radial
};





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NCGShading : public NCFObject<CGShadingRef>
{
public:
										NCGShading();
	virtual                            ~NCGShading() override = default;

										NCGShading(const NCGShading& otherShading);
	NCGShading&                         operator=( const NCGShading& otherShading);

										NCGShading(NCGShading&& otherShading);
	NCGShading&                         operator=( NCGShading&& otherShading);


	// Is the object valid?
	bool                                IsValid() const;


	// Get/set the mode
	NShadingMode                        GetMode() const;
	void                                SetMode(NShadingMode theMode);


	// Get/set the samples
	//
	// Shading samples lie between 0.0 and 1.0, and should be sorted in increasing order.
	//
	// The default Evaluate method will perform a linear interpolation between pairs of
	// colors, where the sample value indicates the location within the 0.0 to 1.0 sample
	// space where that color forms 100% of the output.
	//
	// If the color sample list contains either 1 or 2 entries the default Evaluate method
	// ignores the sample values and displays a single colour or a linear interpolation.
	NVectorShadingSample                GetSamples() const;
	void                                SetSamples(  const NVectorShadingSample& theSamples);


	// Get/set the start point
	NPoint                              GetStartPoint() const;
	void                                SetStartPoint(  const NPoint& thePoint);


	// Get/set the start extend
	bool                                GetStartExtend() const;
	void                                SetStartExtend(bool theValue);


	// Get/set the start radius
	//
	// Only valid for radial shadings.
	CGFloat                             GetStartRadius() const;
	void                                SetStartRadius(CGFloat theValue);


	// Get/set the end point
	NPoint                              GetEndPoint() const;
	void                                SetEndPoint(  const NPoint& thePoint);


	// Get/set the end extend
	bool                                GetEndExtend() const;
	void                                SetEndExtend(bool theValue);


	// Get/set the end radius
	//
	// Only valid for radial shadings.
	CGFloat                             GetEndRadius() const;
	void                                SetEndRadius(CGFloat theValue);


	// Set the start/end points
	//
	// Adjusts the start and end points of the shading based on two points in a rectangle.
	void                                SetStartEndPoints(const NRectangle& theRect, NPosition startPos, NPosition endPos);


	// Operators
										operator CGShadingRef() const;


protected:
	// Evaluate the shading
	//
	// Evaluates the shading at the specified sample between 0.0 and 1.0.
	virtual NColor                      Evaluate(CGFloat theSample);


private:
	void                                CreateShading() const;
	void                                SetDirty();

	static void                         EvaluateCallback(void* userData, const CGFloat* theSamples, CGFloat* theOutput);



private:
	mutable NCFObject<CGShadingRef>     mShading;
	mutable NCFObject<CGFunctionRef>    mEvaluate;

	NShadingMode                        mMode;
	NVectorShadingSample                mSamples;

	NPoint                              mStartPoint;
	bool                                mStartExtend;
	CGFloat                             mStartRadius;

	NPoint                              mEndPoint;
	bool                                mEndExtend;
	CGFloat                             mEndRadius;
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NCGShading.inl"



#endif // NCGSHADING_H

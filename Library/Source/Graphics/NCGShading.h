/*	NAME:
		NCGShading.h

	DESCRIPTION:
		CoreGraphics shading object.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCGSHADING_HDR
#define NCGSHADING_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <vector>

#include "NMathUtilities.h"

#include "NCFObject.h"
#include "NColor.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Color sample
typedef struct {
	float	theValue;
	NColor	theColor;
} NShadingSample;


// Lists
typedef std::vector<NShadingSample>									NShadingSampleList;
typedef NShadingSampleList::iterator								NShadingSampleListIterator;
typedef NShadingSampleList::const_iterator							NShadingSampleListConstIterator;





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Shading mode
typedef enum {
	kShadingNone,
	kShadingLinear,
	kShadingRadial
} NShadingMode;


// Linear orientation
typedef enum {
	kLinearTopBottom,
	kLinearBottomTop,
	kLinearLeftRight,
	kLinearRightLeft
} NLinearOrientation;


// Shadings
extern const NShadingSample kShadingBlackWhite[];
extern const NShadingSample kShadingWhiteBlack[];
extern const NShadingSample kShadingFrontRow[];





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCGShading : public NCFObject {
public:
										NCGShading(NShadingMode theMode=kShadingLinear);
	virtual								~NCGShading(void);


	// Get/set the shading mode
	NShadingMode						GetMode(void) const;
	void								SetMode(NShadingMode theMode);
	
	
	// Get/set the shading samples
	//
	// Shading samples typically range from 0.0 to 1.0, and are sorted by value from
	// 0.0 to 1.0.
	//
	// The default Evaluate method will perform a linear interpolation between pairs
	// of colors, where the color sample value indicates the location within the 0.0
	// to 1.0 sample space where that color forms 100% of the output.
	//
	// If the color sample list contains 1 or 2 entries, the default Evaluate method
	// ignores the sample values and performs a simple linear interpolation.
	//
	// SetShadingSamples may be called with a pointer to an NShadingSample array,
	// which must be terminated with a sample whose value is exactly 1.0.
	NShadingSampleList					GetSamples(void) const;
	void								SetSamples(const NShadingSampleList &theSamples);
	void								SetSamples(const NShadingSample     *theSamples);
	

	// Get/set the start point
	CGPoint								GetStartPoint(void) const;
	void								SetStartPoint(const CGPoint &thePoint);


	// Get/set the start extend
	bool								GetStartExtend(void) const;
	void								SetStartExtend(bool theValue);


	// Get/set the start radius
	//
	// Only used by radial shadings.
	float								GetStartRadius(void) const;
	void								SetStartRadius(float theValue);


	// Get/set the end point
	CGPoint								GetEndPoint(void) const;
	void								SetEndPoint(const CGPoint &thePoint);


	// Get/set the end extend
	bool								GetEndExtend(void) const;
	void								SetEndExtend(bool theValue);


	// Get/set the end radius
	//
	// Only used by radial shadings.
	float								GetEndRadius(void) const;
	void								SetEndRadius(float theValue);


	// Apply a linear shading
	//
	// Adjusts the start and end points of the shading based on the orientation, where
	// a is the top/left edge and b is the bottom/right edge of an appropriate rectangle.
	void								ApplyLinearShading(NLinearOrientation theOrientation, float a, float b);


	// Operators
	operator							CGShadingRef(void) const;


protected:
	// Evaluate the shading
	//
	// Evaluates the shading at the specified sample, between 0.0 and 1.0.
	virtual NColor						Evaluate(float theSample);


	// Interpolate between two colors
	NColor								InterpolateColor(float theWeight, const NColor &firstColor, const NColor &secondColor);


private:
	void								UpdateShading(void);

	static void							EvaluateCallback(void *info, const float *in, float *out);


private:
	bool								mIsValid;

	NShadingMode						mMode;
	NShadingSampleList					mSamples;
	CGFunctionRef						mEvaluate;
	
	CGPoint								mStartPoint;
	bool								mStartExtend;
	float								mStartRadius;

	CGPoint								mEndPoint;
	bool								mEndExtend;
	float								mEndRadius;
};





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
inline bool operator !=(const NShadingSample &value1, const NShadingSample &value2)
{
	return(NMathUtilities::NotEqual(value1.theValue, value2.theValue) || value1.theColor != value2.theColor);
}







#endif // NCGSHADING_HDR




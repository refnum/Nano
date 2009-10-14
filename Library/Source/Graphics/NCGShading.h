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

#include "NCFObject.h"
#include "NColor.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Color sample
typedef struct {
	float	theValue;
	NColor	theColor;
} ColorSample;


// Lists
typedef std::vector<ColorSample>									ColorSampleList;
typedef ColorSampleList::iterator									ColorSampleListIterator;
typedef ColorSampleList::const_iterator								ColorSampleListConstIterator;





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Shading mode
typedef enum {
	kShadingNone,
	kShadingLinear,
	kShadingRadial
} CGShadingMode;


// Shadings
extern const ColorSample kShadingBlackWhite[];
extern const ColorSample kShadingWhiteBlack[];
extern const ColorSample kShadingFrontRow[];





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCGShading : public NCFObject {
public:
										NCGShading(CGShadingMode theMode=kShadingLinear);
	virtual								~NCGShading(void);


	// Get/set the shading mode
	CGShadingMode						GetMode(void) const;
	void								SetMode(CGShadingMode theMode);
	
	
	// Get/set the color samples
	//
	// Color sample values would typically range from 0.0 to 1.0 and be sorted by
	// value, with the smallest value first.
	//
	// The default Evaluate method will perform a linear interpolation between pairs
	// of colors, where the color sample value indicates the location within the 0.0
	// to 1.0 sample space where that color forms 100% of the output.
	//
	// If the color sample list contains 1 or 2 entries, the default Evaluate method
	// ignores the sample values and performs a simple linear interpolation.
	//
	// SetColorSamples may be called with a pointer to a ColorSample array, which
	// must be terminated with a sample whose value is exactly 1.0.
	ColorSampleList						GetColorSamples(void) const;
	void								SetColorSamples(const ColorSampleList &theSamples);
	void								SetColorSamples(const ColorSample     *theSamples);
	

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


protected:
	// Evaluate the shading
	//
	// Evaluates the shading at the specified sample, between 0.0 and 1.0.
	virtual NColor						Evaluate(float theSample);


	// Interpolate between two colors
	NColor								InterpolateColor(float theWeight, const NColor &firstColor, const NColor &secondColor);


private:
	void								CreateShading(void);

	static void							EvaluateCallback(void *info, const float *in, float *out);


private:
	CGShadingMode						mMode;
	ColorSampleList						mSamples;
	CGFunctionRef						mEvaluate;
	
	CGPoint								mStartPoint;
	bool								mStartExtend;
	float								mStartRadius;

	CGPoint								mEndPoint;
	bool								mEndExtend;
	float								mEndRadius;
};








#endif // NCGSHADING_HDR



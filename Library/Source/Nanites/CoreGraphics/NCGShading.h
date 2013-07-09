/*	NAME:
		NCGShading.h

	DESCRIPTION:
		CoreGraphics shading object.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCGSHADING_HDR
#define NCGSHADING_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMathUtilities.h"

#include "NCFObject.h"
#include "NRectangle.h"
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


// Shadings
extern const NShadingSample kShadingBlackWhite[];
extern const NShadingSample kShadingWhiteBlack[];
extern const NShadingSample kShadingFrontRow[];





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCGShading {
public:
										NCGShading(const NCGShading &theShading);

										NCGShading(NShadingMode theMode=kShadingLinear);
	virtual							   ~NCGShading(void);


	// Get/set the mode
	NShadingMode						GetMode(void) const;
	void								SetMode(NShadingMode theMode);
	
	
	// Get/set the samples
	//
	// Shading samples lie between 0.0 and 1.0, and should be sorted in increasing order.
	//
	// The default Evaluate method will perform a linear interpolation between pairs of
	// colors, where the sample value indicates the location within the 0.0 to 1.0 sample
	// space where that color forms 100% of the output.
	//
	// If the color sample list contains either 1 or 2 entries, the default Evaluate method
	// ignores the sample values and displays a single colour or a linear interpolation.
	//
	// SetShadingSamples may be called with a pointer to an NShadingSample array, which
	// must be terminated with a sample whose value is exactly 1.0.
	NShadingSampleList					GetSamples(void) const;
	void								SetSamples(const NShadingSampleList &theSamples);
	void								SetSamples(const NShadingSample     *theSamples);


	// Get/set the start point
	NPoint								GetStartPoint(void) const;
	void								SetStartPoint(const NPoint &thePoint);


	// Get/set the start extend
	bool								GetStartExtend(void) const;
	void								SetStartExtend(bool theValue);


	// Get/set the start radius
	//
	// Only used by radial shadings.
	float								GetStartRadius(void) const;
	void								SetStartRadius(float theValue);


	// Get/set the end point
	NPoint								GetEndPoint(void) const;
	void								SetEndPoint(const NPoint &thePoint);


	// Get/set the end extend
	bool								GetEndExtend(void) const;
	void								SetEndExtend(bool theValue);


	// Get/set the end radius
	//
	// Only used by radial shadings.
	float								GetEndRadius(void) const;
	void								SetEndRadius(float theValue);


	// Set the start/end points
	//
	// Adjusts the start and end points of the shading based on two points in a rectangle.
	void								SetStartEndPoints(const NRectangle &theRect, NPosition startPos, NPosition endPos);


	// Operators
	operator							CGShadingRef(void) const;
	const NCGShading&					operator = (const NCGShading &theShading);


protected:
	// Evaluate the shading
	//
	// Evaluates the shading at the specified sample, between 0.0 and 1.0.
	virtual NColor						Evaluate(float theSample);


private:
	void								InitializeSelf(void);
	
	void								ResetShading( void) const;
	void								UpdateShading(void) const;
	void								CopyShading(const NCGShading &theShading);

	CGFunctionRef						CreateEvaluateCallback(void) const;

	static void							EvaluateCallback(void *info, const CGFloat *in, CGFloat *out);


private:
	NShadingMode						mMode;
	NShadingSampleList					mSamples;

	mutable NCFObject					mShading;
	mutable NCFObject					mEvaluate;
	
	NPoint								mStartPoint;
	bool								mStartExtend;
	float								mStartRadius;

	NPoint								mEndPoint;
	bool								mEndExtend;
	float								mEndRadius;
};





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
inline bool operator != (const NShadingSample &value1, const NShadingSample &value2)
{
	return(!NMathUtilities::AreEqual(value1.theValue, value2.theValue) || value1.theColor != value2.theColor);
}







#endif // NCGSHADING_HDR




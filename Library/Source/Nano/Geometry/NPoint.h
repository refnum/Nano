/*	NAME:
		NPoint.h

	DESCRIPTION:
		Point object.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NPOINT_HDR
#define NPOINT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NStringFormatter.h"
#include "NEncodable.h"
#include "NComparable.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
class NPoint;
class NVector;

extern const NPoint kNPointZero;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NPoint :	public NEncodable,
				public NComparable<NPoint> {
public:
										NENCODABLE_DECLARE(NPoint);

										 NPoint(Float32 x=0.0f, Float32 y=0.0f);
	virtual								~NPoint(void);


	// Clear the point
	void								Clear(void);


	// Compare the value
	NComparison							Compare(const NPoint &theValue) const;


	// Test the point
	bool								IsZero(void) const;


	// Add/subtract a vector
	void								Add(     const NVector &theVector);
	void								Subtract(const NVector &theVector);


	// Get the distance to a points
	Float32								GetDistance( const NPoint &thePoint) const;
	Float32								GetDistance2(const NPoint &thePoint) const;


	// Operators
										operator NFormatArgument(void) const;


protected:
	// Encode/decode the object
	void								EncodeSelf(      NEncoder &theEncoder) const;
	void								DecodeSelf(const NEncoder &theEncoder);


public:
	Float32								x;
	Float32								y;
};




#endif // NPOINT_HDR



/*	NAME:
		NVector.h

	DESCRIPTION:
		Vector object.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NVECTOR_HDR
#define NVECTOR_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NStringFormatter.h"
#include "NEncodable.h"
#include "NComparable.h"
#include "NPoint.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
class NVector;

extern const NVector kNVectorZero;
extern const NVector kNVectorNorth;
extern const NVector kNVectorSouth;
extern const NVector kNVectorEast;
extern const NVector kNVectorWest;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NVector :	public NEncodable,
				public NComparable<NVector> {
public:
										NENCODABLE_DECLARE(NVector);

										 NVector(const NPoint &point1, const NPoint &point2);
										 NVector(Float32 x=0.0f, Float32 y=0.0f);
	virtual								~NVector(void);


	// Clear the vector
	void								Clear(void);


	// Compare the value
	NComparison							Compare(const NVector &theValue) const;


	// Test the vector
	bool								IsZero      (void) const;
	bool								IsNormalized(void) const;


	// Add/subtract a vector
	void								Add(     const NVector &theVector);
	void								Subtract(const NVector &theVector);


	// Manipulate the vector
	void								Scale(Float32 s);
	void								Negate(   void);
	void								Normalize(void);


	// Get the length
	Float32								GetLength (void) const;
	Float32								GetLength2(void) const;


	// Get the dot/cross products
	Float32								GetDot(  const NVector &theVector) const;
	Float32								GetCross(const NVector &theVector) const;


	// Get the angle between two vectors
	NDegrees							GetAngle(const NVector &theVector) const;


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




#endif // NVECTOR_HDR



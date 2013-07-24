/*	NAME:
		NNumber.h

	DESCRIPTION:
		Number object.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NNUMBER_HDR
#define NNUMBER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NStringFormatter.h"
#include "NComparable.h"
#include "NEncodable.h"
#include "NString.h"





//============================================================================
//      Constants
//----------------------------------------------------------------------------
// String values
static const NString kNStringInfinityNeg								= "-infinity";
static const NString kNStringInfinityPos								= "+infinity";
static const NString kNStringNaN										= "nan";


// Numerical precision
typedef enum {
	kNPrecisionInt8,
	kNPrecisionInt16,
	kNPrecisionInt32,
	kNPrecisionInt64,
	kNPrecisionFloat32,
	kNPrecisionFloat64
} NPrecision;





//============================================================================
//      Types
//----------------------------------------------------------------------------
// Values
typedef union {
	int64_t			integer;
	float64_t		real;
} NNumberValue;


// Classes
class NVariant;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NNumber :	public NEncodable,
				public NComparable<NNumber> {
public:
										NENCODABLE_DECLARE(NNumber);

										NNumber(const NVariant &theValue);
										NNumber(const NString  &theValue);
										NNumber(      int64_t   theValue);
										NNumber(      float32_t theValue);
										NNumber(      float64_t theValue);

										NNumber(void);
	virtual							   ~NNumber(void);


	// Is the number an integer?
	bool								IsInteger(void) const;


	// Get the precision
	//
	// Numbers may be stored with a greater precision than an assigned
	// value, however the internal precision will never be lossy.
	NPrecision							GetPrecision(void) const;


	// Get the number as a string
	//
	// May return kNStringNaN or kNStringInfinityXXX.
	NString								GetString(void) const;


	// Compare the value
	NComparison							Compare(const NNumber &theValue) const;


	// Get/set the value
	//
	// If the value needs to be cast from a different precision, conversion
	// is performed using C's normal (potentially lossy) promotion rules.
	uint8_t								GetUInt8(  void) const;
	uint16_t							GetUInt16( void) const;
	uint32_t							GetUInt32( void) const;
	uint64_t							GetUInt64( void) const;
	int8_t								GetInt8(   void) const;
	int16_t								GetInt16(  void) const;
	int32_t								GetInt32(  void) const;
	int64_t								GetInt64(  void) const;
	float32_t							GetFloat32(void) const;
	float64_t							GetFloat64(void) const;

	void								SetUInt8  (uint8_t   theValue);
	void								SetUInt16 (uint16_t  theValue);
	void								SetUInt32 (uint32_t  theValue);
	void								SetUInt64 (uint64_t  theValue);
	void								SetInt8   (int8_t    theValue);
	void								SetInt16  (int16_t   theValue);
	void								SetInt32  (int32_t   theValue);
	void								SetInt64  (int64_t   theValue);
	void								SetFloat32(float32_t theValue);
	void								SetFloat64(float64_t theValue);


	// Set a value
	bool								SetValue(const NVariant &theValue);
	bool								SetValue(const NString  &theValue);


	// Operators
										operator NFormatArgument(void) const;


protected:
	// Encode/decode the object
	void								EncodeSelf(      NEncoder &theEncoder) const;
	void								DecodeSelf(const NEncoder &theEncoder);


private:
	NPrecision							mPrecision;
	NNumberValue						mValue;
};





#endif // NNUMBER_HDR



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
	SInt64		integer;
	Float64		real;
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
										NNumber(      SInt64    theValue);
										NNumber(      Float32   theValue);
										NNumber(      Float64   theValue);

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
	UInt8								GetUInt8(  void) const;
	UInt16								GetUInt16( void) const;
	UInt32								GetUInt32( void) const;
	UInt64								GetUInt64( void) const;
	SInt8								GetSInt8(  void) const;
	SInt16								GetSInt16( void) const;
	SInt32								GetSInt32( void) const;
	SInt64								GetSInt64( void) const;
	Float32								GetFloat32(void) const;
	Float64								GetFloat64(void) const;

	void								SetUInt8  (UInt8   theValue);
	void								SetUInt16 (UInt16  theValue);
	void								SetUInt32 (UInt32  theValue);
	void								SetUInt64 (UInt64  theValue);
	void								SetSInt8  (SInt8   theValue);
	void								SetSInt16 (SInt16  theValue);
	void								SetSInt32 (SInt32  theValue);
	void								SetSInt64 (SInt64  theValue);
	void								SetFloat32(Float32 theValue);
	void								SetFloat64(Float64 theValue);


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



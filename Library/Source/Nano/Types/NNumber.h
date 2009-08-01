/*	NAME:
		NNumber.h

	DESCRIPTION:
		Number object.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NNUMBER_HDR
#define NNUMBER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NComparable.h"
#include "NString.h"





//============================================================================
//      Constants
//----------------------------------------------------------------------------
// String values
static const NString kNStringInfinityNeg								= "-infinity";
static const NString kNStringInfinityPos								= "+infinity";
static const NString kNStringNaN										= "nan";


// Types
typedef enum {
	kNumberInteger,
	kNumberReal
} NumberType;





//============================================================================
//      Types
//----------------------------------------------------------------------------
// Values
typedef union {
	SInt64		integer;
	Float64		real;
} NumberValue;


// Classes
class NVariant;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NNumber : public NComparable<NNumber> {
public:
										 NNumber(const NVariant &theValue);

										 NNumber(void);
	virtual								~NNumber(void);


	// Get the type
	NumberType							GetType(void) const;


	// Get the number as a string
	//
	// Returns kNStringNaN or kNStringInfinityXXX as necessary.
	NString								GetString(void) const;


	// Compare the value
	NComparison							Compare(const NNumber &theValue) const;


	// Get/set the value
	//
	// Numbers will fail to coerce if casting would lose precision.
	bool								GetValueUInt8(  UInt8   &theValue) const;
	bool								GetValueUInt16( UInt16  &theValue) const;
	bool								GetValueUInt32( UInt32  &theValue) const;
	bool								GetValueUInt64( UInt64  &theValue) const;
	bool								GetValueSInt8(  SInt8   &theValue) const;
	bool								GetValueSInt16( SInt16  &theValue) const;
	bool								GetValueSInt32( SInt32  &theValue) const;
	bool								GetValueSInt64( SInt64  &theValue) const;
	bool								GetValueFloat32(Float32 &theValue) const;
	bool								GetValueFloat64(Float64 &theValue) const;

	void								SetValueUInt8  (UInt8   theValue);
	void								SetValueUInt16 (UInt16  theValue);
	void								SetValueUInt32 (UInt32  theValue);
	void								SetValueUInt64 (UInt64  theValue);
	void								SetValueSInt8  (SInt8   theValue);
	void								SetValueSInt16 (SInt16  theValue);
	void								SetValueSInt32 (SInt32  theValue);
	void								SetValueSInt64 (SInt64  theValue);
	void								SetValueFloat32(Float32 theValue);
	void								SetValueFloat64(Float64 theValue);


	// Set a value
	bool								SetValue(const NVariant &theValue);
	bool								SetValue(const NString  &theValue);


private:
	NumberType							mType;
	NumberValue							mValue;
};





#endif // NNUMBER_HDR



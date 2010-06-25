/*	NAME:
		NColor.h

	DESCRIPTION:
		Color object.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCOLOR_HDR
#define NCOLOR_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NStringFormatter.h"
#include "NComparable.h"
#include "NEncodable.h"
#include "NString.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Color formats
//
// Color formats define the order of components within a representation, not
// a specific byte order.
//
// E.g., when represented as a UInt32, a kNColorARGB would be stored as:
//
//		NN_TARGET_ENDIAN_BIG		[AA] [RR] [GG] [BB]
//		NN_TARGET_ENDIAN_LITTLE		[BB] [GG] [RR] [AA]
//
// In both cases, the alpha byte could be accessed by a right shift of 24.
typedef enum {
	kNColorARGB,
	kNColorRGBA,
} NColorFormat;


// Colors
class NColor;

extern const NColor kNColorBlack;
extern const NColor kNColorBlue;
extern const NColor kNColorBrown;
extern const NColor kNColorClear;
extern const NColor kNColorCyan;
extern const NColor kNColorGray;
extern const NColor kNColorGrayDark;
extern const NColor kNColorGrayLight;
extern const NColor kNColorGreen;
extern const NColor kNColorMagenta;
extern const NColor kNColorOrange;
extern const NColor kNColorPurple;
extern const NColor kNColorRed;
extern const NColor kNColorWhite;
extern const NColor kNColorYellow;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NColor :	public NEncodable,
				public NComparable<NColor> {
public:
										NENCODABLE_DECLARE(NColor);

										NColor(Float32 r, Float32 g, Float32 b, Float32 a=1.0f);
										NColor(      UInt32    theColor, NColorFormat theFormat=kNColorRGBA);
										NColor(const NString  &hexColor, NColorFormat theFormat=kNColorRGBA);
										
										NColor(void);
	virtual							   ~NColor(void);


	// Compare the value
	NComparison							Compare(const NColor &theValue) const;


	// Get/set the color
	void								GetColor(Float32 &r, Float32 &g, Float32 &b, Float32 &a) const;
	UInt32								GetColor(NColorFormat theFormat=kNColorRGBA)             const;

	void								SetColor(Float32 r, Float32 g, Float32 b, Float32 a=1.0f);
	void								SetColor(UInt32 theColor, NColorFormat theFormat=kNColorRGBA);
	

	// Get/set the components
	Float32								GetRed(  void) const;
	Float32								GetGreen(void) const;
	Float32								GetBlue( void) const;
	Float32								GetAlpha(void) const;

	void								SetRed(  Float32 theValue);
	void								SetGreen(Float32 theValue);
	void								SetBlue( Float32 theValue);
	void								SetAlpha(Float32 theValue);


	// Operators
										operator NFormatArgument(void) const;


protected:
	// Encode/decode the object
	void								EncodeSelf(      NEncoder &theEncoder) const;
	void								DecodeSelf(const NEncoder &theEncoder);


private:
	Float32								mRed;
	Float32								mGreen;
	Float32								mBlue;
	Float32								mAlpha;
};









#endif // NCOLOR_HDR



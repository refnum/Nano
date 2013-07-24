/*	NAME:
		NColor.h

	DESCRIPTION:
		Color object.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
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
// E.g., when represented as a uint32_t, a kNColorARGB would be stored as:
//
//		NN_TARGET_ENDIAN_BIG		[AA] [RR] [GG] [BB]
//		NN_TARGET_ENDIAN_LITTLE		[BB] [GG] [RR] [AA]
//
// In both cases, the alpha byte could be accessed by a right shift of 24.
typedef enum {
	kNColorARGB,
	kNColorRGBA
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

										NColor(float32_t r, float32_t g, float32_t b, float32_t a=1.0f);
										NColor(      uint32_t  theColor, NColorFormat theFormat=kNColorRGBA);
										NColor(const NString  &hexColor, NColorFormat theFormat=kNColorRGBA);
										
										NColor(void);
	virtual							   ~NColor(void);


	// Test the alpha
	bool								IsOpaque(     void) const;
	bool								IsTransparent(void) const;


	// Compare the value
	NComparison							Compare(const NColor &theValue) const;


	// Get/set the color
	void								GetColor(float32_t &r, float32_t &g, float32_t &b, float32_t &a) const;
	uint32_t							GetColor(NColorFormat theFormat=kNColorRGBA)                     const;

	void								SetColor(float32_t r, float32_t g, float32_t b, float32_t a=1.0f);
	void								SetColor(uint32_t theColor, NColorFormat theFormat=kNColorRGBA);
	

	// Get/set the components
	float32_t							GetRed(  void) const;
	float32_t							GetGreen(void) const;
	float32_t							GetBlue( void) const;
	float32_t							GetAlpha(void) const;

	void								SetRed(  float32_t theValue);
	void								SetGreen(float32_t theValue);
	void								SetBlue( float32_t theValue);
	void								SetAlpha(float32_t theValue);


	// Operators
										operator NFormatArgument(void) const;


	// Interpolate between two colors
	static NColor						Interpolate(const NColor &startColor, const NColor &endColor, float theProgress);


protected:
	// Encode/decode the object
	void								EncodeSelf(      NEncoder &theEncoder) const;
	void								DecodeSelf(const NEncoder &theEncoder);


private:
	float32_t							mRed;
	float32_t							mGreen;
	float32_t							mBlue;
	float32_t							mAlpha;
};









#endif // NCOLOR_HDR



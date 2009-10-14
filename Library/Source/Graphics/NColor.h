/*	NAME:
		NColor.h

	DESCRIPTION:
		Color object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCOLOR_HDR
#define NCOLOR_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NComparable.h"
#include "NString.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Color formats
typedef enum {
	kFormatRGBA
} ColorFormat;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NColor : public NComparable {
public:
										NColor(float r, float g, float b, float a=1.0f);
										NColor(      UInt32    theColor, ColorFormat theFormat=kFormatRGBA);
										NColor(const NString  &hexColor, ColorFormat theFormat=kFormatRGBA);
										
										NColor( void);
	virtual								~NColor(void);


	// Get/set the color
	void								GetColor(float &r, float &g, float &b, float &a) const;
	UInt32								GetColor(ColorFormat theFormat=kFormatRGBA)      const;
	void								GetColor(RGBColor &theColor)                     const;
	CGColorRef							CopyColor(void)                                  const;

	void								SetColor(float r, float g, float b, float a);
	void								SetColor(UInt32 theColor, ColorFormat theFormat=kFormatRGBA);
	void								SetColor(const RGBColor &theColor);
	void								SetColor(CGColorRef cgColor);
	

	// Get/set the components
	float								GetRed(  void) const;
	float								GetGreen(void) const;
	float								GetBlue( void) const;
	float								GetAlpha(void) const;

	void								SetRed(  float theValue);
	void								SetGreen(float theValue);
	void								SetBlue( float theValue);
	void								SetAlpha(float theValue);


protected:
	// Compare two objects
	CFComparisonResult					Compare(const NComparable &theObject) const;


private:
	float								mRed;
	float								mGreen;
	float								mBlue;
	float								mAlpha;
};





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const NColor kColorBlack						= NColor(0.00f, 0.00f, 0.00f, 1.0f);
static const NColor kColorBlue						= NColor(0.00f, 0.00f, 1.00f, 1.0f);
static const NColor kColorBrown						= NColor(0.60f, 0.40f, 0.20f, 1.0f);
static const NColor kColorClear						= NColor(1.00f, 1.00f, 1.00f, 0.0f);
static const NColor kColorCyan						= NColor(0.00f, 1.00f, 1.00f, 1.0f);
static const NColor kColorGray						= NColor(0.50f, 0.50f, 0.50f, 1.0f);
static const NColor kColorGrayDark					= NColor(0.33f, 0.33f, 0.33f, 1.0f);
static const NColor kColorGrayLight					= NColor(0.66f, 0.66f, 0.66f, 1.0f);
static const NColor kColorGreen						= NColor(0.00f, 1.00f, 0.00f, 1.0f);
static const NColor kColorMagenta					= NColor(1.00f, 0.00f, 1.00f, 1.0f);
static const NColor kColorOrange					= NColor(1.00f, 0.50f, 0.00f, 1.0f);
static const NColor kColorPurple					= NColor(0.50f, 0.00f, 0.50f, 1.0f);
static const NColor kColorRed						= NColor(1.00f, 0.00f, 0.00f, 1.0f);
static const NColor kColorWhite						= NColor(1.00f, 1.00f, 1.00f, 1.0f);
static const NColor kColorYellow					= NColor(1.00f, 1.00f, 0.00f, 1.0f);






#endif // NCOLOR_HDR



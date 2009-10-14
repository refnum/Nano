/*	NAME:
		NTextUtilities.cpp

	DESCRIPTION:
		Text utilities.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NGeometryUtilities.h"
#include "NTextUtilities.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Formats
//
// These strings are encoded to property lists, and can never change.
//
// A version number has been included in case "never" has to be broken.
static const char *kFormatPoint								= "0.{%f, %f}";
static const char *kFormatSize								= "0.{%f, %f}";
static const char *kFormatRect								= "0.{{%f, %f}, {%f, %f}}";
static const char *kFormatColor								= "0.{%f, %f, %f, %f}";
static const char *kFormatDate								= "0.{%d/%d/%d, %d:%d:%f}";





//============================================================================
//		NTextUtilities::GetOSType : Convert from a string.
//----------------------------------------------------------------------------
OSType NTextUtilities::GetOSType(const NString &theString)
{	OSType	theValue;



	// Convert the string
	//
	// OSType values are encoded to strings in big-endian order.
	theValue = 0;
	
	if (theString.GetSize() == 4)
		memcpy(&theValue, theString.GetUTF8(), 4);

	theValue = (OSType) EndianU32_BtoN((UInt32) theValue);
	
	return(theValue);
}





//============================================================================
//		NTextUtilities::GetUInt32 : Convert from a string.
//----------------------------------------------------------------------------
UInt32 NTextUtilities::GetUInt32(const NString &theString)
{	SInt32	theValue;



	// Convert the string
	theValue = CFStringGetIntValue(theString);
	
	return(theValue);
}





//============================================================================
//		NTextUtilities::GetFloat64 : Convert from a string.
//----------------------------------------------------------------------------
Float64 NTextUtilities::GetFloat64(const NString &theString)
{	Float64	theValue;



	// Convert the string
	theValue = CFStringGetDoubleValue(theString);
	
	return(theValue);
}





//============================================================================
//		NTextUtilities::GetStr255 : Convert from a string.
//----------------------------------------------------------------------------
void NTextUtilities::GetStr255(const NString &theString, Str255 &theValue, CFStringEncoding cfEncoding)
{


	// Convert the string
	if (!CFStringGetPascalString(theString, theValue, 256, cfEncoding))
		theValue[0] = 0x00;
}





//============================================================================
//		NTextUtilities::GetStr255Hnd : Convert from a string.
//----------------------------------------------------------------------------
//		Note :	Returns a Str255-in-a-Handle, a type used to build QuickTime
//				data references.
//----------------------------------------------------------------------------
Handle NTextUtilities::GetStr255Hnd(const NString &theString)
{	Str255		theValue;
	Handle		theHnd;
	OSStatus	theErr;



	// Convert the string
	GetStr255(theString, theValue);



	// Convert to a handle
	theHnd = NULL;
	theErr = PtrToHand(theValue, &theHnd, theValue[0] + 1);
	NN_ASSERT_NOERR(theErr);
	
	return(theHnd);
}





//============================================================================
//		NTextUtilities::GetPoint : Convert from a string.
//----------------------------------------------------------------------------
HIPoint NTextUtilities::GetPoint(const NString &theString)
{	HIPoint		theValue;



	// Convert the string
	if (sscanf(theString.GetUTF8(), kFormatPoint, &theValue.x, &theValue.y) != 2)
		theValue = kHIPointZero;
	
	return(theValue);
}





//============================================================================
//		NTextUtilities::GetSize : Convert from a string.
//----------------------------------------------------------------------------
HISize NTextUtilities::GetSize(const NString &theString)
{	HISize		theValue;



	// Convert the string
	if (sscanf(theString.GetUTF8(), kFormatSize, &theValue.width, &theValue.height) != 2)
		theValue = kHISizeZero;
	
	return(theValue);
}





//============================================================================
//		NTextUtilities::GetRect : Convert from a string.
//----------------------------------------------------------------------------
HIRect NTextUtilities::GetRect(const NString &theString)
{	HIRect		theValue;



	// Get the value
	if (sscanf(theString.GetUTF8(), kFormatRect, &theValue.origin.x,   &theValue.origin.y,
												 &theValue.size.width, &theValue.size.height) != 4)
		theValue = kHIRectZero;
	
	return(theValue);
}





//============================================================================
//		NTextUtilities::GetColor : Convert from a string.
//----------------------------------------------------------------------------
NColor NTextUtilities::GetColor(const NString &theString)
{	float		r, g, b, a;
	NColor		theValue;



	// Convert the string
	if (sscanf(theString.GetUTF8(), kFormatColor, &r, &g, &b, &a) == 4)
		theValue.SetColor(r, g, b, a);
	
	return(theValue);
}





//============================================================================
//		NTextUtilities::GetDate : Convert from a string.
//----------------------------------------------------------------------------
NCFDate NTextUtilities::GetDate(const NString &theString)
{	NCFDate				theValue;
	CFGregorianDate		theDate;



	// Convert the string
	if (sscanf(theString.GetUTF8(), kFormatDate,
					&theDate.year, &theDate.month,  &theDate.day,
					&theDate.hour, &theDate.minute, &theDate.second) == 6)
		theValue = NCFDate(theDate);
	
	return(theValue);
}





//============================================================================
//		NTextUtilities::FromOSType : Convert to a string.
//----------------------------------------------------------------------------
NString NTextUtilities::FromOSType(OSType theValue)
{	NString		theString;
	OSType		bigValue;



	// Convert the value
	//
	// OSType values are encoded to strings in big-endian order.
	bigValue  = (OSType) EndianU32_BtoN((UInt32) theValue);
	theString = NString((char *) &bigValue, 4);
	
	return(theString);
}





//============================================================================
//		NTextUtilities::FromUInt32 : Convert to a string.
//----------------------------------------------------------------------------
NString NTextUtilities::FromUInt32(UInt32 theValue)
{	NString		theString;



	// Convert the value
	theString.Format("%lu", theValue);
	
	return(theString);
}





//============================================================================
//		NTextUtilities::FromFloat64 : Convert to a string.
//----------------------------------------------------------------------------
NString NTextUtilities::FromFloat64(Float64 theValue)
{	NString		theString;



	// Convert the value
	theString.Format("%lf", theValue);
	
	return(theString);
}





//============================================================================
//		NTextUtilities::FromStr255 : Convert to a string.
//----------------------------------------------------------------------------
NString NTextUtilities::FromStr255(ConstStr255Param theValue, CFStringEncoding cfEncoding)
{	NString		theString;



	// Convert the value
	theString = NString((const char *) &theValue[1], theValue[0], cfEncoding);
	
	return(theString);
}





//============================================================================
//		NTextUtilities::FromPoint : Convert to a string.
//----------------------------------------------------------------------------
NString NTextUtilities::FromPoint(const HIPoint &theValue)
{	NString		theString;



	// Convert the value
	theString.Format(kFormatPoint, theValue.x, theValue.y);
	
	return(theString);
}





//============================================================================
//		NTextUtilities::FromSize : Convert to a string.
//----------------------------------------------------------------------------
NString NTextUtilities::FromSize(const HISize &theValue)
{	NString		theString;



	// Convert the value
	theString.Format(kFormatSize, theValue.width, theValue.height);
	
	return(theString);
}





//============================================================================
//		NTextUtilities::FromRect : Convert to a string.
//----------------------------------------------------------------------------
NString NTextUtilities::FromRect(const HIRect &theValue)
{	NString		theString;



	// Convert the value
	theString.Format(kFormatRect, theValue.origin.x,   theValue.origin.y,
								  theValue.size.width, theValue.size.height);
	
	return(theString);
}





//============================================================================
//		NTextUtilities::FromColor : Convert to a string.
//----------------------------------------------------------------------------
NString NTextUtilities::FromColor(const NColor &theValue)
{	NString		theString;



	// Convert the value
	theString.Format(kFormatColor, theValue.GetRed(),  theValue.GetGreen(),
								   theValue.GetBlue(), theValue.GetAlpha());
	
	return(theString);
}





//============================================================================
//		NTextUtilities::FromDate : Convert to a string.
//----------------------------------------------------------------------------
NString NTextUtilities::FromDate(const NCFDate &theValue)
{	NString				theString;
	CFGregorianDate		theDate;



	// Convert the value
	theDate = theValue.GetDate();
	
	theString.Format(kFormatDate, theDate.year, theDate.month,  theDate.day,
								  theDate.hour, theDate.minute, theDate.second);

	return(theString);
}





//============================================================================
//		NTextUtilities::EncodeURL : Encode a URL.
//----------------------------------------------------------------------------
NString NTextUtilities::EncodeURL(const NString &theURL, const NString &leaveEscaped, CFStringEncoding escapeEncoding)
{	NString		theValue;



	// Encode the URL
	theValue.Set(CFURLCreateStringByAddingPercentEscapes(NULL, theURL, leaveEscaped, NULL, escapeEncoding));

	return(theValue);
}





//============================================================================
//		NTextUtilities::DecodeURL : Decode a URL.
//----------------------------------------------------------------------------
NString NTextUtilities::DecodeURL(const NString &theURL, const NString &leaveEscaped, CFStringEncoding escapeEncoding)
{	NString		theValue;



	// Encode the URL
	theValue.Set(CFURLCreateStringByReplacingPercentEscapesUsingEncoding(NULL, theURL, leaveEscaped, escapeEncoding));

	return(theValue);
}


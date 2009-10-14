/*	NAME:
		NTextUtilities.h

	DESCRIPTION:
		Text utilities.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTEXTUTILITIES_HDR
#define NTEXTUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NColor.h"
#include "NCFDate.h"
#include "NString.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTextUtilities {
public:
	// Convert to/from a string
	static OSType						GetOSType(   const NString &theString);
	static UInt32						GetUInt32(   const NString &theString);
	static Float64						GetFloat64(  const NString &theString);
	static void							GetStr255(   const NString &theString, Str255 &theValue);
	static Handle						GetStr255Hnd(const NString &theString);
	static HIPoint						GetPoint(    const NString &theString);
	static HISize						GetSize(     const NString &theString);
	static HIRect						GetRect(     const NString &theString);
	static NColor						GetColor(    const NString &theString);
	static NCFDate						GetDate(     const NString &theString);

	static NString						FromOSType( OSType            theValue);
	static NString						FromUInt32( UInt32            theValue);
	static NString						FromFloat64(Float64           theValue);
	static NString						FromStr255( ConstStr255Param  theValue);
	static NString						FromPoint(  const HIPoint    &theValue);
	static NString						FromSize(   const HISize     &theValue);
	static NString						FromRect(   const HIRect     &theValue);
	static NString						FromColor(  const NColor     &theValue);
	static NString						FromDate(   const NCFDate    &theValue);


	// Encode/decode a URL string
	static NString						EncodeURL(const NString &theURL, const NString &leaveEscaped="", CFStringEncoding escapeEncoding=kCFStringEncodingUTF8);
	static NString						DecodeURL(const NString &theURL, const NString &leaveEscaped="", CFStringEncoding escapeEncoding=kCFStringEncodingUTF8);
};





#endif // NTEXTUTILITIES_HDR



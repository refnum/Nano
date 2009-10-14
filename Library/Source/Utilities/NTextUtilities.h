/*	NAME:
		NTextUtilities.h

	DESCRIPTION:
		Text utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
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
#include "NString.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTextUtilities {
public:
	// Convert to/from a string
	static OSType						GetOSType(   const NString &theString);
	static SInt64						GetSInt64(   const NString &theString);
	static Float64						GetFloat64(  const NString &theString);
	static void							GetStr255(   const NString &theString, Str255 &theValue, CFStringEncoding cfEncoding=CFStringGetSystemEncoding());
	static Handle						GetStr255Hnd(const NString &theString);
	static HIPoint						GetPoint(    const NString &theString);
	static HISize						GetSize(     const NString &theString);
	static HIRect						GetRect(     const NString &theString);
	static NColor						GetColor(    const NString &theString);

	static NString						FromOSType( OSType            theValue);
	static NString						FromSInt64( SInt64            theValue);
	static NString						FromFloat64(Float64           theValue);
	static NString						FromStr255( ConstStr255Param  theValue, CFStringEncoding cfEncoding=CFStringGetSystemEncoding());
	static NString						FromPoint(  const HIPoint    &theValue);
	static NString						FromSize(   const HISize     &theValue);
	static NString						FromRect(   const HIRect     &theValue);
	static NString						FromColor(  const NColor     &theValue);


	// Encode/decode a URL string
	static NString						EncodeURL(const NString &theURL, CFStringRef leaveEscaped=NULL, CFStringEncoding escapeEncoding=kCFStringEncodingUTF8);
	static NString						DecodeURL(const NString &theURL, CFStringRef leaveEscaped=NULL, CFStringEncoding escapeEncoding=kCFStringEncodingUTF8);


	// Expand a tilde prefix
	static NString						ExpandTilde(const NString &thePath);


	// Collect a NULL-terminated argument list
	static NStringList					CollectArguments(const char *arg1, ...);
	static NStringList					CollectArguments(va_list argList, const char *arg1);


	// Get a transfer info string
	//
	// Returns a Finder-style string indicating the progress of a transfer operation,
	// based on the amount of data transferred, the transfer rate, and the time left.
	//
	// All values but the data transferred are optional.
	static NString						GetTransferInfo(UInt32		dataDone,
														UInt32		dataTotal   = (UInt32) -1,
														UInt32		bytesPerSec = (UInt32) -1,
														EventTime	timeLeft    = 0.0);


	// Combine a list of strings
	static NString						CombineStrings(const NStringList &theValues, const NString &joinWith=" ", bool skipEmpty=true);
};





#endif // NTEXTUTILITIES_HDR



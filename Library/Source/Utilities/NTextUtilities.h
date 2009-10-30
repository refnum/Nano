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
#include <map>

#include "NColor.h"
#include "NString.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const NString kLineEndingUnix								= "\x0A";
static const NString kLineEndingMac									= "\x0D";
static const NString kLineEndingDOS									= "\x0D\x0A";





//============================================================================
//		Types
//----------------------------------------------------------------------------
// String comparator
//
// Although this comparator produces a different ordering to NString::<, the
// ordering is stable and can be used as a more efficient comparator for data
// structures such as std::map.
struct StringCompare {
	bool operator()(const NString &value1, const NString &value2) const
	{	CFStringRef		object1, object2;
		CFHashCode		hash1, hash2;
		
		
		// Check the objects
		//
		// The same object pointer means the objects are equal.
		object1 = value1;
		object2 = value2;
		
		if (object1 == object2)
			return(object1 < object2);


		// Check the hash code
		//
		// A different hash code means the objects are not equal.
		hash1 = value1.GetHash();
		hash2 = value2.GetHash();
		
		if (hash1 != hash2)
			return(hash1 < hash2);
		
		
		// Check the string
		//
		// Anything else needs a general comparison.
		return(value1 < value2);
	}
};


// Containers
typedef std::map<NString, NString, StringCompare>				NStringMap;
typedef NStringMap::iterator									NStringMapIterator;
typedef NStringMap::const_iterator								NStringMapConstIterator;





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
	static NData						GetData(     const NString &theString);
	static void							*GetPointer( const NString &theString);

	static NString						FromOSType( OSType            theValue);
	static NString						FromSInt64( SInt64            theValue);
	static NString						FromFloat64(Float64           theValue);
	static NString						FromStr255( ConstStr255Param  theValue, CFStringEncoding cfEncoding=CFStringGetSystemEncoding());
	static NString						FromPoint(  const HIPoint    &theValue);
	static NString						FromSize(   const HISize     &theValue);
	static NString						FromRect(   const HIRect     &theValue);
	static NString						FromColor(  const NColor     &theValue);
	static NString						FromData(   const NData      &theValue);
	static NString						FromPointer(void             *theValue);


	// Encode/decode a URL string
	static NString						EncodeURL(const NString &theURL, CFStringRef leaveEscaped=NULL, CFStringEncoding escapeEncoding=kCFStringEncodingUTF8);
	static NString						DecodeURL(const NString &theURL, CFStringRef leaveEscaped=NULL, CFStringEncoding escapeEncoding=kCFStringEncodingUTF8);


	// Encode/decode HTML entities
	static NString						EncodeEntities(const NString &theString);
	static NString						DecodeEntities(const NString &theString);


	// Expand a tilde prefix
	static NString						ExpandTilde(const NString &thePath);


	// Normalise line endings
	static NString						NormalizeLineEndings(const NString &theString, const NString &newLine=kLineEndingUnix);


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


	// Get a hex dump
	static NString						GetHexDump(UInt32 theSize, const void *thePtr, char flowSign=0x00, bool hexOnly=false);


	// Combine a list of strings
	static NString						CombineStrings(const NStringList &theValues, const NString &joinWith=" ", bool skipEmpty=true);


private:
	static NString						GetHexLine(UInt32 lineOffset, UInt32 lineSize, UInt32 linePad, const UInt8 *theData, char flowSign, bool hexOnly);
};





#endif // NTEXTUTILITIES_HDR



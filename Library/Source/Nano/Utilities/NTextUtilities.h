/*	NAME:
		NTextUtilities.h

	DESCRIPTION:
		Text utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTEXTUTILITIES_HDR
#define NTEXTUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDictionary.h"
#include "NString.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const NString kNLineEndingUnix								= "\x0A";
static const NString kNLineEndingMac								= "\x0D";
static const NString kNLineEndingDOS								= "\x0D\x0A";





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTextUtilities {
public:
	// Escape/unescape XML entities
	//
	// Replaces special characters with their entities, or the reverse.
	//
	// The standard five entities are always replaced. Additional entities
	// can replaced with a custom dictionary.
	//
	// The keys of this dictionary are the entity characters ("&"), while the
	// values are their expansions ("&amp;"). The dictionary must be invertible.
	static NString						EscapeEntities(  const NString &theValue, const NDictionary &extraEntities=NDictionary());
	static NString						UnescapeEntities(const NString &theValue, const NDictionary &extraEntities=NDictionary());


	// Get a hex dump
	static NString						GetHexDump(NIndex theSize, const void *thePtr, char flowSign=0x00, bool hexOnly=false);


	// Convert line endings
	static NString						ConvertLineEndings(const NString &theString, const NString &newLine=kNLineEndingUnix);


private:
	static NDictionary					GetEntityDictionary(const NDictionary &extraEntities, bool forEscape);
	static void							ProcessEntities(const NString &theKey, const NVariant &theValue, NString *theResult);
	

	static NString						GetHexLine(NIndex lineOffset, NIndex lineSize, NIndex linePad, const UInt8 *theData, char flowSign, bool hexOnly);
};





#endif // NTEXTUTILITIES_HDR



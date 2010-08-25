/*	NAME:
		NTextUtilities.h

	DESCRIPTION:
		Text utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
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
// Tokens
static const NString kNTokenColon										= ":";
static const NString kNTokenComma										= ",";
static const NString kNTokenEquals										= "=";
static const NString kNTokenHypen										= "-";
static const NString kNTokenPipe										= "|";
static const NString kNTokenQuote										= "\"";
static const NString kNTokenQuoteSingle									= "'";
static const NString kNTokenSemicolon									= ";";
static const NString kNTokenSpace										= " ";


// Line endings
static const NString kNLineEndingUnix									= "\x0A";
static const NString kNLineEndingMac									= "\x0D";
static const NString kNLineEndingDOS									= "\x0D\x0A";





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTextUtilities {
public:
	// Encode/decode XML entities
	//
	// Encodes special characters into their entity form, or the reverse.
	//
	// The standard five entities are always replaced. Additional entities
	// can replaced with a custom dictionary.
	//
	// The keys of this dictionary are the entity characters ("&"), while the
	// values are their expansions ("&amp;"). The dictionary must be invertible.
	static NString						EncodeEntities(const NString &theValue, const NDictionary &extraEntities=NDictionary());
	static NString						DecodeEntities(const NString &theValue, const NDictionary &extraEntities=NDictionary());


	// Get a hex dump
	static NString						GetHexDump(NIndex theSize, const void *thePtr, char flowSign=0x00, bool hexOnly=false);


	// Convert line endings
	static NString						ConvertLineEndings(const NString &theString, const NString &newLine=kNLineEndingUnix);


	// Get a NULL-terminated argument list.
	static NStringList					GetArguments(const char *arg1, ...);
	static NStringList					GetArguments(va_list argList, const char *arg1);


private:
	static NDictionary					GetEntityDictionary(const NDictionary &extraEntities, bool forEscape);
	static void							ProcessEntities(const NString &theKey, const NVariant &theValue, NString *theResult);
	

	static NData						GetHexLine(NIndex lineOffset, NIndex lineSize, NIndex linePad, const UInt8 *theData, char flowSign, bool hexOnly);
};





#endif // NTEXTUTILITIES_HDR



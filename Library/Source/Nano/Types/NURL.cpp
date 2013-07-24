/*	NAME:
		NURL.cpp

	DESCRIPTION:
		Uniform Resource Locator.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NEncoder.h"
#include "NURL.h"





//============================================================================
//		Implementation
//----------------------------------------------------------------------------
NENCODABLE_DEFINE(NURL);





//============================================================================
//		NURL::NURL : Constructor.
//----------------------------------------------------------------------------
NURL::NURL(const NString &theURL)
{


	// Initialize ourselves
	mValue = theURL;
}





//============================================================================
//		NURL::NURL : Constructor.
//----------------------------------------------------------------------------
NURL::NURL(const char *theURL)
{


	// Initialize ourselves
	mValue = NString(theURL);
}





//============================================================================
//		NURL::NURL : Constructor.
//----------------------------------------------------------------------------
NURL::NURL(void)
{
}





//============================================================================
//		NURL::~NURL : Destructor.
//----------------------------------------------------------------------------
NURL::~NURL(void)
{
}





//============================================================================
//		NURL::IsValid : Is the UTI valid?
//----------------------------------------------------------------------------
bool NURL::IsValid(void) const
{


	// Check our state
	return(!mValue.IsEmpty());
}





//============================================================================
//		NURL::Clear : Clear the URL.
//----------------------------------------------------------------------------
void NURL::Clear(void)
{


	// Reset our state
	mValue.Clear();
}





//============================================================================
//		NURL::Compare : Compare the value.
//----------------------------------------------------------------------------
NComparison NURL::Compare(const NURL &theValue) const
{


	// Compare the value
	return(mValue.Compare(theValue.mValue));
}





//============================================================================
//		NURL::GetValue : Get the value.
//----------------------------------------------------------------------------
NString NURL::GetValue(void) const
{


	// Get the value
	return(mValue);
}





//============================================================================
//		NURL::SetValue : Set the value.
//----------------------------------------------------------------------------
void NURL::SetValue(const NString &theValue)
{


	// Set the value
	mValue = theValue;
}





//============================================================================
//		NURL::GetScheme : Get the scheme.
//----------------------------------------------------------------------------
NString NURL::GetScheme(void) const
{	NString		theResult;



	// Get the scheme
	theResult = GetToken("^(\\w+)://");
	
	return(theResult);
}





//============================================================================
//		NURL::GetHost : Get the host.
//----------------------------------------------------------------------------
NString NURL::GetHost(void) const
{	NString		theResult;



	// Get the scheme
	theResult = GetToken("^\\w+://(.*?)[:/]");
	
	return(theResult);
}





//============================================================================
//		NURL::GetPort : Get the port.
//----------------------------------------------------------------------------
NIndex NURL::GetPort(void) const
{	NString		theValue, theScheme;
	NNumber		theNumber;
	NIndex		theResult;



	// Get the port
	theValue  = GetToken("^\\w+://.*?:(\\d+)/");
	theResult = kNIndexNone;

	if (!theValue.IsEmpty() && theNumber.SetValue(theValue))
		theResult = theNumber.GetInt32();



	// Provide some defaults
	if (theResult == kNIndexNone)
		{
		theScheme = GetScheme();
		
		if (theScheme == "http")
			theResult = 80;

		else if (theScheme == "https")
			theResult = 443;

		else if (theScheme == "ssh")
			theResult = 22;
		}

	return(theResult);
}





//============================================================================
//		NURL::GetPath : Get the path.
//----------------------------------------------------------------------------
NString NURL::GetPath(void) const
{	NString		theResult;



	// Get the scheme
	theResult = GetToken("\\w+://.*?(/.*)");
	
	return(theResult);
}





//============================================================================
//		NURL::NFormatArgument : NFormatArgument operator.
//----------------------------------------------------------------------------
NURL::operator NFormatArgument(void) const
{


	// Get the value
	return(mValue);
}





#pragma mark protected
//============================================================================
//      NURL::EncodeSelf : Encode the object.
//----------------------------------------------------------------------------
void NURL::EncodeSelf(NEncoder &theEncoder) const
{


	// Encode the object
	theEncoder.EncodeString(kNEncoderValueKey, mValue);
}





//============================================================================
//      NURL::DecodeSelf : Decode the object.
//----------------------------------------------------------------------------
void NURL::DecodeSelf(const NEncoder &theEncoder)
{


	// Decode the object
	mValue = theEncoder.DecodeString(kNEncoderValueKey);
}





#pragma mark private
//============================================================================
//		NURL::GetToken : Get a token.
//----------------------------------------------------------------------------
NString NURL::GetToken(const NString &thePattern) const
{	NString			theResult;
	NRangeList		theRanges;



	// Get the token
	theRanges = mValue.FindAll(thePattern, kNStringPattern);

	if (theRanges.size() == 2)
		theResult = mValue.GetString(theRanges[1]);

	return(theResult);
}






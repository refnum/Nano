/*	NAME:
		NJSONEncoder.cpp

	DESCRIPTION:
		JSON encoder.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "json.h"

#include "NJSONEncoder.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kEncodeIndent									= "\t";





//============================================================================
//		NJSONEncoder::NJSONEncoder : Constructor.
//----------------------------------------------------------------------------
NJSONEncoder::NJSONEncoder(void)
{
}





//============================================================================
//		NJSONEncoder::~NJSONEncoder : Destructor.
//----------------------------------------------------------------------------
NJSONEncoder::~NJSONEncoder(void)
{
}





//============================================================================
//		NJSONEncoder::Encode : Encode a JSON document.
//----------------------------------------------------------------------------
NString NJSONEncoder::Encode(const NVariant &theValue)
{	NString		theJSON;



	// Encode the JSON
	theJSON = EncodeValue(theValue, kEncodeIndent);
	NN_ASSERT(!theJSON.IsEmpty());

	return(theJSON);
}





//============================================================================
//		NJSONEncoder::Decode : Decode a JSON document.
//----------------------------------------------------------------------------
NVariant NJSONEncoder::Decode(const NString &theJSON)
{	json_value		*jsonValue;
	NVariant		theValue;



	// Decode the JSON
	jsonValue = json_parse(theJSON.GetUTF8());
	NN_ASSERT(jsonValue != NULL);

	if (jsonValue != NULL)
		{
		theValue = DecodeValue(jsonValue);
		json_value_free(jsonValue);
		}
	
	return(theValue);
}





#pragma mark private
//============================================================================
//		NJSONEncoder::EncodeValue : Encode an NVariant.
//----------------------------------------------------------------------------
NString NJSONEncoder::EncodeValue(const NVariant &theValue, const NString &theIndent)
{	NDictionary		valueDictionary;
	NString			valueString;
	NArray			valueArray;
	bool			valueBool;
	NString			theText;



	// Encode the value
	if (theValue.GetValue(valueBool))
		theText = EncodeBoolean(valueBool);
	
	else if (theValue.IsNumeric())
		theText = EncodeNumber(theValue);
	
	else if (theValue.GetValue(valueString))
		theText = EncodeString(valueString);
	
	else if (theValue.GetValue(valueArray))
		theText = EncodeArray(valueArray, theIndent);
	
	else if (theValue.GetValue(valueDictionary))
		theText = EncodeDictionary(valueDictionary, theIndent);
	
	else
		NN_LOG("Unable to encode to JSON: %s", theValue.GetType().name());
	
	return(theText);
}





//============================================================================
//		NJSONEncoder::EncodeBoolean : Encode a bool.
//----------------------------------------------------------------------------
NString NJSONEncoder::EncodeBoolean(bool theValue)
{


	// Encode the value
	return(theValue ? "true" : "false");
}





//============================================================================
//		NJSONEncoder::EncodeNumber : Encode an NNumber.
//----------------------------------------------------------------------------
NString NJSONEncoder::EncodeNumber(const NNumber &theValue)
{


	// Encode the value
	return(theValue.GetString());
}





//============================================================================
//		NJSONEncoder::EncodeString : Encode an NString.
//----------------------------------------------------------------------------
NString NJSONEncoder::EncodeString(const NString &theValue)
{	NString			theResult;
	const char		*textUTF8;
	std::string		theBuffer;
	char			theChar;



	// Get the state we need
	textUTF8 = theValue.GetUTF8();



	// Encode the value
	while ((theChar = *textUTF8++) != 0x00)
		{
		switch (theChar) {
			case '\\':
				theBuffer += "\\\\";
				break;

			case '/':
				theBuffer += "\\/";
				break;

			case '"':
				theBuffer += "\\\"";
				break;

			case '\b':
				theBuffer += "\\b";
				break;

			case '\f':
				theBuffer += "\\f";
				break;

			case '\n':
				theBuffer += "\\n";
				break;

			case '\r':
				theBuffer += "\\r";
				break;

			case '\t':
				theBuffer += "\\t";
				break;
			
			default:
				theBuffer += theChar;
				break;
			}
		}

	theResult.Format("\"%s\"", theBuffer.c_str());
	
	return(theResult);
}





//============================================================================
//		NJSONEncoder::EncodeArray : Encode an NArray.
//----------------------------------------------------------------------------
NString NJSONEncoder::EncodeArray(const NArray &theValue, const NString &theIndent)
{	NString		theResult, childIndent;
	NIndex		n, numValues;



	// Get the state we need
	numValues   = theValue.GetSize();
	childIndent = theIndent + kEncodeIndent;



	// Encode the value
	if (numValues == 0)
		theResult = "[ ]";
	else
		{
		theResult = "[\n";

		for (n = 0; n < numValues; n++)
			{
			theResult += theIndent;
			theResult += EncodeValue(theValue.GetValue(n), childIndent);
		
			if (n != (numValues-1))
				theResult += ",";

			theResult += "\n";
			}

		theResult += theIndent;
		theResult.TrimRight(1);

		theResult += "]";
		}

	return(theResult);
}





//============================================================================
//		NJSONEncoder::EncodeDictionary : Encode an NDictionary.
//----------------------------------------------------------------------------
NString NJSONEncoder::EncodeDictionary(const NDictionary &theValue, const NString &theIndent)
{	NString			theResult, childIndent;
	NIndex			n, numValues;
	NStringList		theKeys;



	// Get the state we need
	theKeys     = theValue.GetKeys();
	numValues   = theValue.GetSize();
	childIndent = theIndent + kEncodeIndent;



	// Encode the value
	if (numValues == 0)
		theResult = "{ }";
	else
		{
		theResult = "{\n";

		for (n = 0; n < numValues; n++)
			{
			theResult += theIndent;
			theResult += "\"";
			theResult += theKeys[n];
			theResult += "\": ";
			theResult += EncodeValue(theValue.GetValue(theKeys[n]), childIndent);

			if (n != (numValues-1))
				theResult += ",";

			theResult += "\n";
			}

		theResult += theIndent;
		theResult.TrimRight(1);

		theResult += "}";
		}

	return(theResult);
}





//============================================================================
//		NJSONEncoder::DecodeValue : Decode a value.
//----------------------------------------------------------------------------
NVariant NJSONEncoder::DecodeValue(const json_value_ptr theValue)
{	NVariant	theResult;



	// Decode the value
	switch (theValue->type) {
		case json_boolean:
			theResult = DecodeBoolean(theValue);
			break;

		case json_integer:
			theResult = DecodeInteger(theValue);
			break;
		
		case json_double:
			theResult = DecodeReal(theValue);
			break;
		
		case json_string:
			theResult = DecodeString(theValue);
			break;
		
		case json_array:
			theResult = DecodeArray(theValue);
			break;

		case json_object:
			theResult = DecodeDictionary(theValue);
			break;
		
		case json_null:
			break;
		
		default:
			NN_LOG("Unable to decode from JSON: %d", theValue->type);
			break;
		}
	
	return(theResult);
}





//============================================================================
//		NJSONEncoder::DecodeBoolean : Decode a boolean.
//----------------------------------------------------------------------------
bool NJSONEncoder::DecodeBoolean(const json_value_ptr jsonValue)
{


	// Validate our parameters
	NN_ASSERT(jsonValue->type == json_boolean);



	// Decode the value
	return(jsonValue->u.boolean != 0);
}





//============================================================================
//		NJSONEncoder::DecodeInteger : Decode an integer.
//----------------------------------------------------------------------------
int64_t NJSONEncoder::DecodeInteger(const json_value_ptr jsonValue)
{


	// Validate our parameters
	NN_ASSERT(jsonValue->type == json_integer);



	// Decode the value
	return(jsonValue->u.integer);
}





//============================================================================
//		NJSONEncoder::DecodeReal : Decode a real.
//----------------------------------------------------------------------------
float64_t NJSONEncoder::DecodeReal(const json_value_ptr jsonValue)
{


	// Validate our parameters
	NN_ASSERT(jsonValue->type == json_double);



	// Decode the value
	return(jsonValue->u.dbl);
}





//============================================================================
//		NJSONEncoder::DecodeString : Decode an NString.
//----------------------------------------------------------------------------
NString NJSONEncoder::DecodeString(const json_value_ptr jsonValue)
{


	// Validate our parameters
	NN_ASSERT(jsonValue->type == json_string);



	// Decode the value
	return(NString(jsonValue->u.string.ptr, jsonValue->u.string.length));
}





//============================================================================
//		NJSONEncoder::DecodeArray : Decode an NArray.
//----------------------------------------------------------------------------
NArray NJSONEncoder::DecodeArray(const json_value_ptr jsonValue)
{	NArray		theResult;
	NIndex		n;



	// Validate our parameters
	NN_ASSERT(jsonValue->type == json_array);



	// Decode the value
	for (n = 0; n < (NIndex) jsonValue->u.array.length; n++)
		theResult.AppendValue(DecodeValue(jsonValue->u.array.values[n]));

	return(theResult);
}





//============================================================================
//		NJSONEncoder::DecodeDictionary : Decode an NDictionary.
//----------------------------------------------------------------------------
NDictionary NJSONEncoder::DecodeDictionary(const json_value_ptr jsonValue)
{	NDictionary		theResult;
	NVariant		theValue;
	NString			theKey;
	NIndex			n;



	// Validate our parameters
	NN_ASSERT(jsonValue->type == json_object);



	// Decode the value
	for (n = 0; n < (NIndex) jsonValue->u.object.length; n++)
		{
		theKey   = NString(    jsonValue->u.object.values[n].name, kNStringLength);
		theValue = DecodeValue(jsonValue->u.object.values[n].value);
		
		NN_ASSERT(!theResult.HasKey(theKey));
		theResult.SetValue(theKey, theValue);
		}

	return(theResult);
}



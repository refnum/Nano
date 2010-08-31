/*	NAME:
		NRegistry.cpp

	DESCRIPTION:
		Windows registry.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"
#include "NWinTarget.h"
#include "NWindows.h"
#include "NEncoder.h"
#include "NRegistry.h"





//============================================================================
//		NRegistry::NRegistry : Constructor.
//----------------------------------------------------------------------------
NRegistry::NRegistry(void)
{


	// Initialize ourselves
	mKey = NULL;
}





//============================================================================
//		NRegistry::~NRegistry : Destructor.
//----------------------------------------------------------------------------
NRegistry::~NRegistry(void)
{


	// Clean up
	if (IsOpen())
		Close();
}





//============================================================================
//		NRegistry::IsOpen : Is the registry open?
//----------------------------------------------------------------------------
bool NRegistry::IsOpen(void) const
{


	// Get our state
	return(mKey != NULL);
}





//============================================================================
//		NRegistry::Open : Open the registry.
//----------------------------------------------------------------------------
NStatus NRegistry::Open(HKEY theKey, const NString &subKey, bool canCreate, bool readOnly)
{	REGSAM		theAccess;
	LSTATUS		theErr;



	// Validate our state
	NN_ASSERT(!IsOpen());



	// Get the state we need
	theAccess = readOnly ? KEY_READ : KEY_ALL_ACCESS;



	// Open the key
	if (canCreate)
		theErr = RegCreateKeyEx(theKey, ToWN(subKey), 0, NULL, REG_OPTION_NON_VOLATILE,
								theAccess, NULL, &mKey, NULL);
	else
		theErr = RegOpenKeyEx(theKey, ToWN(subKey), 0, theAccess, &mKey);

	return(NWinTarget::ConvertHRESULT(theErr));
}





//============================================================================
//		NRegistry::Close : Close the registry.
//----------------------------------------------------------------------------
void NRegistry::Close(void)
{	LSTATUS		theErr;



	// Validate our state
	NN_ASSERT(IsOpen());
	
	
	
	// Close the key
	theErr = RegCloseKey(mKey);
	mKey   = NULL;

	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NRegistry::Flush : Flush the registry.
//----------------------------------------------------------------------------
void NRegistry::Flush(void)
{	LSTATUS		theErr;



	// Validate our state
	NN_ASSERT(IsOpen());



	// Flush the registry
	theErr = RegFlushKey(mKey);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NRegistry::HasKey : Does a key exist?
//----------------------------------------------------------------------------
bool NRegistry::HasKey(const NString &theKey) const
{	DWORD		theSize;
	LSTATUS		theErr;
	bool		hasKey;



	// Validate our state
	NN_ASSERT(IsOpen());



	// Query the registry
	theSize = 0;
	theErr  = RegQueryValueEx(mKey, ToWN(theKey), NULL, NULL, NULL, &theSize);
	hasKey  = (theErr == ERROR_SUCCESS && theSize != 0);

	return(hasKey);
}





//============================================================================
//		NRegistry::RemoveKey : Remove a key.
//----------------------------------------------------------------------------
void NRegistry::RemoveKey(const NString &theKey)
{	LSTATUS		theErr;



	// Validate our state
	NN_ASSERT(IsOpen());



	// Remove the key
	theErr = RegDeleteValue(mKey, ToWN(theKey));
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NRegistry::GetValue : Get a value.
//----------------------------------------------------------------------------
NVariant NRegistry::GetValue(const NString &theKey) const
{	DWORD		theType, theSize;
	NString		valueString;
	SInt32		valueInt32;
	SInt64		valueInt64;
	NEncoder	theEncoder;
	NVariant	theValue;
	NData		theData;



	// Validate our parameters and state
	NN_ASSERT(!theKey.IsEmpty());
	NN_ASSERT(IsOpen());



	// Get the state we need
	theType = REG_NONE;



	// Get the value
	if (SUCCEEDED(RegQueryValueEx(mKey, ToWN(theKey), NULL, NULL, NULL, &theSize)))
		{
		if (theData.SetSize(theSize))
			{
			if (FAILED(RegQueryValueEx(mKey, ToWN(theKey), NULL, &theType, theData.GetData(), &theSize)))
				theType = REG_NONE;
			}
		}



	// Decode the value
	//
	// Strings, integers, and data are stored directly - everything else is NEncoded.
	switch (theType) {
		case REG_SZ:
			valueString.SetData(theData, kNStringEncodingUTF16);
			theValue = valueString;
			break;
		
		case REG_DWORD:
			NN_ASSERT(theData.GetSize() == sizeof(UInt32));
			if (theData.GetSize() == sizeof(UInt32))
				{
				memcpy(&valueInt32, theData.GetData(), theData.GetSize());
				theValue = valueInt32;
				}
			break;
		
		case REG_QWORD:
			NN_ASSERT(theData.GetSize() == sizeof(UInt64));
			if (theData.GetSize() == sizeof(UInt64))
				{
				memcpy(&valueInt64, theData.GetData(), theData.GetSize());
				theValue = valueInt64;
				}
			break;
		
		case REG_BINARY:
			theValue = theEncoder.Decode(theData);
			if (!theValue.IsValid())
				theValue = theData;
			break;
		
		case REG_NONE:
			// No data
			break;
		
		default:
			NN_LOG("Unknown registry type: %d", theType);
			break;
		}
	
	return(theValue);
}





//============================================================================
//		NRegistry::SetValue : Set a value.
//----------------------------------------------------------------------------
void NRegistry::SetValue(const NString &theKey, const NVariant &theValue)
{	NData		theData, valueData;
	NString		valueString;
	NNumber		valueNumber;
	SInt32		valueInt32;
	SInt64		valueInt64;
	NEncoder	theEncoder;
	DWORD		theType;
	LONG		theErr;



	// Validate our parameters and state
	NN_ASSERT(!theKey.IsEmpty());
	NN_ASSERT(IsOpen());



	// Get the state we need
	theType = REG_NONE;



	// Encode the value
	//
	// Strings, integers, and data are stored directly - everything else is NEncoded.
	if (theValue.GetValue(valueString))
		{
		theType = REG_SZ;
		theData = valueString.GetData(kNStringEncodingUTF16, kNStringNullTerminate);
		}
	
	else if (theValue.IsNumeric())
		{
		valueNumber.SetValue(theValue);
		
		switch (valueNumber.GetPrecision()) {
			case kNPrecisionInt8:
			case kNPrecisionInt16:
			case kNPrecisionInt32:
				theType    = REG_DWORD;
				valueInt32 = valueNumber.GetSInt32();

				theData.AppendData(sizeof(valueInt32), &valueInt32);
				break;

			case kNPrecisionInt64:
				theType    = REG_QWORD;
				valueInt64 = valueNumber.GetSInt64();

				theData.AppendData(sizeof(valueInt64), &valueInt64);
				break;

			default:
				break;
			}
		}
	
	else if (theValue.GetValue(valueData))
		{
		theType = REG_BINARY;
		theData = valueData;
		}

	if (theType == REG_NONE)
		{
		theType = REG_BINARY;
		theData = theEncoder.Encode(theValue);

		if (theData.IsEmpty())
			NN_LOG("Unable to encode '%@'", theKey);
		}



	// Set the value
	if (theType != REG_NONE && !theData.IsEmpty())
		{
		theErr = RegSetValueEx(mKey, ToWN(theKey), 0, theType, theData.GetData(), theData.GetSize());
		NN_ASSERT_NOERR(theErr);
		}
}





//============================================================================
//		NRegistry::GetValueBoolean : Get a boolean value.
//----------------------------------------------------------------------------
bool NRegistry::GetValueBoolean(const NString &theKey) const
{


	// Get the value
	return(NSystemUtilities::GetBoolean(GetValue(theKey), theKey));
}





//============================================================================
//		NRegistry::GetValueSInt32 : Get a SInt32 value.
//----------------------------------------------------------------------------
SInt32 NRegistry::GetValueSInt32(const NString &theKey) const
{


	// Get the value
	return(NSystemUtilities::GetSInt32(GetValue(theKey), theKey));
}





//============================================================================
//		NRegistry::GetValueSInt64 : Get a SInt64 value.
//----------------------------------------------------------------------------
SInt64 NRegistry::GetValueSInt64(const NString &theKey) const
{


	// Get the value
	return(NSystemUtilities::GetSInt64(GetValue(theKey), theKey));
}





//============================================================================
//		NRegistry::GetValueFloat32 : Get a Float32 value.
//----------------------------------------------------------------------------
Float32 NRegistry::GetValueFloat32(const NString &theKey) const
{


	// Get the value
	return(NSystemUtilities::GetFloat32(GetValue(theKey), theKey));
}





//============================================================================
//		NRegistry::GetValueFloat64 : Get a Float64 value.
//----------------------------------------------------------------------------
Float64 NRegistry::GetValueFloat64(const NString &theKey) const
{


	// Get the value
	return(NSystemUtilities::GetFloat64(GetValue(theKey), theKey));
}





//============================================================================
//		NRegistry::GetValueString : Get a string value.
//----------------------------------------------------------------------------
NString NRegistry::GetValueString(const NString &theKey) const
{


	// Get the value
	return(NSystemUtilities::GetString(GetValue(theKey), theKey));
}





//============================================================================
//		NRegistry::GetValueData : Get a data value.
//----------------------------------------------------------------------------
NData NRegistry::GetValueData(const NString &theKey) const
{


	// Get the value
	return(NSystemUtilities::GetData(GetValue(theKey), theKey));
}





//============================================================================
//		NRegistry::GetValueDate : Get a date value.
//----------------------------------------------------------------------------
NDate NRegistry::GetValueDate(const NString &theKey) const
{


	// Get the value
	return(NSystemUtilities::GetDate(GetValue(theKey), theKey));
}





//============================================================================
//		NRegistry::GetValueColor : Get a color value.
//----------------------------------------------------------------------------
NColor NRegistry::GetValueColor(const NString &theKey) const
{


	// Get the value
	return(NSystemUtilities::GetColor(GetValue(theKey), theKey));
}





//============================================================================
//		NRegistry::GetValuePoint : Get a point value.
//----------------------------------------------------------------------------
NPoint NRegistry::GetValuePoint(const NString &theKey) const
{


	// Get the value
	return(NSystemUtilities::GetPoint(GetValue(theKey), theKey));
}





//============================================================================
//		NRegistry::GetValueSize : Get a size value.
//----------------------------------------------------------------------------
NSize NRegistry::GetValueSize(const NString &theKey) const
{


	// Get the value
	return(NSystemUtilities::GetSize(GetValue(theKey), theKey));
}





//============================================================================
//		NRegistry::GetValueRectangle : Get a rectangle value.
//----------------------------------------------------------------------------
NRectangle NRegistry::GetValueRectangle(const NString &theKey) const
{


	// Get the value
	return(NSystemUtilities::GetRectangle(GetValue(theKey), theKey));
}





//============================================================================
//		NRegistry::GetValueVector : Get a vector value.
//----------------------------------------------------------------------------
NVector NRegistry::GetValueVector(const NString &theKey) const
{


	// Get the value
	return(NSystemUtilities::GetVector(GetValue(theKey), theKey));
}





//============================================================================
//		NRegistry::GetValueArray : Get an array value.
//----------------------------------------------------------------------------
NArray NRegistry::GetValueArray(const NString &theKey) const
{


	// Get the value
	return(NSystemUtilities::GetArray(GetValue(theKey), theKey));
}





//============================================================================
//		NRegistry::GetValueDictionary : Get a dictionary value.
//----------------------------------------------------------------------------
NDictionary NRegistry::GetValueDictionary(const NString &theKey) const
{


	// Get the value
	return(NSystemUtilities::GetDictionary(GetValue(theKey), theKey));
}




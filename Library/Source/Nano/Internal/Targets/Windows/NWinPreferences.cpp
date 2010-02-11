/*	NAME:
		NWindowsPreferences.cpp

	DESCRIPTION:
		Windows preferences support.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NEncoder.h"
#include "NFile.h"
#include "NWindows.h"
#include "NTargetPreferences.h"





//============================================================================
//		Internal functions
//----------------------------------------------------------------------------
//		GetRegAppPath : Get the registry path for the app.
//----------------------------------------------------------------------------
static NString GetRegAppPath(void)
{	static NString	sAppKey;

	TCHAR		theBuffer[MAX_PATH];
	NFile		theFile;



	// Create the app path
	//
	// This may need to be customisable at run-time, to allow apps to store
	// values under "Software\\Company Name\\Product Name" and to allow the
	// executable to be renamed without affecting its prefs.
	if (sAppKey.IsEmpty())
		{
		if (GetModuleFileName(NULL, theBuffer, MAX_PATH))
			{
			theFile = NFile(ToNN(theBuffer));
			if (theFile.IsFile())
				sAppKey.Format("Software\\%@", theFile.GetName(kNNameNoExtension));
			}

		NN_ASSERT(!sAppKey.IsEmpty());
		}
	
	return(sAppKey);
}





//============================================================================
//		GetRegAppKey : Get the registry key for the app.
//----------------------------------------------------------------------------
static HKEY GetRegAppKey(bool canCreate)
{	NString		appPath;
	HKEY		theKey;
	LONG		theErr;



	// Get the state we need
	appPath = GetRegAppPath();
	theKey  = NULL;



	// Get the key
	if (canCreate)
		{
		theErr = RegCreateKeyEx(HKEY_CURRENT_USER, ToWN(appPath), 0, NULL, REG_OPTION_NON_VOLATILE,
								KEY_ALL_ACCESS, NULL, &theKey, NULL);
		NN_ASSERT_NOERR(theErr);
		}
	else
		{
		theErr = RegOpenKeyEx(  HKEY_CURRENT_USER, ToWN(appPath), 0, KEY_ALL_ACCESS, &theKey);
		NN_ASSERT(theErr == ERROR_SUCCESS || theErr == ERROR_FILE_NOT_FOUND);
		}

	return(theKey);
}





//============================================================================
//		NTargetPreferences::HasKey : Does a key exist?
//----------------------------------------------------------------------------
#pragma mark -
bool NTargetPreferences::HasKey(const NString &theKey)
{	DWORD		theSize;
	HKEY		appKey;
	LONG		theErr;
	bool		hasKey;



	// Get the state we need
	appKey = GetRegAppKey(false);
	if (appKey == NULL)
		return(false);



	// Query the registry
	theSize = 0;
	theErr  = RegQueryValueEx(appKey, ToWN(theKey), NULL, NULL, NULL, &theSize);
	hasKey  = (theErr == ERROR_SUCCESS && theSize != 0);



	// Clean up
	RegCloseKey(appKey);
	
	return(hasKey);
}





//============================================================================
//		NTargetPreferences::RemoveKey : Remove a key.
//----------------------------------------------------------------------------
void NTargetPreferences::RemoveKey(const NString &theKey)
{	HKEY		appKey;
	LONG		theErr;



	// Get the state we need
	appKey = GetRegAppKey(false);
	if (appKey == NULL)
		return;



	// Remove the key
	theErr = RegDeleteValue(appKey, ToWN(theKey));
	NN_ASSERT_NOERR(theErr);



	// Clean up
	RegCloseKey(appKey);
}





//============================================================================
//		NTargetPreferences::GetValue : Get a value.
//----------------------------------------------------------------------------
NVariant NTargetPreferences::GetValue(const NString &theKey)
{	DWORD			theType, theSize;
	NString			valueString;
	SInt32			valueInt32;
	SInt64			valueInt64;
	NEncoder		theEncoder;
	NVariant		theValue;
	NData			theData;
	HKEY			appKey;



	// Validate our parameters
	NN_ASSERT(!theKey.IsEmpty());



	// Get the state we need
	theType = REG_NONE;
	appKey  = GetRegAppKey(false);

	if (appKey == NULL)
		return(theValue);



	// Get the value
	if (SUCCEEDED(RegQueryValueEx(appKey, ToWN(theKey), NULL, NULL, NULL, &theSize)))
		{
		if (theData.SetSize(theSize))
			{
			if (FAILED(RegQueryValueEx(appKey, ToWN(theKey), NULL, &theType, theData.GetData(), &theSize)))
				theType = REG_NONE;
			}
		}



	// Decode the value
	//
	// Strings and integers are stored directly, everything else is NEncoded.
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
			break;
		
		case REG_NONE:
			// No data
			break;
		
		default:
			NN_LOG("Unknown registry type: %d", theType);
			break;
		}



	// Clean up
	RegCloseKey(appKey);
	
	return(theValue);
}





//============================================================================
//		NTargetPreferences::SetValue : Set a value.
//----------------------------------------------------------------------------
void NTargetPreferences::SetValue(const NString &theKey, const NVariant &theValue)
{	NString			valueString;
	NNumber			valueNumber;
	SInt32			valueInt32;
	SInt64			valueInt64;
	NEncoder		theEncoder;
	NData			theData;
	DWORD			theType;
	LONG			theErr;
	HKEY			appKey;



	// Validate our parameters
	NN_ASSERT(!theKey.IsEmpty());



	// Get the state we need
	theType = REG_NONE;
	appKey  = GetRegAppKey(true);

	NN_ASSERT(appKey != NULL);
	if (appKey == NULL)
		return;



	// Encode the value
	//
	// Strings and integers are stored directly, everything else is NEncoded.
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
		theErr = RegSetValueEx(appKey, ToWN(theKey), 0, theType, theData.GetData(), theData.GetSize());
		NN_ASSERT_NOERR(theErr);
		}



	// Clean up
	RegCloseKey(appKey);
}



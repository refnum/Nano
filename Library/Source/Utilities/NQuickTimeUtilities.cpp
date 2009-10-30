/*	NAME:
		NQuickTimeUtilities.cpp

	DESCRIPTION:
		QuickTime utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTextUtilities.h"
#include "NQuickTimeUtilities.h"





//============================================================================
//		NQuickTimeUtilities::CreateDataReferenceFromHandle : Create a data reference.
//----------------------------------------------------------------------------
Handle NQuickTimeUtilities::CreateDataReferenceFromHandle(Handle theData, const NUTI &theUTI)
{	Handle		dataRef;
	OSStatus	theErr;



	// Validate our parameters
	NN_ASSERT(theData != NULL);



	// Create the data reference
	dataRef = NULL;
	theErr  = PtrToHand(&theData, &dataRef, sizeof(Handle));
	NN_ASSERT_NOERR(theErr);



	// Assign the meta-data
	if (dataRef != NULL && theUTI.IsValid())
		SetDataReferenceMetaData(dataRef, HandleDataHandlerSubType, theUTI);
	
	return(dataRef);
}





//============================================================================
//		NQuickTimeUtilities::CreateDataReferenceFromPointer : Create a data reference.
//----------------------------------------------------------------------------
Handle NQuickTimeUtilities::CreateDataReferenceFromPointer(UInt32 theSize, const void *thePtr, const NUTI &theUTI)
{	PointerDataRefRecord	dataRecord;
	Handle					dataRef;
	OSStatus				theErr;



	// Validate our parameters
	NN_ASSERT(theSize != 0);
	NN_ASSERT(thePtr  != NULL);



	// Create the data reference
	dataRecord.data       = (void *) thePtr;
	dataRecord.dataLength =          theSize;
	
	dataRef = NULL;
	theErr  = PtrToHand(&dataRecord, &dataRef, sizeof(dataRecord));
	NN_ASSERT_NOERR(theErr);



	// Assign the meta-data
	if (dataRef != NULL && theUTI.IsValid())
		SetDataReferenceMetaData(dataRef, PointerDataHandlerSubType, theUTI);
	
	return(dataRef);
}





//============================================================================
//		NQuickTimeUtilities::SetDataReferenceMetaData : Set a data reference's meta-data.
//----------------------------------------------------------------------------
void NQuickTimeUtilities::SetDataReferenceMetaData(Handle &dataRef, OSType dataType, const NUTI &theUTI)
{	NString		fileExt, mimeType;



	// Validate our parameters
	NN_ASSERT(dataRef != NULL);
	NN_ASSERT(theUTI.IsValid());
	


	// Get the UTI state
	fileExt  = theUTI.GetFileExtension();
	mimeType = theUTI.GetMIMEType();
	


	// Add the extension
	//
	// Note that an extension is required if we're going to add any meta-data
	// (such as mime type), even if the extension itself is an empty string.
	//
	// See http://developer.apple.com/technotes/tn/tn1195.html for more info.
	//
	// We also need to supply a complete file name, although since only the
	// extension is significant we can use a random value.
	if (fileExt.IsNotEmpty() || mimeType.IsNotEmpty())
		{
		if (fileExt.IsNotEmpty())
			fileExt = NString("foo.") + fileExt;

		AddDataReferenceExtension(dataRef, dataType, fileExt, kDataRefExtensionFileName);
		}



	// Add the mime type
	if (mimeType.IsNotEmpty())
		AddDataReferenceExtension(dataRef, dataType, mimeType, kDataRefExtensionMIMEType);
}





//============================================================================
//		NQuickTimeUtilities::AddDataReferenceExtension : Add a data reference extension.
//----------------------------------------------------------------------------
void NQuickTimeUtilities::AddDataReferenceExtension(Handle &dataRef, OSType dataType, const NString &theValue, OSType valueType)
{	UInt32				valueSize, atomHeader[2];
	ComponentInstance	qtHandler;
	Handle				valueHnd;
	OSStatus			theErr;




	// Validate our parameters
	NN_ASSERT(dataType == HandleDataHandlerSubType || dataType == PointerDataHandlerSubType);



	// Get the state we need
	valueHnd = NTextUtilities::GetStr255Hnd(theValue);
	if (valueHnd == NULL)
		return;

	qtHandler = NULL;
	valueSize = GetHandleSize(valueHnd);



	// Add to a handle data reference
	//
	// Handle data references have to be constructed manually.
	if (dataType == HandleDataHandlerSubType)
		{
		switch (valueType) {
			case kDataRefExtensionFileName:
				theErr = PtrAndHand(*valueHnd, dataRef, valueSize);
				NN_ASSERT_NOERR(theErr);
				break;
			
			case kDataRefExtensionMIMEType:
				atomHeader[0] = EndianU32_NtoB(sizeof(atomHeader) + valueSize);
				atomHeader[1] = EndianU32_NtoB(kDataRefExtensionMIMEType);

				theErr = PtrAndHand(atomHeader, dataRef, sizeof(atomHeader));
				NN_ASSERT_NOERR(theErr);

				theErr = PtrAndHand(*valueHnd,  dataRef, valueSize);
				NN_ASSERT_NOERR(theErr);
				break;
			
			default:
				NN_LOG("Unknown value type: %s", NTextUtilities::FromOSType(valueType).GetUTF8());
				break;
			}
		}



	// Add to a pointer data reference
	//
	// Pointer data references are constructed through a data handler.
	//
	// Once the extension has been added, the data reference must be
	// refreshed to retrieve the updated value held by the data handler.
	else
		{
		theErr = OpenADataHandler(dataRef, dataType, NULL, 0, NULL, kDataHCanRead, &qtHandler);
		NN_ASSERT_NOERR(theErr);

		if (theErr == noErr)
			{
			theErr = DataHSetDataRefExtension(qtHandler, valueHnd, valueType);
			NN_ASSERT_NOERR(theErr);
			}

		if (theErr == noErr)
			{
			DisposeHandle(dataRef);
			dataRef = NULL;
	
			theErr = DataHGetDataRef(qtHandler, &dataRef);
			NN_ASSERT_NOERR(theErr);
			}
		}



	// Clean up
	if (qtHandler != NULL)
		CloseComponent(qtHandler);

	DisposeHandle(valueHnd);
}





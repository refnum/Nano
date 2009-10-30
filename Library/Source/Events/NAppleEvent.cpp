/*	NAME:
		NAppleEvent.cpp

	DESCRIPTION:
		Apple Event object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NAppleEvent.h"





//============================================================================
//		NAppleEvent::NAppleEvent : Constructor.
//----------------------------------------------------------------------------
NAppleEvent::NAppleEvent(AEEventClass theClass, AEEventID theID, const AEAddressDesc *theTarget)
{	ProcessSerialNumber		selfProcess = { 0, kCurrentProcess };
	AEAddressDesc			selfTarget;
	AppleEvent				*eventPtr;
	AppleEvent				theEvent;
	OSStatus				theErr;



	// Get the state we need
	AEInitializeDesc(&theEvent);
	AEInitializeDesc(&selfTarget);

	if (theTarget == NULL)
		{
		theErr = AECreateDesc(typeProcessSerialNumber, &selfProcess, sizeof(selfProcess), &selfTarget);
		NN_ASSERT_NOERR(theErr);
		
		if (theErr == noErr)
			theTarget = &selfTarget;
		}



	// Create the event
	eventPtr = &theEvent;
	theErr   = AECreateAppleEvent(theClass, theID, theTarget, kAutoGenerateReturnID, kAnyTransactionID, &theEvent);

	if (theErr != noErr)
		eventPtr = NULL;



	// Initialize ourselves
	InitializeSelf(eventPtr);



	// Clean up
	ReleaseEvent(theEvent);
	ReleaseEvent(selfTarget);
}





//============================================================================
//		NAppleEvent::NAppleEvent : Constructor.
//----------------------------------------------------------------------------
NAppleEvent::NAppleEvent(const NAppleEvent &theEvent)
{


	// Initialize ourselves
	InitializeSelf(&theEvent.mEvent);
}





//============================================================================
//		NAppleEvent::NAppleEvent : Constructor.
//----------------------------------------------------------------------------
NAppleEvent::NAppleEvent(const AppleEvent &theEvent)
{


	// Initialize ourselves
	InitializeSelf(&theEvent);
}





//============================================================================
//		NAppleEvent::NAppleEvent : Constructor.
//----------------------------------------------------------------------------
NAppleEvent::NAppleEvent(void)
{


	// Initialize ourselves
	InitializeSelf(NULL);
}





//============================================================================
//		NAppleEvent::~NAppleEvent : Destructor.
//----------------------------------------------------------------------------
NAppleEvent::~NAppleEvent(void)
{


	// Clean up
	ReleaseEvent(mEvent);
}





//============================================================================
//		NAppleEvent::IsValid : Is the event valid?
//----------------------------------------------------------------------------
bool NAppleEvent::IsValid(void) const
{


	// Check our state
	return(mEvent.descriptorType != typeNull);
}





//============================================================================
//		NAppleEvent::IsClass : Is the event a particular class?
//----------------------------------------------------------------------------
bool NAppleEvent::IsClass(AEEventClass theClass) const
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Check our class
	return(GetClass() == theClass);
}





//============================================================================
//		NAppleEvent::IsID : Is the event a particular ID?
//----------------------------------------------------------------------------
bool NAppleEvent::IsID(AEEventID theID) const
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Check our ID
	return(GetID() == theID);
}





//============================================================================
//		NAppleEvent::GetClass : Get the event class.
//----------------------------------------------------------------------------
AEEventClass NAppleEvent::GetClass(void) const
{	AEEventClass	theClass;
	OSStatus		theErr;
	


	// Validate our state
	NN_ASSERT(IsValid());



	// Get our class
	theErr = GetAttribute(keyEventClassAttr, typeType, sizeof(theClass), &theClass);
	NN_ASSERT_NOERR(theErr);
	
	return(theClass);
}





//============================================================================
//		NAppleEvent::GetID : Get the event ID.
//----------------------------------------------------------------------------
AEEventID NAppleEvent::GetID(void) const
{	AEEventID		theID;
	OSStatus		theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get our class
	theErr = GetAttribute(keyEventIDAttr, typeType, sizeof(theID), &theID);
	NN_ASSERT_NOERR(theErr);
	
	return(theID);
}





//============================================================================
//		NAppleEvent::Send : Send the event.
//----------------------------------------------------------------------------
OSStatus NAppleEvent::Send(AESendMode theMode, AESendPriority thePriority, long theTimeout) const
{	AppleEvent		theReply;
	OSStatus		theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Post the event
	theErr = AESend(&mEvent, &theReply, theMode, thePriority, theTimeout, NULL, NULL);
	NN_ASSERT_NOERR(theErr);
	
	return(theErr);
}





//============================================================================
//		NAppleEvent::GetEvent : Get the event.
//----------------------------------------------------------------------------
const AppleEvent *NAppleEvent::GetEvent(void) const
{


	// Get the event
	return(&mEvent);
}





//============================================================================
//		NAppleEvent::GetParameter : Get an event parameter.
//----------------------------------------------------------------------------
OSStatus NAppleEvent::GetParameter(AEKeyword theParam, DescType theType, UInt32 theSize, void *thePtr) const
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theErr = AEGetParamPtr(&mEvent, theParam, theType, NULL, thePtr, theSize, NULL);
	if (theErr != noErr)
		memset(thePtr, 0x00, theSize);
	
	return(theErr);
}





//============================================================================
//		NAppleEvent::GetParameterString : Get a string parameter.
//----------------------------------------------------------------------------
NString NAppleEvent::GetParameterString(AEKeyword theParam) const
{	NString			theValue;
	Size			theSize;
	NData			theData;
	OSStatus		theErr;



	// Get the state we need
	theSize = 0;
	theErr  = AEGetParamPtr(&mEvent, theParam, typeUTF8Text, NULL, NULL, 0, &theSize);
	NN_ASSERT_NOERR(theErr);
	
	if (theSize == 0 || !theData.SetSize(theSize))
		return(theValue);



	// Get the text
	theErr = AEGetParamPtr(&mEvent, theParam, typeUTF8Text, NULL, theData.GetData(), theData.GetSize(), &theSize);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr == noErr)
		{
		NN_ASSERT(theSize == (Size) theData.GetSize());
		theValue = theData;
		}

	return(theValue);
}





//============================================================================
//		NAppleEvent::GetParameterAEDescList : Get an AEDescList parameter.
//----------------------------------------------------------------------------
AEDescList NAppleEvent::GetParameterAEDescList(AEKeyword theParam) const
{	AEDescList	theValue;
	OSStatus	theErr;



	// Get the descriptor
	AEInitializeDesc(&theValue);
	
	theErr = AEGetParamDesc(&mEvent, theParam, typeAEList, &theValue);
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NAppleEvent::SetParameter : Set an event parameter.
//----------------------------------------------------------------------------
OSStatus NAppleEvent::SetParameter(AEKeyword theParam, DescType theType, UInt32 theSize, const void *thePtr)
{	OSStatus	theErr;



	// Validate our parameters and state
	NN_ASSERT(IsValid());
	NN_ASSERT(theSize != 0);
	NN_ASSERT(thePtr  != NULL);



	// Set the parameter
	theErr = AEPutParamPtr(&mEvent, theParam, theType, thePtr, theSize);
	NN_ASSERT_NOERR(theErr);
	
	return(theErr);
}





//============================================================================
//		NAppleEvent::GetAttribute : Get an event attribute.
//----------------------------------------------------------------------------
OSStatus NAppleEvent::GetAttribute(AEKeyword theParam, DescType theType, UInt32 theSize, void *thePtr) const
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the attribute
	theErr = AEGetAttributePtr(&mEvent, theParam, theType, NULL, thePtr, theSize, NULL);
	if (theErr != noErr)
		memset(thePtr, 0x00, theSize);
	
	return(theErr);
}





//============================================================================
//		NAppleEvent::SetAttribute : Set an event attribute.
//----------------------------------------------------------------------------
OSStatus NAppleEvent::SetAttribute(AEKeyword theParam, DescType theType, UInt32 theSize, const void *thePtr)
{	OSStatus	theErr;



	// Validate our attributes and state
	NN_ASSERT(IsValid());
	NN_ASSERT(theSize != 0);
	NN_ASSERT(thePtr  != NULL);



	// Set the attribute
	theErr = AEPutAttributePtr(&mEvent, theParam, theType, thePtr, theSize);
	NN_ASSERT_NOERR(theErr);
	
	return(theErr);
}





//============================================================================
//		NAppleEvent::= : Assignment operator.
//----------------------------------------------------------------------------
const NAppleEvent& NAppleEvent::operator = (const NAppleEvent &theEvent)
{


	// Assign the object
	if (this != &theEvent)
		{
		ReleaseEvent(mEvent);
		InitializeSelf(&theEvent.mEvent);
		}
	
	return(*this);
}





//============================================================================
//		NAppleEvent::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
void NAppleEvent::InitializeSelf(const AppleEvent *theEvent)
{	OSStatus	theErr;



	// Initialize ourselves
	AEInitializeDesc(&mEvent);

	if (theEvent != NULL)
		{
		theErr = AEDuplicateDesc(theEvent, &mEvent);
		NN_ASSERT_NOERR(theErr);
		}
}





//============================================================================
//		NAppleEvent::ReleaseEvent : Release an event.
//----------------------------------------------------------------------------
void NAppleEvent::ReleaseEvent(AppleEvent &theEvent)
{	OSStatus	theErr;



	// Release the event
	if (theEvent.descriptorType != typeNull)
		{
		theErr = AEDisposeDesc(&theEvent);
		NN_ASSERT_NOERR(theErr);
		}
		
	theEvent.descriptorType = typeNull;
}





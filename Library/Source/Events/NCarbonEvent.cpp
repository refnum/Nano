/*	NAME:
		NCarbonEvent.cpp

	DESCRIPTION:
		Carbon Event object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NData.h"
#include "NHIView.h"
#include "NCarbonEvent.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Parameter types
//
// AEDataModel.h does not define typeUInt64 until 10.5.
#if NN_SDK_VERSION < NN_SDK_VERSION_10_5
enum {
	typeUInt64 = 'ucom'
};
#endif





//============================================================================
//		NCarbonEvent::NCarbonEvent : Constructor.
//----------------------------------------------------------------------------
NCarbonEvent::NCarbonEvent(UInt32 theClass, UInt32 theKind)
{	EventRef	theEvent;
	OSStatus	theErr;



	// Create the event
	theErr = CreateEvent(kCFAllocatorNano, theClass, theKind, GetCurrentEventTime(), kEventAttributeNone, &theEvent);
	if (theErr != noErr)
		theEvent = NULL;



	// Initialize ourselves
	InitializeSelf(theEvent);



	// Clean up
	if (theEvent != NULL)
		ReleaseEvent(theEvent);
}





//============================================================================
//		NCarbonEvent::NCarbonEvent : Constructor.
//----------------------------------------------------------------------------
NCarbonEvent::NCarbonEvent(const NCarbonEvent &theEvent)
{


	// Initialize ourselves
	InitializeSelf(theEvent.mEvent);
}





//============================================================================
//		NCarbonEvent::NCarbonEvent : Constructor.
//----------------------------------------------------------------------------
NCarbonEvent::NCarbonEvent(EventRef theEvent)
{


	// Initialize ourselves
	InitializeSelf(theEvent);
}





//============================================================================
//		NCarbonEvent::NCarbonEvent : Constructor.
//----------------------------------------------------------------------------
NCarbonEvent::NCarbonEvent(void)
{


	// Initialize ourselves
	InitializeSelf(NULL);
}





//============================================================================
//		NCarbonEvent::~NCarbonEvent : Destructor.
//----------------------------------------------------------------------------
NCarbonEvent::~NCarbonEvent(void)
{


	// Clean up
	if (mEvent != NULL)
		ReleaseEvent(mEvent);
}





//============================================================================
//		NCarbonEvent::IsValid : Is the event valid?
//----------------------------------------------------------------------------
bool NCarbonEvent::IsValid(void) const
{


	// Check our state
	return(mEvent != NULL);
}





//============================================================================
//		NCarbonEvent::IsClass : Is the event a particular class?
//----------------------------------------------------------------------------
bool NCarbonEvent::IsClass(UInt32 theClass) const
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Check our class
	return(GetClass() == theClass);
}





//============================================================================
//		NCarbonEvent::IsKind : Is the event a particular kind?
//----------------------------------------------------------------------------
bool NCarbonEvent::IsKind(UInt32 theKind) const
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Check our kind
	return(GetKind() == theKind);
}





//============================================================================
//		NCarbonEvent::GetClass : Get the event class.
//----------------------------------------------------------------------------
UInt32 NCarbonEvent::GetClass(void) const
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Get our class
	return(GetEventClass(mEvent));
}





//============================================================================
//		NCarbonEvent::GetKind : Get the event kind.
//----------------------------------------------------------------------------
UInt32 NCarbonEvent::GetKind(void) const
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Get our kind
	return(GetEventKind(mEvent));
}





//============================================================================
//		NCarbonEvent::PostToQueue : Post the event.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::PostToQueue(EventQueueRef theQueue, EventPriority thePriority) const
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Post the event
	theErr = PostEventToQueue(theQueue, mEvent, thePriority);
	NN_ASSERT_NOERR(theErr);
	
	return(theErr);
}





//============================================================================
//		NCarbonEvent::SendToTarget : Send the event.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SendToTarget(EventTargetRef theTarget, OptionBits theOptions) const
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Send the event
	theErr = SendEventToEventTargetWithOptions(mEvent, theTarget, theOptions);
	NN_ASSERT(theErr == noErr || theErr == eventNotHandledErr);
	
	return(theErr);
}





//============================================================================
//		NCarbonEvent::SendToApplication : Send the event to the application.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SendToApplication(OptionBits theOptions) const
{


	// Send the event
	return(SendToTarget(GetApplicationEventTarget(), theOptions));
}





//============================================================================
//		NCarbonEvent::SendToMenu : Send the event to a menu.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SendToMenu(MenuRef theMenu, OptionBits theOptions) const
{


	// Send the event
	return(SendToTarget(GetMenuEventTarget(theMenu), theOptions));
}





//============================================================================
//		NCarbonEvent::SendToHIWindow : Send the event to a window.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SendToHIWindow(HIWindowRef theWindow, OptionBits theOptions) const
{


	// Send the event
	return(SendToTarget(GetWindowEventTarget(theWindow), theOptions));
}





//============================================================================
//		NCarbonEvent::SendToHIView : Send the event to a view.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SendToHIView(HIViewRef theView, OptionBits theOptions) const
{


	// Send the event
	return(SendToTarget(GetControlEventTarget(theView), theOptions));
}





//============================================================================
//		NCarbonEvent::SendToHIObject : Send the event to an object.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SendToHIObject(HIObjectRef theObject, OptionBits theOptions) const
{


	// Send the event
	return(SendToTarget(HIObjectGetEventTarget(theObject), theOptions));
}





//============================================================================
//		NCarbonEvent::SendToFocus : Send the event to the user focus.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SendToFocus(OptionBits theOptions) const
{


	// Send the event
	return(SendToTarget(GetUserFocusEventTarget(), theOptions));
}





//============================================================================
//		NCarbonEvent::GetEvent : Get the event.
//----------------------------------------------------------------------------
EventRef NCarbonEvent::GetEvent(void) const
{


	// Get the event
	return(mEvent);
}





//============================================================================
//		NCarbonEvent::GetTime : Get the event time.
//----------------------------------------------------------------------------
EventTime NCarbonEvent::GetTime(void) const
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Get the event time
	return(GetEventTime(mEvent));
}





//============================================================================
//		NCarbonEvent::SetTime : Set the event time.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SetTime(EventTime theTime)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Set the event time
	theErr = SetEventTime(mEvent, theTime);
	NN_ASSERT_NOERR(theErr);
	
	return(theErr);
}





//============================================================================
//		NCarbonEvent::HasParameter : Does the event have a parameter?
//----------------------------------------------------------------------------
bool NCarbonEvent::HasParameter(EventParamName theParam) const
{	EventParamType		theType;
	OSStatus			theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Look for the parameter
	theErr = GetEventParameter(mEvent, theParam, typeWildCard, &theType, 0, NULL, NULL);

	return(theErr == noErr);
}





//============================================================================
//		NCarbonEvent::GetParameter : Get an event parameter.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::GetParameter(EventParamName theParam, EventParamType theType, UInt32 theSize, void *thePtr) const
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theErr = GetEventParameter(mEvent, theParam, theType, NULL, theSize, NULL, thePtr);
	if (theErr != noErr)
		memset(thePtr, 0x00, theSize);
	
	return(theErr);
}





//============================================================================
//		NCarbonEvent::GetParameterBoolean : Get a boolean parameter.
//----------------------------------------------------------------------------
bool NCarbonEvent::GetParameterBoolean(EventParamName theParam) const
{	Boolean		theValue;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theErr = GetParameter(theParam, typeBoolean, sizeof(theValue), &theValue);
	
	return((bool) theValue);
}





//============================================================================
//		NCarbonEvent::GetParameterChar : Get a character parameter.
//----------------------------------------------------------------------------
char NCarbonEvent::GetParameterChar(EventParamName theParam) const
{	char		theValue;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theErr = GetParameter(theParam, typeChar, sizeof(theValue), &theValue);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetParameterOSType : Get an OSType parameter.
//----------------------------------------------------------------------------
OSType NCarbonEvent::GetParameterOSType(EventParamName theParam) const
{	OSType		theValue;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theErr = GetParameter(theParam, typeOSStatus, sizeof(theValue), &theValue);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetParameterOSStatus : Get an OSStatus parameter.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::GetParameterOSStatus(EventParamName theParam) const
{	OSStatus	theValue;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theErr = GetParameter(theParam, typeOSStatus, sizeof(theValue), &theValue);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetParameterUInt32 : Get a UInt32 parameter.
//----------------------------------------------------------------------------
UInt32 NCarbonEvent::GetParameterUInt32(EventParamName theParam) const
{	UInt32		theValue;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theErr = GetParameter(theParam, typeUInt32, sizeof(theValue), &theValue);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetParameterSInt32 : Get an SInt32 parameter.
//----------------------------------------------------------------------------
SInt32 NCarbonEvent::GetParameterSInt32(EventParamName theParam) const
{	SInt32		theValue;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theErr = GetParameter(theParam, typeSInt32, sizeof(theValue), &theValue);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetParameterUInt64 : Get a UInt64 parameter.
//----------------------------------------------------------------------------
UInt64 NCarbonEvent::GetParameterUInt64(EventParamName theParam) const
{	UInt64		theValue;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theErr = GetParameter(theParam, typeUInt64, sizeof(theValue), &theValue);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetParameterSInt64 : Get an SInt64 parameter.
//----------------------------------------------------------------------------
SInt64 NCarbonEvent::GetParameterSInt64(EventParamName theParam) const
{	SInt64		theValue;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theErr = GetParameter(theParam, typeSInt64, sizeof(theValue), &theValue);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetParameterFloat32 : Get a Float32 parameter.
//----------------------------------------------------------------------------
Float32 NCarbonEvent::GetParameterFloat32(EventParamName theParam) const
{	Float32		theValue;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theErr = GetParameter(theParam, typeIEEE32BitFloatingPoint, sizeof(theValue), &theValue);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetParameterFloat64 : Get a Float64 parameter.
//----------------------------------------------------------------------------
Float64 NCarbonEvent::GetParameterFloat64(EventParamName theParam) const
{	Float64		theValue;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theErr = GetParameter(theParam, typeIEEE64BitFloatingPoint, sizeof(theValue), &theValue);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetParameterPointer : Get a pointer parameter.
//----------------------------------------------------------------------------
void *NCarbonEvent::GetParameterPointer(EventParamName theParam) const
{	void		*theValue;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	//
	// Work around rdar://4852232 - typeVoidPtr and typePtr are not coerced
	// automatically, so treat them as equivalents ourselves (note this does
	// not apply to SetParameterPointer, which must always use a specific
	// type when setting the parameter - it uses typeVoidPtr).
	theErr = GetParameter(theParam, typeVoidPtr, sizeof(theValue), &theValue);
	if (theErr == errAECoercionFail)
		theErr = GetParameter(theParam, typePtr, sizeof(theValue), &theValue);

	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetParameterString : Get a string parameter.
//----------------------------------------------------------------------------
NString NCarbonEvent::GetParameterString(EventParamName theParam) const
{	UInt32			theSize, numChars;
	NData			theBuffer;
	CFStringRef		cfString;
	NString			theValue;
	OSStatus		theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Check for a CFStringRef
	//
	// This is the optimal path for fetching string parameters, as it avoids
	// a temporary buffer (and CFStringRef->typeUnicodeText promotion is only
	// available on 10.4 and above).
	cfString = NULL;
	theErr   = GetParameter(theParam, typeCFStringRef, sizeof(cfString), &cfString);
	if (theErr == noErr && cfString != NULL)
		{
		theValue.Set(cfString, false);
		return(theValue);
		}



	// Prepare the buffer
	theSize = 0;
	theErr  = GetEventParameter(mEvent, theParam, typeUnicodeText, NULL, 0, &theSize, NULL);

	if (!theBuffer.SetSize(theSize))
		return(theValue);



	// Get the text data
	theErr = GetEventParameter(mEvent, theParam, typeUnicodeText, NULL, theSize, NULL, theBuffer.GetData());
	if (theErr == noErr)
		{
		numChars = theSize / sizeof(UniChar);
		theValue = NString((const UniChar *) theBuffer.GetData(), numChars);
		}
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetParameterHIPoint : Get an HIPoint parameter.
//----------------------------------------------------------------------------
HIPoint NCarbonEvent::GetParameterHIPoint(EventParamName theParam) const
{	HIPoint		theValue;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theErr = GetParameter(theParam, typeHIPoint, sizeof(theValue), &theValue);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetParameterHISize : Get an HISize parameter.
//----------------------------------------------------------------------------
HISize NCarbonEvent::GetParameterHISize(EventParamName theParam) const
{	HISize		theValue;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theErr = GetParameter(theParam, typeHISize, sizeof(theValue), &theValue);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetParameterHIRect : Get an HIRect parameter.
//----------------------------------------------------------------------------
HIRect NCarbonEvent::GetParameterHIRect(EventParamName theParam) const
{	HIRect		theValue;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theErr = GetParameter(theParam, typeHIRect, sizeof(theValue), &theValue);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetParameterHICommand : Get an HICommand parameter.
//----------------------------------------------------------------------------
HICommandExtended NCarbonEvent::GetParameterHICommand(EventParamName theParam) const
{	HICommandExtended	theValue;
	OSStatus			theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theErr = GetParameter(theParam, typeHICommand, sizeof(theValue), &theValue);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetParameterHIObject : Get an HIObjectRef parameter.
//----------------------------------------------------------------------------
HIObjectRef NCarbonEvent::GetParameterHIObject(EventParamName theParam) const
{	HIObjectRef		theValue;
	OSStatus		theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theErr = GetParameter(theParam, typeHIObjectRef, sizeof(theValue), &theValue);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetParameterHIView : Get an HIViewRef parameter.
//----------------------------------------------------------------------------
HIViewRef NCarbonEvent::GetParameterHIView(EventParamName theParam) const
{	HIViewRef		theValue;
	OSStatus		theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theErr = GetParameter(theParam, typeControlRef, sizeof(theValue), &theValue);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetParameterHIWindow : Get an HIWindowRef parameter.
//----------------------------------------------------------------------------
HIWindowRef NCarbonEvent::GetParameterHIWindow(EventParamName theParam) const
{	HIWindowRef		theValue;
	OSStatus		theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theErr = GetParameter(theParam, typeWindowRef, sizeof(theValue), &theValue);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetParameterCFIndex : Get a CFIndex parameter.
//----------------------------------------------------------------------------
CFIndex NCarbonEvent::GetParameterCFIndex(EventParamName theParam) const
{	CFIndex		theValue;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theErr = GetParameter(theParam, typeCFIndex, sizeof(theValue), &theValue);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetParameterCFRange : Get a CFRange parameter.
//----------------------------------------------------------------------------
CFRange NCarbonEvent::GetParameterCFRange(EventParamName theParam) const
{	CFRange		theValue;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theErr = GetParameter(theParam, typeCFRange, sizeof(theValue), &theValue);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetParameterCFType : Get a CFTypeRef parameter.
//----------------------------------------------------------------------------
CFTypeRef NCarbonEvent::GetParameterCFType(EventParamName theParam) const
{	CFTypeRef	theValue;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theErr = GetParameter(theParam, typeCFTypeRef, sizeof(theValue), &theValue);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetParameterDrag : Get a DragRef parameter.
//----------------------------------------------------------------------------
DragRef NCarbonEvent::GetParameterDrag(EventParamName theParam) const
{	DragRef		theValue;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theErr = GetParameter(theParam, typeDragRef, sizeof(theValue), &theValue);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetParameterEvent : Get an EventRef parameter.
//----------------------------------------------------------------------------
EventRef NCarbonEvent::GetParameterEvent(EventParamName theParam) const
{	EventRef	theValue;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theErr = GetParameter(theParam, typeEventRef, sizeof(theValue), &theValue);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetParameterMenu : Get a MenuRef parameter.
//----------------------------------------------------------------------------
MenuRef NCarbonEvent::GetParameterMenu(EventParamName theParam) const
{	MenuRef		theValue;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theErr = GetParameter(theParam, typeMenuRef, sizeof(theValue), &theValue);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetParameterMenuItemIndex : Get a MenuItemIndex parameter.
//----------------------------------------------------------------------------
MenuItemIndex NCarbonEvent::GetParameterMenuItemIndex(EventParamName theParam) const
{	MenuItemIndex	theValue;
	OSStatus		theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theErr = GetParameter(theParam, typeMenuItemIndex, sizeof(theValue), &theValue);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::SetParameter : Set an event parameter.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SetParameter(EventParamName theParam, EventParamType theType, UInt32 theSize, const void *thePtr)
{	OSStatus	theErr;



	// Validate our parameters and state
	NN_ASSERT(IsValid());
	NN_ASSERT(theSize != 0);
	NN_ASSERT(thePtr  != NULL);



	// Set the parameter
	theErr = SetEventParameter(mEvent, theParam, theType, theSize, thePtr);
	NN_ASSERT_NOERR(theErr);
	
	return(theErr);
}





//============================================================================
//		NCarbonEvent::SetParameterBoolean : Set a boolean parameter.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SetParameterBoolean(EventParamName theParam, bool theValue)
{	Boolean		paramValue;



	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	paramValue = (Boolean) theValue;
	
	return(SetParameter(theParam, typeBoolean, sizeof(paramValue), &paramValue));
}





//============================================================================
//		NCarbonEvent::SetParameterChar : Set a character parameter.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SetParameterChar(EventParamName theParam, char theValue)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	return(SetParameter(theParam, typeChar, sizeof(theValue), &theValue));
}





//============================================================================
//		NCarbonEvent::SetParameterOSType : Set an OSType parameter.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SetParameterOSType(EventParamName theParam, OSType theValue)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	return(SetParameter(theParam, typeOSStatus, sizeof(theValue), &theValue));
}





//============================================================================
//		NCarbonEvent::SetParameterOSStatus : Set an OSStatus parameter.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SetParameterOSStatus(EventParamName theParam, OSStatus theValue)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	return(SetParameter(theParam, typeOSStatus, sizeof(theValue), &theValue));
}





//============================================================================
//		NCarbonEvent::SetParameterUInt32 : Set a UInt32 parameter.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SetParameterUInt32(EventParamName theParam, UInt32 theValue)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	return(SetParameter(theParam, typeUInt32, sizeof(theValue), &theValue));
}





//============================================================================
//		NCarbonEvent::SetParameterSInt32 : Set an SInt32 parameter.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SetParameterSInt32(EventParamName theParam, SInt32 theValue)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	return(SetParameter(theParam, typeSInt32, sizeof(theValue), &theValue));
}





//============================================================================
//		NCarbonEvent::SetParameterUInt64 : Set a UInt64 parameter.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SetParameterUInt64(EventParamName theParam, UInt64 theValue)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	return(SetParameter(theParam, typeUInt64, sizeof(theValue), &theValue));
}





//============================================================================
//		NCarbonEvent::SetParameterSInt64 : Set an SInt64 parameter.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SetParameterSInt64(EventParamName theParam, SInt64 theValue)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	return(SetParameter(theParam, typeSInt64, sizeof(theValue), &theValue));
}





//============================================================================
//		NCarbonEvent::SetParameterFloat32 : Set a Float32 parameter.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SetParameterFloat32(EventParamName theParam, Float32 theValue)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	return(SetParameter(theParam, typeIEEE32BitFloatingPoint, sizeof(theValue), &theValue));
}





//============================================================================
//		NCarbonEvent::SetParameterFloat64 : Set a Float64 parameter.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SetParameterFloat64(EventParamName theParam, Float64 theValue)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	return(SetParameter(theParam, typeIEEE64BitFloatingPoint, sizeof(theValue), &theValue));
}





//============================================================================
//		NCarbonEvent::SetParameterPointer : Set a pointer parameter.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SetParameterPointer(EventParamName theParam, void *theValue)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	return(SetParameter(theParam, typeVoidPtr, sizeof(theValue), &theValue));
}





//============================================================================
//		NCarbonEvent::SetParameterString : Set a string parameter.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SetParameterString(EventParamName theParam, const NString &theValue)
{	const UniChar		*thePtr;
	UInt32				theSize;



	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	thePtr  = theValue.GetUTF16();
	theSize = theValue.GetSizeBytes(kCFStringEncodingUTF16);
	
	return(SetParameter(theParam, typeUnicodeText, theSize, thePtr));
}





//============================================================================
//		NCarbonEvent::SetParameterHIPoint : Set an HIPoint parameter.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SetParameterHIPoint(EventParamName theParam, const HIPoint &theValue)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	return(SetParameter(theParam, typeHIPoint, sizeof(theValue), &theValue));
}





//============================================================================
//		NCarbonEvent::SetParameterHISize : Set an HISize parameter.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SetParameterHISize(EventParamName theParam, const HISize &theValue)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	return(SetParameter(theParam, typeHISize, sizeof(theValue), &theValue));
}





//============================================================================
//		NCarbonEvent::SetParameterHIRect : Set an HIRect parameter.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SetParameterHIRect(EventParamName theParam, const HIRect &theValue)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	return(SetParameter(theParam, typeHIRect, sizeof(theValue), &theValue));
}





//============================================================================
//		NCarbonEvent::SetParameterHICommand : Set an HICommand parameter.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SetParameterHICommand(EventParamName theParam, const HICommandExtended &theValue)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	return(SetParameter(theParam, typeHICommand, sizeof(theValue), &theValue));
}





//============================================================================
//		NCarbonEvent::SetParameterHIObject : Set an HIObjectRef parameter.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SetParameterHIObject(EventParamName theParam, HIObjectRef theValue)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	return(SetParameter(theParam, typeHIObjectRef, sizeof(theValue), &theValue));
}





//============================================================================
//		NCarbonEvent::SetParameterHIView : Set an HIViewRef parameter.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SetParameterHIView(EventParamName theParam, HIViewRef theValue)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	return(SetParameter(theParam, typeControlRef, sizeof(theValue), &theValue));
}





//============================================================================
//		NCarbonEvent::SetParameterHIWindow : Set an HIWindowRef parameter.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SetParameterHIWindow(EventParamName theParam, HIWindowRef theValue)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	return(SetParameter(theParam, typeWindowRef, sizeof(theValue), &theValue));
}





//============================================================================
//		NCarbonEvent::SetParameterCFIndex : Set an CFIndex parameter.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SetParameterCFIndex(EventParamName theParam, CFIndex theValue)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	return(SetParameter(theParam, typeCFIndex, sizeof(theValue), &theValue));
}





//============================================================================
//		NCarbonEvent::SetParameterCFRange : Set an CFRange parameter.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SetParameterCFRange(EventParamName theParam, const CFRange &theValue)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	return(SetParameter(theParam, typeCFRange, sizeof(theValue), &theValue));
}





//============================================================================
//		NCarbonEvent::SetParameterCFType : Set an CFTypeRef parameter.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SetParameterCFType(EventParamName theParam, CFTypeRef theValue)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	return(SetParameter(theParam, typeCFTypeRef, sizeof(theValue), &theValue));
}





//============================================================================
//		NCarbonEvent::SetParameterDrag : Set a DragRef parameter.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SetParameterDrag(EventParamName theParam, DragRef theValue)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	return(SetParameter(theParam, typeDragRef, sizeof(theValue), &theValue));
}





//============================================================================
//		NCarbonEvent::SetParameterEvent : Set an EventRef parameter.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SetParameterEvent(EventParamName theParam, EventRef theValue)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	return(SetParameter(theParam, typeEventRef, sizeof(theValue), &theValue));
}





//============================================================================
//		NCarbonEvent::SetParameterMenu : Set an MenuRef parameter.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SetParameterMenu(EventParamName theParam, MenuRef theValue)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	return(SetParameter(theParam, typeMenuRef, sizeof(theValue), &theValue));
}





//============================================================================
//		NCarbonEvent::SetParameterMenuItemIndex : Set an MenuItemIndex parameter.
//----------------------------------------------------------------------------
OSStatus NCarbonEvent::SetParameterMenuItemIndex(EventParamName theParam, MenuItemIndex theValue)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	return(SetParameter(theParam, typeMenuItemIndex, sizeof(theValue), &theValue));
}





//============================================================================
//		NCarbonEvent::GetHICommand : Get the HICommand.
//----------------------------------------------------------------------------
HICommandExtended NCarbonEvent::GetHICommand(void) const
{	HICommandExtended	theValue;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theValue = GetParameterHICommand(kEventParamDirectObject);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetHIWindow : Get the HIWindowRef.
//----------------------------------------------------------------------------
HIWindowRef NCarbonEvent::GetHIWindow(void) const
{	HIWindowRef		theValue;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theValue = GetParameterHIWindow(kEventParamDirectObject);
	NN_ASSERT(theValue != NULL && IsValidWindowPtr(theValue));
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetHIView : Get the HIViewRef.
//----------------------------------------------------------------------------
HIViewRef NCarbonEvent::GetHIView(void) const
{	HIViewRef	theValue;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theValue = GetParameterHIView(kEventParamDirectObject);
	NN_ASSERT(theValue != NULL);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetCGContext : Get the CGContextRef.
//----------------------------------------------------------------------------
CGContextRef NCarbonEvent::GetCGContext(void) const
{	CGContextRef	theValue;
	OSStatus		theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theErr = GetParameter(kEventParamCGContextRef, typeCGContextRef, sizeof(CGContextRef), &theValue);
	NN_ASSERT(theValue != NULL && CFGetTypeID(theValue) == CGContextGetTypeID());
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetMenu : Get the MenuRef.
//----------------------------------------------------------------------------
MenuRef NCarbonEvent::GetMenu(void) const
{	MenuRef		theValue;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theValue = GetParameterMenu(kEventParamDirectObject);
	NN_ASSERT(theValue != NULL && IsValidMenu(theValue));
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::SetHICommand : Set the HICommand.
//----------------------------------------------------------------------------
void NCarbonEvent::SetHICommand(const HICommandExtended &theValue)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	theErr = SetParameterHICommand(kEventParamDirectObject, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NCarbonEvent::SetHIWindow : Set the HIWindowRef.
//----------------------------------------------------------------------------
void NCarbonEvent::SetHIWindow(HIWindowRef theValue)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	theErr = SetParameterHIWindow(kEventParamDirectObject, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NCarbonEvent::SetHIView : Set the HIViewRef.
//----------------------------------------------------------------------------
void NCarbonEvent::SetHIView(HIViewRef theValue)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	theErr = SetParameterHIView(kEventParamDirectObject, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NCarbonEvent::SetCGContext : Set the CGContextRef.
//----------------------------------------------------------------------------
void NCarbonEvent::SetCGContext(CGContextRef theValue)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	theErr = SetParameter(kEventParamCGContextRef, typeCGContextRef, sizeof(CGContextRef), &theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NCarbonEvent::SetMenu : Set the MenuRef.
//----------------------------------------------------------------------------
void NCarbonEvent::SetMenu(MenuRef theValue)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	theErr = SetParameterMenu(kEventParamDirectObject, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NCarbonEvent::GetKeyChar : Get the key character.
//----------------------------------------------------------------------------
char NCarbonEvent::GetKeyChar(void) const
{	char	theValue;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theValue = GetParameterChar(kEventParamKeyMacCharCodes);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetKeyCode : Get the key code.
//----------------------------------------------------------------------------
UInt32 NCarbonEvent::GetKeyCode(void) const
{	UInt32	theValue;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theValue = GetParameterUInt32(kEventParamKeyCode);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetKeyModifiers : Get the key modifiers.
//----------------------------------------------------------------------------
UInt32 NCarbonEvent::GetKeyModifiers(void) const
{	UInt32	theValue;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theValue = GetParameterUInt32(kEventParamKeyModifiers);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::SetKeyChar : Set the key character.
//----------------------------------------------------------------------------
void NCarbonEvent::SetKeyChar(char theValue)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	theErr = SetParameterChar(kEventParamKeyMacCharCodes, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NCarbonEvent::SetKeyCode : Set the key code.
//----------------------------------------------------------------------------
void NCarbonEvent::SetKeyCode(UInt32 theValue)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	theErr = SetParameterUInt32(kEventParamKeyCode, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NCarbonEvent::SetKeyModifiers : Set the key modifiers.
//----------------------------------------------------------------------------
void NCarbonEvent::SetKeyModifiers(UInt32 theValue)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	theErr = SetParameterUInt32(kEventParamKeyModifiers, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NCarbonEvent::GetMouseButton : Get the mouse button.
//----------------------------------------------------------------------------
EventMouseButton NCarbonEvent::GetMouseButton(void) const
{	EventMouseButton	theValue;
	OSStatus			theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theErr = GetParameter(kEventParamMouseButton, typeMouseButton, sizeof(theValue), &theValue);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetMouseClickCount : Get the mouse click count.
//----------------------------------------------------------------------------
UInt32 NCarbonEvent::GetMouseClickCount(void) const
{	UInt32	theValue;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theValue = GetParameterUInt32(kEventParamClickCount);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetMouseDelta : Get the mouse delta.
//----------------------------------------------------------------------------
HIPoint NCarbonEvent::GetMouseDelta(void) const
{	HIPoint		theValue;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theValue = GetParameterHIPoint(kEventParamMouseDelta);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetMouseLocation : Get the mouse location.
//----------------------------------------------------------------------------
HIPoint NCarbonEvent::GetMouseLocation(void) const
{	HIPoint		theValue;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theValue = GetParameterHIPoint(kEventParamMouseLocation);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetMouseWheelAxis : Get the mouse wheel axis.
//----------------------------------------------------------------------------
EventMouseWheelAxis NCarbonEvent::GetMouseWheelAxis(void) const
{	EventMouseWheelAxis		theValue;
	OSStatus				theErr;


	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theErr = GetParameter(kEventParamMouseWheelAxis, typeMouseWheelAxis, sizeof(theValue), &theValue);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetMouseWheelDelta : Get the mouse wheel delta.
//----------------------------------------------------------------------------
SInt32 NCarbonEvent::GetMouseWheelDelta(void) const
{	SInt32		theValue;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theValue = GetParameterSInt32(kEventParamMouseWheelDelta);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetMouseScrollDelta : Get the mouse scroll delta.
//----------------------------------------------------------------------------
HIPoint NCarbonEvent::GetMouseScrollDelta(void) const
{	HIPoint		theValue;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theValue.x = GetParameterSInt32(kEventParamMouseWheelSmoothHorizontalDelta);
	theValue.y = GetParameterSInt32(kEventParamMouseWheelSmoothVerticalDelta);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::SetMouseButton : Set the mouse button.
//----------------------------------------------------------------------------
void NCarbonEvent::SetMouseButton(EventMouseButton theValue)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	theErr = SetParameter(kEventParamMouseButton, typeMouseButton, sizeof(theValue), &theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NCarbonEvent::SetMouseClickCount : Set the mouse click count.
//----------------------------------------------------------------------------
void NCarbonEvent::SetMouseClickCount(UInt32 theValue)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	theErr = SetParameterUInt32(kEventParamClickCount, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NCarbonEvent::SetMouseDelta : Set the mouse delta.
//----------------------------------------------------------------------------
void NCarbonEvent::SetMouseDelta(const HIPoint &theValue)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	theErr = SetParameterHIPoint(kEventParamMouseDelta, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NCarbonEvent::SetMouseLocation : Set the mouse location.
//----------------------------------------------------------------------------
void NCarbonEvent::SetMouseLocation(const HIPoint &theValue)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	theErr = SetParameterHIPoint(kEventParamMouseLocation, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NCarbonEvent::SetMouseWheelAxis : Set the mouse wheel axis.
//----------------------------------------------------------------------------
void NCarbonEvent::SetMouseWheelAxis(EventMouseWheelAxis theValue)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	theErr = SetParameter(kEventParamMouseWheelAxis, typeMouseWheelAxis, sizeof(theValue), &theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NCarbonEvent::SetMouseWheelDelta : Set the mouse wheel delta.
//----------------------------------------------------------------------------
void NCarbonEvent::SetMouseWheelDelta(SInt32 theValue)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	theErr = SetParameterSInt32(kEventParamMouseWheelDelta, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NCarbonEvent::SetMouseScrollDelta : Set the mouse scroll delta.
//----------------------------------------------------------------------------
void NCarbonEvent::SetMouseScrollDelta(const HIPoint &theValue)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	theErr  = SetParameterSInt32(kEventParamMouseWheelSmoothHorizontalDelta, (SInt32) theValue.x);
	theErr |= SetParameterSInt32(kEventParamMouseWheelSmoothVerticalDelta,   (SInt32) theValue.y);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NCarbonEvent::GetViewPartCode : Get a view part code.
//----------------------------------------------------------------------------
HIViewPartCode NCarbonEvent::GetViewPartCode(void) const
{	HIViewPartCode		theValue;
	OSStatus			theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theErr = GetParameter(kEventParamControlPart, typeControlPartCode, sizeof(theValue), &theValue);
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::GetViewTrackingAreaID : Get a view tracking area ID.
//----------------------------------------------------------------------------
HIViewTrackingAreaID NCarbonEvent::GetViewTrackingAreaID(void) const
{	HIViewTrackingAreaID	theValue;
	HIViewTrackingAreaRef	theArea;
	OSStatus				theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the parameter
	theValue = kHIViewTrackingAreaNone;
	theErr   = GetParameter(kEventParamHIViewTrackingArea, typeHIViewTrackingAreaRef, sizeof(theArea), &theArea);
	
	if (theErr == noErr)
		{
		theErr = HIViewGetTrackingAreaID(theArea, &theValue);
		NN_ASSERT_NOERR(theErr);
		}
	
	return(theValue);
}





//============================================================================
//		NCarbonEvent::SetViewPartCode : Set a view part code.
//----------------------------------------------------------------------------
void NCarbonEvent::SetViewPartCode(HIViewPartCode theValue)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Set the parameter
	theErr = SetParameter(kEventParamControlPart, typeControlPartCode, sizeof(theValue), &theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NCarbonEvent::= : Assignment operator.
//----------------------------------------------------------------------------
const NCarbonEvent& NCarbonEvent::operator = (const NCarbonEvent &theEvent)
{


	// Assign the object
	if (this != &theEvent)
		{
		if (mEvent != NULL)
			ReleaseEvent(mEvent);
	
		InitializeSelf(theEvent.mEvent);
		}
	
	return(*this);
}





//============================================================================
//		NCarbonEvent::CreateCommand : Create a kEventCommandProcess event.
//----------------------------------------------------------------------------
NCarbonEvent NCarbonEvent::CreateCommand(UInt32 theCmd)
{	NCarbonEvent			theEvent(kEventClassCommand, kEventCommandProcess);
	OSStatus				theErr;
	HICommandExtended		hiCmd;



	// Create the event
	memset(&hiCmd, 0x00, sizeof(hiCmd));
	
	hiCmd.commandID = theCmd;

	theErr = theEvent.SetParameterHICommand(kEventParamDirectObject, hiCmd);
	NN_ASSERT_NOERR(theErr);

	return(theEvent);
}





//============================================================================
//		NCarbonEvent::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
void NCarbonEvent::InitializeSelf(EventRef theEvent)
{


	// Initialize ourselves
	mEvent = theEvent;

	if (mEvent != NULL)
		RetainEvent(mEvent);
}







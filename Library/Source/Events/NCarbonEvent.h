/*	NAME:
		NCarbonEvent.h

	DESCRIPTION:
		Carbon Event object.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCARBONEVENT_HDR
#define NCARBONEVENT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCarbonEvent {
public:
										NCarbonEvent(UInt32 theClass, UInt32 theKind);
										NCarbonEvent(const NCarbonEvent &theEvent);
										NCarbonEvent(EventRef            theEvent);

										NCarbonEvent(void);
	virtual								~NCarbonEvent(void);


	// Is the event valid?
	bool								IsValid(void) const;


	// Is the event a particular class/kind?
	bool								IsClass(UInt32 theClass) const;
	bool								IsKind( UInt32 theKind)  const;


	// Get the event class/kind
	UInt32								GetClass(void) const;
	UInt32								GetKind( void) const;


	// Post the event
	OSStatus							PostToQueue(EventQueueRef theQueue    = GetMainEventQueue(),
													EventPriority thePriority = kEventPriorityStandard) const;


	// Send the event
	OSStatus							SendToTarget(EventTargetRef theTarget, OptionBits theOptions=0) const;
	OSStatus							SendToApplication(                     OptionBits theOptions=0) const;
	OSStatus							SendToMenu(         MenuRef theMenu,   OptionBits theOptions=0) const;
	OSStatus							SendToHIWindow( HIWindowRef theWindow, OptionBits theOptions=0) const;
	OSStatus							SendToHIView(     HIViewRef theView,   OptionBits theOptions=0) const;
	OSStatus							SendToHIObject( HIObjectRef theObject, OptionBits theOptions=0) const;
	OSStatus							SendToFocus(                           OptionBits theOptions=0) const;


	// Get the event
	EventRef							GetEvent(void) const;


	// Get/set the event time
	EventTime							GetTime(void) const;
	OSStatus							SetTime(EventTime theTime);


	// Does the event have a parameter?
	bool								HasParameter(EventParamName theParam) const;
	

	// Get/set an event parameter
	//
	// If the parameter does not exist, a 0'd result is returned.
	OSStatus							GetParameter(             EventParamName theParam, EventParamType theType, UInt32 theSize, void *thePtr) const;
	bool								GetParameterBoolean(      EventParamName theParam) const;
	char								GetParameterChar(         EventParamName theParam) const;
	OSType								GetParameterOSType(       EventParamName theParam) const;
	OSStatus							GetParameterOSStatus(     EventParamName theParam) const;
	UInt32								GetParameterUInt32(       EventParamName theParam) const;
	SInt32								GetParameterSInt32(       EventParamName theParam) const;
	UInt64								GetParameterUInt64(       EventParamName theParam) const;
	SInt64								GetParameterSInt64(       EventParamName theParam) const;
	Float32								GetParameterFloat32(      EventParamName theParam) const;
	Float64								GetParameterFloat64(      EventParamName theParam) const;
	void							   *GetParameterPointer(      EventParamName theParam) const;
	NString								GetParameterString(       EventParamName theParam) const;
	HIPoint								GetParameterHIPoint(      EventParamName theParam) const;
	HISize								GetParameterHISize(       EventParamName theParam) const;
	HIRect								GetParameterHIRect(       EventParamName theParam) const;
	HICommandExtended					GetParameterHICommand(    EventParamName theParam) const;
	HIObjectRef							GetParameterHIObject(     EventParamName theParam) const;
	HIViewRef							GetParameterHIView(       EventParamName theParam) const;
	HIWindowRef							GetParameterHIWindow(     EventParamName theParam) const;
	CFTypeRef							GetParameterCFType(       EventParamName theParam) const;
	DragRef								GetParameterDrag(         EventParamName theParam) const;
	MenuRef								GetParameterMenu(         EventParamName theParam) const;
	MenuItemIndex						GetParameterMenuItemIndex(EventParamName theParam) const;

	OSStatus							SetParameter(             EventParamName theParam, EventParamType theType, UInt32 theSize, const void *thePtr);
	OSStatus							SetParameterBoolean(      EventParamName theParam, bool                     theValue);
	OSStatus							SetParameterChar(         EventParamName theParam, char                     theValue);
	OSStatus							SetParameterOSType(       EventParamName theParam, OSType                   theValue);
	OSStatus							SetParameterOSStatus(     EventParamName theParam, OSStatus                 theValue);
	OSStatus							SetParameterUInt32(       EventParamName theParam, UInt32                   theValue);
	OSStatus							SetParameterSInt32(       EventParamName theParam, SInt32                   theValue);
	OSStatus							SetParameterUInt64(       EventParamName theParam, UInt64                   theValue);
	OSStatus							SetParameterSInt64(       EventParamName theParam, SInt64                   theValue);
	OSStatus							SetParameterFloat32(      EventParamName theParam, Float32                  theValue);
	OSStatus							SetParameterFloat64(      EventParamName theParam, Float64                  theValue);
	OSStatus							SetParameterPointer(      EventParamName theParam, void                    *theValue);
	OSStatus							SetParameterString(       EventParamName theParam, const NString           &theValue);
	OSStatus							SetParameterHIPoint(      EventParamName theParam, const HIPoint           &theValue);
	OSStatus							SetParameterHISize(       EventParamName theParam, const HISize            &theValue);
	OSStatus							SetParameterHIRect(       EventParamName theParam, const HIRect            &theValue);
	OSStatus							SetParameterHICommand(    EventParamName theParam, const HICommandExtended &theValue);
	OSStatus							SetParameterHIObject(     EventParamName theParam, HIObjectRef              theValue);
	OSStatus							SetParameterHIView(       EventParamName theParam, HIViewRef                theValue);
	OSStatus							SetParameterHIWindow(     EventParamName theParam, HIWindowRef              theValue);
	OSStatus							SetParameterCFType(       EventParamName theParam, CFTypeRef                theValue);
	OSStatus							SetParameterDrag(         EventParamName theParam, DragRef                  theValue);
	OSStatus							SetParameterMenu(         EventParamName theParam, MenuRef                  theValue);
	OSStatus							SetParameterMenuItemIndex(EventParamName theParam, MenuItemIndex            theValue);


	// Get/set object parameters
	HICommandExtended					GetHICommand(void) const;
	HIWindowRef							GetHIWindow( void) const;
	HIViewRef							GetHIView(   void) const;
	CGContextRef						GetCGContext(void) const;
	MenuRef								GetMenu(     void) const;
	
	void								SetHICommand(const HICommandExtended &theValue);
	void								SetHIWindow(       HIWindowRef        theValue);
	void								SetHIView(         HIViewRef          theValue);
	void								SetCGContext(      CGContextRef       theValue);
	void								SetMenu(           MenuRef            theValue);


	// Get/set keyboard parameters
	char								GetKeyChar(void)      const;
	UInt32								GetKeyCode(void)      const;
	UInt32								GetKeyModifiers(void) const;
	
	void								SetKeyChar(     char   theValue);
	void								SetKeyCode(     UInt32 theValue);
	void								SetKeyModifiers(UInt32 theValue);


	// Get/set mouse parameters
	HIPoint								GetMouseLocation(  void) const;
	HIPoint								GetMouseDelta(     void) const;
	EventMouseButton					GetMouseButton(    void) const;
	EventMouseWheelAxis					GetMouseWheelAxis( void) const;
	SInt32								GetMouseWheelDelta(void) const;
	UInt32								GetMouseClickCount(void) const;

	void								SetMouseLocation(  const HIPoint       &theValue);
	void								SetMouseDelta(     const HIPoint       &theValue);
	void								SetMouseButton(    EventMouseButton    theValue);
	void								SetMouseWheelAxis( EventMouseWheelAxis theValue);
	void								SetMouseWheelDelta(SInt32              theValue);
	void								SetMouseClickCount(UInt32              theValue);


	// Get/set view parameters
	HIViewPartCode						GetViewPartCode(void) const;

	void								SetViewPartCode(HIViewPartCode theValue);


	// Operators
	const NCarbonEvent&					operator = (const NCarbonEvent &theEvent);


	// Create a kEventCommandProcess event
	static NCarbonEvent					CreateCommand(UInt32 theCmd);


private:
	void								InitializeSelf(EventRef theEvent);


private:
	EventRef							mEvent;
};





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
inline bool operator ==(const EventTypeSpec &value1, const EventTypeSpec &value2)
{
	return(value1.eventClass == value2.eventClass && value1.eventKind == value2.eventKind);
}




#endif // NCARBONEVENT_HDR



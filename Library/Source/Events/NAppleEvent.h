/*	NAME:
		NAppleEvent.h

	DESCRIPTION:
		Apple Event object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NAPPLEEVENT_HDR
#define NAPPLEEVENT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NAppleEvent {
public:
										NAppleEvent(AEEventClass theClass, AEEventID theID, const AEAddressDesc *theTarget=NULL);
										NAppleEvent(const NAppleEvent &theEvent);
										NAppleEvent(const AppleEvent  &theEvent);

										NAppleEvent(void);
	virtual								~NAppleEvent(void);


	// Is the event valid?
	bool								IsValid(void) const;


	// Is the event a particular class/ID?
	bool								IsClass(AEEventClass theClass) const;
	bool								IsID(   AEEventID    theID)    const;


	// Get the event class/ID
	AEEventClass						GetClass(void) const;
	AEEventID							GetID(   void) const;


	// Send the event
	OSStatus							Send(AESendMode			theMode     = kAENoReply | kAEAlwaysInteract | kAECanSwitchLayer,
											 AESendPriority		thePriority = kAENormalPriority,
											 long				theTimeout  = kAEDefaultTimeout) const;


	// Get the event
	AppleEvent							GetEvent(void) const;


	// Get/set an event parameter
	//
	// If the parameter does not exist, a 0'd result is returned.
	OSStatus							GetParameter(AEKeyword theParam, DescType theType, UInt32 theSize, void *thePtr) const;

	OSStatus							SetParameter(AEKeyword theParam, DescType theType, UInt32 theSize, const void *thePtr);


	// Get/set an event attribute
	//
	// If the attribute does not exist, a 0'd result is returned.
	OSStatus							GetAttribute(AEKeyword theAttribute, DescType theType, UInt32 theSize, void *thePtr) const;

	OSStatus							SetAttribute(AEKeyword theAttribute, DescType theType, UInt32 theSize, const void *thePtr);


	// Operators
	const NAppleEvent&					operator = (const NAppleEvent &theEvent);


private:
	void								InitializeSelf(const AppleEvent *theEvent);

	void								ReleaseEvent(AppleEvent &theEvent);


private:
	AppleEvent							mEvent;
};




#endif // NAPPLEEVENT_HDR



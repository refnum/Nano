/*	NAME:
		NHIObject.h

	DESCRIPTION:
		HIObjectRef object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NHIOBJECT_HDR
#define NHIOBJECT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <map>

#include "NCarbonEventHandler.h"
#include "NString.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Classes
class NHIObject;
class NHIObjectFactory;


// Maps
typedef std::map<HIObjectRef, NHIObject*>					NHIObjectMap;
typedef NHIObjectMap::iterator								NHIObjectMapIterator;
typedef NHIObjectMap::const_iterator						NHIObjectMapConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NHIObject : public NCarbonEventHandler {
friend class NHIObjectFactory;
public:
										NHIObject(void);
	virtual								~NHIObject(void);


	// Get the HIObject
	HIObjectRef							GetHIObject(void) const;


	// Get the class ID
	NString								GetClassID(void) const;


	// Is this object of a certain class?
	bool								IsOfClass(const NString &classID) const;
	

	// Get the NHIObject from an HIObjectRef
	static NHIObject					*GetNHIObject(HIObjectRef hiObject);


protected:
	// Handle events
	virtual OSStatus					DoEventHIObjectConstruct(NCarbonEvent &theEvent);
	virtual OSStatus					DoEventHIObjectDestruct( NCarbonEvent &theEvent);


	// Set the HIObject
	//
	// Objects which are constructed as a wrapper around an existing HIObjectRef,
	// rather than those instantiated as a sub-class of an HIObject class, must
	// assign their HIObjectRef to the base class.
	virtual void						SetHIObject(HIObjectRef hiObject);


private:
	void								SaveObject(  void);
	void								ForgetObject(void);
	
	static NHIObjectMap					&GetObjects(void);
	

private:
	HIObjectRef							mObject;
};



#endif // NHIOBJECT_HDR



/*	NAME:
		NHIObjectFactory.h

	DESCRIPTION:
		HIObject factory.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NHIOBJECTFACTORY_HDR
#define NHIOBJECTFACTORY_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <map>

#include "NString.h"
#include "NHIObject.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Factory constructor
typedef NHIObject *(*HIObjectFactory)(void);


// Factory info
typedef struct {
	NString				baseID;
	HIObjectClassRef	theClass;
	HIObjectFactory		theFactory;
} HIFactoryInfo;


// Maps
typedef std::map<NString, HIFactoryInfo>					HIFactoryInfoMap;
typedef HIFactoryInfoMap::iterator							HIFactoryInfoMapIterator;
typedef HIFactoryInfoMap::const_iterator					HIFactoryInfoMapConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NHIObjectFactory {
public:
	// Create an object
	//
	// Creates an NHIObject using the appropriate factory for the HIObjectRef's class.
	static NHIObject					*CreateObject(HIObjectRef hiObject);


	// Add a factory
	//
	// An HIObjectFactory is used to instantiate instances of a given class,
	// and optionally sub-class a system class.
	//
	// NHIObjects come in two forms; wrappers around HIObjects, and sub-classes
	// of system classes. Wrappers should be constructed with our CreateObject
	// method, while true sub-classes should be created with HIObjectCreate.
	static OSStatus						AddFactory(HIObjectFactory	theFactory,
													const NString	&classID,
													const NString	&baseID="");


	// Remove the factories
	//
	// HIObject classes are registered on a per-process basis.
	//
	// If a factory is added by a plug-in which can be unloaded, that plug-in
	// must unregister its factories before exiting (or the system will reuse
	// the stale factory if future instances of its class are instantiated).
	static void							RemoveFactories(void);


	// Set the next instance
	//
	// When instantiating an HIObject which has a factory registered against it,
	// an existing NHIObject can be supplied to act as the instance rather than
	// using the factory.
	//
	// This allows an NHIObject's constructor to use HIObjectCreate to instantiate
	// the HIObjectRef, and use the existing NHIObject as the instance for the
	// HIObjectRef (rather than creating a second object).
	static void							SetNextInstance(NHIObject *theInstance);


private:
	static HIFactoryInfoMap				&GetFactories(void);
	static OSStatus						EventHandler(EventHandlerCallRef callRef, EventRef eventRef, void *userData);


private:
	static NHIObject					*mNextInstance;
};





#endif // NHIOBJECTFACTORY_HDR



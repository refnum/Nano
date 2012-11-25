/*	NAME:
		NNetworkBrowser.h

	DESCRIPTION:
		Network service browser.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NNETWORKBROWSER_HDR
#define NNETWORKBROWSER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Events
typedef enum {
	kNServiceWasAdded,
	kNServiceWasRemoved
} NBrowserEventKind;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Events
typedef struct {
	NBrowserEventKind	eventKind;
	NString				serviceType;
	NString				serviceName;
	NString				serviceDomain;
	NString				hostName;
	UInt16				hostPort;
} NNetworkBrowserEvent;


// Functors
typedef nfunctor<void (const NNetworkBrowserEvent &theEvent)>		NNetworkBrowserEventFunctor;


// Internal
typedef struct NServiceBrowser *NServiceBrowserRef;


// Lists
typedef std::vector<NServiceBrowserRef>								NServiceBrowserList;
typedef NServiceBrowserList::iterator								NServiceBrowserListIterator;
typedef NServiceBrowserList::const_iterator							NServiceBrowserListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NNetworkBrowser {
public:
										NNetworkBrowser(void);
	virtual							   ~NNetworkBrowser(void);


	// Is the browser active?
	bool								IsActive(void) const;


	// Start/stop browsing
	void								StartBrowsing(void);
	void								StopBrowsing( void);


	// Get/set the event handler
	NNetworkBrowserEventFunctor			GetEventHandler(void);
	void								SetEventHandler(const NNetworkBrowserEventFunctor &theFunctor);


	// Get/set the services
	NStringList							GetServices(void) const;
	void								SetServices(const NStringList &theServices);


private:
	void								DispatchEvent(const NNetworkBrowserEvent &theEvent);


private:
	NNetworkBrowserEventFunctor			mEventHandler;
	NStringList							mServices;
	NServiceBrowserList					mBrowsers;
};




#endif // NNETWORKBROWSER_HDR








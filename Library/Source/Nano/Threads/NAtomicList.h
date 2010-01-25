/*	NAME:
		NAtomicList.h

	DESCRIPTION:
		Atomic list.
	
	COPYRIGHT:
        Copyright (c) 2006-2010, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NATOMICLIST_HDR
#define NATOMICLIST_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NContainer.h"
#include "NLock.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
template<class T> class NAtomicList : public NContainer {
public:
										NAtomicList(void);
	virtual							   ~NAtomicList(void);


	// Get the size
	NIndex								GetSize(void) const;


	// Clear the list
	void								Clear(void);


	// Lock/unlock the list
	//
	// Although individual mutations are atomic, multiple operations can be
	// grouped together by locking the queue for exclusive access.
	bool								Lock(NTime theTime=kNTimeForever);
	void								Unlock(void);


	// Get a value from the list
	T									GetValue(NIndex theIndex) const;


	// Remove a value from the list
	void								RemoveValue(T theValue);


	// Push a value to the list
	void								PushFront(T theValue);
	void								PushBack( T theValue);


	// Pop a value from the list
	//
	// Returns true/false as the list contained any items to remove.
	bool								PopFront(T &theValue);
	bool								PopBack( T &theValue);


private:
	typedef std::list<T>					TList;
	typedef typename TList::iterator		TListIterator;
	typedef typename TList::const_iterator	TListConstIterator;

	mutable NMutexLock					mMutex;
	TList								mList;
};





//============================================================================
//		Template files
//----------------------------------------------------------------------------
#define   NATOMICLIST_CPP
#include "NAtomicList.cpp"
#undef    NATOMICLIST_CPP




#endif // NATOMICLIST_HDR




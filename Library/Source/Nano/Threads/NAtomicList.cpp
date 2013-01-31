/*	NAME:
		NAtomicList.cpp

	DESCRIPTION:
		Atomic list.
	
	COPYRIGHT:
        Copyright (c) 2006-2010, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Linker
//----------------------------------------------------------------------------
#ifndef NATOMICLIST_CPP

void NAtomicList_SuppressNoCodeLinkerWarning(void);
void NAtomicList_SuppressNoCodeLinkerWarning(void)
{
}

#else





//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		NAtomicList::NAtomicList : Constructor.
//----------------------------------------------------------------------------
template<class T> NAtomicList<T>::NAtomicList(void)
{
}





//============================================================================
//		NAtomicList::~NAtomicList : Destructor.
//----------------------------------------------------------------------------
template<class T> NAtomicList<T>::~NAtomicList(void)
{
}





//============================================================================
//		NAtomicList::GetSize : Get the size.
//----------------------------------------------------------------------------
template<class T> NIndex NAtomicList<T>::GetSize(void) const
{	StLock		acquireLock(mLock);



	// Get the size
	return((NIndex) mList.size());
}





//============================================================================
//		NAtomicList::Clear : Clear the list.
//----------------------------------------------------------------------------
template<class T> void NAtomicList<T>::Clear(void)
{	StLock		acquireLock(mLock);



	// Clear the list
	mList.clear();
}





//============================================================================
//		NAtomicList::Lock : Lock the list.
//----------------------------------------------------------------------------
template<class T> bool NAtomicList<T>::Lock(NTime theTime)
{


	// Lock the list
	return(mLock.Lock(theTime));
}





//============================================================================
//		NAtomicList::Unlock : Unlock the list.
//----------------------------------------------------------------------------
template<class T> void NAtomicList<T>::Unlock(void)
{


	// Unlock the list
	mLock.Unlock();
}





//============================================================================
//		NAtomicList::GetValue : Get a value from the list.
//----------------------------------------------------------------------------
template<class T> T NAtomicList<T>::GetValue(NIndex theIndex) const
{	StLock					acquireLock(mLock);
	TListConstIterator		theIter;
	NIndex					n;



	// Validate our parameters
	NN_ASSERT(theIndex < (NIndex) mList.size());



	// Get the value
	theIter = mList.begin();
	
	for (n = 0; n < theIndex; n++)
		theIter++;

	return(*theIter);
}





//============================================================================
//		NAtomicList::RemoveValue : Remove a value from the list.
//----------------------------------------------------------------------------
template<class T> void NAtomicList<T>::RemoveValue(T theValue)
{	StLock		acquireLock(mLock);



	// Remove the value
	mList.remove(theValue);
}





//============================================================================
//		NAtomicList::PushFront : Push a value to the front of the list.
//----------------------------------------------------------------------------
template<class T> void NAtomicList<T>::PushFront(T theValue)
{	StLock		acquireLock(mLock);



	// Add the value
	mList.push_front(theValue);
}





//============================================================================
//		NAtomicList::PushBack : Push a value to the back of the list.
//----------------------------------------------------------------------------
template<class T> void NAtomicList<T>::PushBack(T theValue)
{	StLock		acquireLock(mLock);



	// Add the value
	mList.push_back(theValue);
}





//============================================================================
//		NAtomicList::PopFront : Remove a value to the front of the list.
//----------------------------------------------------------------------------
template<class T> bool NAtomicList<T>::PopFront(T &theValue)
{	StLock		acquireLock(mLock);
	bool		didPop;



	// Remove the value
	didPop = !mList.empty();
	
	if (didPop)
		{
		theValue = mList.front();
		mList.pop_front();
		}
		
	return(didPop);
}





//============================================================================
//		NAtomicList::PopBack : Remove a value to the back of the list.
//----------------------------------------------------------------------------
template<class T> bool NAtomicList<T>::PopBack(T &theValue)
{	StLock		acquireLock(mLock);
	bool		didPop;



	// Remove the value
	didPop = !mList.empty();
	
	if (didPop)
		{
		theValue = mList.back();
		mList.pop_back();
		}
	
	return(didPop);
}

#endif // NATOMICLIST_CPP

/*	NAME:
		NSingleton.h

	DESCRIPTION:
		Singleton object.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSINGLETON_HDR
#define NSINGLETON_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NUncopyable.h"
#include "NAtomicList.h"
#include "NLock.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
typedef NAtomicList<class NSingleton*>							NSingletonList;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NSingleton : public NUncopyable {
public:	
										NSingleton(void);
	virtual							   ~NSingleton(void);


	// Destroy the singletons
	static void							DestroyObjects(void);


protected:
	// Instantiate a singleton
	//
	// Derived classes should invoke CreateInstance from their static "get the
	// instance" method, passing in the address of the variable which holds the
	// instance pointer:
	//
	//			CFoo *CFoo::Get(void)
	//			{	static CFoo		*sInstance = NULL;
	//			
	//				return(CreateInstance<CFoo>(&sInstance));
	//			}
	//
	// CreateInstance will instantiate the singleton if necessary, and arrange for
	// the instance pointer to be cleared to NULL when the singleton is destroyed.
	template <class T> static T*		CreateInstance(T **thePtr);


private:
	void								SetInstance(void **thePtr);

	static NSingletonList			   *GetObjects(void);


private:
	void							  **mInstance;

	static NMutexLock					mLock;
};





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
//		NSingleton::CreateInstance : Instantiate a singleton.
//----------------------------------------------------------------------------
template <class T> T *NSingleton::CreateInstance(T **thePtr)
{	StLock			lockMutex(mLock);



	// Create the instance
	if (*thePtr == NULL)
		*thePtr = new T;

	if (*thePtr != NULL)
		(*thePtr)->SetInstance((void **) thePtr);

	return(*thePtr);
}







#endif // NSINGLETON_HDR



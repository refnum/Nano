/*	NAME:
		TCFObject.cpp

	DESCRIPTION:
		NCFObject tests.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFObject.h"

#include "TCFObject.h"





//============================================================================
//		TCFObject::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TCFObject::Execute(void)
{	NCFObject		theArray;
	CFArrayRef		cfArray;



	// Retain/release
	cfArray = CFArrayCreateMutable(kCFAllocatorDefault, 0, NULL);
	NN_ASSERT(CFGetRetainCount(cfArray) == 1);
	
	CFSafeRetain(cfArray);
	NN_ASSERT(CFGetRetainCount(cfArray) == 2);

	CFRelease(cfArray);
	NN_ASSERT(CFGetRetainCount(cfArray) == 1);

	CFSafeRelease(cfArray);
	NN_ASSERT(cfArray == NULL);
	
	
	
	// NULL handling
	cfArray = NULL;
	CFSafeRetain( cfArray);
	CFSafeRelease(cfArray);



	// Assignment
	NN_ASSERT(!theArray.IsValid());
	theArray.SetObject(CFArrayCreateMutable(kCFAllocatorDefault, 0, NULL));

	NN_ASSERT(theArray.IsValid());
	NN_ASSERT(CFGetRetainCount(theArray) == 1);



	// Copy
	{
		{	NCFObject	theArray2;
	
		theArray2 = theArray;
		NN_ASSERT(CFGetRetainCount(theArray)  == 2);
		NN_ASSERT(CFGetRetainCount(theArray2) == 2);
		}

	NN_ASSERT(CFGetRetainCount(theArray) == 1);
	}



}


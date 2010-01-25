/*	NAME:
		NCFDictionary.h

	DESCRIPTION:
		CFDictionaryRef wrapper.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCFDICTIONARY_HDR
#define NCFDICTIONARY_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDictionary.h"

#include "NCFObject.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCFDictionary : public NDictionary {
public:
										NCFDictionary(const NDictionary &theDictionary);
										NCFDictionary(CFDictionaryRef cfObject, bool takeOwnership);
										
										NCFDictionary(void);
	virtual							   ~NCFDictionary(void);


	// Get/set the object
	NCFObject							GetObject(void) const;
	bool								SetObject(CFDictionaryRef cfObject, bool takeOwnership=true);


private:
	static void							InsertValue(const void *keyPtr, const void *valuePtr, void *userData);


private:


};




#endif // NCFDICTIONARY_HDR



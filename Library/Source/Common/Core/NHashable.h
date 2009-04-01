/*	NAME:
		NHashable.h

	DESCRIPTION:
		Mix-in class for hashable objects.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NHASHABLE_HDR
#define NHASHABLE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NHashable {
public:
										 NHashable(void);
	virtual								~NHashable(void);


	// Get the hash code
	NHashCode							GetHash(void) const;


protected:
	// Clear the hash code
	void								ClearHash(void);


	// Calculate a hash code
	NHashCode							CalculateHash(NIndex theSize, const void *thePtr) const;


	// Calculate the hash code
	virtual NHashCode					CalculateHash(void) const = 0;


private:
	mutable NHashCode					mHashCode;
};





#endif // NHASHABLE_HDR

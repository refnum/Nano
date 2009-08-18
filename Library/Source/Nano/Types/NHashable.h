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
template <class T> struct NHashableCompare
{
	bool operator()(const T &value1, const T &value2) const
	{	NHashCode		hash1, hash2;


		// Check the hash code
		//
		// A different hash code means the objects are not equal.
		hash1 = value1.GetHash();
		hash2 = value2.GetHash();
		
		if (hash1 != hash2)
			return(hash1 < hash2);
		
		
		// General comparison
		//
		// Equal hash codes means the objects might be equal.
		return(value1 < value2);
	}
};





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
	NHashCode							CalculateHash(const NData &theData) const;


	// Calculate the hash code
	virtual NHashCode					CalculateHash(void) const = 0;


private:
	mutable NHashCode					mHashCode;
};





#endif // NHASHABLE_HDR

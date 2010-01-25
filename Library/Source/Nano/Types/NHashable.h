/*	NAME:
		NHashable.h

	DESCRIPTION:
		Mix-in class for hashable objects.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
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
	//============================================================================
	//		NHashableCompare::Compare
	//----------------------------------------------------------------------------
	//		Note :	Performs a cheap comparison of hash values before performing
	//				the more expensive comparision of values.
	//
	//				This allows us to produce a stable ordering, suitable for use
	//				with std::map, while minimising value comparisons.
	//----------------------------------------------------------------------------
	inline NComparison Compare(const T &value1, const T &value2) const
	{	NHashCode		hash1, hash2;


		// Fast comparison
		//
		// A different hash code means the objects are definitely not equal.
		hash1 = value1.GetHash();
		hash2 = value2.GetHash();
		
		if (hash1 != hash2)
			return(GetComparison(hash1, hash2));
		
		
		// General comparison
		//
		// Equal hash codes means the objects might be equal.
		return(GetComparison(value1, value2));
	}
	
	inline bool operator()(const T &value1, const T &value2) const
	{
		return(Compare(value1, value2) == kNCompareLessThan);
	}
};





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NHashable {
public:
										NHashable(void);
	virtual							   ~NHashable(void);


	// Clear the hash value
	//
	// Must be called whenever the object is mutated.
	void								ClearHash(void);


	// Get the hash code
	//
	// The hash algorithm used by built-in Nano is subject to change and
	// should not be archived to disk.
	//
	// Persistent hashes should be explicitly selected via NDataCipher.
	NHashCode							GetHash(void) const;


protected:
	// Calculate the hash
	virtual NHashCode					CalculateHash(void) const = 0;


private:
	mutable NHashCode					mHashCode;
};





#endif // NHASHABLE_HDR

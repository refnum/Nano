/*	NAME:
		NComparable.h

	DESCRIPTION:
		Base class for comparable objects.

	COPYRIGHT:
		Copyright (c) 2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCOMPARABLE_HDR
#define NCOMPARABLE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NComparable {
public:
										NComparable(void);
	virtual								~NComparable(void);


	// Operators
	bool								operator == (const NComparable &theObject) const;
	bool								operator != (const NComparable &theObject) const;
	bool								operator <= (const NComparable &theObject) const;
	bool								operator <	(const NComparable &theObject) const;
	bool								operator >= (const NComparable &theObject) const;
	bool								operator >	(const NComparable &theObject) const;


protected:
	// Compare two objects
	//
	// The base class compares objects by address; sub-classes can
	// override this method to provide a value-based comparison.
	virtual CFComparisonResult			Compare(const NComparable &theObject) const;


private:


};





#endif // NCOMPARABLE_HDR

/*	NAME:
		NComparable.h

	DESCRIPTION:
		Mix-in class for comparable objects.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
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
//		Constants
//----------------------------------------------------------------------------
typedef enum {
	kNCompareLessThan		= -1,
	kNCompareEqualTo		=  0,
	kNCompareGreaterThan	=  1
} NComparison;





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// Compare two values
//
// Returns an NComparisonResult for two types that define the < and > operators.
#define GET_COMPARISON(_a, _b)						((_a) < (_b) ? kNCompareLessThan : ((_a) > (_b) ? kNCompareGreaterThan : kNCompareEqualTo))





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
	virtual NComparison					Compare(const NComparable &theObject) const;


private:


};





#endif // NCOMPARABLE_HDR

/*	NAME:
		NCFDate.h

	DESCRIPTION:
		CoreFoundation date object.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCFDATE_HDR
#define NCFDATE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFObject.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCFDate : public NCFObject {
public:
										NCFDate(CFAbsoluteTime         theTime);
										NCFDate(const CFGregorianDate &theDate);

										NCFDate(void);
	virtual								~NCFDate(void);


	// Get the time
	CFAbsoluteTime						GetTime(void) const;


	// Get the date
	CFGregorianDate						GetDate(void) const;


	// Compare two dates
	CFComparisonResult					Compare(const NCFDate &theDate) const;


	// Operators
	const bool							operator == (const NCFDate &theDate) const;
	const bool							operator != (const NCFDate &theDate) const;
	const bool							operator <  (const NCFDate &theDate) const;


	// Convert between a LongDateRec and a GregorianDate
	static CFGregorianDate				GetGregorianDate(const LongDateRec     &longDate);
	static LongDateRec					GetLongDate(     const CFGregorianDate &gregDate);


	// Convert between a LongDateTime and a GregorianDate
	static CFGregorianDate				GetGregorianDate(const LongDateTime    &longTime);
	static LongDateTime					GetLongTime(     const CFGregorianDate &gregDate);


protected:
	// Get the null value
	CFTypeRef							GetNullValue(void);


private:
	void								InitializeSelf(CFAbsoluteTime theTime);


private:


};





#endif // NCFDATE_HDR



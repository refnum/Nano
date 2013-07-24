/*	NAME:
		NDBQuery.h

	DESCRIPTION:
		Database query.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDBQUERY_HDR
#define NDBQUERY_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDictionary.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Misc
typedef int32_t NDBStatus;
typedef void   *NDBHandleRef;
typedef void   *NDBQueryRef;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDBQuery {
public:
										NDBQuery(const NString &theValue);
										NDBQuery(const char    *theValue);

										NDBQuery(void);
	virtual							   ~NDBQuery(void);


	// Get/set the query
	NString								GetValue(void) const;
	void								SetValue(const NString &theValue);


	// Get/set the parameters
	//
	// Parameters can be passed as either an NArray or an NDictionary, for either
	// indexed or keyed parameters.
	//
	// Indexed parameters are slightly faster, which can be a useful optimization
	// for very large inserts (100K+ statements).
	//
	//
	// Indexed parameters are referenced with a '?' prefix:
	//
	//		INSERT INTO table (name, size) VALUES (?1, ?2);
	//
	// Note that SQL indices are 1-based, whereas NArray is 0-based.
	//
	//
	// Keyed parameters are referenced with a ':' prefix:
	//
	//		INSERT INTO table (name, size) VALUES (:namekey, :sizekey);
	//
	// Note that the ':' prefix should not be present in the key name, so the keys
	// in this example would be "namekey" and "sizekey".
	NVariant							GetParameters(void) const;
	void								SetParameters(const NVariant &theValue);


private:
	NString								mValue;
	NVariant							mParameters;
};




#endif // NDBQUERY_HDR



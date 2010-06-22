/*	NAME:
		NUncopyable.h

	DESCRIPTION:
		Uncopyable object.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NUNCOPYABLE_HDR
#define NUNCOPYABLE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NUncopyable {
public:
										NUncopyable(void);
	virtual							   ~NUncopyable(void);


private:
										NUncopyable(const NUncopyable &theObject);
	const NUncopyable&					operator = (const NUncopyable &theObject);


private:


};





#endif // NUNCOPYABLE_HDR


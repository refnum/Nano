/*	NAME:
		NContainer.h

	DESCRIPTION:
		Mix-in class for container objects.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCONTAINER_HDR
#define NCONTAINER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NRange.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NContainer {
public:
										NContainer(void);
	virtual							   ~NContainer(void);


	// Is this object empty?
	bool								IsEmpty(void) const;


	// Get the size
	virtual NIndex						GetSize(void) const = 0;


protected:
	// Does a range span the container?
	bool								IsFullRange(const NRange &theRange) const;
	
	
	// Get a normalized range
	NRange								GetNormalized(const NRange &theRange) const;


private:


};





#endif // NCONTAINER_HDR

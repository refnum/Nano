/*	NAME:
		NContainer.h

	DESCRIPTION:
		Mix-in class for container objects.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCONTAINER_HDR
#define NCONTAINER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NContainer {
public:
										 NContainer(void);
	virtual								~NContainer(void);


	// Is this object empty/non-empty?
	bool								IsEmpty(   void) const;
	bool								IsNotEmpty(void) const;


	// Get the size
	virtual NIndex						GetSize(void) const = 0;


private:


};





#endif // NCONTAINER_HDR

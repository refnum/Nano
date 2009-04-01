/*	NAME:
		NPoint.h

	DESCRIPTION:
		Point object.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NPOINT_HDR
#define NPOINT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Constants
//----------------------------------------------------------------------------
class NPoint;

extern const NPoint kNPointZero;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NPoint {
public:
										 NPoint(Float32 x=0.0f, Float32 y=0.0f);
	virtual								~NPoint(void);


	// Clear the point
	void								Clear(void);


	// Test the point
	bool								IsZero(   void) const;
	bool								IsNotZero(void) const;


	// Operators
	bool								operator == (const NPoint &thePoint) const;
	bool								operator != (const NPoint &thePoint) const;


public:
	Float32								x;
	Float32								y;
};




#endif // NPOINT_HDR



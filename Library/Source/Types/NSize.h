/*	NAME:
		NSize.h

	DESCRIPTION:
		Size object.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSIZE_HDR
#define NSIZE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Constants
//----------------------------------------------------------------------------
class NSize;

extern const NSize kNSizeZero;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NSize {
public:
										 NSize(Float32 width=0.0f, Float32 height=0.0f);
	virtual								~NSize(void);


	// Clear the size
	void								Clear(void);


	// Test the size
	bool								IsEmpty(   void) const;
	bool								IsNotEmpty(void) const;


	// Operators
	bool								operator == (const NSize &theSize) const;
	bool								operator != (const NSize &theSize) const;


public:
	Float32								width;
	Float32								height;
};




#endif // NSIZE_HDR



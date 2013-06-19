/*	NAME:
		NCGColor.h

	DESCRIPTION:
		CoreGraphics color.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCGCOLOR_HDR
#define NCGCOLOR_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFObject.h"
#include "NColor.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCGColor : public NColor {
public:
										NCGColor(const NColor &theColor);
										NCGColor(CGColorRef cfObject, bool takeOwnership);
				
										NCGColor(void);
	virtual							   ~NCGColor(void);


	// Get/set the object
	NCFObject							GetObject(void) const;
	bool								SetObject(CGColorRef cfObject, bool takeOwnership=true);


	// Get color spaces
	static NCFObject					GetDeviceGray(void);
	static NCFObject					GetDeviceRGB( void);


private:


};




#endif // NCGCOLOR_HDR



/*	NAME:
		NCGImage.h

	DESCRIPTION:
		CoreGraphics image.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCGIMAGE_HDR
#define NCGIMAGE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFObject.h"
#include "NImage.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCGImage : public NImage {
public:
										NCGImage(const NImage &theImage);
										NCGImage(CGImageRef cfObject, bool takeOwnership);
				
										NCGImage(void);
	virtual							   ~NCGImage(void);


	// Get/set the object
	//
	// Since a CGImage may simply be used for drawing, GetObject will return an
	// image that shares our data by default.
	//
	// If the CGImage is to outlive this object then the data should be cloned.
	NCFObject							GetObject(bool cloneData=false) const;
	bool								SetObject(CGImageRef cfObject, bool takeOwnership=true);


	// Get a CGBitmapInfo
	static CGBitmapInfo					GetBitmapInfo(NImageFormat theFormat);


private:


};




#endif // NCGIMAGE_HDR



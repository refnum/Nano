/*	NAME:
		NCFURL.h

	DESCRIPTION:
		CFURLRef wrapper.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCFURL_HDR
#define NCFURL_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFile.h"
#include "NURL.h"

#include "NCFObject.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCFURL : public NURL {
public:
										NCFURL(const NFile &theFile);
										NCFURL(const NURL  &theURL);

										NCFURL(CFURLRef  cfObject, bool takeOwnership);
										NCFURL(NSURL    *nsObject, bool takeOwnership);
										
										NCFURL(void);
	virtual							   ~NCFURL(void);


	// Get/set the object
	NCFObject							GetObject(void) const;
	bool								SetObject(CFURLRef cfObject, bool takeOwnership=true);


private:


};




#endif // NCFURL_HDR



/*  NAME:
        NFileUtilities.h

    DESCRIPTION:
        File utilities.
    
    COPYRIGHT:
        Copyright (c) 2006-2009, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
    __________________________________________________________________________
*/
#ifndef NFILEUTILITIES_HDR
#define NFILEUTILITIES_HDR
//============================================================================
//      Include files
//----------------------------------------------------------------------------
#include "NFile.h"





//============================================================================
//      Class declaration
//----------------------------------------------------------------------------
class NFileUtilities {
public:
	// Get the current working directory
	static NFile						GetCWD(void);
};



#endif // NFILEUTILITIES_HDR



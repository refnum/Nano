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
#include "NData.h"





//============================================================================
//      Class declaration
//----------------------------------------------------------------------------
class NFileUtilities {
public:
	// Get the current working directory
	static NFile						GetCWD(void);


	// Get/set the contents of a file
	static NString						GetFileText(const NFile &theFile, NStringEncoding theEncoding=kNStringEncodingUTF8);
	static NData						GetFileData(const NFile &theFile);

	static NStatus						SetFileText(const NFile &theFile, const NString &theText, NStringEncoding theEncoding=kNStringEncodingUTF8);
	static NStatus						SetFileData(const NFile &theFile, const NData   &theData);
};



#endif // NFILEUTILITIES_HDR



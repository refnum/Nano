/*  NAME:
        NFileUtilities.cpp

    DESCRIPTION:
        File utilities.
    
    COPYRIGHT:
        Copyright (c) 2006-2009, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
    __________________________________________________________________________
*/
//============================================================================
//      Include files
//----------------------------------------------------------------------------
#include "NFileUtilities.h"





//============================================================================
//      NFileUtilities::GetCWD : Get the current working directory.
//----------------------------------------------------------------------------
NFile NFileUtilities::GetCWD(void)
{	char		*thePath;
	NFile		theFile;



	// Get the directory
	thePath = getcwd(NULL, 0);
	if (thePath != NULL)
		{
		theFile = NFile(NString(thePath));
		free(thePath);
		}

	return(theFile);
}

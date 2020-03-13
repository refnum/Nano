/*	NAME:
		NPOSIX.h

	DESCRIPTION:
		POSIX glue.

	COPYRIGHT:
		Copyright (c) 2006-2020, refNum Software
		All rights reserved.

		This software is distributed under the terms of your licensing
		agreement with refNum Software.
		
		This license grants you permission to use, copy, modify, or
		distribute this sofware only under the terms of that license.
		
		refNum Software retains full ownership of this software.
	___________________________________________________________________________
*/
#ifndef NPOSIX_H
#define NPOSIX_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// System
#include <time.h>





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NPOSIX
{
public:
	// time.h
	static struct tm                    localtime(time_t timeUnix);
};



#endif // NPOSIX_H

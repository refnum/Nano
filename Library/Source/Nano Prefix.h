/*	NAME:
		Nano Prefix.h

	DESCRIPTION:
		Nano prefix header.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NANO_PREFIX_HDR
#define NANO_PREFIX_HDR
//============================================================================
//		Build constants
//----------------------------------------------------------------------------
// PCRE
#define HAVE_CONFIG_H													1
#define PCRE_STATIC														1


// Expat
#define HAVE_EXPAT_CONFIG_H												1
#define XML_STATIC														1


// ZLib
#define Z_PREFIX														1
#define z_Byte															uint8_t





#endif // NANO_PREFIX_HDR



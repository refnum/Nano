/*	NAME:
		expat_config.h

	DESCRIPTION:
		Nano expat config.
		
		For Mac builds, suppress compiler warnings with:
			-w

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef CONFIG_HDR
#define CONFIG_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Buld constants
//----------------------------------------------------------------------------
// Endian
#if NN_TARGET_ENDIAN_BIG
	#define BYTEORDER													4321
#else
	#define BYTEORDER													1234
#endif


// Target features
#if NN_TARGET_MACOS || NN_TARGET_IOS
	#define HAVE_BCOPY													1
	#define HAVE_GETPAGESIZE											1
	#define HAVE_MEMMOVE												1
	#define HAVE_MMAP													1

	#define HAVE_DLFCN_H												1
	#define HAVE_FCNTL_H												1
	#define HAVE_INTTYPES_H												1
	#define HAVE_MEMORY_H												1
	#define HAVE_STDINT_H												1
	#define HAVE_STDLIB_H												1
	#define HAVE_STRING_H												1
	#define HAVE_STRINGS_H												1
	#define HAVE_SYS_STAT_H												1
	#define HAVE_SYS_TYPES_H											1
	#define HAVE_UNISTD_H												1
	#define HAVE_UNISTD_H												1
#else
	#define HAVE_MEMMOVE												1

	#define HAVE_STDINT_H												1
	#define HAVE_STDLIB_H												1
	#define HAVE_STRING_H												1
	#define HAVE_STRINGS_H												1
	#define HAVE_SYS_TYPES_H											1
	#define HAVE_UNISTD_H												1
#endif


// Expat features
#define XML_NS															1
#define XML_DTD															1
#define XML_LARGE_SIZE													1
#define XML_CONTEXT_BYTES												1024





#endif // CONFIG_HDR


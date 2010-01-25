/*	NAME:
		NQObject.h

	DESCRIPTION:
		QObject support.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NQOBJECT_HDR
#define NQOBJECT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// Connect a signal
#define ConnectSelf(_src, _signal, _slot)				connect(_src, SIGNAL(_signal), this, SLOT(_slot))



#endif // NQOBJECT_HDR



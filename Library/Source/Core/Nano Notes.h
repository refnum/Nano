/*	NAME:
		Nano Notes.h

	DESCRIPTION:
		Nano release notes.
		
		This file contains information about important changes between
		releases of Nano, particularly for changes that will not produce
		a compiler or linker error.
		
		To turn off these messages, your prefix header should include:
		
			#define NN_HIDE_RELEASE_NOTES_1_1			1
		
		Prior to including Nano.h (after reviewing these warnings).
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NANO_NOTES_HDR
#define NANO_NOTES_HDR
//============================================================================
//		Build Constants
//----------------------------------------------------------------------------
#ifndef NN_HIDE_RELEASE_NOTES_1_1
#define NN_HIDE_RELEASE_NOTES_1_1										0
#endif

#ifndef NN_HIDE_RELEASE_NOTES
#define NN_HIDE_RELEASE_NOTES											0
#endif
	




//============================================================================
//		Release Notes
//----------------------------------------------------------------------------
#if !NN_HIDE_RELEASE_NOTES_1_1 && !NN_HIDE_RELEASE_NOTES


	// kHICommandNToggleToolbar
	//
	// See NCarbonEventHandler.h.
	#warning "kHICommandNToggleToolbar was changed from 'Ntog' to 'tbtg' to match 10.5; update .nib usage"


#endif // !NN_HIDE_RELEASE_NOTES_1_1_0 && !NN_HIDE_RELEASE_NOTES





#endif // NANO_NOTES_HDR



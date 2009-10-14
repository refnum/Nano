/*	NAME:
		NQuickTimeUtilities.h

	DESCRIPTION:
		QuickTime utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NQUICKTIMEUTILITIES_HDR
#define NQUICKTIMEUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"
#include "NData.h"
#include "NUTI.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NQuickTimeUtilities {
public:
	// Create a Data Reference handle
	//
	// Creates a Data Reference given a handle or pointer, with an optional
	// UTI to specify meta-data for the reference.
	static Handle						CreateDataReferenceFromHandle( Handle theData,                     const NUTI &theUTI="");
	static Handle						CreateDataReferenceFromPointer(UInt32 theSize, const void *thePtr, const NUTI &theUTI="");


	// Set the meta-data for a Data Reference
	//
	// Assigns the UTI's meta-data to a data reference.
	//
	// The existing data reference handle may be replaced with a new handle.
	static void							SetDataReferenceMetaData(Handle &dataRef, OSType dataType, const NUTI &theUTI);


	// Add a Data Reference extension
	//
	// The existing data reference handle may be replaced with a new handle.
	static void							AddDataReferenceExtension(Handle &dataRef, OSType dataType, const NString &theValue, OSType valueType);
};




#endif // NQUICKTIMEUTILITIES_HDR



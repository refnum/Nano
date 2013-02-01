/*	NAME:
		NDBResult.cpp

	DESCRIPTION:
		Database result.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "nano_sqlite.h"

#include "NDBResult.h"





//============================================================================
//		NDBResult::NDBResult : Constructor.
//----------------------------------------------------------------------------
NDBResult::NDBResult(const NDBResult &theValue)
{


	// Initialize ourselves
	mResult = theValue.mResult;
}





//============================================================================
//		NDBResult::NDBResult : Constructor.
//----------------------------------------------------------------------------
NDBResult::NDBResult(NDBQueryRef theResult)
{


	// Initialize ourselves
	mResult = theResult;
}





//============================================================================
//		NDBResult::~NDBResult : Destructor.
//----------------------------------------------------------------------------
NDBResult::~NDBResult(void)
{
}





//============================================================================
//		NDBResult::GetSize : Get the number of columns.
//----------------------------------------------------------------------------
NIndex NDBResult::GetSize(void) const
{


	// Get the number of columns
	return(sqlite3_column_count((sqlite3_stmt *) mResult));
}





//============================================================================
//		NDBResult::GetName : Get the name of a column.
//----------------------------------------------------------------------------
NString NDBResult::GetName(NIndex theIndex) const
{


	// Validate our parameters
	NN_ASSERT(theIndex < GetSize());
	


	// Get the number of columns
	return(sqlite3_column_name((sqlite3_stmt *) mResult, theIndex));
}





//============================================================================
//		NDBResult::GetValueSInt32 : Get an SInt32 value.
//----------------------------------------------------------------------------
SInt32 NDBResult::GetValueSInt32(NIndex theIndex) const
{


	// Validate our parameters
	NN_ASSERT(theIndex < GetSize());



	// Get the value
	return(sqlite3_column_int((sqlite3_stmt *) mResult, theIndex));
}





//============================================================================
//		NDBResult::GetValueSInt64 : Get an SInt64 value.
//----------------------------------------------------------------------------
SInt64 NDBResult::GetValueSInt64(NIndex theIndex) const
{


	// Validate our parameters
	NN_ASSERT(theIndex < GetSize());



	// Get the value
	return(sqlite3_column_int64((sqlite3_stmt *) mResult, theIndex));
}





//============================================================================
//		NDBResult::GetValueFloat32 : Get a Float32 value.
//----------------------------------------------------------------------------
Float32 NDBResult::GetValueFloat32(NIndex theIndex) const
{


	// Validate our parameters
	NN_ASSERT(theIndex < GetSize());



	// Get the value
	return((Float32) GetValueFloat64(theIndex));
}





//============================================================================
//		NDBResult::GetValueFloat64 : Get a Float64 value.
//----------------------------------------------------------------------------
Float64 NDBResult::GetValueFloat64(NIndex theIndex) const
{


	// Validate our parameters
	NN_ASSERT(theIndex < GetSize());



	// Get the value
	return(sqlite3_column_double((sqlite3_stmt *) mResult, theIndex));
}





//============================================================================
//		NDBResult::GetValueString : Get a string value.
//----------------------------------------------------------------------------
NString NDBResult::GetValueString(NIndex theIndex) const
{	NString			theResult;
	const void		*thePtr;
	NIndex			theSize;



	// Validate our parameters
	NN_ASSERT(theIndex < GetSize());



	// Get the value
	thePtr  = (const void *) sqlite3_column_text( (sqlite3_stmt *) mResult, theIndex);
	theSize = (NIndex      ) sqlite3_column_bytes((sqlite3_stmt *) mResult, theIndex);

	if (thePtr != NULL && theSize != 0)
		theResult = NString(thePtr, theSize, kNStringEncodingUTF8);

	return(theResult);
}





//============================================================================
//		NDBResult::GetValueData : Get a data value.
//----------------------------------------------------------------------------
NData NDBResult::GetValueData(NIndex theIndex) const
{	NData			theResult;
	const void		*thePtr;
	NIndex			theSize;



	// Validate our parameters
	NN_ASSERT(theIndex < GetSize());



	// Get the value
	thePtr  = (const void *) sqlite3_column_blob( (sqlite3_stmt *) mResult, theIndex);
	theSize = (NIndex      ) sqlite3_column_bytes((sqlite3_stmt *) mResult, theIndex);
	
	if (thePtr != NULL && theSize != 0)
		theResult.AppendData(theSize, thePtr);
	
	return(theResult);
}





//============================================================================
//		NDBResult::GetRowValueSInt32 : Get a row value.
//----------------------------------------------------------------------------
void NDBResult::GetRowValueSInt32(const NDBResult &theRow, NIndex theIndex, SInt32 *theValue)
{


	// Get the value
	*theValue = theRow.GetValueSInt32(theIndex);
}





//============================================================================
//		NDBResult::GetRowValueSInt64 : Get a row value.
//----------------------------------------------------------------------------
void NDBResult::GetRowValueSInt64(const NDBResult &theRow, NIndex theIndex, SInt64 *theValue)
{


	// Get the value
	*theValue = theRow.GetValueSInt64(theIndex);
}





//============================================================================
//		NDBResult::GetRowValueFloat32 : Get a row value.
//----------------------------------------------------------------------------
void NDBResult::GetRowValueFloat32(const NDBResult &theRow, NIndex theIndex, Float32 *theValue)
{


	// Get the value
	*theValue = theRow.GetValueFloat32(theIndex);
}





//============================================================================
//		NDBResult::GetRowValueFloat64 : Get a row value.
//----------------------------------------------------------------------------
void NDBResult::GetRowValueFloat64(const NDBResult &theRow, NIndex theIndex, Float64 *theValue)
{


	// Get the value
	*theValue = theRow.GetValueFloat64(theIndex);
}





//============================================================================
//		NDBResult::GetRowValueString : Get a row value.
//----------------------------------------------------------------------------
void NDBResult::GetRowValueString(const NDBResult &theRow, NIndex theIndex, NString *theValue)
{


	// Get the value
	*theValue = theRow.GetValueString(theIndex);
}





//============================================================================
//		NDBResult::GetRowValueData : Get a row value.
//----------------------------------------------------------------------------
void NDBResult::GetRowValueData(const NDBResult &theRow, NIndex theIndex, NData *theValue)
{


	// Get the value
	*theValue = theRow.GetValueData(theIndex);
}





//============================================================================
//		NDBResult::GetRowValuesSInt32 : Get a list of row values.
//----------------------------------------------------------------------------
void NDBResult::GetRowValuesSInt32(const NDBResult &theRow, NIndex theIndex, SInt32List *theValues)
{


	// Get the value
	theValues->push_back(theRow.GetValueSInt32(theIndex));
}





//============================================================================
//		NDBResult::GetRowValuesSInt64 : Get a list of row values.
//----------------------------------------------------------------------------
void NDBResult::GetRowValuesSInt64(const NDBResult &theRow, NIndex theIndex, SInt64List *theValues)
{


	// Get the value
	theValues->push_back(theRow.GetValueSInt64(theIndex));
}





//============================================================================
//		NDBResult::GetRowValuesFloat32 : Get a list of row values.
//----------------------------------------------------------------------------
void NDBResult::GetRowValuesFloat32(const NDBResult &theRow, NIndex theIndex, Float32List *theValues)
{


	// Get the value
	theValues->push_back(theRow.GetValueFloat32(theIndex));
}





//============================================================================
//		NDBResult::GetRowValuesFloat64 : Get a list of row values.
//----------------------------------------------------------------------------
void NDBResult::GetRowValuesFloat64(const NDBResult &theRow, NIndex theIndex, Float64List *theValues)
{


	// Get the value
	theValues->push_back(theRow.GetValueFloat64(theIndex));
}





//============================================================================
//		NDBResult::GetRowValuesString : Get a list of row values.
//----------------------------------------------------------------------------
void NDBResult::GetRowValuesString(const NDBResult &theRow, NIndex theIndex, NStringList *theValues)
{


	// Get the value
	theValues->push_back(theRow.GetValueString(theIndex));
}





//============================================================================
//		NDBResult::GetRowValuesData : Get a list of row values.
//----------------------------------------------------------------------------
void NDBResult::GetRowValuesData(const NDBResult &theRow, NIndex theIndex, NDataList *theValues)
{


	// Get the value
	theValues->push_back(theRow.GetValueData(theIndex));
}





//============================================================================
//		NDBResult::= : Assignment operator.
//----------------------------------------------------------------------------
const NDBResult& NDBResult::operator = (const NDBResult &theValue)
{


	// Assign the object
	if (this != &theValue)
		mResult = theValue.mResult;

	return(*this);
}


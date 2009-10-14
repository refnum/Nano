/*	NAME:
		NArray.h

	DESCRIPTION:
		Array object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NARRAY_HDR
#define NARRAY_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NContainer.h"
#include "NCFObject.h"
#include "NString.h"
#include "NColor.h"
#include "NDate.h"
#include "NData.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
class NDictionary;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NArray :	public NCFObject,
				public NContainer {
public:
										NArray(CFArrayRef cfArray);
										NArray(CFTypeRef  cfValue=NULL, ...);

	virtual								~NArray(void);


	// Get the size
	UInt32								GetSize(void) const;


	// Join two arrays
	void								Join(const NArray &theValue);


	// Sort the array
	void								Sort(CFComparatorFunction sortFunc, void *sortData=NULL, const CFRange &theRange=kCFRangeAll);


	// Does a value exist?
	bool								HasValue(CFTypeRef theValue) const;
	

	// Get/set a value
	bool								GetValueBoolean(   UInt32 theIndex) const;
	OSType								GetValueOSType(    UInt32 theIndex) const;
	UInt32								GetValueUInt32(    UInt32 theIndex) const;
	Float64								GetValueFloat64(   UInt32 theIndex) const;
	NString								GetValueString(    UInt32 theIndex) const;
	HIPoint								GetValuePoint(     UInt32 theIndex) const;
	HISize								GetValueSize(      UInt32 theIndex) const;
	HIRect								GetValueRect(      UInt32 theIndex) const;
	NColor								GetValueColor(     UInt32 theIndex) const;
	NDate								GetValueDate(      UInt32 theIndex) const;
	NArray								GetValueArray(     UInt32 theIndex) const;
	NDictionary							GetValueDictionary(UInt32 theIndex) const;
	NData								GetValueData(      UInt32 theIndex) const;

	void								SetValueBoolean(   UInt32 theIndex, bool               theValue);
	void								SetValueOSType(    UInt32 theIndex, OSType             theValue);
	void								SetValueUInt32(    UInt32 theIndex, UInt32             theValue);
	void								SetValueFloat64(   UInt32 theIndex, Float64            theValue);
	void								SetValueString(    UInt32 theIndex, const NString     &theValue);
	void								SetValuePoint(     UInt32 theIndex, const HIPoint     &theValue);
	void								SetValueSize(      UInt32 theIndex, const HISize      &theValue);
	void								SetValueRect(      UInt32 theIndex, const HIRect      &theValue);
	void								SetValueColor(     UInt32 theIndex, const NColor      &theValue);
	void								SetValueDate(      UInt32 theIndex, const NDate       &theValue);
	void								SetValueArray(     UInt32 theIndex, const NArray      &theValue);
	void								SetValueDictionary(UInt32 theIndex, const NDictionary &theValue);
	void								SetValueData(      UInt32 theIndex, const NData       &theValue);


	// Append a value
	void								AppendValueBoolean(   bool               theValue);
	void								AppendValueOSType(    OSType             theValue);
	void								AppendValueUInt32(    UInt32             theValue);
	void								AppendValueFloat64(   Float64            theValue);
	void								AppendValueString(    const NString     &theValue);
	void								AppendValuePoint(     const HIPoint     &theValue);
	void								AppendValueSize(      const HISize      &theValue);
	void								AppendValueRect(      const HIRect      &theValue);
	void								AppendValueColor(     const NColor      &theValue);
	void								AppendValueDate(      const NDate       &theValue);
	void								AppendValueArray(     const NArray      &theValue);
	void								AppendValueDictionary(const NDictionary &theValue);
	void								AppendValueData(      const NData       &theValue);
	

	// Operators
	CFTypeRef							operator [](int    theIndex) const;
	CFTypeRef							operator [](UInt32 theIndex) const;


protected:
	// Get the null value
	CFTypeRef							GetNullValue(void);


private:
	CFTypeRef							GetValue(UInt32 theIndex) const;
	void								SetValue(UInt32 theIndex, CFTypeRef cfValue);
	void								AppendValue(              CFTypeRef cfValue);


private:


};





#endif // NARRAY_HDR



/*	NAME:
		NRegistry.h

	DESCRIPTION:
		Windows registry.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NREGISTRY_HDR
#define NREGISTRY_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NUncopyable.h"
#include "NString.h"
#include "NPoint.h"
#include "NSize.h"
#include "NRectangle.h"
#include "NColor.h"
#include "NDate.h"
#include "NArray.h"
#include "NVariant.h"
#include "NData.h"
#include "NDictionary.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NRegistry : public NUncopyable {
public:
										NRegistry(void);
	virtual							   ~NRegistry(void);


	// Is a key open?
	bool								IsOpen(void) const;


	// Open/close the registry
	NStatus								Open(HKEY theKey, const NString &subKey, bool canCreate=false, bool readOnly=true);
	void								Close(void);


	// Flush the registry
	void								Flush(void);
	

	// Does a key exist?
	bool								HasKey(const NString &theKey) const;


	// Remove a key
	void								RemoveKey(const NString &theKey);


	// Get a value size
	//
	// If the value does not exist, 0 is returned.
	NIndex								GetSize(const NString &theKey) const;


	// Get/set a value
	NVariant							GetValue(const NString &theKey) const;
	void								SetValue(const NString &theKey, const NVariant &theValue);


	// Get a type'd value
	//
	// If the value does not exist, or can not be returned as the specified type, 0/empty is returned.
	bool								GetValueBoolean(   const NString &theKey) const;
	int32_t								GetValueInt32(     const NString &theKey) const;
	int64_t								GetValueInt64(     const NString &theKey) const;
	float32_t							GetValueFloat32(   const NString &theKey) const;
	float64_t							GetValueFloat64(   const NString &theKey) const;
	NString								GetValueString(    const NString &theKey) const;
	NData								GetValueData(      const NString &theKey) const;
	NDate								GetValueDate(      const NString &theKey) const;
	NColor								GetValueColor(     const NString &theKey) const;
	NPoint								GetValuePoint(     const NString &theKey) const;
	NSize								GetValueSize(      const NString &theKey) const;
	NRectangle							GetValueRectangle( const NString &theKey) const;
	NVector								GetValueVector(    const NString &theKey) const;
	NArray								GetValueArray(     const NString &theKey) const;
	NDictionary							GetValueDictionary(const NString &theKey) const;


private:
	HKEY								mKey;
};




#endif // NREGISTRY_HDR






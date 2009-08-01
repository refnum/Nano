/*	NAME:
		NEncodable.h

	DESCRIPTION:
		Mix-in class for encodable objects.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NENCODABLE_HDR
#define NENCODABLE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NComparable.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Classes
class NEncoder;
class NString;
class NVariant;





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// Declare an NEncodable sub-class
//
// Example usage:
//
//		class NFoo : public NEncodable, public NComparable<NFoo> {
//		public:
//							DECLARE_NENCODABLE(NEncodable);
//
//							 NFoo(void);
//			virtual			~NFoo(void);
//			...
//		};
//
// Sub-clases of NEncodable must include a DECLARE_NENCODABLE in their class
// declaration, and a DEFINE_NENCODABLE in their class implementation.
//
// To allow NEncoder::DecodeObject to return new objects through an NVariant,
// sub-classes of NEncodable must also conform to the NComparable interface.
//
// Unfortunately NEncodable can not derive from this interface automatically,
// or it would produce an ambigious parentage if an NEncodable sub-class also
// derived from NComparable.
#define DECLARE_NENCODABLE(_class)														\
																						\
	private:																			\
	static bool sEncodableRegistered;													\
																						\
	static bool							EncodableRegister(void);						\
	static NVariant						EncodableCreate(const NString &className);		\
																						\
	public:																				\
	NString								EncodableGetClass(void) const


#define DEFINE_NENCODABLE(_class)														\
																						\
	bool _class::sEncodableRegistered = _class::EncodableRegister();					\
																						\
	bool _class::EncodableRegister(void)												\
	{																					\
		NEncoder::RegisterClass(#_class, BindFunction(_class::EncodableCreate, _1));	\
		return(true);																	\
	}																					\
																						\
	NVariant _class::EncodableCreate(const NString &className)							\
	{	_class		theObject;															\
																						\
		NN_ASSERT(className == #_class);												\
		(void) className;																\
																						\
		return(NVariant(theObject));													\
	}																					\
																						\
	NString _class::EncodableGetClass(void) const										\
	{																					\
		return(#_class);																\
	}																					\
																						\
	void *kEatLastSemiColonForPedanticWarning ## _class





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NEncodable {
friend class NEncoder;
public:
										 NEncodable(void);
	virtual								~NEncodable(void);


protected:
	// Get the encoder class name
	//
	// This method is implemented automatically by DECLARE_NENCODABLE. 
	virtual NString						EncodableGetClass(void) const = 0;


	// Encode/decode the object
	virtual void						EncodeSelf(      NEncoder &theEncoder) const = 0;
	virtual void						DecodeSelf(const NEncoder &theEncoder)       = 0;


private:


};





#endif // NENCODABLE_HDR

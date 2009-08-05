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
//		class NFoo : public NEncodable {
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
#define DECLARE_NENCODABLE(_class)																				\
																												\
	private:																									\
	static bool sEncodableRegistered;																			\
																												\
	static bool							EncodableRegister(void);												\
	static bool							EncodableEncode(      NEncoder &theEncoder, const NString &theKey, const NVariant &theValue);	\
	static NVariant						EncodableDecode(const NEncoder &theEncoder);							\
																												\
	public:																										\
	virtual NString						EncodableGetClass(void) const


#define DEFINE_NENCODABLE(_class)																				\
																												\
	bool _class::sEncodableRegistered = _class::EncodableRegister();											\
																												\
	bool _class::EncodableRegister(void)																		\
	{	NEncoderClassInfo		classInfo;																		\
																												\
		classInfo.encodeObject = BindFunction(_class::EncodableEncode, _1, _2, _3);								\
		classInfo.decodeObject = BindFunction(_class::EncodableDecode, _1);										\
																												\
		NEncoder::RegisterClass(#_class, classInfo);															\
		return(true);																							\
	}																											\
																												\
	bool _class::EncodableEncode(NEncoder &theEncoder, const NString &theKey, const NVariant &theValue)			\
	{	_class		theObject;																					\
		bool		didEncode;																					\
																												\
		didEncode = theValue.GetValue(theObject);																\
		if (didEncode)																							\
			theEncoder.EncodeObject(theKey, theObject);															\
																												\
		return(didEncode);																						\
	}																											\
																												\
	NVariant _class::EncodableDecode(const NEncoder &theEncoder)												\
	{	_class		theObject;																					\
																												\
		theObject.DecodeSelf(theEncoder);																		\
		return(NVariant(theObject));																			\
	}																											\
																												\
	NString _class::EncodableGetClass(void) const																\
	{																											\
		return(#_class);																						\
	}																											\
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
	virtual NString						EncodableGetClass(void) const;


	// Encode/decode the object
	virtual void						EncodeSelf(      NEncoder &theEncoder) const;
	virtual void						DecodeSelf(const NEncoder &theEncoder);


private:


};





#endif // NENCODABLE_HDR

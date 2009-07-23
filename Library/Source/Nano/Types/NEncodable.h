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
class NEncoder;
class NString;





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
	virtual NString						GetEncoderClass(void) const = 0;


	// Encode/decode the object
	virtual void						EncodeSelf(      NEncoder &theEncoder) const = 0;
	virtual void						DecodeSelf(const NEncoder &theEncoder)       = 0;


private:


};





#endif // NENCODABLE_HDR

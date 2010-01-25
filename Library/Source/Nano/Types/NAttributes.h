/*	NAME:
		NAttributes.h

	DESCRIPTION:
		Mix-in class for objects with attributes.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NATTRIBUTES_HDR
#define NATTRIBUTES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const NBitfield kNAttributesNone								= 0;
static const NBitfield kNAttributesAll								= (NBitfield) -1;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NAttributes {
public:
										NAttributes(void);
	virtual							   ~NAttributes(void);


	// Is an attribute set?
	bool								HasAttribute(NBitfield theAttribute) const;


	// Clear attributes
	void								ClearAttributes(NBitfield theAttributes=kNAttributesAll);


	// Get/set the attributes
	//
	// When assigning, attributes are always cleared before being set.
	NBitfield							GetAttributes(void) const;
	virtual void						SetAttributes(NBitfield setThese, NBitfield clearThese=kNAttributesNone);


private:
	NBitfield							mAttributes;
};





#endif // NATTRIBUTES_HDR

/*	NAME:
		NFormatter.h

	DESCRIPTION:
		String formatter.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NFORMATTER_HDR
#define NFORMATTER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"





//============================================================================
//		Macros
//----------------------------------------------------------------------------
#define FORMAT_ARG(_n)					const NFormatArgument &arg ## _n

#define FORMAT_ARG1						FORMAT_ARG(1)
#define FORMAT_ARG2						FORMAT_ARG1,  FORMAT_ARG(2)
#define FORMAT_ARG3						FORMAT_ARG2,  FORMAT_ARG(3)
#define FORMAT_ARG4						FORMAT_ARG3,  FORMAT_ARG(4)
#define FORMAT_ARG5						FORMAT_ARG4,  FORMAT_ARG(5)
#define FORMAT_ARG6						FORMAT_ARG5,  FORMAT_ARG(6)
#define FORMAT_ARG7						FORMAT_ARG6,  FORMAT_ARG(7)
#define FORMAT_ARG8						FORMAT_ARG7,  FORMAT_ARG(8)
#define FORMAT_ARG9						FORMAT_ARG8,  FORMAT_ARG(9)
#define FORMAT_ARG10					FORMAT_ARG9,  FORMAT_ARG(10)
#define FORMAT_ARG11					FORMAT_ARG10, FORMAT_ARG(11)
#define FORMAT_ARG12					FORMAT_ARG11, FORMAT_ARG(12)
#define FORMAT_ARG13					FORMAT_ARG12, FORMAT_ARG(13)
#define FORMAT_ARG14					FORMAT_ARG13, FORMAT_ARG(14)
#define FORMAT_ARG15					FORMAT_ARG14, FORMAT_ARG(15)





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Classes
class NFormatArgument;


// Functors
typedef nfunctor<NString (const NString &theFormat)>				NFormatFunctor;


// Lists
typedef std::vector<NFormatArgument>								NFormatArgumentList;
typedef NFormatArgumentList::iterator								NFormatArgumentListIterator;
typedef NFormatArgumentList::const_iterator							NFormatArgumentListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NFormatArgument {
public:
										 NFormatArgument(unsigned int theValue);
										 NFormatArgument(  signed int theValue);

										 NFormatArgument(unsigned long theValue);
										 NFormatArgument(  signed long theValue);

										 NFormatArgument(unsigned long long theValue);
										 NFormatArgument(  signed long long theValue);

										 NFormatArgument(double      theValue);
										 NFormatArgument(const void *theValue);

										 NFormatArgument(const NFormatFunctor &getValue);
	virtual								~NFormatArgument(void);


	// Get the value
	NString								GetValue(const NString &theFormat) const;


private:
	NString								GetValueUInt(const NString &theFormat, unsigned int theValue);
	NString								GetValueSInt(const NString &theFormat,   signed int theValue);

	NString								GetValueULong(const NString &theFormat, unsigned long theValue);
	NString								GetValueSLong(const NString &theFormat,   signed long theValue);

	NString								GetValueULongLong(const NString &theFormat, unsigned long long theValue);
	NString								GetValueSLongLong(const NString &theFormat,   signed long long theValue);

	NString								GetValueDouble( const NString &theFormat, double      theValue);
	NString								GetValuePointer(const NString &theFormat, const void *theValue);

	NString								GetValue(   const NString &theFormat, const NString &validTypes, ...);
	bool								IsValidType(const NString &theFormat, const NString &validTypes);


private:
	NFormatFunctor						mGetValue;
};





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NFormatter {
public:
										 NFormatter(void);
	virtual								~NFormatter(void);


	// Format an argument list
	//
	// NFormatter accepts the standard IEEE format specifiers:
	//
	//		http://www.opengroup.org/onlinepubs/009695399/functions/printf.html
	//
	// Each argument is wrapped in an NFormatArgument object, whose GetValue
	// method is invoked to obtain the formatted text.
	//
	// Custom objects can be printed using a '%@' specifier by providing an
	// NFormatArgument cast operator that returns the text for the object.
	NString								Format(const NString &theFormat);
	NString								Format(const NString &theFormat, FORMAT_ARG1);
	NString								Format(const NString &theFormat, FORMAT_ARG2);
	NString								Format(const NString &theFormat, FORMAT_ARG3);
	NString								Format(const NString &theFormat, FORMAT_ARG4);
	NString								Format(const NString &theFormat, FORMAT_ARG5);
	NString								Format(const NString &theFormat, FORMAT_ARG6);
	NString								Format(const NString &theFormat, FORMAT_ARG7);
	NString								Format(const NString &theFormat, FORMAT_ARG8);
	NString								Format(const NString &theFormat, FORMAT_ARG9);
	NString								Format(const NString &theFormat, FORMAT_ARG10);
	NString								Format(const NString &theFormat, FORMAT_ARG11);
	NString								Format(const NString &theFormat, FORMAT_ARG12);
	NString								Format(const NString &theFormat, FORMAT_ARG13);
	NString								Format(const NString &theFormat, FORMAT_ARG14);
	NString								Format(const NString &theFormat, FORMAT_ARG15);


private:
	NString								Format(const NString &theFormat, const NFormatArgumentList &theArguments);


private:


};





#endif // NFORMATTER_HDR

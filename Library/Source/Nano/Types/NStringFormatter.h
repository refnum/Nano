/*	NAME:
		NStringFormatter.h

	DESCRIPTION:
		String formatter.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSTRINGFORMATTER_HDR
#define NSTRINGFORMATTER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <string>





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// Format arguments
#define NN_FORMAT_ARGS					const NFormatArgument &arg1  = NFormatArgument(),	\
										const NFormatArgument &arg2  = NFormatArgument(),	\
										const NFormatArgument &arg3  = NFormatArgument(),	\
										const NFormatArgument &arg4  = NFormatArgument(),	\
										const NFormatArgument &arg5  = NFormatArgument(),	\
										const NFormatArgument &arg6  = NFormatArgument(),	\
										const NFormatArgument &arg7  = NFormatArgument(),	\
										const NFormatArgument &arg8  = NFormatArgument(),	\
										const NFormatArgument &arg9  = NFormatArgument(),	\
										const NFormatArgument &arg10 = NFormatArgument(),	\
										const NFormatArgument &arg11 = NFormatArgument(),	\
										const NFormatArgument &arg12 = NFormatArgument(),	\
										const NFormatArgument &arg13 = NFormatArgument(),	\
										const NFormatArgument &arg14 = NFormatArgument(),	\
										const NFormatArgument &arg15 = NFormatArgument()

#define NN_FORMAT_ARGS_PARAM			const NFormatArgument &arg1,						\
										const NFormatArgument &arg2, 						\
										const NFormatArgument &arg3, 						\
										const NFormatArgument &arg4, 						\
										const NFormatArgument &arg5, 						\
										const NFormatArgument &arg6, 						\
										const NFormatArgument &arg7, 						\
										const NFormatArgument &arg8, 						\
										const NFormatArgument &arg9, 						\
										const NFormatArgument &arg10,						\
										const NFormatArgument &arg11,						\
										const NFormatArgument &arg12,						\
										const NFormatArgument &arg13,						\
										const NFormatArgument &arg14,						\
										const NFormatArgument &arg15

#define NN_FORMAT_ARGS_LIST				arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Classes
class NRange;
class NFormatArgument;
class NFormatFunctor;


// Lists
typedef std::vector<const NFormatArgument*>							NFormatArgumentList;
typedef NFormatArgumentList::iterator								NFormatArgumentListIterator;
typedef NFormatArgumentList::const_iterator							NFormatArgumentListConstIterator;


// Context
typedef struct {
	NFormatArgumentList		theArguments;
	bool					gotError;
	NIndex					argIndex;
} NFormatContext;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
//		Note :	See NFormatter.cpp for background - this class is needed to
//				work around a gcc 4.0 bug, but user code should always use
//				NString rather than this class.
//----------------------------------------------------------------------------
class NStringUTF8 : public std::string {
public:
	NStringUTF8(const std::string &theString)			: std::string(theString)			{ }
	NStringUTF8(const char *theText)					: std::string(theText)				{ }
	NStringUTF8(const char *theText, NIndex theSize)	: std::string(theText, theSize)		{ }

										NStringUTF8(void)		{ }
	virtual							   ~NStringUTF8(void)		{ }

	bool								IsEmpty(void) const		{ return(empty());         }
	NIndex								GetSize(void) const		{ return((NIndex) size()); }
	const char						   *GetUTF8(void) const		{ return(c_str());         }
	
	NRange								Find(const NStringUTF8 &theString) const;
	void								Replace(const NRange &theRange, const NStringUTF8 &replaceWith);
};





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

										NFormatArgument(double             theValue);
										NFormatArgument(const void        *theValue);
										NFormatArgument(const char        *theValue);
										NFormatArgument(const NStringUTF8 &theValue);

										NFormatArgument(void);
	virtual							   ~NFormatArgument(void);


	// Is the argument valid?
	bool								IsValid(void) const;
	

	// Get the value
	NStringUTF8							GetValue(const NStringUTF8 &theFormat) const;


private:
	NStringUTF8							GetValueULong(const NStringUTF8 &theFormat, unsigned long theValue);
	NStringUTF8							GetValueSLong(const NStringUTF8 &theFormat,   signed long theValue);

	NStringUTF8							GetValueULongLong(const NStringUTF8 &theFormat, unsigned long long theValue);
	NStringUTF8							GetValueSLongLong(const NStringUTF8 &theFormat,   signed long long theValue);

	NStringUTF8							GetValueDouble( const NStringUTF8 &theFormat, double             theValue);
	NStringUTF8							GetValuePointer(const NStringUTF8 &theFormat, const void        *theValue);
	NStringUTF8							GetValueCharPtr(const NStringUTF8 &theFormat, const char        *theValue);
	NStringUTF8							GetValueString( const NStringUTF8 &theFormat, const NStringUTF8 &theValue);
	
	NStringUTF8							GetValue(   const NStringUTF8 &theFormat, const char *validTypes, ...);
	bool								IsValidType(const NStringUTF8 &theFormat, const char *validTypes);


private:
	NFormatFunctor					   *mGetValue;
};





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NStringFormatter {
public:
										NStringFormatter(void);
	virtual							   ~NStringFormatter(void);


	// Format an argument list
	//
	// NFormatter accepts the standard IEEE format specifiers:
	//
	//		http://www.opengroup.org/onlinepubs/009695399/functions/printf.html
	//
	// Example specifiers include:
	//
	//			Type				Specifier
	//			=============================
	//			UInt8				%d
	//			UInt16				%d
	//			UInt32				%ld
	//			UInt64				%lld
	//
	//			SInt8				%d
	//			SInt16				%d
	//			SInt32				%ld
	//			SInt64				%lld
	//
	//			Float32				%f
	//			Float64				%lf
	//
	//			Objects				%@
	//
	// Arguments are printed by wrapping them in an NFormatArgument object,
	// whose GetValue method is invoked to obtain the formatted text.
	//
	// This allows custom objects to be printed using the %@ specifier, by
	// providing a suitable NFormatArgument cast operator.
	//
	//
	// On some platforms the UInt32/SInt32 types use a different underlying type
	// for 64-bit vs 32-bit builds due to existing definitions in system headers.
	//
	// This means that a UInt32 is declared as an 'int' typedef on Mac 64-bit, vs
	// a 'long' on Mac 32-bit.
	//
	// Since 'int' and 'long' strictly speaking require different format specifiers
	// ('%d' and '%ld' respectively), NFormatArgument promotes all 'int' arguments
	// to 'long' to allow '%ld' to be used for both.
	NStringUTF8							Format(const NStringUTF8 &theFormat, NN_FORMAT_ARGS);


private:
	NStringUTF8							Format(const NStringUTF8 &theFormat, const NFormatArgumentList &theArguments);

	NStringUTF8							ParseToken( NFormatContext &theContext, const NStringUTF8 &theToken);
	NStringUTF8							ParseFailed(NFormatContext &theContext, const NStringUTF8 &theToken, const NStringUTF8 &theError);

	NIndex								ParseIndexRef(NFormatContext &theContext, const NStringUTF8 &theToken, const NStringUTF8 &thePrefix, NRange &theRange);
	NIndex								ParseIndex(const char *textPtr, NRange &theRange);

	bool								IsValidArg( NFormatContext &theContext, NIndex theIndex);
	NStringUTF8							GetArgValue(NFormatContext &theContext, NIndex theIndex, const NStringUTF8 &theFormat);
	
	NIndex								ToDigit(char c);


private:


};





#endif // NSTRINGFORMATTER_HDR

/*	NAME:
		NStringFormatter.h

	DESCRIPTION:
		String formatter.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSTRINGFORMATTER_HDR
#define NSTRINGFORMATTER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#if NN_TARGET_MACOS
	#include <CoreFoundation/CoreFoundation.h>

	#if defined(__OBJC__)
		#include <Foundation/Foundation.h>
	#endif
#endif

#include "NFunctor.h"





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// Format arguments
#define NN_FORMAT_ARGS					const NFormatArgument &arg1  = kNFormatArgumentNone,	\
										const NFormatArgument &arg2  = kNFormatArgumentNone,	\
										const NFormatArgument &arg3  = kNFormatArgumentNone,	\
										const NFormatArgument &arg4  = kNFormatArgumentNone,	\
										const NFormatArgument &arg5  = kNFormatArgumentNone,	\
										const NFormatArgument &arg6  = kNFormatArgumentNone,	\
										const NFormatArgument &arg7  = kNFormatArgumentNone,	\
										const NFormatArgument &arg8  = kNFormatArgumentNone,	\
										const NFormatArgument &arg9  = kNFormatArgumentNone,	\
										const NFormatArgument &arg10 = kNFormatArgumentNone,	\
										const NFormatArgument &arg11 = kNFormatArgumentNone,	\
										const NFormatArgument &arg12 = kNFormatArgumentNone,	\
										const NFormatArgument &arg13 = kNFormatArgumentNone,	\
										const NFormatArgument &arg14 = kNFormatArgumentNone,	\
										const NFormatArgument &arg15 = kNFormatArgumentNone

#define NN_FORMAT_ARGS_PARAM			const NFormatArgument &arg1,							\
										const NFormatArgument &arg2, 							\
										const NFormatArgument &arg3, 							\
										const NFormatArgument &arg4, 							\
										const NFormatArgument &arg5, 							\
										const NFormatArgument &arg6, 							\
										const NFormatArgument &arg7, 							\
										const NFormatArgument &arg8, 							\
										const NFormatArgument &arg9, 							\
										const NFormatArgument &arg10,							\
										const NFormatArgument &arg11,							\
										const NFormatArgument &arg12,							\
										const NFormatArgument &arg13,							\
										const NFormatArgument &arg14,							\
										const NFormatArgument &arg15

#define NN_FORMAT_ARGS_LIST				arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Classes
class NRange;
class NString;
class NFormatArgument;


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


// Functors
typedef nfunctor<NString (const NString &theFormat)>				NFormatFunctor;


// Obj-C
#if NN_TARGET_MACOS && !defined(__OBJC__)
	typedef struct ptrNSObject *NSObject;
#endif





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

										NFormatArgument(double         theValue);
										NFormatArgument(const void    *theValue);
										NFormatArgument(const char    *theValue);
										NFormatArgument(const NString &theValue);

										NFormatArgument(const NFormatFunctor &getValue);

#if NN_TARGET_MACOS
										NFormatArgument(NSObject    *nsValue);
										NFormatArgument(CFStringRef  cfValue);
#endif

										NFormatArgument(void);
	virtual							   ~NFormatArgument(void);


	// Is the argument valid?
	bool								IsValid(void) const;
	

	// Get the value
	NString								GetValue(const NString &theFormat) const;


private:
	NString								GetValueULong(const NString &theFormat, unsigned long theValue);
	NString								GetValueSLong(const NString &theFormat,   signed long theValue);

	NString								GetValueULongLong(const NString &theFormat, unsigned long long theValue);
	NString								GetValueSLongLong(const NString &theFormat,   signed long long theValue);

	NString								GetValueDouble( const NString &theFormat, double         theValue);
	NString								GetValuePointer(const NString &theFormat, const void    *theValue);
	NString								GetValueCharPtr(const NString &theFormat, const char    *theValue);
	NString								GetValueString( const NString &theFormat, const NString &theValue);
	
	NString								GetValue(   const NString &theFormat, const char *validTypes, ...);
	bool								IsValidType(const NString &theFormat, const char *validTypes);


private:
	NFormatFunctor						mGetValue;
};





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const NFormatArgument kNFormatArgumentNone;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NStringFormatter {
public:
										NStringFormatter(void);
	virtual							   ~NStringFormatter(void);


	// Format an argument list
	//
	// NStringFormatter accepts the standard IEEE format specifiers:
	//
	//		http://www.opengroup.org/onlinepubs/009695399/functions/printf.html
	//
	// Example specifiers include:
	//
	//			Type				Specifier
	//			=============================
	//			uint8_t				%d
	//			uint16_t			%d
	//			uint32_t			%ld
	//			uint64_t			%lld
	//
	//			int8_t				%d
	//			int16_t				%d
	//			int32_t				%ld
	//			int64_t				%lld
	//
	//			float32_t			%f
	//			float64_t			%lf
	//
	//			char				%c
	//			wchar_t				%lc
	//
	//			char *				%s
	//			wchar_t *			%ls
	//
	//			Objects				%@
	//
	// Arguments are printed by wrapping them in an NFormatArgument object, whose
	// GetValue method is invoked to obtain the formatted text.
	//
	// This allows custom objects to be printed using the %@ specifier, by providing
	// a suitable NFormatArgument cast operator.
	//
	//
	// On some platforms the following behaviour is undefined:
	//
	//		uint32_t	value32 = 1234;
	//		uint64_t	value64 = 1234;
	//
	//		Format("%lld", value32);	// Use a 64-bit length modifier, and 32-bit value
	//		Format("%ld",  value64);	// Use a 32-bit length modifier, and 64-bit value
	//
	// The former case is promoted correctly on x86/x86_64, however the latter case
	// is a potential bug on all platforms (PPC produces 0 in both cases).
	//
	// As such, a warning will be issued if a 32-bit length modifier is used for a
	// 64-bit value, or a 64-bit length modifier used for a 32-bit value.
	NString								Format(const NString &theFormat, NN_FORMAT_ARGS);


private:
	NString								Format(const NString &theFormat, const NFormatArgumentList &theArguments);

	NString								ParseToken( NFormatContext &theContext, const NString &theToken);
	NString								ParseFailed(NFormatContext &theContext, const NString &theToken, const NString &theError);

	NIndex								ParseIndexRef(NFormatContext &theContext, const NString &theToken, const NString &thePrefix, NRange &theRange);
	NIndex								ParseIndex(const char *textPtr, NRange &theRange);

	bool								IsValidArg( NFormatContext &theContext, NIndex theIndex);
	NString								GetArgValue(NFormatContext &theContext, NIndex theIndex, const NString &theFormat);
	
	NIndex								ToDigit(char c);


private:


};





#endif // NSTRINGFORMATTER_HDR

/*	NAME:
		NStringFormatter.cpp

	DESCRIPTION:
		String formatter.
		
		NStringFormatter uses NStringUTF8, rather than NString, since its header
		is included in the precompiled header. This is so that the NN_LOG macros
		from NDebug.h can be used to format objects:
		
			NString		theString = "test";
			NColor		theColor  = kColorRed;
			
			NN_LOG("Text is %@, color is %@", theString, theColor);

		Unfortunately gcc 4.0 contains a bug which prevents the use of anonymous
		namespaces within precompiled header files, and NFunctor pulls in parts
		of TR1 that use anonymous namespaces.
		
			http://gcc.gnu.org/bugzilla/show_bug.cgi?id=10591

		This prevents us from using NFunctor, or classes which reference it, in
		the precompiled header file.
		
		Instead NStringFormatter defines its own simple string class, which NString
		provides a copy contructor and cast operator for to allow it to be used
		interchangeably with NString outside of NDebug.h.


		This bug has been fixed in gcc 4.2, but that can only support Mac OS X
		10.5 or higher and does not support the iPhone.

		At some point Nano will require gcc 4.2, at which point the implementation
		can be simplified:
		
			- Nano.h can pull in all header files into the precompiled header,
			  including NFunctor.h, speeding up builds and avoiding this issue.
		
			- NStringFormatter.h can declare NFormatFunctor, and NFormatArgument
			  can use this type directly rather than through a pointer.

			- NFormatArgument will be able to take an NFormatFunctor for the
			  the %@ case, rather than an explicit string, allowing an arbitrary
			  callback for formatting custom objects (the current functor-based
			  approach for formatting integer/float values could be simplified
			  with a union, but the generalised functor approach is a better
			  design so this has been kept with a view to simplifying the changes
			  for gcc 4.2).
			
			- NStringUTF8 can be removed, since NString can be used in NDebug.h.
		
		Until then we must use this custom string class, and hide the implementation
		of NFormatFunctor, so that custom objects can be printed via NDebug macros.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NRange.h"
#include "NFunctor.h"
#include "NStringFormatter.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const char *kFormatTypesInteger									= "diouxXcC";
static const char *kFormatTypesDouble									= "fFeEgGaA";
static const char *kFormatTypesPointer									= "sSpn";
static const char *kFormatTypesCustom									= "@";

static const UInt32      kFormatBufferSize								= 256;
static const NStringUTF8 kFormatTypes									= NStringUTF8(kFormatTypesInteger) +
																		  NStringUTF8(kFormatTypesDouble)  +
																		  NStringUTF8(kFormatTypesPointer) +
																		  NStringUTF8(kFormatTypesCustom);





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
// Log without using NStringFormatter
#if NN_DEBUG
	#define NN_LOG_FORMATTER(...)										\
		do																\
			{															\
			NDebug_LogMessage(__FILE__, __LINE__, __VA_ARGS__);			\
			}															\
		while (0)
#else
	#define NN_LOG_FORMATTER(...)				do { } while (0)
#endif


// Collect an argument list
#define NN_FORMAT_ARGS_COLLECT(_argList)		if (arg1.IsValid())		{								_argList.push_back(&arg1);	} \
												if (arg2.IsValid())		{ NN_ASSERT(arg1.IsValid());	_argList.push_back(&arg2);	} \
												if (arg3.IsValid())		{ NN_ASSERT(arg2.IsValid());	_argList.push_back(&arg3);	} \
												if (arg4.IsValid())		{ NN_ASSERT(arg3.IsValid());	_argList.push_back(&arg4);	} \
												if (arg5.IsValid())		{ NN_ASSERT(arg4.IsValid());	_argList.push_back(&arg5);	} \
												if (arg6.IsValid())		{ NN_ASSERT(arg5.IsValid());	_argList.push_back(&arg6);	} \
												if (arg7.IsValid())		{ NN_ASSERT(arg6.IsValid());	_argList.push_back(&arg7);	} \
												if (arg8.IsValid())		{ NN_ASSERT(arg7.IsValid());	_argList.push_back(&arg8);	} \
												if (arg9.IsValid())		{ NN_ASSERT(arg8.IsValid());	_argList.push_back(&arg9);	} \
												if (arg10.IsValid())	{ NN_ASSERT(arg9.IsValid());	_argList.push_back(&arg10);	} \
												if (arg11.IsValid())	{ NN_ASSERT(arg10.IsValid());	_argList.push_back(&arg11);	} \
												if (arg12.IsValid())	{ NN_ASSERT(arg11.IsValid());	_argList.push_back(&arg12);	} \
												if (arg13.IsValid())	{ NN_ASSERT(arg12.IsValid());	_argList.push_back(&arg13);	} \
												if (arg14.IsValid())	{ NN_ASSERT(arg13.IsValid());	_argList.push_back(&arg14);	} \
												if (arg15.IsValid())	{ NN_ASSERT(arg14.IsValid());	_argList.push_back(&arg15);	}





//============================================================================
//		Internal types
//----------------------------------------------------------------------------
typedef nfunctor<NStringUTF8 (const NStringUTF8 &theFormat)>		NFormatFunctorBase;

class NFormatFunctor : public NFormatFunctorBase {
public:
			 NFormatFunctor(const NFormatFunctorBase &theFunctor) : NFormatFunctorBase(theFunctor)	{ }
	virtual ~NFormatFunctor(void)																	{ }
};





//============================================================================
//		NStringUTF8::Find : Find a substring.
//----------------------------------------------------------------------------
NRange NStringUTF8::Find(const NStringUTF8 &theString) const
{	size_type		theOffset;
	NRange			theRange;



	// Validate our parameters
	NN_ASSERT(!theString.IsEmpty());
	
	
	
	// Find the string
	theOffset = find(theString);
	theRange  = kNRangeNone;

	if (theOffset != npos)
		theRange = NRange(theOffset, theString.GetSize());
	
	return(theRange);
}





//============================================================================
//		NStringUTF8::Replace : Replace a portion of a string.
//----------------------------------------------------------------------------
void NStringUTF8::Replace(const NRange &theRange, const NStringUTF8 &replaceWith)
{


	// Validate our parameters
	NN_ASSERT(!theRange.IsEmpty());
	
	
	
	// Replace the string
	replace(theRange.GetLocation(), theRange.GetSize(), replaceWith);
}





//============================================================================
//		NFormatArgument::NFormatArgument : Constructor.
//----------------------------------------------------------------------------
NFormatArgument::NFormatArgument(unsigned int theValue)				{	mGetValue = new NFormatFunctor(BindSelf(NFormatArgument::GetValueUInt,		_1, theValue));	}
NFormatArgument::NFormatArgument(  signed int theValue)				{	mGetValue = new NFormatFunctor(BindSelf(NFormatArgument::GetValueSInt,		_1, theValue));	}

NFormatArgument::NFormatArgument(unsigned long theValue)			{	mGetValue = new NFormatFunctor(BindSelf(NFormatArgument::GetValueULong,		_1, theValue));	}
NFormatArgument::NFormatArgument(  signed long theValue)			{	mGetValue = new NFormatFunctor(BindSelf(NFormatArgument::GetValueSLong,		_1, theValue));	}

NFormatArgument::NFormatArgument(unsigned long long theValue)		{	mGetValue = new NFormatFunctor(BindSelf(NFormatArgument::GetValueULongLong,	_1, theValue));	}
NFormatArgument::NFormatArgument(  signed long long theValue)		{	mGetValue = new NFormatFunctor(BindSelf(NFormatArgument::GetValueSLongLong,	_1, theValue));	}

NFormatArgument::NFormatArgument(double				 theValue)		{	mGetValue = new NFormatFunctor(BindSelf(NFormatArgument::GetValueDouble,	_1, theValue));	}
NFormatArgument::NFormatArgument(const void			*theValue)		{	mGetValue = new NFormatFunctor(BindSelf(NFormatArgument::GetValuePointer,	_1, theValue));	}
NFormatArgument::NFormatArgument(const char			*theValue)		{	mGetValue = new NFormatFunctor(BindSelf(NFormatArgument::GetValueCharPtr,	_1, theValue));	}
NFormatArgument::NFormatArgument(const NStringUTF8	&theValue)		{	mGetValue = new NFormatFunctor(BindSelf(NFormatArgument::GetValueString,	_1, theValue));	}





//============================================================================
//		NFormatArgument::NFormatArgument : Constructor.
//----------------------------------------------------------------------------
NFormatArgument::NFormatArgument(void)
{


	// Initialize ourselves
	mGetValue = NULL;
}





//============================================================================
//		NFormatArgument::~NFormatArgument : Destructor.
//----------------------------------------------------------------------------
NFormatArgument::~NFormatArgument(void)
{


	// Clean up
	delete mGetValue;
}





//============================================================================
//		NFormatArgument::IsValid : Is the argument valid?
//----------------------------------------------------------------------------
bool NFormatArgument::IsValid(void) const
{


	// Check our state
	return(mGetValue != NULL);
}





//============================================================================
//		NFormatArgument::GetValue : Get the value.
//----------------------------------------------------------------------------
NStringUTF8 NFormatArgument::GetValue(const NStringUTF8 &theFormat) const
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Get the value
	return((*mGetValue)(theFormat));
}





//============================================================================
//		NFormatArgument::GetValueUInt : Get an unsigned int value.
//----------------------------------------------------------------------------
#pragma mark -
NStringUTF8 NFormatArgument::GetValueUInt(const NStringUTF8 &theFormat, unsigned int theValue)
{


	// Get the value
	return(GetValue(theFormat, kFormatTypesInteger, theValue));
}





//============================================================================
//		NFormatArgument::GetValueSInt : Get a signed int value.
//----------------------------------------------------------------------------
NStringUTF8 NFormatArgument::GetValueSInt(const NStringUTF8 &theFormat, signed int theValue)
{


	// Get the value
	return(GetValue(theFormat, kFormatTypesInteger, theValue));
}





//============================================================================
//		NFormatArgument::GetValueULong : Get an unsigned long value.
//----------------------------------------------------------------------------
NStringUTF8 NFormatArgument::GetValueULong(const NStringUTF8 &theFormat, unsigned long theValue)
{


	// Get the value
	return(GetValue(theFormat, kFormatTypesInteger, theValue));
}





//============================================================================
//		NFormatArgument::GetValueSLong : Get a signed long value.
//----------------------------------------------------------------------------
NStringUTF8 NFormatArgument::GetValueSLong(const NStringUTF8 &theFormat, signed long theValue)
{


	// Get the value
	return(GetValue(theFormat, kFormatTypesInteger, theValue));
}





//============================================================================
//		NFormatArgument::GetValueULongLong : Get an unsigned long long value.
//----------------------------------------------------------------------------
NStringUTF8 NFormatArgument::GetValueULongLong(const NStringUTF8 &theFormat, unsigned long long theValue)
{


	// Get the value
	return(GetValue(theFormat, kFormatTypesInteger, theValue));
}





//============================================================================
//		NFormatArgument::GetValueSLongLong : Get a signed long long value.
//----------------------------------------------------------------------------
NStringUTF8 NFormatArgument::GetValueSLongLong(const NStringUTF8 &theFormat, signed long long theValue)
{


	// Get the value
	return(GetValue(theFormat, kFormatTypesInteger, theValue));
}





//============================================================================
//		NFormatArgument::GetValueDouble : Get a double value.
//----------------------------------------------------------------------------
NStringUTF8 NFormatArgument::GetValueDouble(const NStringUTF8 &theFormat, double theValue)
{


	// Get the value
	return(GetValue(theFormat, kFormatTypesDouble, theValue));
}





//============================================================================
//		NFormatArgument::GetValuePointer : Get a pointer value.
//----------------------------------------------------------------------------
NStringUTF8 NFormatArgument::GetValuePointer(const NStringUTF8 &theFormat, const void *theValue)
{


	// Get the value
	return(GetValue(theFormat, kFormatTypesPointer, theValue));
}





//============================================================================
//		NFormatArgument::GetValueCharPtr : Get a char* value.
//----------------------------------------------------------------------------
NStringUTF8 NFormatArgument::GetValueCharPtr(const NStringUTF8 &theFormat, const char *theValue)
{


	// Validate our parameters
	NN_ASSERT(theFormat == "%s");
	NN_UNUSED(theFormat);



	// Get the value
	return(NStringUTF8(theValue));
}





//============================================================================
//		NFormatArgument::GetValueString : Get a string value.
//----------------------------------------------------------------------------
NStringUTF8 NFormatArgument::GetValueString(const NStringUTF8 &theFormat, const NStringUTF8 &theValue)
{


	// Validate our parameters
	NN_ASSERT(theFormat == "%@");
	NN_UNUSED(theFormat);



	// Get the value
	return(theValue);
}





//============================================================================
//		NFormatArgument::GetValue : Get a formatted value.
//----------------------------------------------------------------------------
NStringUTF8 NFormatArgument::GetValue(const NStringUTF8 &theFormat, const char *validTypes, ...)
{	char			stackBuffer[kFormatBufferSize];
	char			*heapBuffer;
	NStringUTF8		theResult;
	NIndex			numChars;
	va_list			argList;



	// Check our parameters
	//
	// Although sprintf will convert some types automatically, we complain if we see
	// this kind of implicit conversion since the caller should use the correct type.
	if (NN_DEBUG && !IsValidType(theFormat, validTypes))
		NN_LOG("Invalid type used for '%s'", theFormat.c_str());



	// Prepare to format
	//
	// va_list's behaviour is undefined if it is passed a reference rather than
	// a POD, and so validTypes must always be a char* or similar POD type.
	NN_ASSERT( typeid(validTypes) == typeid(const char *));
	va_start(argList, validTypes);



	// Format to the stack
	numChars = vsnprintf(stackBuffer, sizeof(stackBuffer), theFormat.GetUTF8(), argList);
	if (numChars < (NIndex) sizeof(stackBuffer))
		theResult = NStringUTF8(stackBuffer, numChars);


	// Format to the heap
	else
		{
		numChars   = numChars + 1;
		heapBuffer = (char *) malloc(numChars);
		if (heapBuffer != NULL)
			{
			numChars  = vsnprintf(heapBuffer, numChars, theFormat.GetUTF8(), argList);
			theResult = NStringUTF8(heapBuffer, numChars);

			free(heapBuffer);
			}
		}



	// Clean up
	va_end(argList);

	return(theResult);
}





//============================================================================
//		NFormatArgument::IsValidType : Is a format type valid?
//----------------------------------------------------------------------------
bool NFormatArgument::IsValidType(const NStringUTF8 &theFormat, const char *validTypes)
{	const char		*formatUTF8, *theType;



	// Get the state we need
	formatUTF8  = theFormat.GetUTF8();
	formatUTF8 += strlen(formatUTF8) - 1;



	// Find the type
	theType = strpbrk(formatUTF8, validTypes);

	return(theType != NULL);
}





//============================================================================
//		NStringFormatter::NStringFormatter : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
NStringFormatter::NStringFormatter(void)
{
}





//============================================================================
//		NStringFormatter::~NStringFormatter : Destructor.
//----------------------------------------------------------------------------
NStringFormatter::~NStringFormatter(void)
{
}





//============================================================================
//		NStringFormatter::Format : Format an argument list.
//----------------------------------------------------------------------------
NStringUTF8 NStringFormatter::Format(const NStringUTF8 &theFormat, NN_FORMAT_ARGS_PARAM)
{	NFormatArgumentList		theArguments;



	// Format the arguments
	NN_FORMAT_ARGS_COLLECT(theArguments);
	
	return(Format(theFormat, theArguments));
}





//============================================================================
//		NStringFormatter::Format : Format an argument list.
//----------------------------------------------------------------------------
#pragma mark -
NStringUTF8 NStringFormatter::Format(const NStringUTF8 &theFormat, const NFormatArgumentList &theArguments)
{	const char			*textUTF8, *typesUTF8, *tokenStart, *tokenEnd;
	NStringUTF8			theResult, theToken;
	NFormatContext		theContext;
	bool				areDone;



	// Get the state we need
	theContext.theArguments = theArguments;
	theContext.gotError     = false;
	theContext.argIndex     = 0;

	textUTF8  = theFormat.GetUTF8();
	typesUTF8 = kFormatTypes.GetUTF8();
	areDone   = false;



	// Format the string
	while (!areDone)
		{
		// Find the next format token
		tokenStart = strstr(textUTF8, "%");
		if (tokenStart == NULL)
			break;



		// Append the text to this point
		theResult += NStringUTF8(textUTF8, tokenStart - textUTF8);
		textUTF8   = tokenStart + 1;



		// Process the token
		switch (*textUTF8) {
			case 0x00:
				areDone = true;
				break;
			
			case '%':
				theResult += "%";
				textUTF8  += 1;
				break;
			
			default:
				// Find the type
				tokenEnd = strpbrk(textUTF8, typesUTF8);
				if (tokenEnd == NULL)
					{
					NN_LOG_FORMATTER("Missing type: found '%%' without type in '%s'", theFormat.GetUTF8());
					theResult += "%";
					areDone    = true;
					}


				// Evaluate the token
				else
					{
					// Evaluate the token
					theToken   = NStringUTF8(tokenStart, tokenEnd - tokenStart + 1);
					theResult += ParseToken(theContext, theToken);



					// Advance the token
					textUTF8 = tokenEnd + 1;
					areDone  = theContext.gotError;
					}
				break;
			}
		}



	// Append any remaining text
	if (*textUTF8 != 0x00)
		theResult += NStringUTF8(textUTF8);

	return(theResult);
}





//============================================================================
//		NStringFormatter::ParseToken : Parse a format token.
//----------------------------------------------------------------------------
//		Note :	Positional references are supported by evaluating references
//				within the token substituting their value before processing.
//
//				This allows us to support both "next argument" and positional
//				argument index references, even if the platform printf engine
//				does not.
//----------------------------------------------------------------------------
NStringUTF8 NStringFormatter::ParseToken(NFormatContext &theContext, const NStringUTF8 &theToken)
{	NRange			theRange, indexRange;
	NStringUTF8		finalToken;
	NStringUTF8		theResult;
	NIndex			theIndex;



	// Validate our parameters
	NN_ASSERT(theToken.size() >= 2);



	// Get the state we need
	finalToken = theToken;



	// Process position
	//
	// The leading '%' can be followed by a reference in the form '%ddd$' (indexed argument).
	//
	// Although parsing the index reference will advance argIndex, the position reference
	// simply resets the next argument to that index.
	//
	// Although not required by POSIX, this allows us to mix both referenced and non-referenced
	// arguments, where subsequent non-referenced arguments are processed sequentially.
	theIndex = ParseIndexRef(theContext, finalToken, "%", theRange);
	if (theIndex != kNIndexNone)
		{
		if (!IsValidArg(theContext, theIndex))
			return(ParseFailed(theContext, theToken, "Invalid index"));

		theContext.argIndex = theIndex;
		finalToken.Replace(theRange, "%");
		}



	// Process precision
	//
	// The precision specifier can contain a reference in the form '.*' (next argument)
	// or '.*ddd$' (indexed argument).
	//
	// Negative precision values must be discarded.
	theIndex = ParseIndexRef(theContext, finalToken, ".*", theRange);
	if (theIndex != kNIndexNone)
		{
		if (!IsValidArg(theContext, theIndex))
			return(ParseFailed(theContext, theToken, "Invalid index"));

		theResult = "." + GetArgValue(theContext, theIndex, "%ld");
		if (theResult.Find("-") != kNRangeNone)
			theResult = "";

		finalToken.Replace(theRange, theResult);
		}



	// Processs width
	//
	// The width specifier can contain an reference in the form '*' (next argument)
	// or '*ddd$' (indexed argument).
	//
	// Although the width specifier comes first in the token, we process the precision
	// specifier above since it starts with a leading '.' prefix.
	//
	// This mean that once precision references have been replaced, any remaining
	// references must belong to the width specifier.
	theIndex = ParseIndexRef(theContext, finalToken, "*", theRange);
	if (theIndex != kNIndexNone)
		{
		if (!IsValidArg(theContext, theIndex))
			return(ParseFailed(theContext, theToken, "Invalid index"));

		theResult = GetArgValue(theContext, theIndex, "%ld");
		finalToken.Replace(theRange, theResult);
		}



	// Evaluate the token
	//
	// Once references have been replaced, the argument can be evaluated.
	if (!IsValidArg(theContext, theContext.argIndex))
		return(ParseFailed(theContext, theToken, "Invalid index"));

	theResult = GetArgValue(theContext, theContext.argIndex, finalToken);
	theContext.argIndex++;

	return(theResult);
}





//============================================================================
//		NStringFormatter::ParseFailed : Handle failure.
//----------------------------------------------------------------------------
NStringUTF8 NStringFormatter::ParseFailed(NFormatContext &theContext, const NStringUTF8 &theToken, const NStringUTF8 &theError)
{


	// Compiler warnings
	NN_UNUSED(theToken);
	NN_UNUSED(theError);



	// Handle failure
	NN_LOG("%s: '%s'", theError.c_str(), theToken.c_str());
	theContext.gotError = true;

	return("");
}





//============================================================================
//		NStringFormatter::ParseIndexRef : Parse an index reference.
//----------------------------------------------------------------------------
NIndex NStringFormatter::ParseIndexRef(NFormatContext &theContext, const NStringUTF8 &theToken, const NStringUTF8 &thePrefix, NRange &theRange)
{	NRange			indexRange;
	const char		*textPtr;
	NIndex			theIndex;
	char			ch;



	// Find the reference
	theRange = theToken.Find(thePrefix);
	if (theRange.IsEmpty())
		return(kNIndexNone);



	// Get the state we need
	indexRange = NRange(theRange.GetLocation() + thePrefix.size(), 1);
	if (indexRange.GetLast() >= (NIndex) theToken.size())
		{
		ParseFailed(theContext, theToken, "Unterminated index");
		return(kNIndexNone);
		}



	// Index is a positional index
	//
	// Positional indices have a prefix, a series of digits, and a '$' terminator.
	//
	// Positional indices are indexed from 1, not 0, so we need to convert back to
	// a 0-based index to locate the argument it refers to.
	textPtr = theToken.c_str();
	ch      = textPtr[indexRange.GetLocation()];

	if (ch >= '1' && ch <= '9')
		{
		theIndex = ParseIndex(textPtr, indexRange);
		if (theIndex != kNIndexNone)
			{
			theIndex = theIndex - 1;
			theRange = theRange.GetUnion(indexRange);
			}
		}



	// Index is an implicit index
	//
	// Implicit indices are simply the prefix, and the next argument provides the index.
	else
		{
		theIndex = theContext.argIndex;
		theContext.argIndex++;
		}

	return(theIndex);
}





//============================================================================
//		NStringFormatter::ParseIndex : Parse a $-terminated index value.
//----------------------------------------------------------------------------
NIndex NStringFormatter::ParseIndex(const char *textPtr, NRange &theRange)
{	const char		*startPtr, *charPtr;
	char			ch;
	NIndex			n;



	// Get the state we need
	startPtr = textPtr + theRange.GetLocation();
	charPtr  = startPtr;

	ch = *charPtr++;
	n  = 0;

	NN_ASSERT(isdigit(ch));
	
	
	
	// Parse the index
	//
	// The index is one or more digits, terminated with a '$'.
	do
		{
		n  = (n * 10) + ToDigit(ch);
		ch = *charPtr++;
		}
	while (isdigit(ch) && ch != 0);



	// Parse the terminator
	if (ch == '$')
		theRange.SetSize(charPtr - startPtr);
	else
		{
		n        = kNIndexNone;
		theRange = kNRangeNone;
		}

	return(n);
}





//============================================================================
//		NStringFormatter::IsValidArg : Is an argument index valid?
//----------------------------------------------------------------------------
bool NStringFormatter::IsValidArg(NFormatContext &theContext, NIndex theIndex)
{	bool	isValid;



	// Check the index
	isValid = (theIndex >= 0 && theIndex < (NIndex) theContext.theArguments.size());
	
	return(isValid);
}





//============================================================================
//		NStringFormatter::GetArgValue : Get an argument value.
//----------------------------------------------------------------------------
NStringUTF8 NStringFormatter::GetArgValue(NFormatContext &theContext, NIndex theIndex, const NStringUTF8 &theFormat)
{	NStringUTF8		theResult;



	// Validate our parameters
	NN_ASSERT(IsValidArg(theContext, theIndex));
	NN_ASSERT(!theFormat.IsEmpty());
	
	
	
	// Get the value
	theResult = theContext.theArguments.at(theIndex)->GetValue(theFormat);
	return(theResult);
}





//============================================================================
//		NStringFormatter::ToDigit : Convert a numeric character.
//----------------------------------------------------------------------------
NIndex NStringFormatter::ToDigit(char c)
{


	// Validate our parameters
	NN_ASSERT(c >= '0' && c <= '9');
	
	
	
	// Convert to an integer
	return((NIndex) (c - '0'));
}





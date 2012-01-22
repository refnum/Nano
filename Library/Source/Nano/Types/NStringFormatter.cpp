/*	NAME:
		NStringFormatter.cpp

	DESCRIPTION:
		String formatter.

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
#include "NString.h"
#include "NStringFormatter.h"





//============================================================================
//		Windows
//----------------------------------------------------------------------------
// Work around Visual Studio warning bug:
//
//		http://connect.microsoft.com/VisualStudio/feedback/details/522094/warning-c4717-recursive-on-all-control-paths-error-erroneous
//
// Warning incorrectly emitted for NFormatFunctor::NFormatFunctor.
#if NN_TARGET_WINDOWS
	#pragma warning(disable: 4717)
#endif





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Literals
//
// Must be #defines to ensure formatting can be perfored during static initialization.
#define kFormatTypesInteger												"diouxXcC"
#define kFormatTypesDouble												"fFeEgGaA"
#define kFormatTypesPointer												"sSpn"
#define kFormatTypesCustom												"@"
#define kFormatTypes													kFormatTypesInteger		\
																		kFormatTypesDouble		\
																		kFormatTypesPointer		\
																		kFormatTypesCustom

static const UInt32 kFormatBufferSize									= 256;





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
//		NFormatArgument::NFormatArgument : Constructor.
//----------------------------------------------------------------------------
NFormatArgument::NFormatArgument(unsigned int theValue)				{	mGetValue = BindSelf(NFormatArgument::GetValueULong,		_1, (unsigned long) theValue);	}
NFormatArgument::NFormatArgument(  signed int theValue)				{	mGetValue = BindSelf(NFormatArgument::GetValueSLong,		_1, (  signed long) theValue);	}

NFormatArgument::NFormatArgument(unsigned long theValue)			{	mGetValue = BindSelf(NFormatArgument::GetValueULong,		_1, theValue);	}
NFormatArgument::NFormatArgument(  signed long theValue)			{	mGetValue = BindSelf(NFormatArgument::GetValueSLong,		_1, theValue);	}

NFormatArgument::NFormatArgument(unsigned long long theValue)		{	mGetValue = BindSelf(NFormatArgument::GetValueULongLong,	_1, theValue);	}
NFormatArgument::NFormatArgument(  signed long long theValue)		{	mGetValue = BindSelf(NFormatArgument::GetValueSLongLong,	_1, theValue);	}

NFormatArgument::NFormatArgument(double         theValue)			{	mGetValue = BindSelf(NFormatArgument::GetValueDouble,	_1, theValue);	}
NFormatArgument::NFormatArgument(const void    *theValue)			{	mGetValue = BindSelf(NFormatArgument::GetValuePointer,	_1, theValue);	}
NFormatArgument::NFormatArgument(const char    *theValue)			{	mGetValue = BindSelf(NFormatArgument::GetValueCharPtr,	_1, theValue);	}
NFormatArgument::NFormatArgument(const NString &theValue)			{	mGetValue = BindSelf(NFormatArgument::GetValueString,	_1, theValue);	}

NFormatArgument::NFormatArgument(const NFormatFunctor &getValue)	{	mGetValue = getValue;	}





//============================================================================
//		NFormatArgument::NFormatArgument : Constructor.
//----------------------------------------------------------------------------
NFormatArgument::NFormatArgument(void)
{
}





//============================================================================
//		NFormatArgument::~NFormatArgument : Destructor.
//----------------------------------------------------------------------------
NFormatArgument::~NFormatArgument(void)
{
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
NString NFormatArgument::GetValue(const NString &theFormat) const
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Get the value
	return(mGetValue(theFormat));
}





//============================================================================
//		NFormatArgument::GetValueULong : Get an unsigned long value.
//----------------------------------------------------------------------------
#pragma mark -
NString NFormatArgument::GetValueULong(const NString &theFormat, unsigned long theValue)
{


	// Validate our parameters
	//
	// See NStringFormatter::Format declaration.
	if (NN_DEBUG && theFormat.Contains("ll"))
		NN_LOG_FORMATTER("Formatting a 32-bit value with 64-bit length modifier, use 'l' in '%s'", theFormat.GetUTF8());



	// Get the value
	return(GetValue(theFormat, kFormatTypesInteger, theValue));
}





//============================================================================
//		NFormatArgument::GetValueSLong : Get a signed long value.
//----------------------------------------------------------------------------
NString NFormatArgument::GetValueSLong(const NString &theFormat, signed long theValue)
{


	// Validate our parameters
	//
	// See NStringFormatter::Format declaration.
	if (NN_DEBUG && theFormat.Contains("ll"))
		NN_LOG_FORMATTER("Formatting a 32-bit value with 64-bit length modifier, use 'l' in '%s'", theFormat.GetUTF8());



	// Get the value
	return(GetValue(theFormat, kFormatTypesInteger, theValue));
}





//============================================================================
//		NFormatArgument::GetValueULongLong : Get an unsigned long long value.
//----------------------------------------------------------------------------
NString NFormatArgument::GetValueULongLong(const NString &theFormat, unsigned long long theValue)
{


	// Validate our parameters
	//
	// See NStringFormatter::Format declaration.
	if (NN_DEBUG && !theFormat.Contains("ll"))
		NN_LOG_FORMATTER("Formatting a 64-bit value with 32-bit length modifier, use 'll' in '%s'", theFormat.GetUTF8());



	// Get the value
	return(GetValue(theFormat, kFormatTypesInteger, theValue));
}





//============================================================================
//		NFormatArgument::GetValueSLongLong : Get a signed long long value.
//----------------------------------------------------------------------------
NString NFormatArgument::GetValueSLongLong(const NString &theFormat, signed long long theValue)
{


	// Validate our parameters
	//
	// See NStringFormatter::Format declaration.
	if (NN_DEBUG && !theFormat.Contains("ll"))
		NN_LOG_FORMATTER("Formatting a 64-bit value with 32-bit length modifier, use 'll' in '%s'", theFormat.GetUTF8());



	// Get the value
	return(GetValue(theFormat, kFormatTypesInteger, theValue));
}





//============================================================================
//		NFormatArgument::GetValueDouble : Get a double value.
//----------------------------------------------------------------------------
NString NFormatArgument::GetValueDouble(const NString &theFormat, double theValue)
{


	// Get the value
	return(GetValue(theFormat, kFormatTypesDouble, theValue));
}





//============================================================================
//		NFormatArgument::GetValuePointer : Get a pointer value.
//----------------------------------------------------------------------------
NString NFormatArgument::GetValuePointer(const NString &theFormat, const void *theValue)
{


	// Get the value
	return(GetValue(theFormat, kFormatTypesPointer, theValue));
}





//============================================================================
//		NFormatArgument::GetValueCharPtr : Get a char* value.
//----------------------------------------------------------------------------
NString NFormatArgument::GetValueCharPtr(const NString &theFormat, const char *theValue)
{


	// Validate our parameters
	NN_ASSERT(theFormat == "%s");
	NN_UNUSED(theFormat);



	// Get the value
	//
	// Despite our parameter being const, we must use the copy-data constructor
	// since we don't know if the supplied string is really a literal string.
	return(NString(theValue, kNStringLength));
}





//============================================================================
//		NFormatArgument::GetValueString : Get a string value.
//----------------------------------------------------------------------------
NString NFormatArgument::GetValueString(const NString &theFormat, const NString &theValue)
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
NString NFormatArgument::GetValue(const NString &theFormat, const char *validTypes, ...)
{	char			stackBuffer[kFormatBufferSize];
	char			*heapBuffer;
	NString			theResult;
	NIndex			numChars;
	va_list			argList;



	// Check our parameters
	//
	// Although sprintf will convert some types automatically, we complain if we see
	// this kind of implicit conversion since the caller should use the correct type.
	if (NN_DEBUG && !IsValidType(theFormat, validTypes))
		NN_LOG("Invalid type used for '%s'", theFormat.GetUTF8());



	// Prepare to format
	//
	// va_list's behaviour is undefined if it is passed a reference rather than
	// a POD, and so validTypes must always be a char* or similar POD type.
	NN_ASSERT( typeid(validTypes) == typeid(const char *));
	va_start(argList, validTypes);



	// Format to the stack
	numChars = vsnprintf(stackBuffer, sizeof(stackBuffer), theFormat.GetUTF8(), argList);
	if (numChars < (NIndex) sizeof(stackBuffer))
		theResult = NString(stackBuffer, numChars);


	// Format to the heap
	else
		{
		numChars   = numChars + 1;
		heapBuffer = (char *) malloc(numChars);
		if (heapBuffer != NULL)
			{
			numChars  = vsnprintf(heapBuffer, numChars, theFormat.GetUTF8(), argList);
			theResult = NString(heapBuffer, numChars);

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
bool NFormatArgument::IsValidType(const NString &theFormat, const char *validTypes)
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
NString NStringFormatter::Format(const NString &theFormat, NN_FORMAT_ARGS_PARAM)
{	NFormatArgumentList		theArguments;



	// Format the arguments
	NN_FORMAT_ARGS_COLLECT(theArguments);
	
	return(Format(theFormat, theArguments));
}





//============================================================================
//		NStringFormatter::Format : Format an argument list.
//----------------------------------------------------------------------------
#pragma mark -
NString NStringFormatter::Format(const NString &theFormat, const NFormatArgumentList &theArguments)
{	const char			*textUTF8, *tokenStart, *tokenEnd;
	NString				theResult, theToken;
	NFormatContext		theContext;
	bool				areDone;



	// Get the state we need
	theContext.theArguments = theArguments;
	theContext.gotError     = false;
	theContext.argIndex     = 0;

	textUTF8 = theFormat.GetUTF8();
	areDone  = false;



	// Format the string
	while (!areDone)
		{
		// Find the next format token
		tokenStart = strstr(textUTF8, "%");
		if (tokenStart == NULL)
			break;



		// Append the text to this point
		theResult += NString(textUTF8, tokenStart - textUTF8);
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
				tokenEnd = strpbrk(textUTF8, kFormatTypes);
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
					theToken   = NString(tokenStart, tokenEnd - tokenStart + 1);
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
		theResult += NString(textUTF8);

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
NString NStringFormatter::ParseToken(NFormatContext &theContext, const NString &theToken)
{	NString			theResult, finalToken;
	NRange			theRange, indexRange;
	NIndex			theIndex;



	// Validate our parameters
	NN_ASSERT(theToken.GetSize() >= 2);



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

		theResult = NString(".") + GetArgValue(theContext, theIndex, "%ld");
		if (theResult.Contains("-"))
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
NString NStringFormatter::ParseFailed(NFormatContext &theContext, const NString &theToken, const NString &theError)
{


	// Compiler warnings
	NN_UNUSED(theToken);
	NN_UNUSED(theError);



	// Handle failure
	NN_LOG("%s: '%s'", theError.GetUTF8(), theToken.GetUTF8());
	theContext.gotError = true;

	return("");
}





//============================================================================
//		NStringFormatter::ParseIndexRef : Parse an index reference.
//----------------------------------------------------------------------------
NIndex NStringFormatter::ParseIndexRef(NFormatContext &theContext, const NString &theToken, const NString &thePrefix, NRange &theRange)
{	NRange			indexRange;
	const char		*textPtr;
	NIndex			theIndex;
	char			ch;



	// Find the reference
	theRange = theToken.Find(thePrefix);
	if (theRange.IsEmpty())
		return(kNIndexNone);



	// Get the state we need
	indexRange = NRange(theRange.GetLocation() + thePrefix.GetSize(), 1);
	if (indexRange.GetLast() >= (NIndex) theToken.GetSize())
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
	textPtr = theToken.GetUTF8();
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
NString NStringFormatter::GetArgValue(NFormatContext &theContext, NIndex theIndex, const NString &theFormat)
{	NString		theResult;



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





/*	NAME:
		NFormatter.cpp

	DESCRIPTION:
		String formatter.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFormatter.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kFormatTypesInteger								= "diouxXcC";
static const NString kFormatTypesDouble									= "fFeEgGaA";
static const NString kFormatTypesPointer								= "sSpn";

static const NString kFormatTypes										= kFormatTypesInteger + kFormatTypesDouble + kFormatTypesPointer;
static const UInt32  kFormatBufferSize									= 256;





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
#define DECLARE_ARGS					NFormatArgumentList		 _argList
#define FORMAT_ARGS						return(Format(theFormat, _argList))

#define PUSH_ARGS1										_argList.push_back(arg1);
#define PUSH_ARGS2						PUSH_ARGS1;		_argList.push_back(arg2);
#define PUSH_ARGS3						PUSH_ARGS2;		_argList.push_back(arg3);
#define PUSH_ARGS4						PUSH_ARGS3;		_argList.push_back(arg4);
#define PUSH_ARGS5						PUSH_ARGS4;		_argList.push_back(arg5);
#define PUSH_ARGS6						PUSH_ARGS5;		_argList.push_back(arg6);
#define PUSH_ARGS7						PUSH_ARGS6;		_argList.push_back(arg7);
#define PUSH_ARGS8						PUSH_ARGS7;		_argList.push_back(arg8);
#define PUSH_ARGS9						PUSH_ARGS8;		_argList.push_back(arg9);
#define PUSH_ARGS10						PUSH_ARGS9;		_argList.push_back(arg10);
#define PUSH_ARGS11						PUSH_ARGS10;	_argList.push_back(arg11);
#define PUSH_ARGS12						PUSH_ARGS11;	_argList.push_back(arg12);
#define PUSH_ARGS13						PUSH_ARGS12;	_argList.push_back(arg13);
#define PUSH_ARGS14						PUSH_ARGS13;	_argList.push_back(arg14);
#define PUSH_ARGS15						PUSH_ARGS14;	_argList.push_back(arg15);





//============================================================================
//		NFormatArgument::NFormatArgument : Constructor.
//----------------------------------------------------------------------------
NFormatArgument::NFormatArgument(unsigned int theValue)				{	mGetValue = BindSelf(NFormatArgument::GetValueUInt,			_1, theValue);	}
NFormatArgument::NFormatArgument(  signed int theValue)				{	mGetValue = BindSelf(NFormatArgument::GetValueSInt,			_1, theValue);	}

NFormatArgument::NFormatArgument(unsigned long theValue)			{	mGetValue = BindSelf(NFormatArgument::GetValueULong,		_1, theValue);	}
NFormatArgument::NFormatArgument(  signed long theValue)			{	mGetValue = BindSelf(NFormatArgument::GetValueULong,		_1, theValue);	}

NFormatArgument::NFormatArgument(unsigned long long theValue)		{	mGetValue = BindSelf(NFormatArgument::GetValueULongLong,	_1, theValue);	}
NFormatArgument::NFormatArgument(  signed long long theValue)		{	mGetValue = BindSelf(NFormatArgument::GetValueSLongLong,	_1, theValue);	}

NFormatArgument::NFormatArgument(double                theValue)	{	mGetValue = BindSelf(NFormatArgument::GetValueDouble,		_1, theValue);	}
NFormatArgument::NFormatArgument(const void           *theValue)	{	mGetValue = BindSelf(NFormatArgument::GetValuePointer,		_1, theValue);	}
NFormatArgument::NFormatArgument(const NFormatFunctor &getValue)	{	mGetValue = getValue;														}





//============================================================================
//		NFormatArgument::~NFormatArgument : Destructor.
//----------------------------------------------------------------------------
NFormatArgument::~NFormatArgument(void)
{
}





//============================================================================
//		NFormatArgument::GetValue : Get the value.
//----------------------------------------------------------------------------
NString NFormatArgument::GetValue(const NString &theFormat) const
{


	// Get the value
	return(mGetValue(theFormat));
}





//============================================================================
//		NFormatArgument::GetValueUInt : Get an unsigned int value.
//----------------------------------------------------------------------------
#pragma mark -
NString NFormatArgument::GetValueUInt(const NString &theFormat, unsigned int theValue)
{


	// Get the value
	return(GetValue(theFormat, kFormatTypesInteger, theValue));
}





//============================================================================
//		NFormatArgument::GetValueSInt : Get a signed int value.
//----------------------------------------------------------------------------
NString NFormatArgument::GetValueSInt(const NString &theFormat, signed int theValue)
{


	// Get the value
	return(GetValue(theFormat, kFormatTypesInteger, theValue));
}





//============================================================================
//		NFormatArgument::GetValueULong : Get an unsigned long value.
//----------------------------------------------------------------------------
NString NFormatArgument::GetValueULong(const NString &theFormat, unsigned long theValue)
{


	// Get the value
	return(GetValue(theFormat, kFormatTypesInteger, theValue));
}





//============================================================================
//		NFormatArgument::GetValueSLong : Get a signed long value.
//----------------------------------------------------------------------------
NString NFormatArgument::GetValueSLong(const NString &theFormat, signed long theValue)
{


	// Get the value
	return(GetValue(theFormat, kFormatTypesInteger, theValue));
}





//============================================================================
//		NFormatArgument::GetValueULongLong : Get an unsigned long long value.
//----------------------------------------------------------------------------
NString NFormatArgument::GetValueULongLong(const NString &theFormat, unsigned long long theValue)
{


	// Get the value
	return(GetValue(theFormat, kFormatTypesInteger, theValue));
}





//============================================================================
//		NFormatArgument::GetValueSLongLong : Get a signed long long value.
//----------------------------------------------------------------------------
NString NFormatArgument::GetValueSLongLong(const NString &theFormat, signed long long theValue)
{


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
//		NFormatArgument::GetValue : Get a formatted value.
//----------------------------------------------------------------------------
NString NFormatArgument::GetValue(const NString &theFormat, const NString &validTypes, ...)
{	char		stackBuffer[kFormatBufferSize];
	char		*heapBuffer;
	NString		theResult;
	NIndex		numChars;
	va_list		argList;



	// Validate our parameters
	NN_ASSERT(IsValidType(theFormat, validTypes));



	// Prepare to format
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
bool NFormatArgument::IsValidType(const NString &theFormat, const NString &validTypes)
{	const char		*textUTF8, *theType;



	// Get the state we need
	textUTF8  = theFormat.GetUTF8();
	textUTF8 += strlen(textUTF8) - 1;



	// Find the type
	theType = strpbrk(textUTF8, validTypes.GetUTF8());

	return(theType != NULL);
}





//============================================================================
//		NFormatter::NFormatter : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
NFormatter::NFormatter(void)
{
}





//============================================================================
//		NFormatter::~NFormatter : Destructor.
//----------------------------------------------------------------------------
NFormatter::~NFormatter(void)
{
}





//============================================================================
//		NFormatter::Format : Format an argument list.
//----------------------------------------------------------------------------
NString NFormatter::Format(const NString &theFormat)					{	DECLARE_ARGS;						FORMAT_ARGS;	}
NString NFormatter::Format(const NString &theFormat, FORMAT_ARG1)		{	DECLARE_ARGS;		PUSH_ARGS1;		FORMAT_ARGS;	}
NString NFormatter::Format(const NString &theFormat, FORMAT_ARG2)		{	DECLARE_ARGS;		PUSH_ARGS2;		FORMAT_ARGS;	}
NString NFormatter::Format(const NString &theFormat, FORMAT_ARG3)		{	DECLARE_ARGS;		PUSH_ARGS3;		FORMAT_ARGS;	}
NString NFormatter::Format(const NString &theFormat, FORMAT_ARG4)		{	DECLARE_ARGS;		PUSH_ARGS4;		FORMAT_ARGS;	}
NString NFormatter::Format(const NString &theFormat, FORMAT_ARG5)		{	DECLARE_ARGS;		PUSH_ARGS5;		FORMAT_ARGS;	}
NString NFormatter::Format(const NString &theFormat, FORMAT_ARG6)		{	DECLARE_ARGS;		PUSH_ARGS6;		FORMAT_ARGS;	}
NString NFormatter::Format(const NString &theFormat, FORMAT_ARG7)		{	DECLARE_ARGS;		PUSH_ARGS7;		FORMAT_ARGS;	}
NString NFormatter::Format(const NString &theFormat, FORMAT_ARG8)		{	DECLARE_ARGS;		PUSH_ARGS8;		FORMAT_ARGS;	}
NString NFormatter::Format(const NString &theFormat, FORMAT_ARG9)		{	DECLARE_ARGS;		PUSH_ARGS9;		FORMAT_ARGS;	}
NString NFormatter::Format(const NString &theFormat, FORMAT_ARG10)		{	DECLARE_ARGS;		PUSH_ARGS10;	FORMAT_ARGS;	}
NString NFormatter::Format(const NString &theFormat, FORMAT_ARG11)		{	DECLARE_ARGS;		PUSH_ARGS11;	FORMAT_ARGS;	}
NString NFormatter::Format(const NString &theFormat, FORMAT_ARG12)		{	DECLARE_ARGS;		PUSH_ARGS12;	FORMAT_ARGS;	}
NString NFormatter::Format(const NString &theFormat, FORMAT_ARG13)		{	DECLARE_ARGS;		PUSH_ARGS13;	FORMAT_ARGS;	}
NString NFormatter::Format(const NString &theFormat, FORMAT_ARG14)		{	DECLARE_ARGS;		PUSH_ARGS14;	FORMAT_ARGS;	}
NString NFormatter::Format(const NString &theFormat, FORMAT_ARG15)		{	DECLARE_ARGS;		PUSH_ARGS15;	FORMAT_ARGS;	}





//============================================================================
//		NFormatter::Format : Format an argument list.
//----------------------------------------------------------------------------
#pragma mark -
NString NFormatter::Format(const NString &theFormat, const NFormatArgumentList &theArguments)
{	const char		*textUTF8, *typesUTF8, *formatStart, *formatEnd;
	NString			theResult, argFormat;
	UInt32			numFound, numUsed;
	bool			areDone;



	// Get the state we need
	textUTF8  = theFormat.GetUTF8();
	typesUTF8 = kFormatTypes.GetUTF8();
	
	areDone  = false;
	numFound = 0;
	numUsed  = 0;



	// Format the string
	while (!areDone)
		{
		// Find the next format specifier
		formatStart = strstr(textUTF8, "%");
		if (formatStart == NULL)
			break;



		// Append the text to this point
		theResult += NString(textUTF8, formatStart - textUTF8);
		textUTF8   = formatStart + 1;



		// Process the format specifier
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
				formatEnd = strpbrk(textUTF8, typesUTF8);
				if (formatEnd == NULL)
					{
					NN_LOG("Missing type: found '%%' without type in '%s'", theFormat.GetUTF8());
					theResult += "%";
					areDone    = true;
					}

				// Process the argument
				else
					{
					// Append the argument
					if (numUsed < theArguments.size())
						{
						argFormat  = NString(formatStart, formatEnd - formatStart + 1);
						theResult += theArguments[numUsed].GetValue(argFormat);
					
						numUsed++;
						}


					// Advance the text
					numFound += 1;
					textUTF8  = formatEnd + 1;
					}
				break;
			}
		}



	// Append any remaining text
	if (*textUTF8 != 0x00)
		theResult += NString(textUTF8);



	// Log errors
	if (numFound != theArguments.size())
		NN_LOG("Wrong argument count: '%s' references %d arguments, but %d supplied", theFormat.GetUTF8(), numFound, theArguments.size());

	return(theResult);
}





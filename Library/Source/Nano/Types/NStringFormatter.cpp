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
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFunctor.h"
#include "NStringFormatter.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NStringUTF8 kFormatTypesInteger							= "diouxXcC";
static const NStringUTF8 kFormatTypesDouble								= "fFeEgGaA";
static const NStringUTF8 kFormatTypesPointer							= "sSpn";
static const NStringUTF8 kFormatTypesCustom								= "@";

static const UInt32      kFormatBufferSize								= 256;
static const NStringUTF8 kFormatTypes									= kFormatTypesInteger +
																		  kFormatTypesDouble  +
																		  kFormatTypesPointer +
																		  kFormatTypesCustom;





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
//		NFormatArgument::NFormatArgument : Constructor.
//----------------------------------------------------------------------------
NFormatArgument::NFormatArgument(unsigned int theValue)				{	mGetValue = new NFormatFunctor(BindSelf(NFormatArgument::GetValueUInt,		_1, theValue));	}
NFormatArgument::NFormatArgument(  signed int theValue)				{	mGetValue = new NFormatFunctor(BindSelf(NFormatArgument::GetValueSInt,		_1, theValue));	}

NFormatArgument::NFormatArgument(unsigned long theValue)			{	mGetValue = new NFormatFunctor(BindSelf(NFormatArgument::GetValueULong,		_1, theValue));	}
NFormatArgument::NFormatArgument(  signed long theValue)			{	mGetValue = new NFormatFunctor(BindSelf(NFormatArgument::GetValueULong,		_1, theValue));	}

NFormatArgument::NFormatArgument(unsigned long long theValue)		{	mGetValue = new NFormatFunctor(BindSelf(NFormatArgument::GetValueULongLong,	_1, theValue));	}
NFormatArgument::NFormatArgument(  signed long long theValue)		{	mGetValue = new NFormatFunctor(BindSelf(NFormatArgument::GetValueSLongLong,	_1, theValue));	}

NFormatArgument::NFormatArgument(double				 theValue)		{	mGetValue = new NFormatFunctor(BindSelf(NFormatArgument::GetValueDouble,	_1, theValue));	}
NFormatArgument::NFormatArgument(const void			*theValue)		{	mGetValue = new NFormatFunctor(BindSelf(NFormatArgument::GetValuePointer,	_1, theValue));	}
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
NStringUTF8 NFormatArgument::GetValue(const NStringUTF8 &theFormat, const NStringUTF8 &validTypes, ...)
{	char			stackBuffer[kFormatBufferSize];
	char			*heapBuffer;
	NStringUTF8		theResult;
	NIndex			numChars;
	va_list			argList;



	// Validate our parameters
	NN_ASSERT(IsValidType(theFormat, validTypes));



	// Prepare to format
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
bool NFormatArgument::IsValidType(const NStringUTF8 &theFormat, const NStringUTF8 &validTypes)
{	const char		*formatUTF8, *theType;



	// Get the state we need
	formatUTF8  = theFormat.GetUTF8();
	formatUTF8 += strlen(formatUTF8) - 1;



	// Find the type
	theType = strpbrk(formatUTF8, validTypes.GetUTF8());

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
{	const char		*textUTF8, *typesUTF8, *formatStart, *formatEnd;
	NStringUTF8		theResult, argFormat;
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
		theResult += NStringUTF8(textUTF8, formatStart - textUTF8);
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
					NN_LOG_FORMATTER("Missing type: found '%%' without type in '%s'", theFormat.GetUTF8());
					theResult += "%";
					areDone    = true;
					}

				// Process the argument
				else
					{
					// Append the argument
					if (numUsed < theArguments.size())
						{
						argFormat  = NStringUTF8(formatStart, formatEnd - formatStart + 1);
						theResult += theArguments.at(numUsed)->GetValue(argFormat);
					
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
		theResult += NStringUTF8(textUTF8);



	// Log errors
	if (numFound != theArguments.size())
		NN_LOG_FORMATTER("Wrong argument count: '%s' references %d arguments, but %d supplied", theFormat.GetUTF8(), numFound, theArguments.size());

	return(theResult);
}





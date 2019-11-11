/*	NAME:
		NanoTypes.h

	DESCRIPTION:
		Nano data types.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NANO_TYPES_HDR
#define NANO_TYPES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#if defined(__cplusplus)
	#include <algorithm>
	#include <iterator>
	#include <vector>
	#include <list>
	#include <map>
#endif

#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <time.h>





//============================================================================
//		Standard Types
//----------------------------------------------------------------------------
// Integer
//
// MSVC does not provide stdint.h until VS 2010, so we provide a fallback.
//
#if NN_TARGET_WINDOWS

	#if NN_TARGET_COMPILER_MSC >= 160000000
		#include <stdint.h>
	#else
		typedef unsigned char										uint8_t;
		typedef unsigned short										uint16_t;
		typedef unsigned int										uint32_t;
		typedef unsigned long long									uint64_t;

		typedef signed char											int8_t;
		typedef signed short										int16_t;
		typedef signed int											int32_t;
		typedef signed long long									int64_t;

		#if NN_ARCH_32
			typedef uint32_t										uintptr_t;
		#else
			typedef uint64_t										uintptr_t;
		#endif
	#endif

#else
	#include <stdint.h>
#endif


// Floating point
typedef float														float32_t;
typedef double														float64_t;


// Unicode
//
// To ensure NString::GetUTF16() is equivalent to a LPCWSTR on Windows, we define
// the UTF16 char type for this platform to match its native UTF16 type (wchar_t).
typedef char														utf8_t;
typedef uint32_t													utf32_t;

#if NN_TARGET_WINDOWS
	typedef wchar_t													utf16_t;
#else
	typedef uint16_t												utf16_t;
#endif





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Misc
//
// These types have no defined size, and may change size between releases
// or architectures. They are intended to be optimal for most cases.
typedef int32_t														NIndex;
typedef int32_t														NStatus;
typedef uint32_t													NHashCode;
typedef uint32_t													NBitfield;
typedef float64_t													NRadians;
typedef float64_t													NDegrees;
typedef float64_t													NTime;


// Lists
#if defined(__cplusplus)
typedef std::vector<uint8_t>										UInt8List;
typedef UInt8List::iterator											UInt8ListIterator;
typedef UInt8List::const_iterator									UInt8ListConstIterator;

typedef std::vector<uint16_t>										UInt16List;
typedef UInt16List::iterator										UInt16ListIterator;
typedef UInt16List::const_iterator									UInt16ListConstIterator;

typedef std::vector<uint32_t>										UInt32List;
typedef UInt32List::iterator										UInt32ListIterator;
typedef UInt32List::const_iterator									UInt32ListConstIterator;

typedef std::vector<uint64_t>										UInt64List;
typedef UInt64List::iterator										UInt64ListIterator;
typedef UInt64List::const_iterator									UInt64ListConstIterator;


typedef std::vector<int8_t>											Int8List;
typedef Int8List::iterator											Int8ListIterator;
typedef Int8List::const_iterator									Int8ListConstIterator;

typedef std::vector<int16_t>										Int16List;
typedef Int16List::iterator											Int16ListIterator;
typedef Int16List::const_iterator									Int16ListConstIterator;

typedef std::vector<int32_t>										Int32List;
typedef Int32List::iterator											Int32ListIterator;
typedef Int32List::const_iterator									Int32ListConstIterator;

typedef std::vector<int64_t>										Int64List;
typedef Int64List::iterator											Int64ListIterator;
typedef Int64List::const_iterator									Int64ListConstIterator;


typedef std::vector<float32_t>										Float32List;
typedef Float32List::iterator										Float32ListIterator;
typedef Float32List::const_iterator									Float32ListConstIterator;

typedef std::vector<float64_t>										Float64List;
typedef Float64List::iterator										Float64ListIterator;
typedef Float64List::const_iterator									Float64ListConstIterator;


typedef std::vector<utf8_t>											UTF8List;
typedef UTF8List::iterator											UTF8ListIterator;
typedef UTF8List::const_iterator									UTF8ListConstIterator;

typedef std::vector<utf16_t>										UTF16List;
typedef UTF16List::iterator											UTF16ListIterator;
typedef UTF16List::const_iterator									UTF16ListConstIterator;

typedef std::vector<utf32_t>										UTF32List;
typedef UTF32List::iterator											UTF32ListIterator;
typedef UTF32List::const_iterator									UTF32ListConstIterator;


typedef std::vector<NIndex>											NIndexList;
typedef NIndexList::iterator										NIndexListIterator;
typedef NIndexList::const_iterator									NIndexListConstIterator;

typedef std::map<NIndex, NIndex>									NIndexMap;
typedef NIndexMap::iterator											NIndexMapIterator;
typedef NIndexMap::const_iterator									NIndexMapConstIterator;

typedef std::vector<NHashCode>										NHashCodeList;
typedef NHashCodeList::iterator										NHashCodeListIterator;
typedef NHashCodeList::const_iterator								NHashCodeListConstIterator;

typedef std::vector<NBitfield>										NBitfieldList;
typedef NBitfieldList::iterator										NBitfieldListIterator;
typedef NBitfieldList::const_iterator								NBitfieldListConstIterator;
#endif // __cplusplus





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Limits
static const uint8_t  kUInt8Min										= 0;
static const uint8_t  kUInt8Max										= UCHAR_MAX;
static const uint16_t kUInt16Min									= 0;
static const uint16_t kUInt16Max									= USHRT_MAX;
static const uint32_t kUInt32Min									= 0;
static const uint32_t kUInt32Max									= UINT_MAX;
static const uint64_t kUInt64Min									= 0;
static const uint64_t kUInt64Max									= ULLONG_MAX;

static const int8_t  kInt8Min										= SCHAR_MIN;
static const int8_t  kInt8Max										= SCHAR_MAX;
static const int16_t kInt16Min										= SHRT_MIN;
static const int16_t kInt16Max										= SHRT_MAX;
static const int32_t kInt32Min										= INT_MIN;
static const int32_t kInt32Max										= INT_MAX;
static const int64_t kInt64Min										= LLONG_MIN;
static const int64_t kInt64Max										= LLONG_MAX;

static const float32_t kFloat32Min									= FLT_MIN;
static const float32_t kFloat32Max									= FLT_MAX;
static const float64_t kFloat64Min									= DBL_MIN;
static const float64_t kFloat64Max									= DBL_MAX;

static const NIndex kNIndexMin										= INT_MIN;
static const NIndex kNIndexMax										= INT_MAX;


// Misc
static const NIndex    kNIndexNone									= -1;
static const NHashCode kNHashCodeNone								= 0;


// Memory
static const NIndex kNKilobyte										= 1024;
static const NIndex kNMegabyte										= 1024 * 1024;
static const NIndex kNGigabyte										= 1024 * 1024 * 1024;


// Times
static const NTime kNTimeNone										= 0.0;
static const NTime kNTimeNanosecond									= 1.0 / 1000000000.0;
static const NTime kNTimeMicrosecond								= 1.0 / 1000000.0;
static const NTime kNTimeMillisecond								= 1.0 / 1000.0;
static const NTime kNTimeSecond										= 1.0;
static const NTime kNTimeMinute										= 60.0;
static const NTime kNTimeHour										= 60.0 * 60.0;
static const NTime kNTimeDay										= 60.0 * 60.0 * 24.0;
static const NTime kNTimeWeek										= 60.0 * 60.0 * 24.0 * 7.0;
static const NTime kNTimeMonthish									= 60.0 * 60.0 * 24.0 * 7.0 * 30.0;
static const NTime kNTimeYearish									= 60.0 * 60.0 * 24.0 * 7.0 * 30.0 * 365.0;
static const NTime kNTimeForever									= -1.0;


// Status
//
// Other than kNoErr, NStatus values may change between releases.
static const NStatus kNoErr											= 0;
static const NStatus kNErrBusy										= -1;
static const NStatus kNErrCancelled									= -2;
static const NStatus kNErrChecksum									= -3;
static const NStatus kNErrDiskFull									= -4;
static const NStatus kNErrDuplicate									= -5;
static const NStatus kNErrExhaustedDst								= -6;
static const NStatus kNErrExhaustedSrc								= -7;
static const NStatus kNErrInternal									= -8;
static const NStatus kNErrLocked									= -9;
static const NStatus kNErrMalformed									= -10;
static const NStatus kNErrMemory									= -11;
static const NStatus kNErrNotHandled								= -12;
static const NStatus kNErrNotFound									= -13;
static const NStatus kNErrNotSupported								= -14;
static const NStatus kNErrParam										= -15;
static const NStatus kNErrPermission								= -16;
static const NStatus kNErrTimeout									= -17;
static const NStatus kNErrVersionTooOld								= -18;
static const NStatus kNErrVersionTooNew								= -19;





#endif // NANO_TYPES_HDR





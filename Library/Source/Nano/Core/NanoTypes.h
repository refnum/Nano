/*	NAME:
		NanoTypes.h

	DESCRIPTION:
		Nano data types.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
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
#include <time.h>





//============================================================================
//		Bootstrap
//----------------------------------------------------------------------------
// Baseline
//
// To handle platform-specific requirements for some types (e.g., gcc using
// the LP64 model and VC++ using LLP64), we define a set of meta-types that
// are generally correct.
//
// These types are then adjusted to suit the appropriate system headers for
// each platform, and then used to define the primitive Nano types.
#define NANO_INT8													char
#define NANO_INT16													short
#define NANO_INT32													int
#define NANO_INT64													long long

#define NANO_UTF8													UInt8
#define NANO_UTF16													UInt16
#define NANO_UTF32													UInt32


// Mac/iPhone
#if NN_TARGET_MAC || NN_TARGET_IPHONE
	// UInt32/SInt32
	//
	// MacTypes.h uses a different type for UInt32/SInt32 on 32-bit architectures
	// vs 64-bit, so to avoid a type redeclaration error we must make ours match.
	#undef NANO_INT32

	#if NN_TARGET_ARCH_64
		#define NANO_INT32											int
	#else
		#define NANO_INT32											long
	#endif
#endif


// Windows
#if NN_TARGET_WINDOWS
	// UTF16Char
	//
	// To ensure NString::GetUTF16() is equivalent to a LPCWSTR, we define the
	// UTF16 char type to match the native Win32 UTF16 type (wchar_t).
	#undef  NANO_UTF16
	#define NANO_UTF16												wchar_t
#endif





//============================================================================
//		Core Types
//----------------------------------------------------------------------------
// Primitives
//
// These types are a consistent size on all targets and architectures.
//
// All other types may change their size between releases or targets.
typedef unsigned NANO_INT8											UInt8;
typedef unsigned NANO_INT16											UInt16;
typedef unsigned NANO_INT32											UInt32;
typedef unsigned NANO_INT64											UInt64;

typedef signed NANO_INT8											SInt8;
typedef signed NANO_INT16											SInt16;
typedef signed NANO_INT32											SInt32;
typedef signed NANO_INT64											SInt64;

typedef float														Float32;
typedef double														Float64;

typedef NANO_UTF8													UTF8Char;
typedef NANO_UTF16													UTF16Char;
typedef NANO_UTF32													UTF32Char;


// Misc
typedef uintptr_t													UIntPtr;

typedef SInt32														NIndex;
typedef SInt32														NStatus;
typedef UInt32														NHashCode;
typedef UInt32														NBitfield;
typedef Float64														NRadians;
typedef Float64														NDegrees;
typedef Float64														NTime;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Lists
#if defined(__cplusplus)
typedef std::vector<UInt8>											UInt8List;
typedef UInt8List::iterator											UInt8ListIterator;
typedef UInt8List::const_iterator									UInt8ListConstIterator;

typedef std::vector<UInt16>											UInt16List;
typedef UInt16List::iterator										UInt16ListIterator;
typedef UInt16List::const_iterator									UInt16ListConstIterator;

typedef std::vector<UInt32>											UInt32List;
typedef UInt32List::iterator										UInt32ListIterator;
typedef UInt32List::const_iterator									UInt32ListConstIterator;

typedef std::vector<UInt64>											UInt64List;
typedef UInt64List::iterator										UInt64ListIterator;
typedef UInt64List::const_iterator									UInt64ListConstIterator;


typedef std::vector<SInt8>											SInt8List;
typedef SInt8List::iterator											SInt8ListIterator;
typedef SInt8List::const_iterator									SInt8ListConstIterator;

typedef std::vector<SInt16>											SInt16List;
typedef SInt16List::iterator										SInt16ListIterator;
typedef SInt16List::const_iterator									SInt16ListConstIterator;

typedef std::vector<SInt32>											SInt32List;
typedef SInt32List::iterator										SInt32ListIterator;
typedef SInt32List::const_iterator									SInt32ListConstIterator;

typedef std::vector<SInt64>											SInt64List;
typedef SInt64List::iterator										SInt64ListIterator;
typedef SInt64List::const_iterator									SInt64ListConstIterator;


typedef std::vector<Float32>										Float32List;
typedef Float32List::iterator										Float32ListIterator;
typedef Float32List::const_iterator									Float32ListConstIterator;

typedef std::vector<Float64>										Float64List;
typedef Float64List::iterator										Float64ListIterator;
typedef Float64List::const_iterator									Float64ListConstIterator;


typedef std::vector<NIndex>											NIndexList;
typedef NIndexList::iterator										NIndexListIterator;
typedef NIndexList::const_iterator									NIndexListConstIterator;

typedef std::vector<NHashCode>										NHashCodeList;
typedef NHashCodeList::iterator										NHashCodeListIterator;
typedef NHashCodeList::const_iterator								NHashCodeListConstIterator;
#endif // __cplusplus





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Limits
static const UInt8  kUInt8Min										= 0;
static const UInt8  kUInt8Max										= UCHAR_MAX;
static const UInt16 kUInt16Min										= 0;
static const UInt16 kUInt16Max										= USHRT_MAX;
static const UInt32 kUInt32Min										= 0;
static const UInt32 kUInt32Max										= UINT_MAX;
static const UInt64 kUInt64Min										= 0;
static const UInt64 kUInt64Max										= ULLONG_MAX;

static const SInt8  kSInt8Min										= SCHAR_MIN;
static const SInt8  kSInt8Max										= SCHAR_MAX;
static const SInt16 kSInt16Min										= SHRT_MIN;
static const SInt16 kSInt16Max										= SHRT_MAX;
static const SInt32 kSInt32Min										= INT_MIN;
static const SInt32 kSInt32Max										= INT_MAX;
static const SInt64 kSInt64Min										= LLONG_MIN;
static const SInt64 kSInt64Max										= LLONG_MAX;

static const Float32 kFloat32Min									= FLT_MIN;
static const Float32 kFloat32Max									= FLT_MAX;
static const Float64 kFloat64Min									= DBL_MIN;
static const Float64 kFloat64Max									= DBL_MAX;

static const NIndex kNIndexMin										= INT_MIN;
static const NIndex kNIndexMax										= INT_MAX;


// Misc
static const NIndex    kNIndexNone									= -1;
static const NHashCode kNHashCodeNone								= 0;


// Memory
static const NIndex kKilobyte										= 1024;
static const NIndex kMegabyte										= 1024 * 1024;
static const NIndex kGigabyte										= 1024 * 1024 * 1024;


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





#endif // NANO_TYPES_HDR





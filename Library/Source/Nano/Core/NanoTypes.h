/*	NAME:
		NanoTypes.h

	DESCRIPTION:
		Nano data types.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NANO_TYPES_HDR
#define NANO_TYPES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <algorithm>
#include <iterator>
#include <vector>
#include <list>
#include <map>

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
// gcc uses the LP64 model, however Visual C++ uses the LLP64 model.
//
// We define a set of types that are the correct size in both models, then
// adust them to suit the current platform's system headers.
#define NANO_INT8													char
#define NANO_INT16													short
#define NANO_INT32													int
#define NANO_INT64													long long


// Mac/iPhone
//
// MacTypes.h uses a different type for UInt32/SInt32 for each architecture
// size, so to avoid a type redeclaration error we must define ours to match.
#if NN_TARGET_MAC || NN_TARGET_IPHONE
	#undef NANO_INT32

	#if NN_TARGET_ARCH_64
		#define NANO_INT32											int
	#else
		#define NANO_INT32											long
	#endif
#endif





//============================================================================
//		Types
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

typedef UInt8														UTF8Char;
typedef UInt16														UTF16Char;
typedef UInt32														UTF32Char;


// Misc
typedef SInt32														NIndex;
typedef SInt32														NStatus;
typedef UInt32														NHashCode;
typedef UInt32														NBitfield;
typedef Float64														NRadians;
typedef Float64														NDegrees;
typedef Float64														NTime;

#if NN_TARGET_ARCH_64
typedef UInt64														UIntPtr;
#else
typedef UInt32														UIntPtr;
#endif


// Lists
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

static const NIndex kNIndexMin										= kSInt32Min;
static const NIndex kNIndexMax										= kSInt32Max;


// Misc
static const NIndex    kNIndexNone									= -1;
static const NHashCode kNHashCodeNone								= 0;


// Memory
static const NIndex kKilobyte										= 1024;
static const NIndex kMegabyte										= 1024 * kKilobyte;
static const NIndex kGigabyte										= 1024 * kMegabyte;


// Times
static const NTime kNTimeNone										= 0.0;
static const NTime kNTimeNanosecond									= 1.0 / 1000000000.0;
static const NTime kNTimeMicrosecond								= 1.0 / 1000000.0;
static const NTime kNTimeMillisecond								= 1.0 / 1000.0;
static const NTime kNTimeSecond										= 1.0;
static const NTime kNTimeMinute										= 60.0  * kNTimeSecond;
static const NTime kNTimeHour										= 60.0  * kNTimeMinute;
static const NTime kNTimeDay										= 24.0  * kNTimeHour;
static const NTime kNTimeWeek										=  7.0  * kNTimeDay;
static const NTime kNTimeMonthish									= 30.0  * kNTimeDay;
static const NTime kNTimeYearish									= 365.0 * kNTimeDay;
static const NTime kNTimeForever									= -1.0;


// Status
static const NStatus kNoErr											= 0;
static const NStatus kNErrParam										= -1;
static const NStatus kNErrInternal									= -2;
static const NStatus kNErrBusy										= -3;
static const NStatus kNErrTimeout									= -4;
static const NStatus kNErrCancelled									= -5;
static const NStatus kNErrMalformed									= -6;
static const NStatus kNErrPermission								= -7;
static const NStatus kNErrLocked									= -8;
static const NStatus kNErrDiskFull									= -9;
static const NStatus kNErrMemory									= -10;
static const NStatus kNErrNotFound									= -11;
static const NStatus kNErrNotSupported								= -12;
static const NStatus kNErrExhaustedSrc								= -13;
static const NStatus kNErrExhaustedDst								= -14;





#endif // NANO_TYPES_HDR





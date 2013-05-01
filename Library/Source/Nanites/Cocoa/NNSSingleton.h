/*	NAME:
		NNSSingleton.h

	DESCRIPTION:
		Objective-C singleton.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NNSSINGLETON_HDR
#define NNSSINGLETON_HDR
//============================================================================
//		Macros
//----------------------------------------------------------------------------
// Declare an Objective-C singleton
//
// Example usage:
//
//		@interface MyClass : NSObject {
//			...
//		}
//		...
//		NNSSINGLETON_DECLARE(MyClass);
//		@end
//
//
//		@implementation MyClass
//		NNSSINGLETON_DEFINE(MyClass);
//		...
//		@end
//
// Follows Apple's standard pattern from:
//
//		http://developer.apple.com/documentation/Cocoa/Conceptual/CocoaFundamentals/CocoaObjects/CocoaObjects.html
//
#define NNSSINGLETON_DECLARE(_class)										\
																			\
	+ (_class *) sharedInstance


#define NNSSINGLETON_DEFINE(_class)											\
																			\
	static _class *gSharedInstance = NULL;									\
																			\
	+ (_class *) sharedInstance												\
	{																		\
		@synchronized(self)													\
		{																	\
			if (gSharedInstance == NULL)									\
				[[self alloc] init];										\
		}																	\
																			\
		return(gSharedInstance);											\
	}																		\
																			\
	+ (id) allocWithZone:(NSZone *)zone										\
	{																		\
		@synchronized(self)													\
		{																	\
			if (gSharedInstance == NULL)									\
				{															\
				gSharedInstance = [super allocWithZone:zone];				\
				return(gSharedInstance);									\
				}															\
		}																	\
																			\
		return(NULL);														\
	}																		\
																			\
	- (id) copyWithZone:(NSZone *)zone										\
	{																		\
		(void) zone;														\
																			\
		return(self);														\
	}																		\
																			\
	- (id) retain															\
	{																		\
		return(self);														\
	}																		\
																			\
	- (unsigned) retainCount												\
	{																		\
		return(UINT_MAX);													\
	}																		\
																			\
	- (oneway void) release													\
	{																		\
	}																		\
																			\
	- (id) autorelease														\
	{																		\
		return(self);														\
	}																		\
																			\
	void *kEatLastSemiColonForPedanticWarning ## _class





#endif // NNSSINGLETON_HDR






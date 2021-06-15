/*	NAME:
		NFunction.h

	DESCRIPTION:
		Functor object.

	COPYRIGHT:
		Copyright (c) 2006-2021, refNum Software
		All rights reserved.

		Redistribution and use in source and binary forms, with or without
		modification, are permitted provided that the following conditions
		are met:
		
		1. Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.
		
		2. Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.
		
		3. Neither the name of the copyright holder nor the names of its
		contributors may be used to endorse or promote products derived from
		this software without specific prior written permission.
		
		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
		"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
		LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
		A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
		HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
		DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
		THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
		(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	___________________________________________________________________________
*/
#ifndef NFUNCTION_H
#define NFUNCTION_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include <functional>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Placeholders
//
// Shorter form of std::placeholders.
#define kNArg1                                              std::placeholders::_1
#define kNArg2                                              std::placeholders::_2
#define kNArg3                                              std::placeholders::_3
#define kNArg4                                              std::placeholders::_4
#define kNArg5                                              std::placeholders::_5
#define kNArg6                                              std::placeholders::_6
#define kNArg7                                              std::placeholders::_7
#define kNArg8                                              std::placeholders::_8
#define kNArg9                                              std::placeholders::_9
#define kNArg10                                             std::placeholders::_10





//=============================================================================
//		Macros
//-----------------------------------------------------------------------------
// Bindings
//
// Macros to simplify std::bind:
//
//		NBind						Bind an arbitrary function
//		NBindMethod					Bind a method of an object
//		NBindSelf					Bind a method of the current object
//
#define NBind(_function, ...)                               std::bind(&_function, ##__VA_ARGS__)

#define NBindMethod(_object, _method, ...)                  NBind(_method, _object, ##__VA_ARGS__)

#define NBindSelf(_method, ...)                             NBind(_method, this, ##__VA_ARGS__)



#endif // NFUNCTION_H

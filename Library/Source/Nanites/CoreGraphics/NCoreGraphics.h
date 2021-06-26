/*	NAME:
		NCoreGraphics.h

	DESCRIPTION:
		CoreGraphics support.

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
#ifndef NCORE_GRAPHICS_H
#define NCORE_GRAPHICS_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NCFObject.h"
#include "NPoint.h"
#include "NRectangle.h"
#include "NSize.h"

// System
#include <CoreGraphics/CoreGraphics.h>
#include <vector>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// NCFObject helpers
using NCGDataProvider                                       = NCFObject<CGDataProviderRef>;
using NCGFont                                               = NCFObject<CGFontRef>;


// Containers
using NVectorCGFloat                                        = std::vector<CGFloat>;





//=============================================================================
//		Public Functions
//-----------------------------------------------------------------------------
// Nano to CoreGraphics
CGPoint ToCG(const NPoint& thePoint);
CGSize  ToCG(const NSize& theSize);
CGRect  ToCG(const NRectangle& theRect);


// CoreGraphics to Nano
NPoint     ToNN(const CGPoint& thePoint);
NSize      ToNN(const CGSize& theSize);
NRectangle ToNN(const CGRect& theRect);


// Operators
bool operator==(const CGPoint& value1, const CGPoint& value2);
bool operator==(const CGSize& value1, const CGSize& value2);
bool operator==(const CGRect& value1, const CGRect& value2);

bool operator!=(const CGPoint& value1, const CGPoint& value2);
bool operator!=(const CGSize& value1, const CGSize& value2);
bool operator!=(const CGRect& value1, const CGRect& value2);


// CoreGraphics
CGRect CGRectMake(const CGSize& theSize);
CGRect CGRectMake(CGFloat theWidth, CGFloat theHeight);





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NCoreGraphics.inl"



#endif // NCORE_GRAPHICS_H

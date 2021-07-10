/*	NAME:
		NGeomUtils.h

	DESCRIPTION:
		Geometry utilities.

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
#ifndef NGEOM_UTILS_H
#define NGEOM_UTILS_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NPoint.h"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Clip codes
using NClipFlags                                            = uint8_t;

inline constexpr NClipFlags kNClipNone                      = 0;
inline constexpr NClipFlags kNClipLeft                      = (1 << 0);
inline constexpr NClipFlags kNClipRight                     = (1 << 1);
inline constexpr NClipFlags kNClipBottom                    = (1 << 2);
inline constexpr NClipFlags kNClipTop                       = (1 << 3);


// Placement
enum class NPlacement
{
	Inside,
	Outside,
	Intersects
};





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declarations
class NShape;
class NRectangle;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NGeomUtils
{
public:
	// Get the length of a line
	static float64_t                    GetLength(const NVectorPoint& thePoints);


	// Get a bounding box
	static NRectangle                   GetBounds(const NVectorPoint& thePoints);


	// Get a point on a line
	//
	// Returns the point at the specified offset within the line, where 0.0
	// is the first point in the line and 1.0 the last point in the line.
	static NPoint                       GetPointOnLine(const NVectorPoint& thePoints, float64_t theOffset);


	// Get the closest point to a line
	NPoint                              GetClosestPointOnLine(const NPoint& thePoint,
															  const NPoint& lineStart,
															  const NPoint& lineEnd,
															  float64_t*    lineU = nullptr);


	// Compare a rectangle to a shape
	static NPlacement                   CompareRectangleToShape(const NRectangle& theRectangle,
																const NShape&     theShape);


	// Compare a rectangle to a polygon
	static NPlacement                   CompareRectangleToPolygon(const NRectangle& theRect,
																  size_t            numPoints,
																  const NPoint*     thePoints);


	// Compare a point to a polygon
	static NPlacement                   ComparePointToPolygon(const NPoint& thePoint,
															  size_t        numPoints,
															  const NPoint* thePoints);


	// Clip a polygon
	static NVectorPoint                 ClipPolygonToRectangle(const NVectorPoint& thePolygon,
															   const NRectangle&   theRectangle);


	// Clip a line
	static NPlacement                   ClipLineToRectangle(const NRectangle&   theRect,
															const NVectorPoint& theInput,
															NVectorPoint&       theOutput);


	// Get a Cohen-Sutherland clipping code
	static NClipFlags                   GetClipCode(const NRectangle& theRect, const NPoint& thePoint);


private:
	static void                         ClipPolygonToEdge(const NPoint&       edgeStart,
														  const NPoint&       edgeEnd,
														  const NVectorPoint& theInput,
														  NVectorPoint&       theOutput);

	static bool                         IsInside(const NPoint& edgeStart, const NPoint& edgeEnd, const NPoint& thePoint);

	static NPoint                       GetIntersection(const NPoint& edgeStart,
														const NPoint& edgeEnd,
														const NPoint& lineStart,
														const NPoint& lineEnd);
};



#endif // NGEOM_UTILS_H

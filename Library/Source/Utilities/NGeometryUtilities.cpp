/*	NAME:
		NGeometryUtilities.cpp

	DESCRIPTION:
		Geometry utilities.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NGeometryUtilities.h"





//============================================================================
//		NGeometryUtilities::ConvertPoint : Convert a point.
//----------------------------------------------------------------------------
void NGeometryUtilities::ConvertPoint(const HIPoint &inPoint, Point &outPoint)
{


	// Validate our parameters
	NN_ASSERT(inPoint.x > SHRT_MIN && inPoint.x <= SHRT_MAX);
	NN_ASSERT(inPoint.y > SHRT_MIN && inPoint.y <= SHRT_MAX);



	// Convert the point
	outPoint.h = (SInt16) inPoint.x;
	outPoint.v = (SInt16) inPoint.y;
}





//============================================================================
//		NGeometryUtilities::ConvertPoint : Convert a point.
//----------------------------------------------------------------------------
void NGeometryUtilities::ConvertPoint(const Point &inPoint, HIPoint &outPoint)
{


	// Convert the point
	outPoint.x = (float) inPoint.h;
	outPoint.y = (float) inPoint.v;
}





//============================================================================
//		NGeometryUtilities::ConvertRect : Convert a rectangle.
//----------------------------------------------------------------------------
void NGeometryUtilities::ConvertRect(const HIRect &inRect, Rect &outRect)
{


	// Validate our parameters
	NN_ASSERT(inRect.origin.x > SHRT_MIN && (inRect.origin.x + inRect.size.width)  <= SHRT_MAX);
	NN_ASSERT(inRect.origin.y > SHRT_MIN && (inRect.origin.y + inRect.size.height) <= SHRT_MAX);



	// Convert the rectangle
	outRect.left   = (SInt16)  inRect.origin.x;
	outRect.top    = (SInt16)  inRect.origin.y;
	outRect.right  = (SInt16) (inRect.origin.x + inRect.size.width);
	outRect.bottom = (SInt16) (inRect.origin.y + inRect.size.height);
}





//============================================================================
//		NGeometryUtilities::ConvertRect : Convert a rectangle.
//----------------------------------------------------------------------------
void NGeometryUtilities::ConvertRect(const Rect &inRect, HIRect &outRect)
{


	// Convert the rectangle
	outRect.origin.x    = (float)  inRect.left;
	outRect.origin.y    = (float)  inRect.top;
	outRect.size.width  = (float) (inRect.right  - inRect.left);
	outRect.size.height = (float) (inRect.bottom - inRect.top);
}





//============================================================================
//		NGeometryUtilities::ConvertRect : Convert a rectangle.
//----------------------------------------------------------------------------
void NGeometryUtilities::ConvertRect(const PMRect &inRect, HIRect &outRect)
{


	// Convert the rectangle
	outRect.origin.x    =  inRect.left;
	outRect.origin.y    =  inRect.top;
	outRect.size.width  = (inRect.right  - inRect.left);
	outRect.size.height = (inRect.bottom - inRect.top);
}





//============================================================================
//		NGeometryUtilities::ScaleRect : Scale a rectangle.
//----------------------------------------------------------------------------
void NGeometryUtilities::ScaleRect(HIRect &theRect, float scaleBy)
{


	// Scale the rectangle
	theRect.origin.x    *= scaleBy;
	theRect.origin.y    *= scaleBy;
	theRect.size.width  *= scaleBy;
	theRect.size.height *= scaleBy;
}





//============================================================================
//		NGeometryUtilities::ScaleToFit : Scale a rectangle to fit.
//----------------------------------------------------------------------------
void NGeometryUtilities::ScaleToFit(HIRect &theRect, const HIRect &fitRect)
{	float		aspectRatio;
	HISize		theSize;



	// Calculate the new size
	//
	// We expand the rectangle as much as possible, up to the size of fitRect.
	aspectRatio = theRect.size.width / theRect.size.height;

	theSize.width  = fitRect.size.width;
	theSize.height = fitRect.size.width / aspectRatio;

	if (theSize.height > fitRect.size.height)
		{
		theSize.width  = fitRect.size.height * aspectRatio;
		theSize.height = fitRect.size.height;
		}



	// Position the rectangle
	theRect.size     = theSize;
	theRect.origin.x = fitRect.origin.x + ((fitRect.size.width  - theSize.width)  / 2.0f);
	theRect.origin.y = fitRect.origin.y + ((fitRect.size.height - theSize.height) / 2.0f);
}





//============================================================================
//		NGeometryUtilities::GetCenter : Get the center point.
//----------------------------------------------------------------------------
HIPoint NGeometryUtilities::GetCenter(const HIRect &theRect)
{	HIPoint		thePoint;



	// Get the center point
	thePoint.x = theRect.origin.x + (theRect.size.width  / 2.0f);
	thePoint.y = theRect.origin.y + (theRect.size.height / 2.0f);

	return(thePoint);
}





//============================================================================
//		NGeometryUtilities::GetCenter : Get the center point.
//----------------------------------------------------------------------------
HIPoint NGeometryUtilities::GetCenter(const HISize &theSize)
{


	// Get the center point
	return(GetCenter(CGRectMake(0.0f, 0.0f, theSize.width, theSize.height)));
}





//============================================================================
//		NGeometryUtilities::PositionRect : Position a rectangle.
//----------------------------------------------------------------------------
void NGeometryUtilities::PositionRect(HIRect &theRect, const HIRect &posRect, NPosition thePosition)
{	float		theOffset;
	HIPoint		theCenter;



	// Calculate the center point
	theCenter.x = posRect.origin.x + (posRect.size.width  / 2.0f);
	theCenter.y = posRect.origin.y + (posRect.size.height / 2.0f);



	// Position the rectangle
	switch (thePosition) {
		case kPositionCenter:
			// Center the rectangle
			theRect.origin.x = theCenter.x - (theRect.size.width  / 2.0f);
			theRect.origin.y = theCenter.y - (theRect.size.height / 2.0f);
			break;
		
		
		case kPositionAlert:
			// Calculate the "alert position"
			//
			// The alert position leaves 20% of the total available space
			// above the specified rectangle, such to space.
			if (theRect.size.height >= posRect.size.height)
				theOffset = posRect.origin.y;
			else
				theOffset = posRect.origin.y + (posRect.size.height * 0.2f);
			
			theRect.origin.x = theCenter.x - (theRect.size.width / 2.0f);
			theRect.origin.y = theOffset;
			break;


		default:
			NN_LOG("Unknown position: %d", thePosition);
			break;
		}
}





//============================================================================
//		NGeometryUtilities::Transform : Transform a point.
//----------------------------------------------------------------------------
HIPoint NGeometryUtilities::Transform(const HIPoint		  &inPoint,
										HICoordinateSpace  inSpace, void  *inObject,
										HICoordinateSpace outSpace, void *outObject)
{	HIPoint		thePoint;



	// Validate our parameters
	//
	// For now we just assume transforms are needed for scaling, but we should
	// support general conversions between views and windows on older systems.
	if (HIPointConvert == NULL)
		{
		NN_ASSERT( inSpace == kHICoordSpace72DPIGlobal ||  inSpace == kHICoordSpaceScreenPixel);
		NN_ASSERT(outSpace == kHICoordSpace72DPIGlobal || outSpace == kHICoordSpaceScreenPixel);
		}



	// Transform the geometry
	//
	// Resolution-independence is only available from Mac OS X 10.4 onwards,
	// so for older systems we can assume that the scaling is always 1:1.
	thePoint = inPoint;
	
	if (HIPointConvert != NULL)
		HIPointConvert(&thePoint, inSpace, inObject, outSpace, outObject);
	
	return(thePoint);
}





//============================================================================
//		NGeometryUtilities::Transform : Transform a size.
//----------------------------------------------------------------------------
HISize NGeometryUtilities::Transform(const HISize		  &inSize,
										HICoordinateSpace  inSpace, void  *inObject,
										HICoordinateSpace outSpace, void *outObject)
{	HISize		theSize;



	// Validate our parameters
	//
	// For now we just assume transforms are needed for scaling, but we should
	// support general conversions between views and windows on older systems.
	if (HISizeConvert == NULL)
		{
		NN_ASSERT( inSpace == kHICoordSpace72DPIGlobal ||  inSpace == kHICoordSpaceScreenPixel);
		NN_ASSERT(outSpace == kHICoordSpace72DPIGlobal || outSpace == kHICoordSpaceScreenPixel);
		}



	// Transform the geometry
	//
	// Resolution-independence is only available from Mac OS X 10.4 onwards,
	// so for older systems we can assume that the scaling is always 1:1.
	theSize = inSize;
	
	if (HISizeConvert != NULL)
		HISizeConvert(&theSize, inSpace, inObject, outSpace, outObject);
	
	return(theSize);
}





//============================================================================
//		NGeometryUtilities::Transform : Transform a rectangle.
//----------------------------------------------------------------------------
HIRect NGeometryUtilities::Transform(const HIRect		  &inRect,
										HICoordinateSpace  inSpace, void  *inObject,
										HICoordinateSpace outSpace, void *outObject)
{	HIRect		theRect;



	// Validate our parameters
	//
	// For now we just assume transforms are needed for scaling, but we should
	// support general conversions between views and windows on older systems.
	if (HIRectConvert == NULL)
		{
		NN_ASSERT( inSpace == kHICoordSpace72DPIGlobal ||  inSpace == kHICoordSpaceScreenPixel);
		NN_ASSERT(outSpace == kHICoordSpace72DPIGlobal || outSpace == kHICoordSpaceScreenPixel);
		}



	// Transform the geometry
	//
	// Resolution-independence is only available from Mac OS X 10.4 onwards,
	// so for older systems we can assume that the scaling is always 1:1.
	theRect = inRect;
	
	if (HIRectConvert != NULL)
		HIRectConvert(&theRect, inSpace, inObject, outSpace, outObject);
	
	return(theRect);
}



/*	NAME:
		NRectangle.cpp

	DESCRIPTION:
		Rectangle object.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"

#ifndef NRECTANGLE_CPP

#include "NEncoder.h"
#include "NRectangle.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kNRectangleOriginXKey							= "x";
static const NString kNRectangleOriginYKey							= "y";
static const NString kNRectangleSizeWidthKey						= "width";
static const NString kNRectangleSizeHeightKey						= "height";





//============================================================================
//		Implementation
//----------------------------------------------------------------------------
NENCODABLE_DEFINE(NRectangle);





//============================================================================
//      NRectangle::NRectangle : Constructor.
//----------------------------------------------------------------------------
NRectangle::NRectangle(const NRectangle32 &theRect)
		: NRectangle32(theRect.origin.x, theRect.origin.y, theRect.size.width, theRect.size.height)
{
}





//============================================================================
//      NRectangle::NRectangle : Constructor.
//----------------------------------------------------------------------------
NRectangle::NRectangle(const NRectangle64 &theRect)
		: NRectangle32(theRect.origin.x, theRect.origin.y, theRect.size.width, theRect.size.height)
{
}





//============================================================================
//		NRectangle::NRectangle : Constructor.
//----------------------------------------------------------------------------
NRectangle::NRectangle(const Float32 x, Float32 y, Float32 width, Float32 height)
		: NRectangle32(x, y, width, height)
{
}





//============================================================================
//		NRectangle::NRectangle : Constructor.
//----------------------------------------------------------------------------
NRectangle::NRectangle(const Float64 x, Float64 y, Float64 width, Float64 height)
		: NRectangle32(x, y, width, height)
{
}





//============================================================================
//		NRectangle::NRectangle : Constructor.
//----------------------------------------------------------------------------
NRectangle::NRectangle(const NPoint32 &origin, const NSize32 &size)
		: NRectangle32(origin, size)
{
}





//============================================================================
//		NRectangle::NRectangle : Constructor.
//----------------------------------------------------------------------------
NRectangle::NRectangle(const NPoint64 &origin, const NSize64 &size)
		: NRectangle32(origin.x, origin.y, size.width, size.height)
{
}





//============================================================================
//		NRectangle::NRectangle : Constructor.
//----------------------------------------------------------------------------
NRectangle::NRectangle(void)
{
}





//============================================================================
//		NRectangle::~NRectangle : Destructor.
//----------------------------------------------------------------------------
NRectangle::~NRectangle(void)
{
}





//============================================================================
//		NRectangle::NRectangle64 : NRectangle64 operator.
//----------------------------------------------------------------------------
NRectangle::operator NRectangle64(void) const
{	NRectangle64		theResult;



	// Get the value
	theResult.origin.x    = origin.x;
	theResult.origin.y    = origin.y;
	theResult.size.width  = size.width;
	theResult.size.height = size.height;
	
	return(theResult);
}





//============================================================================
//      NRectangle::EncodeSelf : Encode the object.
//----------------------------------------------------------------------------
void NRectangle::EncodeSelf(NEncoder &theEncoder) const
{


	// Encode the object
	theEncoder.EncodeNumber(kNRectangleOriginXKey,    origin.x);
	theEncoder.EncodeNumber(kNRectangleOriginYKey,    origin.y);
	theEncoder.EncodeNumber(kNRectangleSizeWidthKey,  size.width);
	theEncoder.EncodeNumber(kNRectangleSizeHeightKey, size.height);
}





//============================================================================
//      NRectangle::DecodeSelf : Decode the object.
//----------------------------------------------------------------------------
void NRectangle::DecodeSelf(const NEncoder &theEncoder)
{


	// Decode the object
	origin.x    = theEncoder.DecodeNumber(kNRectangleOriginXKey).GetFloat32();
	origin.y    = theEncoder.DecodeNumber(kNRectangleOriginYKey).GetFloat32();
	size.width  = theEncoder.DecodeNumber(kNRectangleSizeWidthKey).GetFloat32();
	size.height = theEncoder.DecodeNumber(kNRectangleSizeHeightKey).GetFloat32();
}





#else

//============================================================================
//		NRectangleT::NRectangleT : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
template<class T> NRectangleT<T>::NRectangleT(T x, T y, T width, T height)
{


	// Initialize ourselves
	origin.x    = x;
	origin.y    = y;
	size.width  = width;
	size.height = height;
}





//============================================================================
//		NRectangleT::NRectangleT : Constructor.
//----------------------------------------------------------------------------
template<class T> NRectangleT<T>::NRectangleT(const NPointT<T> &valOrigin, const NSizeT<T> &valSize)
{


	// Initialize ourselves
	origin = valOrigin;
	size   = valSize;
}





//============================================================================
//		NRectangleT::NRectangleT : Constructor.
//----------------------------------------------------------------------------
template<class T> NRectangleT<T>::NRectangleT(void)
{
}





//============================================================================
//		NRectangleT::~NRectangleT : Destructor.
//----------------------------------------------------------------------------
template<class T> NRectangleT<T>::~NRectangleT(void)
{
}





//============================================================================
//		NRectangleT::Clear : Clear the Rectangle.
//----------------------------------------------------------------------------
template<class T> void NRectangleT<T>::Clear(void)
{


	// Clear the Rectangle
	origin.Clear();
	size.Clear();
}





//============================================================================
//		NRectangleT::IsEmpty : Is the rectangle empty?
//----------------------------------------------------------------------------
template<class T> bool NRectangleT<T>::IsEmpty(void) const
{


	// Test the rectangle
	return(size.IsEmpty());
}





//============================================================================
//		NRectangleT::Compare : Compare the value.
//----------------------------------------------------------------------------
template<class T> NComparison NRectangleT<T>::Compare(const NRectangleT<T> &theValue) const
{	NComparison		theResult;



	// Compare the value
	//
	// We have no natural order, so the only real comparison is equality.
	theResult = size.Compare(theValue.size);
		
	if (theResult == kNCompareEqualTo)
		theResult = origin.Compare(theValue.origin);

	return(theResult);
}





//============================================================================
//		NRectangleT::Contains : Does a rectangle contain a point?
//----------------------------------------------------------------------------
template<class T> bool NRectangleT<T>::Contains(const NPointT<T> &thePoint) const
{


	// Check our state
	return(	thePoint.x >= GetMinX() && thePoint.x <= GetMaxX() &&
			thePoint.y >= GetMinY() && thePoint.y <= GetMaxY());
}





//============================================================================
//		NRectangleT::Contains : Does a rectangle contain a rectangle?
//----------------------------------------------------------------------------
template<class T> bool NRectangleT<T>::Contains(const NRectangleT<T> &theRect) const
{


	// Check our state
	return(GetUnion(theRect) == *this);
}





//============================================================================
//		NRectangleT::Intersects : Does a rectangle intersect another?
//----------------------------------------------------------------------------
template<class T> bool NRectangleT<T>::Intersects(const NRectangleT<T> &theRect) const
{


	// Check our state
	return(!GetIntersection(theRect).IsEmpty());
}





//============================================================================
//		NRectangleT::Normalize : Normalize the rectangle.
//----------------------------------------------------------------------------
template<class T> void NRectangleT<T>::Normalize(void)
{


	// Update our state
	*this = GetNormalized();
}





//============================================================================
//		NRectangleT::UnionWith : Union two rectangles.
//----------------------------------------------------------------------------
template<class T> void NRectangleT<T>::UnionWith(const NRectangleT<T> &theRect)
{


	// Update our state
	*this = GetUnion(theRect);
}





//============================================================================
//		NRectangleT::IntersectWith : Intersect two rectangles.
//----------------------------------------------------------------------------
template<class T> void NRectangleT<T>::IntersectWith(const NRectangleT<T> &theRect)
{


	// Update our state
	*this = GetIntersection(theRect);
}





//============================================================================
//		NRectangleT::Inset : Inset the rectangle.
//----------------------------------------------------------------------------
template<class T> void NRectangleT<T>::Inset(T deltaX, T deltaY)
{


	// Update our state
	*this = GetInset(deltaX, deltaY);
}





//============================================================================
//		NRectangleT::Offset : Offset the rectangle.
//----------------------------------------------------------------------------
template<class T> void NRectangleT<T>::Offset(T deltaX, T deltaY)
{


	// Update our state
	*this = GetOffset(deltaX, deltaY);
}





//============================================================================
//		NRectangleT::GetNormalized : Get the normalized rectangle.
//----------------------------------------------------------------------------
template<class T> NRectangleT<T> NRectangleT<T>::GetNormalized(void) const
{	T					minX, minY, maxX, maxY;
	NRectangleT<T>		theResult;



	// Get the rectangle
	//
	// A normalized rectangle has positive width and height.
	minX = GetMinX();
	minY = GetMinY();

	maxX = GetMaxX();
	maxY = GetMaxY();

	theResult = NRectangleT<T>(minX, minY, maxX - minX, maxY - minY);
	
	return(theResult);
}





//============================================================================
//		NRectangleT::GetUnion : Get the union with a rectangle.
//----------------------------------------------------------------------------
template<class T> NRectangleT<T> NRectangleT<T>::GetUnion(const NRectangleT<T> &theRect) const
{	T					minX, minY, maxX, maxY;
	NRectangleT<T>		theResult;



	// Check our state
	if (IsEmpty())
		return(theRect);
	
	if (theRect.IsEmpty())
		return(*this);



	// Get the rectangle
	minX = std::min(GetMinX(), theRect.GetMinX());
	minY = std::min(GetMinY(), theRect.GetMinY());

	maxX = std::max(GetMaxX(), theRect.GetMaxX());
	maxY = std::max(GetMaxY(), theRect.GetMaxX());

	theResult = NRectangleT<T>(minX, minY, maxX - minX, maxY - minY);
	
	return(theResult);
}





//============================================================================
//		NRectangleT::GetIntersection : Get the intersection with a rectangle.
//----------------------------------------------------------------------------
template<class T> NRectangleT<T> NRectangleT<T>::GetIntersection(const NRectangleT<T> &theRect) const
{	T					minX, minY, maxX, maxY;
	bool				notIntersect;
	NRectangleT<T>		theResult;



	// Check our state
	if (IsEmpty() || theRect.IsEmpty())
		return(theResult);



	// Get the rectangle
	notIntersect = (theRect.GetMinX() > GetMaxX() ||
					theRect.GetMaxX() < GetMinX() ||
					theRect.GetMinY() > GetMaxY() ||
					theRect.GetMaxY() < GetMinY());

	if (!notIntersect)
		{
		minX = std::max(GetMinX(), theRect.GetMinX());
		minY = std::max(GetMinY(), theRect.GetMinY());

		maxX = std::min(GetMaxX(), theRect.GetMaxX());
		maxY = std::min(GetMaxY(), theRect.GetMaxX());

		theResult = NRectangleT<T>(minX, minY, maxX - minX, maxY - minY);
		}

	return(theResult);
}





//============================================================================
//		NRectangleT::GetInset : Inset the rectangle.
//----------------------------------------------------------------------------
template<class T> NRectangleT<T> NRectangleT<T>::GetInset(T deltaX, T deltaY) const
{	NRectangleT<T>		theResult;



	// Check our state
	if (IsEmpty())
		return(theResult);



	// Get the rectangle
	theResult = GetNormalized();
	
	theResult.origin.x    +=  deltaX;
	theResult.origin.y    +=  deltaY;
	theResult.size.width  -= (deltaX * 2.0);
	theResult.size.height -= (deltaY * 2.0);
	
	return(theResult);
}





//============================================================================
//		NRectangleT::GetOffset : Offset the rectangle.
//----------------------------------------------------------------------------
template<class T> NRectangleT<T> NRectangleT<T>::GetOffset(T deltaX, T deltaY) const
{	NRectangleT<T>		theResult;



	// Get the rectangle
	theResult = GetNormalized();
	
	theResult.origin.x += deltaX;
	theResult.origin.y += deltaY;
	
	return(theResult);
}





//============================================================================
//		NRectangleT::GetMinX : Get the minimum X value.
//----------------------------------------------------------------------------
template<class T> T NRectangleT<T>::GetMinX(void) const
{


	// Get the value
	return(std::min(origin.x, origin.x + size.width));
}





//============================================================================
//		NRectangleT::GetMinY : Get the minimum Y value.
//----------------------------------------------------------------------------
template<class T> T NRectangleT<T>::GetMinY(void) const
{


	// Get the value
	return(std::min(origin.y, origin.y + size.height));
}





//============================================================================
//		NRectangleT::GetMidX : Get the middle X value.
//----------------------------------------------------------------------------
template<class T> T NRectangleT<T>::GetMidX(void) const
{


	// Get the value
	return(GetMinX() + ((GetMaxX() - GetMinX()) / 2.0));
}





//============================================================================
//		NRectangleT::GetMidY : Get the middle Y value.
//----------------------------------------------------------------------------
template<class T> T NRectangleT<T>::GetMidY(void) const
{


	// Get the value
	return(GetMinY() + ((GetMaxY() - GetMinY()) / 2.0));
}





//============================================================================
//		NRectangleT::GetMaxX : Get the maximum X value.
//----------------------------------------------------------------------------
template<class T> T NRectangleT<T>::GetMaxX(void) const
{


	// Get the value
	return(std::max(origin.x, origin.x + size.width));
}





//============================================================================
//		NRectangleT::GetMaxY : Get the maximum Y value.
//----------------------------------------------------------------------------
template<class T> T NRectangleT<T>::GetMaxY(void) const
{


	// Get the value
	return(std::max(origin.y, origin.y + size.height));
}





//============================================================================
//		NRectangleT::NFormatArgument : NFormatArgument operator.
//----------------------------------------------------------------------------
template<class T> NRectangleT<T>::operator NFormatArgument(void) const
{	NString		theResult;



	// Get the value
	theResult.Format("{x=%g, y=%g, w=%g, h=%g}", origin.x, origin.y, size.width, size.height);

	return(theResult);
}



#endif // NRECTANGLE_CPP




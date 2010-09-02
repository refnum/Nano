/*	NAME:
		NRectangle.cpp

	DESCRIPTION:
		Rectangle object.
		
		NEncodable support uses a helper class to avoid a v-table.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
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
#endif

#ifdef NRECTANGLE_CPP





//============================================================================
//		Internal class declaration
//----------------------------------------------------------------------------
class NRectangleX :	public NEncodable {
public:
										NENCODABLE_DECLARE(NRectangleX);

										NRectangleX(const NNumber &x, const NNumber &y, const NNumber &width, const NNumber &height);
										NRectangleX(void);
	virtual							   ~NRectangleX(void);


protected:
	// Encode/decode the object
	void								EncodeSelf(      NEncoder &theEncoder) const;
	void								DecodeSelf(const NEncoder &theEncoder);


private:
	NNumber								mX;
	NNumber								mY;
	NNumber								mWidth;
	NNumber								mHeight;
};





//============================================================================
//		NRectangleT::NRectangleT : Constructor.
//----------------------------------------------------------------------------
template<class T> NRectangleT<T>::NRectangleT(const NVariant &theValue)
{	NRectangle64	rectangle64;
	NRectangle32	rectangle32;
	NRectangle		rectangle;



	// Initialize ourselves
	NRectangleX::EncodableRegister();
	
	if (theValue.GetValue(rectangle64))
		{
		origin.x    = (T) rectangle64.origin.x;
		origin.y    = (T) rectangle64.origin.y;
		size.width  = (T) rectangle64.size.width;
		size.height = (T) rectangle64.size.height;
		}

	else if (theValue.GetValue(rectangle32))
		{
		origin.x    = (T) rectangle32.origin.x;
		origin.y    = (T) rectangle32.origin.y;
		size.width  = (T) rectangle32.size.width;
		size.height = (T) rectangle32.size.height;
		}

	else if (theValue.GetValue(rectangle))
		{
		origin.x    = (T) rectangle.origin.x;
		origin.y    = (T) rectangle.origin.y;
		size.width  = (T) rectangle.size.width;
		size.height = (T) rectangle.size.height;
		}
	
	else
		NN_LOG("Unknown type!");
}





//============================================================================
//		NRectangleT::NRectangleT : Constructor.
//----------------------------------------------------------------------------
template<class T> NRectangleT<T>::NRectangleT(T x, T y, T width, T height)
{


	// Initialize ourselves
	NRectangleX::EncodableRegister();
	
	origin.x    = x;
	origin.y    = y;
	size.width  = width;
	size.height = height;
}





//============================================================================
//		NRectangleT::NRectangleT : Constructor.
//----------------------------------------------------------------------------
template<class T> NRectangleT<T>::NRectangleT(T width, T height)
{


	// Initialize ourselves
	NRectangleX::EncodableRegister();
	
	size.width  = width;
	size.height = height;
}





//============================================================================
//		NRectangleT::NRectangleT : Constructor.
//----------------------------------------------------------------------------
template<class T> NRectangleT<T>::NRectangleT(const NPointT<T> &valOrigin, const NSizeT<T> &valSize)
{


	// Initialize ourselves
	NRectangleX::EncodableRegister();
	
	origin = valOrigin;
	size   = valSize;
}





//============================================================================
//		NRectangleT::NRectangleT : Constructor.
//----------------------------------------------------------------------------
template<class T> NRectangleT<T>::NRectangleT(const NSizeT<T> &valSize)
{


	// Initialize ourselves
	NRectangleX::EncodableRegister();
	
	size = valSize;
}





//============================================================================
//		NRectangleT::NRectangleT : Constructor.
//----------------------------------------------------------------------------
template<class T> NRectangleT<T>::NRectangleT(void)
{


	// Initialize ourselves
	NRectangleX::EncodableRegister();
}





//============================================================================
//		NRectangleT::Clear : Clear the rectangle.
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
//		NRectangleT::ScaleToFit : Scale to fit.
//----------------------------------------------------------------------------
template<class T> void NRectangleT<T>::ScaleToFit(const NRectangleT<T> &theRect)
{	T			aspectRatio;
	NSizeT<T>	theSize;



	// Calculate the new size
	//
	// We expand ourselves as much as possible, up to the size of theRect.
	aspectRatio = size.width / size.height;

	theSize.width  = theRect.size.width;
	theSize.height = theRect.size.width / aspectRatio;

	if (theSize.height > theRect.size.height)
		{
		theSize.width  = theRect.size.height * aspectRatio;
		theSize.height = theRect.size.height;
		}



	// Update our state
	size     = theSize;
	origin.x = theRect.origin.x + (T) ((theRect.size.width  - theSize.width)  / 2.0);
	origin.y = theRect.origin.y + (T) ((theRect.size.height - theSize.height) / 2.0);
}





//============================================================================
//		NRectangleT::SetPosition : Set the position.
//----------------------------------------------------------------------------
template<class T> void NRectangleT<T>::SetPosition(const NRectangleT<T> &theRect, NPosition thePosition)
{	T	minX, midX, maxX, minY, midY, maxY;
	T	halfWidth, halfHeight;



	// Get the state we need
	minX = theRect.origin.x;
	maxX = theRect.origin.x + theRect.size.width;
	midX = minX + (T) ((maxX - minX) / 2.0);

	minY = theRect.origin.y;
	maxY = theRect.origin.y + theRect.size.height;
	midY = minY + (T) ((maxY - minY) / 2.0);

	halfWidth  = (T) (size.width  / 2.0);
	halfHeight = (T) (size.height / 2.0);



	// Position the rectangle
	//
	// The alert position was defined by the Mac HIG as being a gap of "20%
	// of the available height" above the rectangle, subject to space.
	switch (thePosition) {
		case kNPositionAlert:
			origin.x = midX - halfWidth;
			origin.y = minY;

			if (size.height < theRect.size.height)
				origin.y += (T) (theRect.size.height * 0.2);
			break;

		case kNPositionCenter:
			origin.x = midX - halfWidth;
			origin.y = midY - halfHeight;
			break;

		case kNPositionTopLeft:
			origin.x = minX;
			origin.y = minY;
			break;

		case kNPositionTopRight:
			origin.x = maxX - size.width;
			origin.y = minY;
			break;

		case kNPositionBottomLeft:
			origin.x = minX;
			origin.y = maxY - size.height;
			break;

		case kNPositionBottomRight:
			origin.x = maxX - size.width;
			origin.y = maxY - size.height;
			break;

		default:
			NN_LOG("Unknown position: %d", thePosition);
			break;
		}
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
//		NRectangleT::MakeIntegral : Make the rectangle integral.
//----------------------------------------------------------------------------
template<class T> void NRectangleT<T>::MakeIntegral(void)
{


	// Update our state
	*this = GetIntegral();
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
//		NRectangleT::Scale : Scale the rectangle.
//----------------------------------------------------------------------------
template<class T> void NRectangleT<T>::Scale(T scaleBy)
{


	// Update our state
	*this = GetScaled(scaleBy);
}





//============================================================================
//		NRectangleT::GetNormalized : Get the normalized rectangle.
//----------------------------------------------------------------------------
template<class T> NRectangleT<T> NRectangleT<T>::GetNormalized(void) const
{	T					minX, minY, maxX, maxY;
	NRectangleT<T>		theResult;



	// Noramlize the rectangle
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
//		NRectangleT::GetIntegral : Get the integral rectangle.
//----------------------------------------------------------------------------
template<class T> NRectangleT<T> NRectangleT<T>::GetIntegral(void) const
{	NRectangleT<T>		theResult;



	// Get the integral rectangle
	//
	// An integral rectangle has its origin rounded down and size rounded up,
	// to create a (potentially larger) rectangle with integer coordinates.
	theResult.origin.x    = floor(origin.x);
	theResult.origin.y    = floor(origin.y);
	theResult.size.width  = ceil(origin.x + size.width)  - theResult.origin.x;
	theResult.size.height = ceil(origin.y + size.height) - theResult.origin.y;
	
	
	
	// Validate our state
	NN_ASSERT(theResult.Contains(*this));

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



	// Get the union
	minX = std::min(GetMinX(), theRect.GetMinX());
	minY = std::min(GetMinY(), theRect.GetMinY());

	maxX = std::max(GetMaxX(), theRect.GetMaxX());
	maxY = std::max(GetMaxY(), theRect.GetMaxY());

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



	// Get the intersection
	notIntersect = (theRect.GetMinX() > GetMaxX() ||
					theRect.GetMaxX() < GetMinX() ||
					theRect.GetMinY() > GetMaxY() ||
					theRect.GetMaxY() < GetMinY());

	if (!notIntersect)
		{
		minX = std::max(GetMinX(), theRect.GetMinX());
		minY = std::max(GetMinY(), theRect.GetMinY());

		maxX = std::min(GetMaxX(), theRect.GetMaxX());
		maxY = std::min(GetMaxY(), theRect.GetMaxY());

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



	// Inset the rectangle
	theResult = GetNormalized();
	
	theResult.origin.x += deltaX;
	theResult.origin.y += deltaY;

	theResult.size.width  -= (T) (deltaX * 2.0);
	theResult.size.height -= (T) (deltaY * 2.0);
	
	return(theResult);
}





//============================================================================
//		NRectangleT::GetOffset : Offset the rectangle.
//----------------------------------------------------------------------------
template<class T> NRectangleT<T> NRectangleT<T>::GetOffset(T deltaX, T deltaY) const
{	NRectangleT<T>		theResult;



	// Offset the rectangle
	theResult = GetNormalized();
	
	theResult.origin.x += deltaX;
	theResult.origin.y += deltaY;
	
	return(theResult);
}





//============================================================================
//		NRectangleT::GetScaled : Scale the rectangle.
//----------------------------------------------------------------------------
template<class T> NRectangleT<T> NRectangleT<T>::GetScaled(T scaleBy) const
{	NRectangleT<T>		theResult;
	NPointT<T>			theCenter;



	// Scale the rectangle
	theResult = GetNormalized();
	theCenter = theResult.GetCenter();
	
	theResult.size.width  *= scaleBy;
	theResult.size.height *= scaleBy;

	theResult.origin.x = theCenter.x - (T) (theResult.size.width  / 2.0);
	theResult.origin.y = theCenter.y - (T) (theResult.size.height / 2.0);
	
	return(theResult);
}





//============================================================================
//		NRectangleT::GetCenter : Get the center.
//----------------------------------------------------------------------------
template<class T> NPointT<T> NRectangleT<T>::GetCenter(void) const
{


	// Get the center
	return(GetCorner(kNPositionCenter));
}





//============================================================================
//		NRectangleT::GetCorner : Get a corner.
//----------------------------------------------------------------------------
template<class T> NPointT<T> NRectangleT<T>::GetCorner(NPosition thePosition) const
{	NPointT<T>		theResult;



	// Get the corner
	switch (thePosition) {
		case kNPositionCenter:
			theResult.x = GetMidX();
			theResult.y = GetMidY();
			break;

		case kNPositionTopLeft:
			theResult.x = GetMinX();
			theResult.y = GetMinY();
			break;

		case kNPositionTopRight:
			theResult.x = GetMaxX();
			theResult.y = GetMinY();
			break;

		case kNPositionBottomLeft:
			theResult.x = GetMinX();
			theResult.y = GetMaxY();
			break;

		case kNPositionBottomRight:
			theResult.x = GetMaxX();
			theResult.y = GetMaxY();
			break;

		default:
			NN_LOG("Invalid corner position: %d", thePosition);
			break;
		}
	
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
	return(GetMinX() + (T) ((GetMaxX() - GetMinX()) / 2.0));
}





//============================================================================
//		NRectangleT::GetMidY : Get the middle Y value.
//----------------------------------------------------------------------------
template<class T> T NRectangleT<T>::GetMidY(void) const
{


	// Get the value
	return(GetMinY() + (T) ((GetMaxY() - GetMinY()) / 2.0));
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
//		NRectangleT::NEncodable : NEncodable operator.
//----------------------------------------------------------------------------
template<class T> NRectangleT<T>::operator NEncodable(void) const
{	NRectangleX		theResult(origin.x, origin.y, size.width, size.height);



	// Get the value
	return(theResult);
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





#else // NRECTANGLE_CPP
#pragma mark -
//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kNRectangleOriginXKey							= "x";
static const NString kNRectangleOriginYKey							= "y";
static const NString kNRectangleSizeWidthKey						= "width";
static const NString kNRectangleSizeHeightKey						= "height";





//============================================================================
//		NEncodable
//----------------------------------------------------------------------------
NENCODABLE_DEFINE_REGISTER(NRectangleX, NRectangle);

bool NRectangleX::EncodableCanEncode(const NVariant &theValue)
{	bool	canEncode;

	canEncode  = theValue.IsType(typeid(NRectangle64));
	canEncode |= theValue.IsType(typeid(NRectangle32));
	canEncode |= theValue.IsType(typeid(NRectangle));

	return(canEncode);
}

void NRectangleX::EncodableEncodeObject(NEncoder &theEncoder, const NVariant &theValue)
{	NRectangle64	rectangle64;
	NRectangle32	rectangle32;
	NRectangle		rectangle;
	NRectangleX		theObject;

	if (theValue.GetValue(rectangle64))
		theObject = NRectangleX(rectangle64.origin.x, rectangle64.origin.y, rectangle64.size.width, rectangle64.size.height);

	else if (theValue.GetValue(rectangle32))
		theObject = NRectangleX(rectangle32.origin.x, rectangle32.origin.y, rectangle32.size.width, rectangle32.size.height);

	else if (theValue.GetValue(rectangle))
		theObject = NRectangleX(rectangle.origin.x, rectangle.origin.y, rectangle.size.width, rectangle.size.height);

	else
		NN_LOG("Unknown type!");

	theObject.EncodeSelf(theEncoder);
}

NVariant NRectangleX::EncodableDecodeObject(const NEncoder &theEncoder)
{	NRectangleX		theObject;

	theObject.DecodeSelf(theEncoder);

	if (theObject.mX.GetPrecision()      == kNPrecisionFloat64 ||
		theObject.mY.GetPrecision()      == kNPrecisionFloat64 ||
		theObject.mWidth.GetPrecision()  == kNPrecisionFloat64 ||
		theObject.mHeight.GetPrecision() == kNPrecisionFloat64)
		return(NRectangle64(theObject.mX.GetFloat64(), theObject.mY.GetFloat64(), theObject.mWidth.GetFloat64(), theObject.mHeight.GetFloat64()));
	else
		return(NRectangle32(theObject.mX.GetFloat32(), theObject.mY.GetFloat32(), theObject.mWidth.GetFloat32(), theObject.mHeight.GetFloat32()));
}





//============================================================================
//      NRectangleX::NRectangleX : Constructor.
//----------------------------------------------------------------------------
NRectangleX::NRectangleX(const NNumber &x, const NNumber &y, const NNumber &width, const NNumber &height)
{


	// Initialise ourselves
	mX      = x;
	mY      = y;
	mWidth  = width;
	mHeight = height;
}





//============================================================================
//      NRectangleX::NRectangleX : Constructor.
//----------------------------------------------------------------------------
NRectangleX::NRectangleX()
{
}





//============================================================================
//      NRectangleX::~NRectangleX : Destructor.
//----------------------------------------------------------------------------
NRectangleX::~NRectangleX(void)
{
}





//============================================================================
//      NRectangleX::EncodeSelf : Encode the object.
//----------------------------------------------------------------------------
void NRectangleX::EncodeSelf(NEncoder &theEncoder) const
{


	// Encode the object
	theEncoder.EncodeNumber(kNRectangleOriginXKey,    mX);
	theEncoder.EncodeNumber(kNRectangleOriginYKey,    mY);
	theEncoder.EncodeNumber(kNRectangleSizeWidthKey,  mWidth);
	theEncoder.EncodeNumber(kNRectangleSizeHeightKey, mHeight);
}





//============================================================================
//      NRectangleX::DecodeSelf : Decode the object.
//----------------------------------------------------------------------------
void NRectangleX::DecodeSelf(const NEncoder &theEncoder)
{


	// Decode the object
	mX      = theEncoder.DecodeNumber(kNRectangleOriginXKey);
	mY      = theEncoder.DecodeNumber(kNRectangleOriginYKey);
	mWidth  = theEncoder.DecodeNumber(kNRectangleSizeWidthKey);
	mHeight = theEncoder.DecodeNumber(kNRectangleSizeHeightKey);
}



#endif // NRECTANGLE_CPP




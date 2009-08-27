/*	NAME:
		NShape.cpp

	DESCRIPTION:
		Shape object.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMathUtilities.h"
#include "NString.h"

#ifndef NSHAPE_CPP

#include "NEncoder.h"
#include "NShape.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kNShapeLoopsKey								= "loops";
static const NString kNShapePointsKey								= "points";





//============================================================================
//		Implementation
//----------------------------------------------------------------------------
NENCODABLE_DEFINE(NShape);





//============================================================================
//      NShape::NShape : Constructor.
//----------------------------------------------------------------------------
NShape::NShape(const NShape32 &theShape)
		: NShape32(theShape.loops, theShape.points)
{
}





//============================================================================
//      NShape::NShape : Constructor.
//----------------------------------------------------------------------------
NShape::NShape(const NShape64 &theShape)
		: NShape32(theShape.loops, Convert64To32(theShape.points))
{
}





//============================================================================
//		NShape::NShape : Constructor.
//----------------------------------------------------------------------------
NShape::NShape(const NIndexList &loops, const NPoint32List &points)
	: NShape32(loops, points)
{
}





//============================================================================
//		NShape::NShape : Constructor.
//----------------------------------------------------------------------------
NShape::NShape(const NIndexList &loops, const NPoint64List &points)
	: NShape32(loops, Convert64To32(points))
{
}





//============================================================================
//		NShape::NShape : Constructor.
//----------------------------------------------------------------------------
NShape::NShape(void)
{
}





//============================================================================
//		NShape::~NShape : Destructor.
//----------------------------------------------------------------------------
NShape::~NShape(void)
{
}





//============================================================================
//		NShape::NShape64 : NShape64 operator.
//----------------------------------------------------------------------------
NShape::operator NShape64(void) const
{	NShape64		theResult;



	// Get the value
	theResult.loops  = loops;
	theResult.points = Convert32To64(points);
	
	return(theResult);
}





//============================================================================
//      NShape::EncodeSelf : Encode the object.
//----------------------------------------------------------------------------
void NShape::EncodeSelf(NEncoder &theEncoder) const
{


	// Encode the object
	NN_UNUSED(theEncoder);
	NN_LOG("NShape encoded representation is TBD - skipping!");
}





//============================================================================
//      NShape::DecodeSelf : Decode the object.
//----------------------------------------------------------------------------
void NShape::DecodeSelf(const NEncoder &theEncoder)
{


	// Decode the object
	NN_UNUSED(theEncoder);
	NN_LOG("NShape encoded representation is TBD - skipping!");
}





//============================================================================
//      NShape::Convert32To64 : Convert an NPoint32List to an NPoint64List.
//----------------------------------------------------------------------------
NPoint64List NShape::Convert32To64(const NPoint32List &points32) const
{	NPoint64List					points64;
	NPoint32ListConstIterator		theIter;



	// Convert the points
	points64.reserve(points32.size());
	
	for (theIter = points32.begin(); theIter != points32.end(); theIter++)
		points64.push_back(NPoint64(theIter->x, theIter->y));
	
	return(points64);
}





//============================================================================
//      NShape::Convert64To32 : Convert an NPoint64List to an NPoint32List.
//----------------------------------------------------------------------------
NPoint32List NShape::Convert64To32(const NPoint64List &points64) const
{	NPoint32List					points32;
	NPoint64ListConstIterator		theIter;



	// Convert the points
	points32.reserve(points64.size());
	
	for (theIter = points64.begin(); theIter != points64.end(); theIter++)
		points32.push_back(NPoint32(theIter->x, theIter->y));
	
	return(points32);
}





#else

//============================================================================
//		NShapeT::NShapeT : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
template<class T> NShapeT<T>::NShapeT(const NIndexList &valLoops, const std::vector< NPointT<T> > &valPoints)
{


	// Initialize ourselves
	loops  = valLoops;
	points = valPoints;
}





//============================================================================
//		NShapeT::NShapeT : Constructor.
//----------------------------------------------------------------------------
template<class T> NShapeT<T>::NShapeT(void)
{
}





//============================================================================
//		NShapeT::~NShapeT : Destructor.
//----------------------------------------------------------------------------
template<class T> NShapeT<T>::~NShapeT(void)
{
}





//============================================================================
//		NShapeT::Clear : Clear the size.
//----------------------------------------------------------------------------
template<class T> void NShapeT<T>::Clear(void)
{


	// Clear the size
	loops.clear();
	points.clear();
}





//============================================================================
//		NShapeT::IsEmpty : Is the size empty?
//----------------------------------------------------------------------------
template<class T> bool NShapeT<T>::IsEmpty(void) const
{


	// Test the size
	return(loops.empty() || points.empty());
}





//============================================================================
//		NShapeT::Compare : Compare the value.
//----------------------------------------------------------------------------
template<class T> NComparison NShapeT<T>::Compare(const NShapeT &theValue) const
{	NComparison		theResult;



	// Compare the value
	//
	// We have no natural order, so the only real comparison is equality.
	theResult = CompareData(loops.size(), &loops[0], theValue.loops.size(), &theValue.loops[0]);
		
	if (theResult == kNCompareEqualTo)
		theResult = CompareData(points.size(), &points[0], theValue.points.size(), &theValue.points[0]);
	
	return(theResult);
}





//============================================================================
//		NShapeT::NFormatArgument : NFormatArgument operator.
//----------------------------------------------------------------------------
template<class T> NShapeT<T>::operator NFormatArgument(void) const
{	NString		theResult;



	// Get the value
	theResult.Format("{loops=%ld, points=%ld}", loops.size(), points.size());

	return(theResult);
}







#endif // NSHAPE_CPP


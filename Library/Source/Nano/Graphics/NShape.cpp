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
static const NString kNShape64Key									= "64";
static const NString kNShapePointsKey								= "points";
static const NString kNShapeLoopsKey								= "loops";





//============================================================================
//		Internal class declaration
//----------------------------------------------------------------------------
class NShapeX :	public NEncodable {
public:
										NENCODABLE_DECLARE(NShapeX);

										NShapeX(const NPoint32List &thePoints, const NIndexList &theLoops);
										NShapeX(const NPoint64List &thePoints, const NIndexList &theLoops);

										NShapeX(void);
	virtual							   ~NShapeX(void);


protected:
	// Encode the object
	void								EncodeSelf(NEncoder &theEncoder) const;


private:
	template<class T> std::vector<T>				Flatten(  const std::vector< NPointT<T> > &thePoints);
	template<class T> std::vector< NPointT<T> >		Unflatten(const std::vector<T>            &theValues);


private:
	bool								mIs64;
	NArray								mPoints;
	NArray								mLoops;
};





//============================================================================
//		Implementation
//----------------------------------------------------------------------------
NENCODABLE_DEFINE_NODECODE(NShapeX);





//============================================================================
//      NShapeX::NShapeX : Constructor.
//----------------------------------------------------------------------------
NShapeX::NShapeX(const NPoint32List &thePoints, const NIndexList &theLoops)
{


	// Initialise ourselves
	mIs64   = false;
	mPoints = NArray(Flatten(thePoints));
	mLoops  = NArray(theLoops);
}





//============================================================================
//      NShapeX::NShapeX : Constructor.
//----------------------------------------------------------------------------
NShapeX::NShapeX(const NPoint64List &thePoints, const NIndexList &theLoops)
{


	// Initialise ourselves
	mIs64   = true;
	mPoints = NArray(Flatten(thePoints));
	mLoops  = NArray(theLoops);
}





//============================================================================
//      NShapeX::NShapeX : Constructor.
//----------------------------------------------------------------------------
NShapeX::NShapeX()
{
}





//============================================================================
//      NShapeX::~NShapeX : Destructor.
//----------------------------------------------------------------------------
NShapeX::~NShapeX(void)
{
}





//============================================================================
//      NShapeX::EncodableGetDecoded : Get a decoded object.
//----------------------------------------------------------------------------
NVariant NShapeX::EncodableGetDecoded(const NEncoder &theEncoder)
{	NArray		thePoints, theLoops;
	bool		is64;



	// Decode the object
	is64 = theEncoder.DecodeBoolean(kNShape64Key);
	
	if (theEncoder.DecodeObject(kNShapePointsKey).GetValue(thePoints) &&
		theEncoder.DecodeObject(kNShapeLoopsKey).GetValue(theLoops))
		{
		if (is64)
			return(NShape64(Unflatten(thePoints.GetValuesFloat64()), thePoints.GetValuesSInt32()));
		else
			return(NShape32(Unflatten(thePoints.GetValuesFloat32()), thePoints.GetValuesSInt32()));
		}
	
	return(NVariant());
}





//============================================================================
//      NShapeX::EncodeSelf : Encode the object.
//----------------------------------------------------------------------------
void NShapeX::EncodeSelf(NEncoder &theEncoder) const
{


	// Encode the object
	theEncoder.EncodeBoolean(kNShape64Key,    mIs64);
	theEncoder.EncodeObject(kNShapePointsKey, mPoints);
	theEncoder.EncodeObject(kNShapeLoopsKey,  mLoops);
}





//============================================================================
//      NShapeX::Flatten : Flatten a point list.
//----------------------------------------------------------------------------
template<class T> std::vector<T> NShapeX::Flatten(const std::vector< NPointT<T> > &thePoints)
{	NIndex				n, numItems;
	std::vector<T>		theValues;



	// Flatten the list
	numItems = thePoints.size();
	
	for (n = 0; n < numItems; n++)
		{
		theValues.push_back(thePoints[n].x);
		theValues.push_back(thePoints[n].y);
		}
	
	return(theValues);
}





//============================================================================
//      NShapeX::Unflatten : Unflatten a point list.
//----------------------------------------------------------------------------
template<class T> std::vector< NPointT<T> > NShapeX::Unflatten(const std::vector<T> &theValues)
{	NIndex							n, numItems;
	std::vector< NPointT<T> >		thePoints;
	NPointT<T>						thePoint;
	


	// Unflatten the list
	numItems = theValues.size();
	
	for (n = 0; n < numItems; n += 2)
		{
		thePoint.x = theValues[n+0];
		thePoint.y = theValues[n+1];
		
		thePoints.push_back(thePoint);
		}
	
	return(thePoints);
}





#else

//============================================================================
//		NShapeT::NShapeT : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
template<class T> NShapeT<T>::NShapeT(const std::vector< NPointT<T> > &thePoints, const NIndexList &theLoops)
{


	// Initialize ourselves
	points = thePoints;
	loops  = theLoops;
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
	points.clear();
	loops.clear();
}





//============================================================================
//		NShapeT::IsEmpty : Is the size empty?
//----------------------------------------------------------------------------
template<class T> bool NShapeT<T>::IsEmpty(void) const
{


	// Test the size
	return(points.empty() || loops.empty());
}





//============================================================================
//		NShapeT::Compare : Compare the value.
//----------------------------------------------------------------------------
template<class T> NComparison NShapeT<T>::Compare(const NShapeT &theValue) const
{	NComparison		theResult;



	// Compare the value
	//
	// We have no natural order, so the only real comparison is equality.
	theResult = CompareData(points.size(), &points[0], theValue.points.size(), &theValue.points[0]);
		
	if (theResult == kNCompareEqualTo)
		theResult = CompareData(loops.size(), &loops[0], theValue.loops.size(), &theValue.loops[0]);
	
	return(theResult);
}





//============================================================================
//		NShapeT::NEncodable : NEncodable operator.
//----------------------------------------------------------------------------
template<class T> NShapeT<T>::operator NEncodable(void) const
{	NShapeX		theResult(points, loops);



	// Get the value
	return(theResult);
}





//============================================================================
//		NShapeT::NFormatArgument : NFormatArgument operator.
//----------------------------------------------------------------------------
template<class T> NShapeT<T>::operator NFormatArgument(void) const
{	NString		theResult;



	// Get the value
	theResult.Format("{points=%ld, loops=%ld}", points.size(), loops.size());

	return(theResult);
}







#endif // NSHAPE_CPP


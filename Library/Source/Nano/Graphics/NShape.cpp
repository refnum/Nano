/*	NAME:
		NShape.cpp

	DESCRIPTION:
		Shape object.

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
#include "NMathUtilities.h"
#include "NString.h"

#ifndef NSHAPE_CPP
	#include "NEncoder.h"
	#include "NShape.h"
#endif

#ifdef NSHAPE_CPP





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
	// Encode/decode the object
	void								EncodeSelf(      NEncoder &theEncoder) const;
	void								DecodeSelf(const NEncoder &theEncoder);


private:
	template<class T> std::vector<T>				Flatten(  const std::vector< NPointT<T> > &thePoints);
	template<class T> std::vector< NPointT<T> >		Unflatten(const std::vector<T>            &theValues);


private:
	bool								mIs64;
	NArray								mPoints;
	NArray								mLoops;
};





//============================================================================
//		NShapeT::NShapeT : Constructor.
//----------------------------------------------------------------------------
template<class T> NShapeT<T>::NShapeT(const NVariant &theValue)
{	NIndex		n, numPoints, numLoops;
	NShape64	shape64;
	NShape32	shape32;
	NShape		shape;



	// Initialize ourselves
	NShapeX::EncodableRegister();
	
	if (theValue.GetValue(shape64))
		{
		numPoints = shape64.points.size();
		numLoops  = shape64.loops.size();

		points.resize(numPoints);
		points.resize(numLoops);

		for (n = 0; n < numPoints; n++)
			points[n] = (T) shape64.points[n];

		for (n = 0; n < numLoops; n++)
			loops[n] = (T) shape64.loops[n];
		}

	else if (theValue.GetValue(shape32))
		{
		numPoints = shape32.points.size();
		numLoops  = shape32.loops.size();

		points.resize(numPoints);
		points.resize(numLoops);

		for (n = 0; n < numPoints; n++)
			points[n] = (T) shape32.points[n];

		for (n = 0; n < numLoops; n++)
			loops[n] = (T) shape32.loops[n];
		}

	else if (theValue.GetValue(shape))
		{
		numPoints = shape.points.size();
		numLoops  = shape.loops.size();

		points.resize(numPoints);
		points.resize(numLoops);

		for (n = 0; n < numPoints; n++)
			points[n] = (T) shape.points[n];

		for (n = 0; n < numLoops; n++)
			loops[n] = (T) shape.loops[n];
		}
	
	else
		NN_LOG("Unknown type!");
}





//============================================================================
//		NShapeT::NShapeT : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
template<class T> NShapeT<T>::NShapeT(const std::vector< NPointT<T> > &thePoints, const NIndexList &theLoops)
{


	// Initialize ourselves
	NShapeX::EncodableRegister();
	
	points = thePoints;
	loops  = theLoops;
}





//============================================================================
//		NShapeT::NShapeT : Constructor.
//----------------------------------------------------------------------------
template<class T> NShapeT<T>::NShapeT(void)
{


	// Initialize ourselves
	NShapeX::EncodableRegister();
}





//============================================================================
//		NShapeT::Clear : Clear the shape.
//----------------------------------------------------------------------------
template<class T> void NShapeT<T>::Clear(void)
{


	// Clear the size
	points.clear();
	loops.clear();
}





//============================================================================
//		NShapeT::IsEmpty : Is the shape empty?
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





#else // NSHAPE_CPP
#pragma mark -
//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kNShape64Key									= "64";
static const NString kNShapePointsKey								= "points";
static const NString kNShapeLoopsKey								= "loops";





//============================================================================
//		NEncodable
//----------------------------------------------------------------------------
NENCODABLE_DEFINE_REGISTER(NShapeX, NShape);

bool NShapeX::EncodableCanEncode(const NVariant &theValue)
{	bool	canEncode;

	canEncode  = theValue.IsType(typeid(NShape64));
	canEncode |= theValue.IsType(typeid(NShape32));
	canEncode |= theValue.IsType(typeid(NShape));

	return(canEncode);
}

void NShapeX::EncodableEncodeObject(NEncoder &theEncoder, const NVariant &theValue)
{	NShapeX		theObject;
	NShape64	shape64;
	NShape32	shape32;
	NShape		shape;

	if (theValue.GetValue(shape64))
		theObject = NShapeX(shape64.points, shape64.loops);

	else if (theValue.GetValue(shape32))
		theObject = NShapeX(shape32.points, shape32.loops);

	else if (theValue.GetValue(shape))
		theObject = NShapeX(shape.points, shape.loops);

	else
		NN_LOG("Unknown type!");

	theObject.EncodeSelf(theEncoder);
}

NVariant NShapeX::EncodableDecodeObject(const NEncoder &theEncoder)
{	NShapeX		theObject;

	theObject.DecodeSelf(theEncoder);

	if (theObject.mIs64)
		return(NShape64(theObject.Unflatten(theObject.mPoints.GetValuesFloat64()), theObject.mLoops.GetValuesSInt32()));
	else
		return(NShape32(theObject.Unflatten(theObject.mPoints.GetValuesFloat32()), theObject.mLoops.GetValuesSInt32()));
}





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
//      NShapeX::DecodeSelf : Decode the object.
//----------------------------------------------------------------------------
void NShapeX::DecodeSelf(const NEncoder &theEncoder)
{	bool	wasOK;


	// Encode the object
	mIs64 = theEncoder.DecodeBoolean(kNShape64Key);

	wasOK  = theEncoder.DecodeObject(kNShapePointsKey).GetValue(mPoints);
	wasOK |= theEncoder.DecodeObject(kNShapeLoopsKey).GetValue( mLoops);

	NN_ASSERT(wasOK);
}





//============================================================================
//      NShapeX::Flatten : Flatten a point list.
//----------------------------------------------------------------------------
template<class T> std::vector<T> NShapeX::Flatten(const std::vector< NPointT<T> > &thePoints)
{	NIndex				n, numItems;
	std::vector<T>		theValues;



	// Flatten the list
	numItems = (NIndex) thePoints.size();
	
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
	numItems = (NIndex) theValues.size();
	
	for (n = 0; n < numItems; n += 2)
		{
		thePoint.x = theValues[n+0];
		thePoint.y = theValues[n+1];
		
		thePoints.push_back(thePoint);
		}
	
	return(thePoints);
}



#endif // NSHAPE_CPP





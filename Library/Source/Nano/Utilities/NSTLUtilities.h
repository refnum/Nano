/*	NAME:
		NSTLUtilities.h

	DESCRIPTION:
		STL utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSTLUTILITIES_HDR
#define NSTLUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Vectors
//----------------------------------------------------------------------------
// Create a vector from an array
template <class T> std::vector<T> vector_create_n(UInt32 numValues, const T *theValues) 
{
	return(std::vector<T>(theValues, theValues + numValues));
} 

#define vector_create(_a)			 vector_create_n(NN_ARRAY_SIZE(_a), (_a))


// Create a vector from a list of parameters
#define _NN_VECTOR1( p1)																	std::vector<T> result;														result.push_back(p1);
#define _NN_VECTOR2( p1, p2)																_NN_VECTOR1( p1);															result.push_back(p2);
#define _NN_VECTOR3( p1, p2, p3)															_NN_VECTOR2( p1, p2);														result.push_back(p3);
#define _NN_VECTOR4( p1, p2, p3, p4)														_NN_VECTOR3( p1, p2, p3);													result.push_back(p4);
#define _NN_VECTOR5( p1, p2, p3, p4, p5)													_NN_VECTOR4( p1, p2, p3, p4);												result.push_back(p5);
#define _NN_VECTOR6( p1, p2, p3, p4, p5, p6)												_NN_VECTOR5( p1, p2, p3, p4, p5);											result.push_back(p6);
#define _NN_VECTOR7( p1, p2, p3, p4, p5, p6, p7)											_NN_VECTOR6( p1, p2, p3, p4, p5, p6);										result.push_back(p7);
#define _NN_VECTOR8( p1, p2, p3, p4, p5, p6, p7, p8)										_NN_VECTOR7( p1, p2, p3, p4, p5, p6, p7);									result.push_back(p8);
#define _NN_VECTOR9( p1, p2, p3, p4, p5, p6, p7, p8, p9)									_NN_VECTOR8( p1, p2, p3, p4, p5, p6, p7, p8);								result.push_back(p9);
#define _NN_VECTOR10(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)								_NN_VECTOR9( p1, p2, p3, p4, p5, p6, p7, p8, p9);							result.push_back(p10);
#define _NN_VECTOR11(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11)							_NN_VECTOR10(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);						result.push_back(p11);
#define _NN_VECTOR12(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12)						_NN_VECTOR11(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);					result.push_back(p12);
#define _NN_VECTOR13(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13)				_NN_VECTOR12(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);			result.push_back(p13);
#define _NN_VECTOR14(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14)			_NN_VECTOR13(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);		result.push_back(p14);
#define _NN_VECTOR15(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15)		_NN_VECTOR14(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);	result.push_back(p15);

template <class T> std::vector<T> mkvector(const T &p1)
{
	_NN_VECTOR1(p1);
	return(result);
}

template <class T> std::vector<T> mkvector(const T &p1, const T &p2)
{
	_NN_VECTOR2(p1, p2);
	return(result);
}

template <class T> std::vector<T> mkvector(const T &p1, const T &p2, const T &p3)
{
	_NN_VECTOR3(p1, p2, p3);
	return(result);
}

template <class T> std::vector<T> mkvector(const T &p1, const T &p2, const T &p3, const T &p4)
{
	_NN_VECTOR4(p1, p2, p3, p4);
	return(result);
}

template <class T> std::vector<T> mkvector(const T &p1, const T &p2, const T &p3, const T &p4, const T &p5)
{
	_NN_VECTOR5(p1, p2, p3, p4, p5);
	return(result);
}

template <class T> std::vector<T> mkvector(const T &p1, const T &p2, const T &p3, const T &p4, const T &p5, const T &p6)
{
	_NN_VECTOR6(p1, p2, p3, p4, p5, p6);
	return(result);
}

template <class T> std::vector<T> mkvector(const T &p1, const T &p2, const T &p3, const T &p4, const T &p5, const T &p6, const T &p7)
{
	_NN_VECTOR7(p1, p2, p3, p4, p5, p6, p7);
	return(result);
}

template <class T> std::vector<T> mkvector(const T &p1, const T &p2, const T &p3, const T &p4, const T &p5, const T &p6, const T &p7, const T &p8)
{
	_NN_VECTOR8(p1, p2, p3, p4, p5, p6, p7, p8);
	return(result);
}

template <class T> std::vector<T> mkvector(const T &p1, const T &p2, const T &p3, const T &p4, const T &p5, const T &p6, const T &p7, const T &p8, const T &p9)
{
	_NN_VECTOR9(p1, p2, p3, p4, p5, p6, p7, p8, p9);
	return(result);
}

template <class T> std::vector<T> mkvector(const T &p1, const T &p2, const T &p3, const T &p4, const T &p5, const T &p6, const T &p7, const T &p8, const T &p9, const T &p10)
{
	_NN_VECTOR10(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
	return(result);
}

template <class T> std::vector<T> mkvector(const T &p1, const T &p2, const T &p3, const T &p4, const T &p5, const T &p6, const T &p7, const T &p8, const T &p9, const T &p10, const T &p11)
{
	_NN_VECTOR11(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
	return(result);
}

template <class T> std::vector<T> mkvector(const T &p1, const T &p2, const T &p3, const T &p4, const T &p5, const T &p6, const T &p7, const T &p8, const T &p9, const T &p10, const T &p11, const T &p12)
{
	_NN_VECTOR12(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
	return(result);
}

template <class T> std::vector<T> mkvector(const T &p1, const T &p2, const T &p3, const T &p4, const T &p5, const T &p6, const T &p7, const T &p8, const T &p9, const T &p10, const T &p11, const T &p12, const T &p13)
{
	_NN_VECTOR13(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
	return(result);
}

template <class T> std::vector<T> mkvector(const T &p1, const T &p2, const T &p3, const T &p4, const T &p5, const T &p6, const T &p7, const T &p8, const T &p9, const T &p10, const T &p11, const T &p12, const T &p13, const T &p14)
{
	_NN_VECTOR14(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
	return(result);
}

template <class T> std::vector<T> mkvector(const T &p1, const T &p2, const T &p3, const T &p4, const T &p5, const T &p6, const T &p7, const T &p8, const T &p9, const T &p10, const T &p11, const T &p12, const T &p13, const T &p14, const T &p15)
{
	_NN_VECTOR15(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15);
	return(result);
}


// Remove a value from the front of a vector
template <class T> void vector_pop_front(std::vector<T> &vector)
{
	NN_ASSERT(!vector.empty());
	
	vector.erase(vector.begin());
}


// Insert a value at the front of a vector
template <class T> void vector_push_front(std::vector<T> &vector, const T &value)
{
	vector.insert(vector.begin(), 1, value);
}





//============================================================================
//		Containers
//----------------------------------------------------------------------------
// Insert container2 at the front of container 1
template <class T> void prepend(T &container1, const T &container2) 
{
	container1.insert(container1.begin(), container2.begin(), container2.end());
}


// Insert container2 at the end of container 1
template <class T> void append(T &container1, const T &container2) 
{
	container1.insert(container1.end(), container2.begin(), container2.end());
}


// Combine container1+container2
template <class T> T combine(const T &container1, const T &container2) 
{	T	result;

	result = container1;
	append(result, container2);
	
	return(result);
}


// Erase a value from a container
template <class T> bool erase(T &container, const typename T::value_type &value)
{	bool					hadValue;
	typename T::iterator	iter;

	hadValue = false;

	while (true)
		{
		iter = find(container.begin(), container.end(), value);
		if (iter == container.end())
			break;

		container.erase(iter);
		hadValue = true;
		}

	return(hadValue);
}


// Extract the last item in a container
template <class T> typename T::value_type extract_back(T &container)
{	typename T::value_type	 value;

	NN_ASSERT(!container.empty());

	value = container.back();
	container.pop_back();
	
	return(value);
} 


// Extract the first value in a container
template <class T> typename T::value_type extract_front(T &container)
{	typename T::value_type	 value;

	NN_ASSERT(!container.empty());

	value = container.front();
	container.erase(container.begin());
	
	return(value);
}


// Search a container
template <class T> bool find(const T &container, const typename T::value_type &value)
{
	return(std::find(container.begin(), container.end(), value) != container.end());
}


// Reverse a container
template <class T> void reverse(T &container) 
{
	std::reverse(container.begin(), container.end());
}


// Sort a container
template <class T, class C> void sort(T &container, const C &comparator) 
{
	std::sort(container.begin(), container.end(), comparator);
}

template <class T> void sort(T &container) 
{
	std::sort(container.begin(), container.end());
}


// Remove duplicates from container
template <class T> void uniquify(T &container, bool preserveOrder=true)
{	T											theResult;
	typename T::iterator						theIter;
	std::map<typename T::value_type, bool>		theMap;


	// Use a temporary map to preserve the order
	if (preserveOrder)
		{
		for (theIter = container.begin(); theIter != container.end(); theIter++)
			{
			if (theMap.find(*theIter) == theMap.end())
				{
				theResult.push_back(*theIter);
				theMap[*theIter] = true;
				}
			}
		container = theResult;
		}
	
	
	// Or remove them via a sort
	else
		{
						std::sort(	container.begin(), container.end());
		container.erase(std::unique(container.begin(), container.end()), container.end());
		}
}





#endif // NSTLUTILITIES_HDR




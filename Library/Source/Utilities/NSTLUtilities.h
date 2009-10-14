/*	NAME:
		NSTLUtilities.h

	DESCRIPTION:
		STL utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSTLUTILITIES_HDR
#define NSTLUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <algorithm>
#include <vector>
#include <list>

#include "NSystemUtilities.h"





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
// Create a vector from an array
template <class T> std::vector<T> _vector_create(UInt32 numValues, const T *theValues) 
{
	return(std::vector<T>(theValues, theValues + numValues));
} 

#define vector_create(_a)			 _vector_create(GET_ARRAY_SIZE(_a), (_a))


// Insert value at the front of vector
template <class T> void vector_push_front(std::vector<T> &vector, const T &value)
{
	vector.insert(vector.begin(), 1, value);
} 





// Reverse a container
template <class T> void reverse(T &container) 
{
	std::reverse(container.begin(), container.end());
}


// Search a container
template <class T> bool contains(const T &container, const typename T::value_type &value) 
{
	return(std::find(container.begin(), container.end(), value) != container.end());
}


// Append container2 to the end of container 1
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





// Extract the first value in a container
template <class T> typename T::value_type extract_front(T &container)
{	typename T::value_type	 value;

	NN_ASSERT(!container.empty());

	value = container.front();
	container.erase(container.begin());
	
	return(value);
} 


// Extract the last item in a container
template <class T> typename T::value_type extract_back(T &container)
{	typename T::value_type	 value;

	NN_ASSERT(!container.empty());

	value = container.back();
	container.pop_back();
	
	return(value);
} 




#endif // NSTLUTILITIES_HDR




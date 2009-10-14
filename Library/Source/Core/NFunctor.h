/*	NAME:
		NFunctor.h

	DESCRIPTION:
		Functor support.

	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NFUNCTOR_HDR
#define NFUNCTOR_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <tr1/functional>
#include <vector>





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// Primitives
//
// A "functor" is a self-contained object that can represent an arbitrary
// function.
//
// To produce a functor, an arbitrary function+parameter list are "bound"
// into a self-contained object that can be invoked later to perform that
// function.
//
// Either TR1 or Boost would be suitable, so we select one of them here.
#define _nfunctor_function											std::tr1::function
#define _nfunctor_bind(_func, ...)									std::tr1::bind(&_func, ##__VA_ARGS__)


// Bindings
//
// Three macros have been provided to simplify the bind syntax:
//
//		BindFunction				Bind an arbitrary function
//		BindMethod					Bind a method of an   object
//		BindSelf					Bind a method of this object
//
// Functors can be bound on either a traditional static method, or an
// arbitrary method of an object.
//
// If bound to a method's object, the functor will invoke the method
// on the specified object when it is executed.
//
// We rely on a gcc extension, whereby ##__VA_ARGS__ will swallow the
// comma if __VA_ARGS__ is empty.
#define BindFunction(_function,        ...)							_nfunctor_bind(_function,        ##__VA_ARGS__)
#define BindMethod(  _object, _method, ...)							_nfunctor_bind(_method, _object, ##__VA_ARGS__)
#define BindSelf(    _method,          ...)							_nfunctor_bind(_method, this,    ##__VA_ARGS__)





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Functors
typedef _nfunctor_function<void (void)>								NFunctor;


// Lists
typedef std::vector<NFunctor>										NFunctorList;
typedef NFunctorList::iterator										NFunctorListIterator;
typedef NFunctorList::const_iterator								NFunctorListConstIterator;
typedef NFunctorList::reverse_iterator								NFunctorListReverseIterator;




#endif // NFUNCTOR_HDR


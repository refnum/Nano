/*	NAME:
		NFunctor.h

	DESCRIPTION:
		Functor object.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NFUNCTOR_HDR
#define NFUNCTOR_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#if NN_TARGET_MAC || NN_TARGET_IOS
	#include <tr1/functional>
#else
	#include <functional>
#endif





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// Primitives
//
// A "functor" is a self-contained object that represents a function.
//
// To produce a functor, a function and a parameter list can be "bound" into
// a self-contained object which will perform that function when invoked.
//
// Parameters can be actual values, or "placeholders" (_1 to _9) that stand in
// for the parameter supplied during invocation:
//
//			void MyFunction(int a, int b);
//
//			nfunctor func1 = nbind(MyFunction, 55, _1);
//			nfunctor func2 = nbind(MyFunction, 55, 66);
//
//			func1(33);			// Equivalent to MyFunction(55, 33);
//			func2();			// Equivalent to MyFunction(55, 66);
//
// Function binding is extremely flexible, and allows the invoker to provide
// a parameter list that is combined with the parameters supplied when binding
// to produce the final parameter list.
//
// These parameters can be re-ordered, replace bind-time values, or be ignored:
// the only requirement is that the final parameter list is type-compatible
// with the bound function (which is detected at compile time).
using namespace std::tr1::placeholders;

#define nfunctor													std::tr1::function
#define nbind(_func, ...)											std::tr1::bind(&_func, ##__VA_ARGS__)


// Bindings
//
// Three macros have been provided to simplify the bind syntax:
//
//		BindFunction				Bind an arbitrary function
//		BindMethod					Bind a method of an   object
//		BindSelf					Bind a method of this object
//
// Bound methods can be a static method, or an arbitrary method of an object.
//
// If bound to a method's object, the functor will invoke the method on the
// specified object when it is executed.
//
// We rely on a gcc extension, whereby ##__VA_ARGS__ will swallow the comma
// if __VA_ARGS__ is empty.
#define BindFunction(_function,        ...)							nbind(_function,        ##__VA_ARGS__)
#define BindMethod(  _object, _method, ...)							nbind(_method, _object, ##__VA_ARGS__)
#define BindSelf(    _method,          ...)							nbind(_method, this,    ##__VA_ARGS__)





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Functors
typedef nfunctor<void (void)>										NFunctor;


// Lists
typedef std::vector<NFunctor>										NFunctorList;
typedef NFunctorList::iterator										NFunctorListIterator;
typedef NFunctorList::const_iterator								NFunctorListConstIterator;
typedef NFunctorList::reverse_iterator								NFunctorListReverseIterator;





//============================================================================
//		Function prototypes
//----------------------------------------------------------------------------
// The "null" functor
void FunctorNull(void);


// Execute a list of functors
void FunctorList(const NFunctorList &theList);




#endif // NFUNCTOR_HDR


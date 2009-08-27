/*	NAME:
		NUTIRegistry.h

	DESCRIPTION:
		UTI Registry.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NUTIREGISTRY_HDR
#define NUTIREGISTRY_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NLock.h"
#include "NUTI.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// UTI info
typedef struct {
	NUTITagMap		theTags;
	NStringList		conformsTo;
} NUTIInfo;


// Lists
typedef std::map<NString, NUTIInfo, NStringHashCompare>				NUTIInfoMap;
typedef NUTIInfoMap::iterator										NUTIInfoMapIterator;
typedef NUTIInfoMap::const_iterator									NUTIInfoMapConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NUTIRegistry {
public:
										NUTIRegistry(void);
	virtual							   ~NUTIRegistry(void);


	// Get a UTI
	NString								GetUTI(NUTITagClass theClass, const NString &theTag);


	// Get a UTI's state
	NUTITagMap							GetTags(      const NString &theUTI);
	NString								GetTagValue(  const NString &theUTI, NUTITagClass theClass);
	NStringList							GetConformsTo(const NString &theUTI);


	// Add a UTI
	void								AddUTI(	const NString		&theUTI,
												const NString		&conformsTo,
												      NUTITagClass	 theClass,
												const NString		&theTag);


	// Add UTI state
	void								AddTag(       const NString &theUTI, NUTITagClass theClass, const NString &theTag);
	void								AddConformsTo(const NString &theUTI, const NString &conformsTo);


    // Get the registry
    static NUTIRegistry					*Get(void);


private:
	void								InitializeRegistry(void);


private:
	NMutexLock							mLock;
	NUTIInfoMap							mInfo;
};





#endif // NUTIREGISTRY_HDR

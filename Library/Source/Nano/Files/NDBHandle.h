/*	NAME:
		NDBHandle.h

	DESCRIPTION:
		Database handle.

	COPYRIGHT:
		Copyright (c) 2006-2021, refNum Software
		All rights reserved.

		Redistribution and use in source and binary forms, with or without
		modification, are permitted provided that the following conditions
		are met:
		
		1. Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.
		
		2. Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.
		
		3. Neither the name of the copyright holder nor the names of its
		contributors may be used to endorse or promote products derived from
		this software without specific prior written permission.
		
		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
		"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
		LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
		A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
		HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
		DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
		THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
		(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	___________________________________________________________________________
*/
#ifndef NDBHANDLE_H
#define NDBHANDLE_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NFile.h"
#include "NMap.h"
#include "NProgressable.h"
#include "NTime.h"

// System
#include <functional>
#include <vector>





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// DB flags
using NDBFlags                                              = uint8_t;

inline constexpr NDBFlags kNDBNone                          = 0;
inline constexpr NDBFlags kNDBReadOnly                      = (1 << 0);





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declarations
class NDBQuery;
class NDBResult;
class NString;
class NAny;
class NData;


// DB types
using NDBStatus    = int32_t;
using NDBHandleRef = void*;
using NDBQueryRef  = void*;


// Functions
using NFunctionDBResult = std::function<void(const NDBResult& theRow)>;


// Containers
using NVectorDBQuery = std::vector<NDBQueryRef>;
using NDBQueryCache  = NMap<NString, NVectorDBQuery>;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NDBHandle final : public NProgressable
{
public:
										NDBHandle();
	virtual                            ~NDBHandle();

										NDBHandle(const NDBHandle& otherDB) = delete;
	NDBHandle&                          operator=(const NDBHandle& otherDB) = delete;

										NDBHandle(const NDBHandle&& otherDB) = delete;
	NDBHandle&                          operator=(const NDBHandle&& otherDB) = delete;


	// Is the handle open?
	bool                                IsOpen() const;


	// Is the database mutable?
	bool                                IsMutable() const;


	// Does a table exist?
	bool                                HasTable(const NString& theTable);


	// Open/close the database
	NStatus                             Open(const NFile& theFile, NDBFlags theFlags = kNDBNone, const NString& theVFS = "");
	void                                Close();


	// Get the database file
	NFile                               GetFile() const;


	// Create a backup of the database
	//
	// The database will be locked for the duration of the backup.
	NStatus                             CreateBackup(const NFile& dstFile);


	// Execute a query
	//
	// Although a database may be manipulated by multiple threads at a time,
	// each handle to the database must be used by one thread at a time.
	//
	// If the database is busy the current thread will be blocked until it
	// can execute its query or the timeout occurs.
	NStatus                             Execute(const NDBQuery&          theQuery,
												const NFunctionDBResult& theResult = nullptr,
												NInterval                waitFor   = kNTimeForever);


	// Execute a return-one-value query
	//
	// Returns the first value of the query result, or 0/empty on failure.
	int64_t                             ExecuteInt64(  const NDBQuery& theQuery);
	float64_t                           ExecuteFloat64(const NDBQuery& theQuery);
	NString                             ExecuteString( const NDBQuery& theQuery);
	NData                               ExecuteData(   const NDBQuery& theQuery);


	// Cancel the current operation
	void                                Cancel();



private:
	NDBStatus                           SQLiteExecute(const NDBQuery&          theQuery,
													  const NFunctionDBResult& theResult,
													  NInterval                waitFor);

	NDBQueryRef                         SQLiteAcquireQuery(const NString& theSQL);
	void                                SQLiteReleaseQuery(const NString& theSQL, NDBQueryRef dbQuery);
	NDBQueryRef                         SQLiteCreateQuery( const NString& theSQL);
	void                                SQLiteDestroyQuery(NDBQueryRef dbQuery);
	void                                SQLiteDestroyQueries();

	void                                SQLiteBindParameters(      NDBQueryRef dbQuery, const NAny& theParameters);
	void                                SQLiteBindParameterByIndex(NDBQueryRef dbQuery, size_t theIndex, const       NAny& theValue);
	void                                SQLiteBindParameterByKey(  NDBQueryRef dbQuery, const NString& theKey, const NAny& theValue);

	NStatus                             SQLiteGetStatus(NDBStatus dbErr);

	void                                SQLiteProgressBegin();
	void                                SQLiteProgressEnd();
	static int                          SQLiteProgressContinue(void* userData);


private:
	NFile                               mFile;
	NDBFlags                            mFlags;
	NDBHandleRef                        mHandle;
	NDBQueryCache                       mQueries;
};



#endif // NDBHANDLE_H

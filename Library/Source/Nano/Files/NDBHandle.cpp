/*	NAME:
		NDBHandle.cpp

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
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NDBHandle.h"

// Nano
#include "NArray.h"
#include "NDBQuery.h"
#include "NDBResult.h"
#include "NDictionary.h"
#include "NFormat.h"
#include "NFunction.h"
#include "NStdAlgorithm.h"
#include "NThread.h"
#include "NTimeUtils.h"
#include "Nano_sqlite3.h"





//=============================================================================
//		Internal Macros
//-----------------------------------------------------------------------------
#define mDatabase                                           reinterpret_cast<sqlite3*>(mHandle)





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
inline constexpr NInterval kNExecuteSleep                   = 0.1;
inline constexpr NInterval kNBackupSleep                    = 0.5;
inline constexpr int       kNBackupStep                     = 128;
inline constexpr int       kNProgressStep                   = 1000;





//=============================================================================
//		NDBHandle::NDBHandle : Constructor.
//-----------------------------------------------------------------------------
NDBHandle::NDBHandle()
	: mFile()
	, mAccess(NFileAccess::ReadOnly)
	, mHandle(nullptr)
{
}





//=============================================================================
//		NDBHandle::~NDBHandle : Destructor.
//-----------------------------------------------------------------------------
NDBHandle::~NDBHandle()
{


	// Clean up
	if (IsOpen())
	{
		Close();
	}
}





//=============================================================================
//		NDBHandle::IsOpen : Is the database open?
//-----------------------------------------------------------------------------
bool NDBHandle::IsOpen() const
{


	// Get our state
	return mHandle != nullptr;
}





//=============================================================================
//		NDBHandle::IsMutable : Is the database mutable?
//-----------------------------------------------------------------------------
bool NDBHandle::IsMutable() const
{


	// Get our state
	return mAccess != NFileAccess::ReadOnly;
}





//=============================================================================
//		NDBHandle::HasTable : Does a table exist?
//-----------------------------------------------------------------------------
bool NDBHandle::HasTable(const NString& theTable)
{


	// Check for the table
	//
	// SQLite does not support "IF NOT EXISTS" for virtual tables, however
	// we can identify the presence of tables by querying the master table.
	NString theSQL  = NFormat("SELECT count() FROM sqlite_master WHERE name=\"{}\";", theTable);
	int64_t numRows = ExecuteInt64(theSQL);

	return numRows != 0;
}





//=============================================================================
//		NDBHandle::Open : Open the database.
//-----------------------------------------------------------------------------
NStatus NDBHandle::Open(const NFile& theFile, NFileAccess theAccess, const NString& theVFS)
{


	// Validate our state
	NN_REQUIRE(!IsOpen());



	// Get the state we need
	const char* vfsName  = theVFS.IsEmpty() ? nullptr : theVFS.GetUTF8();
	NFilePath   thePath  = theFile.GetPath();
	sqlite3*    sqlDB    = nullptr;
	int         sqlFlags = 0;

	if (theAccess == NFileAccess::ReadOnly)
	{
		sqlFlags |= SQLITE_OPEN_READONLY;
	}
	else
	{
		sqlFlags |= SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
	}



	// Open the database
	NDBStatus dbErr = sqlite3_open_v2(thePath.GetUTF8(), &sqlDB, sqlFlags, vfsName);
	if (dbErr != SQLITE_OK)
	{
		NN_LOG_ERROR("Failed to open '{}' due to {}", theFile, sqlite3_errmsg(sqlDB));
	}



	// Update our state
	if (dbErr == SQLITE_OK)
	{
		mFile   = theFile;
		mAccess = theAccess;
		mHandle = sqlDB;
	}

	return SQLiteGetStatus(dbErr);
}





//=============================================================================
//		NDBHandle::Close : Close the database.
//-----------------------------------------------------------------------------
void NDBHandle::Close()
{


	// Validate our state
	NN_REQUIRE(IsOpen());



	// Clean up
	SQLiteDestroyQueries();



	// Close the database
	NDBStatus dbErr = sqlite3_close(mDatabase);
	if (dbErr != SQLITE_OK)
	{
		NN_LOG_ERROR("Failed to close '{}' due to {}", mFile, sqlite3_errmsg(mDatabase));
	}



	// Reset our state
	mFile.Clear();
	mAccess = NFileAccess::ReadOnly;
	mHandle = nullptr;
}





//=============================================================================
//		NDBHandle::GetFile : Get the file.
//-----------------------------------------------------------------------------
NFile NDBHandle::GetFile() const
{


	// Get the file
	return mFile;
}





//=============================================================================
//		NDBHandle::CreateBackup : Create a backup.
//-----------------------------------------------------------------------------
NStatus NDBHandle::CreateBackup(const NFile& dstFile)
{


	// Open the destination database
	int             sqlFlags  = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
	NFilePath       thePath   = dstFile.GetPath();
	sqlite3*        dstDB     = nullptr;
	sqlite3_backup* sqlBackup = nullptr;

	NDBStatus dbErr = sqlite3_open_v2(thePath.GetUTF8(), &dstDB, sqlFlags, nullptr);
	if (dbErr == SQLITE_OK)
	{
		sqlBackup = sqlite3_backup_init(dstDB, "main", mDatabase, "main");
	}

	if (dbErr != SQLITE_OK || sqlBackup == nullptr)
	{
		NN_LOG_ERROR("Failed to open '{}' due to {}", thePath, sqlite3_errmsg(dstDB));
	}



	// Perform the backup
	if (sqlBackup != nullptr)
	{
		BeginProgress();

		do
		{
			// Copy some pages
			dbErr          = sqlite3_backup_step(sqlBackup, kNBackupStep);
			int pagesTotal = sqlite3_backup_pagecount(sqlBackup);
			int pagesLeft  = sqlite3_backup_remaining(sqlBackup);
			int pagesDone  = pagesTotal - pagesLeft;

			NN_REQUIRE(pagesTotal >= 0);
			NN_REQUIRE(pagesDone >= 0);

			UpdateProgress(size_t(pagesDone), size_t(pagesTotal));


			// Give up some time
			if (dbErr == SQLITE_BUSY || dbErr == SQLITE_LOCKED)
			{
				NThread::Sleep(kNBackupSleep);
			}
		} while (dbErr == SQLITE_OK || dbErr == SQLITE_BUSY || dbErr == SQLITE_LOCKED);

		(void) sqlite3_backup_finish(sqlBackup);
		EndProgress();
	}



	// Clean up
	if (dstDB != nullptr)
	{
		dbErr = sqlite3_close(dstDB);
		if (dbErr != SQLITE_OK)
		{
			NN_LOG_ERROR("Failed to close '{}' due to {}", thePath, sqlite3_errmsg(dstDB));
		}
	}

	return SQLiteGetStatus(dbErr);
}





//=============================================================================
//		NDBHandle::Execute : Execute a query.
//-----------------------------------------------------------------------------
NStatus NDBHandle::Execute(const NDBQuery&          theQuery,
						   const NFunctionDBResult& theResult,
						   NInterval                waitFor)
{


	// Validate our state
	NN_REQUIRE(IsOpen());



	// Prepare to execute
	SQLiteProgressBegin();

	NTime     endTime = NTimeUtils::GetAbsolute(waitFor);
	NDBStatus dbErr   = SQLITE_OK;
	bool      areDone = false;



	// Execute the query
	//
	// SQLITE_LOCKED may be returned when using a shared page cache.
	//
	// If so then, unlike SQLITE_BUSY, we need to retry the entire query,
	// rather than the current statement.
	//
	// Since sqlite3_unlock_notify only supports a single pending unlock
	// notification each Execute() performs its own try-sleep-try loop
	// until it succeeds or hits some limit to allow multiple threads to
	// retry in parallel.
	while (!areDone)
	{
		dbErr = SQLiteExecute(theQuery, theResult, waitFor);
		switch (dbErr)
		{
			case SQLITE_LOCKED:
				areDone = NTimeUtils::IsAfter(endTime);
				if (!areDone)
				{
					NThread::Sleep(kNExecuteSleep);
				}
				break;

			default:
				areDone = true;
				break;
		}
	}



	// Finish executing
	SQLiteProgressEnd();

	return SQLiteGetStatus(dbErr);
}





//=============================================================================
//		NDBHandle::ExecuteInt64 : Execute a query to obtain an int64_t.
//-----------------------------------------------------------------------------
int64_t NDBHandle::ExecuteInt64(const NDBQuery& theQuery)
{


	// Validate our state
	NN_REQUIRE(IsOpen());



	// Execute the query
	int64_t theValue = 0;
	NStatus theErr   = Execute(theQuery, NBind(NDBResult::GetRowInt64, kNArg1, 0, &theValue));
	NN_REQUIRE_NOT_ERR(theErr);

	return theValue;
}





//=============================================================================
//		NDBHandle::ExecuteFloat64 : Execute a query to obtain a float64_t.
//-----------------------------------------------------------------------------
float64_t NDBHandle::ExecuteFloat64(const NDBQuery& theQuery)
{


	// Validate our state
	NN_REQUIRE(IsOpen());



	// Execute the query
	float64_t theValue = 0.0;
	NStatus   theErr   = Execute(theQuery, NBind(NDBResult::GetRowFloat64, kNArg1, 0, &theValue));
	NN_REQUIRE_NOT_ERR(theErr);

	return theValue;
}





//=============================================================================
//		NDBHandle::ExecuteString : Execute a query to obtain a string.
//-----------------------------------------------------------------------------
NString NDBHandle::ExecuteString(const NDBQuery& theQuery)
{


	// Validate our state
	NN_REQUIRE(IsOpen());



	// Execute the query
	NString theValue;
	NStatus theErr = Execute(theQuery, NBind(NDBResult::GetRowString, kNArg1, 0, &theValue));
	NN_REQUIRE_NOT_ERR(theErr);

	return theValue;
}





//=============================================================================
//		NDBHandle::ExecuteData : Execute a query to obtain data.
//-----------------------------------------------------------------------------
NData NDBHandle::ExecuteData(const NDBQuery& theQuery)
{


	// Validate our state
	NN_REQUIRE(IsOpen());



	// Execute the query
	NData   theValue;
	NStatus theErr = Execute(theQuery, NBind(NDBResult::GetRowData, kNArg1, 0, &theValue));
	NN_REQUIRE_NOT_ERR(theErr);

	return theValue;
}





//=============================================================================
//		NDBHandle::Cancel : Cancel the current operation.
//-----------------------------------------------------------------------------
void NDBHandle::Cancel()
{


	// Validate our state
	NN_REQUIRE(IsOpen());



	// Cancel the operation
	sqlite3_interrupt(mDatabase);
}





#pragma mark private
//=============================================================================
//		NDBHandle::SQLiteExecute : Execute a query.
//-----------------------------------------------------------------------------
NDBStatus NDBHandle::SQLiteExecute(const NDBQuery&          theQuery,
								   const NFunctionDBResult& theResult,
								   NInterval                waitFor)
{


	// Get the query
	NString theSQL = theQuery.GetSQL();
	NN_REQUIRE(!theSQL.IsEmpty());

	NDBQueryRef   dbQuery  = SQLiteAcquireQuery(theSQL);
	sqlite3_stmt* sqlQuery = reinterpret_cast<sqlite3_stmt*>(dbQuery);

	if (sqlQuery == nullptr)
	{
		return SQLITE_INTERNAL;
	}

	SQLiteBindParameters(sqlQuery, theQuery.GetParameters());



	// Execute the query
	NTime     endTime = NTimeUtils::GetAbsolute(waitFor);
	NDBStatus dbErr   = SQLITE_OK;
	bool      areDone = false;

	while (!areDone)
	{
		// Execute the query
		dbErr = sqlite3_step(sqlQuery);

		switch (dbErr)
		{
			case SQLITE_ROW:
				if (theResult != nullptr)
				{
					theResult(NDBQueryRef(sqlQuery));
				}
				break;

			case SQLITE_BUSY:
				areDone = NTimeUtils::IsAfter(endTime);
				if (!areDone)
				{
					NThread::Sleep(kNExecuteSleep);
				}
				break;

			default:
				areDone = true;
				break;
		}
	}



	// Clean up
	SQLiteReleaseQuery(theSQL, dbQuery);

	return dbErr;
}





//=============================================================================
//		NDBHandle::SQLiteAcquireQuery : Acquire an SQLite query.
//-----------------------------------------------------------------------------
NDBQueryRef NDBHandle::SQLiteAcquireQuery(const NString& theSQL)
{


	// Validate our parameters
	NN_REQUIRE(!theSQL.IsEmpty());


	// Check the cache
	//
	// We maintain a cache of SQL to SQLite statements, allowing us to reuse
	// the statement if the SQL is the same.
	//
	// This can be a significant optimisation when performing large numbers
	// of identical statements (e.g., populating a DB where only the bound
	// parameters are changed on each execution).
	auto theIter = mQueries.find(theSQL);
	if (theIter != mQueries.end())
	{
		auto& theQueries = theIter.value();
		if (!theQueries.empty())
		{
			return nstd::extract_back(theQueries);
		}
	}



	// Create a new query
	return SQLiteCreateQuery(theSQL);
}





//=============================================================================
//		NDBHandle::SQLiteReleaseQuery : Release an SQLite query.
//-----------------------------------------------------------------------------
void NDBHandle::SQLiteReleaseQuery(const NString& theSQL, NDBQueryRef dbQuery)
{


	// Validate our parameters
	NN_REQUIRE(!theSQL.IsEmpty());
	NN_REQUIRE(dbQuery != nullptr);



	// Reset the statement
	sqlite3_stmt* sqlQuery = reinterpret_cast<sqlite3_stmt*>(dbQuery);
	NDBStatus     dbErr    = SQLITE_OK;

	dbErr = sqlite3_reset(sqlQuery);
	NN_EXPECT_NOT_ERR(dbErr);

	dbErr = sqlite3_clear_bindings(sqlQuery);
	NN_EXPECT_NOT_ERR(dbErr);



	// Update the cache
	mQueries[theSQL].emplace_back(dbQuery);
}





//=============================================================================
//		NDBHandle::SQLiteCreateQuery : Get an SQLite query.
//-----------------------------------------------------------------------------
NDBQueryRef NDBHandle::SQLiteCreateQuery(const NString& theSQL)
{


	// Validate our parameters and state
	NN_REQUIRE(!theSQL.IsEmpty());
	NN_REQUIRE(IsOpen());



	// Validate the query
	//
	// Since we use prepared statement, queries can only contain a single SQL
	// statement - any subsequent statements would be ignored by sqlite.
	if constexpr (NN_DEBUG)
	{
		if (theSQL.FindAll(";").size() > 1 || theSQL.Find(";").GetLocation() != theSQL.GetSize())
		{
			NN_LOG_WARNING("Ignoring additional SQL statements in '{}'", theSQL);
		}
	}



	// Create the query
	//
	// Preparing a query may require a lock, so we may need to spin if the
	// database is currently locked.
	//
	// Ideally this would also honour the timeout passed to Execute, but this
	// happens rarely enough that for now we treat it as uninterruptable.
	const char*   textUTF8 = theSQL.GetUTF8();
	int           textLen  = int(strlen(textUTF8));
	sqlite3_stmt* sqlQuery = nullptr;
	const char*   sqlTail  = nullptr;
	NDBStatus     dbErr    = SQLITE_OK;

	do
	{
		dbErr = sqlite3_prepare_v2(mDatabase, textUTF8, textLen, &sqlQuery, &sqlTail);
		if (dbErr == SQLITE_BUSY)
		{
			NThread::Sleep(kNExecuteSleep);
		}
	} while (dbErr == SQLITE_BUSY);



	// Handle failure
	if (dbErr != SQLITE_OK)
	{
		NN_LOG_ERROR("Failed to create statement for '{}' due to {}",
					 theSQL,
					 sqlite3_errmsg(mDatabase));
	}

	return sqlQuery;
}





//=============================================================================
//		NDBHandle::SQLiteDestroyQuery : Destroy an SQLite query.
//-----------------------------------------------------------------------------
void NDBHandle::SQLiteDestroyQuery(NDBQueryRef dbQuery)
{


	// Validate our parameters and state
	NN_REQUIRE(dbQuery != nullptr);
	NN_REQUIRE(IsOpen());



	// Destroy the query
	sqlite3_stmt* sqlQuery = reinterpret_cast<sqlite3_stmt*>(dbQuery);
	NDBStatus     dbErr    = SQLITE_OK;

	do
	{
		dbErr = sqlite3_finalize(sqlQuery);
		if (dbErr != SQLITE_OK)
		{
			NThread::Sleep(kNExecuteSleep);
		}
	} while (dbErr == SQLITE_BUSY || dbErr == SQLITE_LOCKED);

	NN_REQUIRE(dbErr == SQLITE_OK);
}





//=============================================================================
//		NDBHandle::SQLiteDestroyQueries : Destroy any cached queries.
//-----------------------------------------------------------------------------
void NDBHandle::SQLiteDestroyQueries()
{


	// Validate our state
	NN_REQUIRE(IsOpen());


	// Destroy the queries
	for (auto& sqlQueries : mQueries)
	{
		for (NDBQueryRef theQuery : sqlQueries.second)
		{
			SQLiteDestroyQuery(theQuery);
		}
	}

	mQueries.clear();
}





//=============================================================================
//		NDBHandle::SQLiteBindParameters : Bind the query parameters.
//-----------------------------------------------------------------------------
void NDBHandle::SQLiteBindParameters(NDBQueryRef dbQuery, const NAny& theParameters)
{


	// Validate our parameters and state
	NN_REQUIRE(dbQuery != nullptr);
	NN_REQUIRE(IsOpen());



	// Bind indexed parameters
	if (theParameters.Is<NArray>())
	{
		NArray theArray = theParameters.Get<NArray>();

		for (size_t n = 0; n < theArray.GetSize(); n++)
		{
			SQLiteBindParameterByIndex(dbQuery, n + 1, theArray[n]);
		}
	}



	// Bind keyed parameters
	else if (theParameters.Is<NDictionary>())
	{
		NDictionary theDictionary = theParameters.Get<NDictionary>();

		for (const auto& keyValue : theDictionary)
		{
			SQLiteBindParameterByKey(dbQuery, keyValue.first, keyValue.second);
		}
	}
}





//=============================================================================
//		NDBHandle::SQLiteBindParameterByIndex : Bind a parameter to a query.
//-----------------------------------------------------------------------------
void NDBHandle::SQLiteBindParameterByIndex(NDBQueryRef dbQuery,
										   size_t      theIndex,
										   const NAny& theValue)
{


	// Validate our parameters and state
	NN_REQUIRE(dbQuery != nullptr);
	NN_REQUIRE(theIndex <= size_t(std::numeric_limits<int>::max()));
	NN_REQUIRE(!theValue.IsEmpty());
	NN_REQUIRE(IsOpen());



	// Get the state we need
	sqlite3_stmt* sqlQuery = reinterpret_cast<sqlite3_stmt*>(dbQuery);
	NDBStatus     dbErr    = SQLITE_OK;



	// Bind the parameter
	NN_DIAGNOSTIC_PUSH();
	NN_DIAGNOSTIC_IGNORE_CLANG("-Wold-style-cast");

	NNumber theNumber;
	int     sqlIndex = int(theIndex);

	if (theNumber.SetValue(theValue))
	{
		if (theNumber.IsInteger())
		{
			dbErr = sqlite3_bind_int64(sqlQuery, sqlIndex, theNumber.GetInt64());
		}
		else
		{
			dbErr = sqlite3_bind_double(sqlQuery, sqlIndex, theNumber.GetFloat64());
		}
	}

	else if (theValue.IsBool())
	{
		dbErr = sqlite3_bind_int(sqlQuery, sqlIndex, theValue.GetBool() ? 1 : 0);
	}

	else if (theValue.IsString())
	{
		NString     theString = theValue.GetString();
		const char* textUTF8  = theValue.GetString().GetUTF8();
		int         textLen   = int(strlen(textUTF8));

		dbErr = sqlite3_bind_text(sqlQuery, sqlIndex, textUTF8, textLen, SQLITE_TRANSIENT);
	}

	else if (theValue.IsData())
	{
		NData theData = theValue.GetData();

		dbErr = sqlite3_bind_blob(sqlQuery,
								  sqlIndex,
								  theData.GetData(),
								  int(theData.GetSize()),
								  SQLITE_TRANSIENT);
	}
	else
	{
		NN_LOG_ERROR("Unknown parameter type '{}' at index {}",
					 theValue.GetType().name(),
					 theIndex);
	}

	NN_DIAGNOSTIC_POP();



	// Handle failure
	if (dbErr != SQLITE_OK)
	{
		NN_LOG_ERROR("Failed to bind parameter {} due to {}", theIndex, sqlite3_errmsg(mDatabase));
	}
}





//=============================================================================
//		NDBHandle::SQLiteBindParameterByKey : Bind a parameter to a query.
//-----------------------------------------------------------------------------
void NDBHandle::SQLiteBindParameterByKey(NDBQueryRef    dbQuery,
										 const NString& theKey,
										 const NAny&    theValue)
{


	// Validate our parameters and state
	NN_REQUIRE(dbQuery != nullptr);
	NN_REQUIRE(!theKey.IsEmpty());
	NN_REQUIRE(!theValue.IsEmpty());
	NN_REQUIRE(IsOpen());



	// Get the index
	//
	// Unused parameters are silently ignored.
	//
	// This allows a single dictionary to be used for multiple queries,
	// where each query may use a different subset of the keys.
	sqlite3_stmt* sqlQuery = reinterpret_cast<sqlite3_stmt*>(dbQuery);
	NString       keyName  = ":" + theKey;

	int sqlIndex = sqlite3_bind_parameter_index(sqlQuery, keyName.GetUTF8());
	NN_REQUIRE(sqlIndex >= 0);

	if (sqlIndex == 0)
	{
		return;
	}



	// Bind the parameter
	SQLiteBindParameterByIndex(dbQuery, size_t(sqlIndex), theValue);
}





//=============================================================================
//		NDBHandle::SQLiteGetStatus : Convert an SQLite status to an NStatus.
//-----------------------------------------------------------------------------
NStatus NDBHandle::SQLiteGetStatus(NDBStatus dbErr)
{


	// Convert the error
	switch (dbErr)
	{
		case SQLITE_OK:
		case SQLITE_DONE:
			return NStatus::OK;
			break;

		case SQLITE_ERROR:
		case SQLITE_INTERNAL:
			return NStatus::Param;
			break;

		case SQLITE_PERM:
		case SQLITE_READONLY:
			return NStatus::Permission;
			break;

		case SQLITE_ABORT:
		case SQLITE_INTERRUPT:
			return NStatus::Cancelled;
			break;

		case SQLITE_BUSY:
			return NStatus::Busy;
			break;

		case SQLITE_LOCKED:
			return NStatus::Locked;
			break;

		case SQLITE_NOMEM:
			return NStatus::Memory;
			break;

		case SQLITE_IOERR:
		case SQLITE_CORRUPT:
			return NStatus::Malformed;
			break;

		case SQLITE_NOTFOUND:
		case SQLITE_CANTOPEN:
			return NStatus::NotFound;
			break;

		default:
			NN_LOG_ERROR("Failed to map error {}", dbErr);
			return NStatus::Internal;
			break;
	}

	NN_UNREACHABLE();
}





//=============================================================================
//		NDBHandle::SQLiteProgressBegin : Begin an SQLite operation.
//-----------------------------------------------------------------------------
void NDBHandle::SQLiteProgressBegin()
{


	// Begin the operation
	sqlite3_progress_handler(mDatabase, kNProgressStep, SQLiteProgressContinue, this);

	BeginProgress(kNProgressUnknown);
}





//=============================================================================
//		NDBHandle::SQLiteProgressEnd : End an SQLite operation.
//-----------------------------------------------------------------------------
void NDBHandle::SQLiteProgressEnd()
{


	// End the operation
	sqlite3_progress_handler(mDatabase, 0, nullptr, nullptr);

	EndProgress();
}





//=============================================================================
//		NDBHandle::SQLiteProgressContinue : SQLite progress callback.
//-----------------------------------------------------------------------------
int NDBHandle::SQLiteProgressContinue(void* userData)
{


	// Update the progress
	NDBHandle* thisPtr = reinterpret_cast<NDBHandle*>(userData);
	NStatus    theErr  = thisPtr->UpdateProgress(kNProgressUnknown);

	return (theErr == NStatus::OK) ? SQLITE_OK : SQLITE_ABORT;
}

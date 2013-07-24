/*	NAME:
		NWindowsSystem.cpp

	DESCRIPTION:
		Windows system support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <tchar.h>
#include <wincodec.h>
#include <wincodecsdk.h>

#include "NSTLUtilities.h"
#include "NTimeUtilities.h"
#include "NWindows.h"
#include "NBundle.h"
#include "NRegistry.h"
#include "NTargetFile.h"
#include "NTargetSystem.h"





//============================================================================
//		Library glue
//----------------------------------------------------------------------------
// SHCreateMemStream
#define INDEX_SHCreateMemStream										(LPCSTR) 12

typedef IStream* (__stdcall *SHCreateMemStreamProc)(const BYTE *pInit, UINT cbInit);





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Buffers
static const NIndex kTaskBufferSize									= 2048;
static const NIndex kLocaleBufferSize								= 10;





//============================================================================
//      Internal types
//----------------------------------------------------------------------------
// Pipe info
typedef struct {
	HANDLE		readHnd;
	HANDLE		writeHnd;
} PipeInfo;





//============================================================================
//      Internal prototypes
//----------------------------------------------------------------------------
static void ClosePipe(NTaskPipeRef &thePipe);





//============================================================================
//      Internal functions
//----------------------------------------------------------------------------
//      CreateMemoryStream : Create a memory stream.
//----------------------------------------------------------------------------
static IStream *CreateMemoryStream(const NData &theData=NData())
{	static SHCreateMemStreamProc		sCreateMemStream = NULL;

	IStream		*theStream;
	HMODULE		hLibrary;



	// Load the function
	if (sCreateMemStream == NULL)
		{
		hLibrary = LoadLibrary(_T("shlwapi.dll"));
		if (hLibrary != NULL)
			sCreateMemStream = (SHCreateMemStreamProc) GetProcAddress(hLibrary, INDEX_SHCreateMemStream);
		}



	// Create the stream
	if (sCreateMemStream != NULL)
		theStream = sCreateMemStream(theData.GetData(), theData.GetSize());
	else
		theStream = NULL;

	return(theStream);
}





//============================================================================
//      CreatePipe : Create a pipe.
//----------------------------------------------------------------------------
static NTaskPipeRef CreatePipe(bool forChildRead)
{	PipeInfo				*theInfo;
	NTaskPipeRef			thePipe;
	SECURITY_ATTRIBUTES		secAttr; 
	BOOL					wasOK;



	// Get the state we need
	secAttr.nLength              = sizeof(secAttr); 
	secAttr.bInheritHandle       = TRUE; 
	secAttr.lpSecurityDescriptor = NULL; 



	// Create the pipe info
	theInfo = new PipeInfo;
	thePipe = (NTaskPipeRef) theInfo;

	if (theInfo == NULL)
		return(kNTaskPipeRefNone);

	theInfo->readHnd  = NULL;
	theInfo->writeHnd = NULL;



	// Create the pipes
	//
	// If the pipe is for our child to read from then the write handle of that pipe
	// must not be inheritable (we want it to stop with our child), and vice-versa.
	wasOK = CreatePipe(&theInfo->readHnd, &theInfo->writeHnd, &secAttr, 0);
	if (wasOK)
		wasOK = SetHandleInformation(forChildRead ? theInfo->writeHnd : theInfo->readHnd, HANDLE_FLAG_INHERIT, 0);



	// Handle failure
	if (!wasOK)
		ClosePipe(thePipe);
	
	return(thePipe);
}





//============================================================================
//      ClosePipe : Close a pipe.
//----------------------------------------------------------------------------
static void ClosePipe(NTaskPipeRef &thePipe)
{	PipeInfo		*theInfo = (PipeInfo *) thePipe;



	// Destroy the pipe
	if (thePipe != kNTaskPipeRefNone)
		{
		WNSafeCloseHandle(theInfo->readHnd);
		WNSafeCloseHandle(theInfo->writeHnd);
	
		delete theInfo;
		thePipe = kNTaskPipeRefNone;
		}
}





//============================================================================
//      GetPipeHandle : Get a pipe handle.
//----------------------------------------------------------------------------
static HANDLE GetPipeHandle(NTaskPipeRef thePipe, bool forRead)
{	PipeInfo		*theInfo = (PipeInfo *) thePipe;



	// Get the handle
	return(forRead ? theInfo->readHnd : theInfo->writeHnd);
}





//============================================================================
//		ReadPipe : Read from a pipe.
//----------------------------------------------------------------------------
static NString ReadPipe(NTaskPipeRef &thePipe, bool lastRead)
{	PipeInfo		*theInfo = (PipeInfo *) thePipe;
	char			theBuffer[kTaskBufferSize];
	DWORD			numAvail, numRead;
	NString			theString;



	// Check the pipe
	if (thePipe == kNTaskPipeRefNone)
		return(theString);



	// Read from the pipe
	do
		{
		// Prepare to read
		numAvail = 0;
		numRead  = 0;



		// Get the size
		if (!PeekNamedPipe(theInfo->readHnd, NULL, 0, NULL, &numAvail, NULL))
			numAvail = 0;
		
		if (numAvail > kTaskBufferSize)
			numAvail = kTaskBufferSize;



		// Read the data
		if (numAvail != 0 && !ReadFile(theInfo->readHnd, theBuffer, numAvail, &numRead, NULL))
			numRead = 0;

		if (numRead >= 1)
			theString += NString(theBuffer, numRead);
		}
	while (numRead >= 1);



	// Clean up
	//
	// If this was the last read from the pipe, close it when done.
	if (lastRead && numRead == 0)
		ClosePipe(thePipe);

	return(theString);
}





//============================================================================
//		WritePipe : Write to a pipe.
//----------------------------------------------------------------------------
static void WritePipe(NTaskPipeRef thePipe, const NString &theText)
{	PipeInfo		*theInfo = (PipeInfo *) thePipe;
	DWORD			numWritten;
	NData			theData;
	BOOL			wasOK;



	// Check the pipe
	if (thePipe == kNTaskPipeRefNone)
		return;



	// Write to the pipe
	theData = theText.GetData();
	wasOK   = WriteFile(theInfo->writeHnd, theData.GetData(), theData.GetSize(), &numWritten, NULL);

	NN_ASSERT(wasOK);
	NN_ASSERT(((NIndex) numWritten) == theData.GetSize());
}





#pragma mark NTargetSystem
//============================================================================
//      NTargetSystem::DebugLog : Emit a debug message.
//----------------------------------------------------------------------------
void NTargetSystem::DebugLog(const char *theMsg)
{	static TCHAR		sLogFilePath[MAX_PATH];
	static bool			sLogFileInited = false;

	DWORD		bytesWritten;
	HANDLE		hFile;



	// Update our state
	if (!sLogFileInited)
		{
		if (!GetEnvironmentVariable(L"NANO_LOG", sLogFilePath, MAX_PATH))
			memset(sLogFilePath, 0x00, sizeof(sLogFilePath));

		sLogFileInited = true;
		}



	// Log the message
	OutputDebugStringA(theMsg);
	
	if (sLogFilePath[0])
		{
		hFile = CreateFile(sLogFilePath, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile != NULL)
			{
			WriteFile(hFile, theMsg, strlen(theMsg), &bytesWritten, NULL);
			CloseHandle(hFile);
			}
		}
}





//============================================================================
//      NTargetSystem::FindBundle : Find a bundle.
//----------------------------------------------------------------------------
NFile NTargetSystem::FindBundle(const NString &bundleID)
{	TCHAR		theBuffer[MAX_PATH];
	NFile		theFile, theParent;



	// Locate the executable
	if (bundleID.IsEmpty())
		{
		if (GetModuleFileName(NULL, theBuffer, MAX_PATH))
			theFile = NFile(ToNN(theBuffer));
		}
	else
		NN_LOG("NTargetSystem::FindBundle not implemented (%@)", bundleID);



	// Locate the bundle
	//
	// If the executable is within a bundle then we return the root bundle
	// folder, otherwise we return the directory containing the executable.
	theFile = theFile.GetParent();
	if (theFile.GetName() == "Windows")
		{
		theParent = theFile.GetParent();
		if (theParent.GetName() == "Contents")
			theFile = theParent.GetParent();
		}

	return(theFile);
}





//============================================================================
//      NTargetSystem::GetOSVersion : Get the OS version.
//----------------------------------------------------------------------------
OSVersion NTargetSystem::GetOSVersion(void)
{	OSVERSIONINFOEX		theInfo;
	OSVersion			theVers;
	


	// Get the state we need
	memset(&theInfo, 0x00, sizeof(theInfo));
	theInfo.dwOSVersionInfoSize = sizeof(theInfo);

	if (!GetVersionEx((LPOSVERSIONINFO) &theInfo))
		return(kOSUnknown);



	// Identify the OS
	theVers = kOSWindows;
	
	if (	theInfo.dwMajorVersion == 6 && theInfo.dwMinorVersion == 2 && theInfo.wProductType == VER_NT_WORKSTATION)
		theVers = kOSWindowsEight;

	else if (theInfo.dwMajorVersion == 6 && theInfo.dwMinorVersion == 1 && theInfo.wProductType == VER_NT_WORKSTATION)
		theVers = kOSWindowsSeven;

	else if (theInfo.dwMajorVersion == 6 && theInfo.dwMinorVersion == 0 && theInfo.wProductType == VER_NT_WORKSTATION)
		theVers = kOSWindowsVista;

	else if (theInfo.dwMajorVersion == 5 && theInfo.dwMinorVersion == 1)
		{
		if (theInfo.wServicePackMajor == 3)
			theVers = kOSWindowsXP_SP3;

		else if (theInfo.wServicePackMajor == 2)
			theVers = kOSWindowsXP_SP2;

		else if (theInfo.wServicePackMajor == 1)
			theVers = kOSWindowsXP_SP1;

		else
			theVers = kOSWindowsXP;
		}
	
	return(theVers);
}





//============================================================================
//		NTargetSystem::GetOSName : Get the OS name.
//----------------------------------------------------------------------------
NString NTargetSystem::GetOSName(void)
{	NString				theResult;
	OSVERSIONINFOEX		versInfo;



	// Get the state we need
	memset(&versInfo, 0x00, sizeof(versInfo));
	versInfo.dwOSVersionInfoSize = sizeof(versInfo);



	// Get the OS name
	if (GetVersionEx((LPOSVERSIONINFO) &versInfo))
		theResult.Format("Windows %ld.%ld.%ld-%ld.%ld-%ld",
							versInfo.dwMajorVersion,
							versInfo.dwMinorVersion,
							versInfo.dwBuildNumber,
							versInfo.wServicePackMajor,
							versInfo.wServicePackMinor,
							versInfo.wProductType);

	return(theResult);
}





//============================================================================
//		NTargetSystem::GetProcessName : Get the process name.
//----------------------------------------------------------------------------
NString NTargetSystem::GetProcessName(void)
{	TCHAR		theBuffer[MAX_PATH];
	NString		theName;
	NFile		theFile;



	// Get the name
	if (GetModuleFileName(NULL, theBuffer, MAX_PATH))
		{
		theFile = NFile(ToNN(theBuffer));
		if (theFile.IsFile())
			theName = theFile.GetName(kNNameNoExtension);
		}

	return(theName);
}





//============================================================================
//		NTargetSystem::GetSystemCPU : Get the clock speed.
//----------------------------------------------------------------------------
uint64_t NTargetSystem::GetSystemCPU(void)
{	NRegistry		theRegistry;
	uint64_t		theResult;
	NStatus			theErr;
	


	// Get the speed
	theResult = 0;
	theErr    = theRegistry.Open(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0");

	if (theErr == kNoErr)
		theResult = ((uint64_t) theRegistry.GetValueInt32("~MHz")) * 1000000;
	
	return(theResult);
}





//============================================================================
//		NTargetSystem::GetSystemRAM : Get the physical memory.
//----------------------------------------------------------------------------
uint64_t NTargetSystem::GetSystemRAM(void)
{	MEMORYSTATUSEX		memInfo;



	// Get the state we need
	memset(&memInfo, 0x00, sizeof(memInfo));
	memInfo.dwLength = sizeof(memInfo);



	// Get the memory
	if (!GlobalMemoryStatusEx(&memInfo))
		memInfo.ullTotalPhys = 0;

	return(memInfo.ullTotalPhys);
}





//============================================================================
//		NTargetSystem::GetSystemArch : Get the system architecture.
//----------------------------------------------------------------------------
NString NTargetSystem::GetSystemArch(void)
{	NString		theArch;



	// Get the architecture
#if defined(_M_IX86)
	theArch = "x86";
	
#else
	UNKNOWN ARCH
#endif

	return(theArch);
}





//============================================================================
//      NTargetSystem::TaskCreate : Create a task.
//----------------------------------------------------------------------------
TaskInfo NTargetSystem::TaskCreate(const NString &theCmd, const NStringList &theArgs)
{	TCHAR					*cmdLinePtr;
	STARTUPINFO				startInfo;
	PROCESS_INFORMATION		procInfo;
	TaskInfo				theTask;
	NString					cmdLine;
	BOOL					wasOK;



	// Get the state we need
	theTask.taskID     = kNTaskIDNone;
	theTask.taskResult = 0;
	
	theTask.stdIn  = kNTaskPipeRefNone;
	theTask.stdOut = kNTaskPipeRefNone;
	theTask.stdErr = kNTaskPipeRefNone;

	memset(&startInfo, 0x00, sizeof(startInfo));
	memset(&procInfo,  0x00, sizeof(procInfo));

	startInfo.cb          = sizeof(STARTUPINFO); 
	startInfo.dwFlags    |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	startInfo.wShowWindow = SW_HIDE;

	cmdLine.Format("\"%@\" \"%@\"", theCmd, NString::Join(theArgs, "\" \""));
	cmdLinePtr = _wcsdup(ToWN(cmdLine));



	// Create the pipes
	theTask.stdIn  = CreatePipe(true);
	theTask.stdOut = CreatePipe(false);
	theTask.stdErr = CreatePipe(false);

	startInfo.hStdError  = GetPipeHandle(theTask.stdErr, false);
	startInfo.hStdOutput = GetPipeHandle(theTask.stdOut, false);
	startInfo.hStdInput  = GetPipeHandle(theTask.stdIn,  true);



	// Create the task
	//
	// The Unicode version of CreateProcess may modify the command
	// line argument, so we pass a mutable copy of the string.
	wasOK = CreateProcess(	NULL,							// App name passed in cmd line
							cmdLinePtr,						// Mutable command line
							NULL,							// Process security attributes 
							NULL,							// Primary thread security attributes 
							TRUE,							// Handles are inherited 
							CREATE_NEW_CONSOLE |			// Create (hidden) console  for TaskSignal
							CREATE_NEW_PROCESS_GROUP,		// Create new process group for TaskSignal
							NULL,							// Use parent's environment 
							NULL,							// Use parent's current directory 
							&startInfo,						// Startup info
							&procInfo);						// Receives process info



	// Get the task info
	//
	// We keep the task handle around to query its exit status, but
	// can close the handle to the task's main thread immediately.
	if (wasOK)
		{
		theTask.taskID = (NTaskID) procInfo.hProcess;
		WNSafeCloseHandle(procInfo.hThread);
		}
	
	
	
	// Clean up
	free(cmdLinePtr);
	
	if (!wasOK)
		TaskDestroy(theTask);
	
	return(theTask);
}





//============================================================================
//      NTargetSystem::TaskDestroy : Destroy a task.
//----------------------------------------------------------------------------
void NTargetSystem::TaskDestroy(TaskInfo &theTask)
{


	// Clean up
	if (theTask.taskID != kNTaskIDNone)
		{
		CloseHandle((HANDLE) theTask.taskID);
		theTask.taskID = kNTaskIDNone;
		}

	ClosePipe(theTask.stdIn);
	ClosePipe(theTask.stdOut);
	ClosePipe(theTask.stdErr);
}





//============================================================================
//      NTargetSystem::TaskUpdate : Update a task.
//----------------------------------------------------------------------------
void NTargetSystem::TaskUpdate(TaskInfo &theTask)
{	DWORD		theStatus;
	BOOL		wasOK;



	// Get the child status
	theStatus = (DWORD) -1;
	wasOK     = GetExitCodeProcess((HANDLE) theTask.taskID, &theStatus);

	if (wasOK && theStatus == STILL_ACTIVE)
		return;



	// Update the task
	theTask.taskID     = kNTaskIDNone;
	theTask.taskResult = theStatus;



	// Free our resources
	//
	// The stdout/err pipes are left open since they may contain buffered data
	// that can still be read. However since the child has exited, no more data
	// can be written to it and so the stdin pipe can be closed.
	ClosePipe(theTask.stdIn);
}





//============================================================================
//      NTargetSystem::TaskRead : Read from a task.
//----------------------------------------------------------------------------
NString NTargetSystem::TaskRead(TaskInfo &theTask, bool stdErr)
{	NString		theResult;
	bool		lastRead;
	


	// Read from the task
	lastRead = (theTask.taskID == kNTaskIDNone);
	
	if (stdErr)
		theResult = ReadPipe(theTask.stdErr, lastRead);
	else
		theResult = ReadPipe(theTask.stdOut, lastRead);
	
	return(theResult);
}





//============================================================================
//      NTargetSystem::TaskWrite : Write to a task.
//----------------------------------------------------------------------------
void NTargetSystem::TaskWrite(const TaskInfo &theTask, const NString &theText)
{


	// Write to the task
	WritePipe(theTask.stdIn, theText);
}





//============================================================================
//      NTargetSystem::TaskWait : Block for a task.
//----------------------------------------------------------------------------
void NTargetSystem::TaskWait(const TaskInfo &/*theTask*/, NTime waitFor)
{	NTime		endTime;
	MSG			theMsg;



	// Get the state we need
	endTime = NTimeUtilities::GetTime() + waitFor;



	// Wait for the task
	//
	// We must allow timers to run while waiting, since they poll the status.
	while (NTimeUtilities::GetTime() < endTime)
		{
		if (PeekMessage(&theMsg, NULL, 0, 0, PM_REMOVE))
			{
			TranslateMessage(&theMsg);
			DispatchMessage( &theMsg);
			}
		}
}





//============================================================================
//      NTargetSystem::TaskSignal : Signal a task.
//----------------------------------------------------------------------------
void NTargetSystem::TaskSignal(const TaskInfo &theTask, NTaskSignal theSignal)
{	HANDLE	processHnd;
	DWORD	processID;
	BOOL	wasOK;



	// Get the state we need
	processHnd = (HANDLE) theTask.taskID;
	processID  = GetProcessId(processHnd);



	// Signal the task
	//
	// Windows does not have a true IPC signal mechanism, and the closest approach is the
	// console "Ctrl Event" system.
	//
	// The dispatch for these events is quite complex, and requires some special handling.
	//
	//
	// TaskCreate gives each child has its own console (with CREATE_NEW_CONSOLE), hides
	// these by default (with SW_HIDE), and puts each child into its own process group
	// (CREATE_NEW_PROCESS_GROUP).
	//
	// The parent can then use GenerateConsoleCtrlEvent to send a CTRL_BREAK_EVENT to a
	// specific process group, i.e., to the child being signalled.
	//
	// However GenerateConsoleCtrlEvent only sends CTRL_BREAK_EVENTs to members of the
	// group that share the same console as the process sending the event, which means
	// the parent must temporarily attach to the child's console.
	//
	// This allows the parent to send CTRL_BREAK_EVENT to only the child being signalled.
	//
	//
	// We could have used CTRL_C_EVENT, but this can't be sent to process groups. This
	// event is sent to all processes that share the same console.
	//
	// We could have used the same attach-to-child-console approach as above to ensure
	// the event was sent to the child, but would need some way to ignore the event in
	// the parent.
	//
	// In theory having the parent install a NULL event handler, or a handler which just
	// returns TRUE, should prevent the the event from reaching the parent's default event
	// handler (which calls ExitProcess).
	//
	// In practice, Windows 7 systems would sometimes (about 20% of the time) send the
	// event to the parent's default handler (calling ExitProcess) even if a NULL/dummy
	// handler had been set in the parent.
	switch (theSignal) {
		case kTaskKill:
			wasOK = TerminateProcess(processHnd, (UINT) -1);
			NN_ASSERT(wasOK);
			break;

		case kTaskInterrupt:
			wasOK = AttachConsole(processID);
			NN_ASSERT(wasOK);

			if (wasOK)
				{
				wasOK = GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT, processID);
				NN_ASSERT(wasOK);

				if (!AttachConsole(ATTACH_PARENT_PROCESS))
					{
					wasOK = FreeConsole();
					NN_ASSERT(wasOK);
					}
				}
			break;
		
		default:
			NN_LOG("Unknown signal: %d", theSignal);
			break;
		}
}





//============================================================================
//      NTargetSystem::GetLocaleValue : Get a locale value.
//----------------------------------------------------------------------------
NVariant NTargetSystem::GetLocaleValue(const NString &theID, const NString &theKey)
{	TCHAR			theValue[kLocaleBufferSize];
	NVariant		theResult;
	


	// Validate our parameters
	NN_ASSERT(theID == kNLocaleUser);
	NN_UNUSED(theID);



	// Get the value
	if (theKey == kNLocaleIsMetricKey)
		{
		if (GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IMEASURE, theValue, kLocaleBufferSize))
			theResult = (bool) (theValue[0] == '0');
		}

	else
		NN_LOG("Unknown locale key: %@", theKey);

	return(theResult);
}





//============================================================================
//      NTargetSystem::GetLocaleBroadcaster : Get the locale broadcaster.
//----------------------------------------------------------------------------
NBroadcaster *NTargetSystem::GetLocaleBroadcaster(const NString &/*theID*/)
{	static NBroadcaster		sBroadcaster;



	// Get the broadcaster
	//
	// For now, all locales share the same broadcaster.
	return(&sBroadcaster);
}





//============================================================================
//      NTargetSystem::TransformString : Transform a string.
//----------------------------------------------------------------------------
NString NTargetSystem::TransformString(const NString &theString, NStringTransform theTransform)
{	std::vector<TCHAR>		resultChars;
	NIndex					n, numChars;
	const TCHAR				*srcChars;
	NString					theResult;
	std::vector<TCHAR>		dstChars;



	// Validate our parameters
	NN_ASSERT(!theString.IsEmpty());
	NN_ASSERT(theTransform & kNStringStripDiacritics);



	// Get the state we need
	theResult = theString;



	// Transform the string
	if (theTransform & kNStringStripDiacritics)
		{
		// Get the state we need
		srcChars = ToWN(theResult);
		numChars = FoldString(MAP_COMPOSITE, &srcChars[0], theResult.GetSize(), NULL, 0);
	
		dstChars.resize(numChars);
		numChars = FoldString(MAP_COMPOSITE, &srcChars[0], theResult.GetSize(), &dstChars[0], numChars);
		NN_ASSERT(numChars == (NIndex) dstChars.size());


		// Extract the characters
		//
		// This is only correct for Roman-style languages; may need to invoke FoldString
		// on each character in turn, to determine which ones MAP_COMPOSITE expanded.
		for (n = 0; n < numChars; n++)
			{
			if (isascii(dstChars[n]))
				resultChars.push_back(dstChars[n]);
			}

		resultChars.push_back(0x0000);
		theResult = ToNN(&resultChars[0]);
		}

	return(theResult);
}





//============================================================================
//		NTargetSystem::ImageEncode : Encode an image.
//----------------------------------------------------------------------------
NData NTargetSystem::ImageEncode(const NImage &theImage, const NUTI &theType)
{	GUID						srcPixels, dstPixels, containerID;
	NIndex						theWidth, theHeight, rowBytes;
	NData						srcData, dstData;
	StCOM						coInitialize;
	IWICFormatConverter			*icConverter;
	IStream						*theStream;
	IWICBitmapEncoder			*icEncoder;
	IWICImagingFactory			*icFactory;
	STATSTG						streamInfo;
	IWICBitmap					*icBitmap;
	NImage						srcImage;
	IWICBitmapFrameEncode		*icFrame;
	HRESULT						winErr;



	// Validate our parameters
	NN_ASSERT(theImage.IsValid());
	NN_ASSERT( theType.IsValid());



	// Get the state we need
	icFactory   = NULL;
	icBitmap    = NULL;
	icConverter = NULL;
	icEncoder   = NULL;
	icFrame     = NULL;

	winErr    = ERROR_SUCCESS;
	theStream = CreateMemoryStream();

	if (theStream == NULL)
		return(dstData);



	// Prepare the source image
	//
	// Ideally we can use the image as-is, however if its format isn't one
	// that WIC supports we'll need to convert it before we can encode.
	srcImage = theImage;

	switch (srcImage.GetFormat()) {
		case kNImageFormat_RGB_888:
			srcPixels = GUID_WICPixelFormat24bppRGB;
			break;

		case kNImageFormat_BGR_888:
			srcPixels = GUID_WICPixelFormat24bppBGR;
			break;

		case kNImageFormat_BGRX_8888:
			srcPixels = GUID_WICPixelFormat32bppBGR;
			break;

		case kNImageFormat_BGRA_8888:
			srcPixels = GUID_WICPixelFormat32bppBGRA;
			break;

		default:
			srcImage.SetFormat(kNImageFormat_BGRA_8888);
			srcPixels = GUID_WICPixelFormat32bppBGRA;
			break;
		}

	srcData   = srcImage.GetData();
	theWidth  = srcImage.GetWidth();
	theHeight = srcImage.GetHeight();
	rowBytes  = srcImage.GetBytesPerRow();



	// Prepare the destination format
	//
	// Some encoders can only encode to a single format.
	if (theType == kNUTTypePNG)
		{
		containerID = GUID_ContainerFormatPng;
		dstPixels   = GUID_WICPixelFormat32bppBGRA;
		}

	else if (theType == kNUTTypeJPEG)
		{
		containerID = GUID_ContainerFormatJpeg;
		dstPixels   = GUID_WICPixelFormat24bppBGR;
		}

	else if (theType == kNUTTypeTIFF)
		{
		containerID = GUID_ContainerFormatTiff;
		dstPixels   = GUID_WICPixelFormat32bppBGRA;
		}

	else if (theType == kNUTTypeGIF)
		{
		containerID = GUID_ContainerFormatGif;
		dstPixels   = GUID_WICPixelFormat8bppIndexed;
		}

	else
		{
		NN_LOG("Unsupported format: %@", theType);
		return(dstData);
		}



	// Create the converter
	//
	// The converter will handle transcoding from our source pixel format
	// into the encoder-supported format, including colour reduction.
	if (SUCCEEDED(winErr))
		winErr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&icFactory));

	if (SUCCEEDED(winErr))
		winErr = icFactory->CreateBitmapFromMemory(theWidth, theHeight, srcPixels, rowBytes, srcData.GetSize(), srcData.GetData(), &icBitmap);

	if (SUCCEEDED(winErr))
		winErr = icFactory->CreateFormatConverter(&icConverter);

	if (SUCCEEDED(winErr))
		winErr = icConverter->Initialize(icBitmap, dstPixels, WICBitmapDitherTypeErrorDiffusion, NULL, 1.0, WICBitmapPaletteTypeMedianCut);

	NN_ASSERT_SUCCESS(winErr);



	// Create the encoder
	if (SUCCEEDED(winErr))
		winErr = icFactory->CreateEncoder(containerID, NULL, &icEncoder);

	if (SUCCEEDED(winErr))
		winErr = icEncoder->Initialize(theStream, WICBitmapEncoderNoCache);

	if (SUCCEEDED(winErr))
		winErr = icEncoder->CreateNewFrame(&icFrame, NULL);

	if (SUCCEEDED(winErr))
		winErr = icFrame->Initialize(NULL);

	if (SUCCEEDED(winErr))
		winErr = icFrame->SetSize(theWidth, theHeight);

	NN_ASSERT_SUCCESS(winErr);



	// Encode the image
	if (SUCCEEDED(winErr))
		winErr = icFrame->WriteSource(icConverter, NULL);

	if (SUCCEEDED(winErr))
		winErr = icFrame->Commit();

	if (SUCCEEDED(winErr))
		winErr = icEncoder->Commit();

	NN_ASSERT_SUCCESS(winErr);




	// Extract the data
	if (SUCCEEDED(winErr))
		winErr = theStream->Stat(&streamInfo, STATFLAG_NONAME);

	if (SUCCEEDED(winErr))
		{
		if (!dstData.SetSize((NIndex) ToNN(streamInfo.cbSize)))
			winErr = ERROR_NOT_ENOUGH_MEMORY;
		}

	if (SUCCEEDED(winErr))
		winErr = theStream->Seek(ToWN((int64_t) 0), STREAM_SEEK_SET, NULL);

	if (SUCCEEDED(winErr))
		winErr = theStream->Read(dstData.GetData(), dstData.GetSize(), NULL);
	
	NN_ASSERT_SUCCESS(winErr);

	if (FAILED(winErr))
		dstData.Clear();



	// Clean up
	WNSafeRelease(icFrame);
	WNSafeRelease(icEncoder);
	WNSafeRelease(icConverter);
	WNSafeRelease(icBitmap);
	WNSafeRelease(icFactory);

	WNSafeRelease(theStream);

	return(dstData);
}





//============================================================================
//		NTargetSystem::ImageDecode : Decode an image.
//----------------------------------------------------------------------------
NImage NTargetSystem::ImageDecode(const NData &theData)
{	UINT						theWidth, theHeight;
	StCOM						coInitialize;
	IWICImagingFactory			*icFactory;
	IWICBitmapDecoder			*icDecoder;
	IStream						*theStream;
	NImageFormat				dstFormat;
	GUID						dstPixels;
	IWICBitmapSource			*icBitmap;
	IWICBitmapFrameDecode		*icFrame;
	NIndex						rowBytes;
	NImage						theImage;
	HRESULT						winErr;



	// Get the state we need
	icFactory = NULL;
	icDecoder = NULL;
	icFrame   = NULL;
	icBitmap  = NULL;

	theWidth  = 0;
	theHeight = 0;
	rowBytes  = 0;

	winErr    = ERROR_SUCCESS;
	theStream = CreateMemoryStream(theData);

	if (theStream == NULL)
		return(theImage);



	// Create the decoder
	if (SUCCEEDED(winErr))
		winErr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&icFactory));

	if (SUCCEEDED(winErr))
		winErr = icFactory->CreateDecoderFromStream(theStream, 0, WICDecodeMetadataCacheOnDemand, &icDecoder);

	NN_ASSERT_SUCCESS(winErr);



	// Get the image data
	if (SUCCEEDED(winErr))
		winErr = icDecoder->GetFrame(0, &icFrame);

	if (SUCCEEDED(winErr))
		winErr = icFrame->GetSize(&theWidth, &theHeight);

	if (SUCCEEDED(winErr))
		winErr = icFrame->GetPixelFormat(&dstPixels);

	NN_ASSERT_SUCCESS(winErr);



	// Prepare the destination format
	if (dstPixels == GUID_WICPixelFormat24bppRGB)
		dstFormat = kNImageFormat_RGB_888;

	else if (dstPixels == GUID_WICPixelFormat24bppBGR)
		dstFormat = kNImageFormat_BGR_888;

	else if (dstPixels == GUID_WICPixelFormat32bppBGR)
		dstFormat = kNImageFormat_BGRX_8888;

	else if (dstPixels == GUID_WICPixelFormat32bppBGRA)
		dstFormat = kNImageFormat_BGRA_8888;

	else
		{
		dstPixels = GUID_WICPixelFormat32bppBGRA;
		dstFormat = kNImageFormat_BGRA_8888;
		}



	// Decode the image
	if (SUCCEEDED(winErr))
		{
		theImage  = NImage(NSize((float32_t) theWidth, (float32_t) theHeight), dstFormat);
		rowBytes  = theImage.GetBytesPerRow();
		}

	if (SUCCEEDED(winErr))
		winErr = WICConvertBitmapSource(dstPixels, icFrame, &icBitmap);

	if (SUCCEEDED(winErr))
		winErr = icBitmap->CopyPixels(NULL, rowBytes, rowBytes * theHeight, theImage.GetPixels());

	NN_ASSERT_SUCCESS(winErr);

	if (FAILED(winErr))
		theImage.Clear();



	// Clean up
	WNSafeRelease(icBitmap);
	WNSafeRelease(icFrame);
	WNSafeRelease(icDecoder);
	WNSafeRelease(icFactory);

	WNSafeRelease(theStream);

	return(theImage);
}



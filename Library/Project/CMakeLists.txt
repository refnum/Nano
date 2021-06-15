#==============================================================================
#	NAME:
#		CMakeLists.txt
#
#	DESCRIPTION:
#		Nano project.
#
#	COPYRIGHT:
#		Copyright (c) 2006-2021, refNum Software
#		All rights reserved.
#
#		Redistribution and use in source and binary forms, with or without
#		modification, are permitted provided that the following conditions
#		are met:
#		
#		1. Redistributions of source code must retain the above copyright
#		notice, this list of conditions and the following disclaimer.
#		
#		2. Redistributions in binary form must reproduce the above copyright
#		notice, this list of conditions and the following disclaimer in the
#		documentation and/or other materials provided with the distribution.
#		
#		3. Neither the name of the copyright holder nor the names of its
#		contributors may be used to endorse or promote products derived from
#		this software without specific prior written permission.
#		
#		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#		"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#		LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#		A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
#		HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#		DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#		THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#		(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#==============================================================================
#		Project
#------------------------------------------------------------------------------
project(Nano VERSION 4.0)

nano_project(STATIC)





#==============================================================================
#		Common
#------------------------------------------------------------------------------
nano_project_prefix("${NN_SOURCE_DIR}/Nano/Nano Prefix.h")

nano_project_sources(
	PRIVATE
		"${NN_SOURCE_DIR}/Nano/Application/NCommandLine.cpp"
		"${NN_SOURCE_DIR}/Nano/Application/NCommandLine.h"
		"${NN_SOURCE_DIR}/Nano/Application/NUndoManager.cpp"
		"${NN_SOURCE_DIR}/Nano/Application/NUndoManager.h"
		"${NN_SOURCE_DIR}/Nano/Application/NUndoSource.cpp"
		"${NN_SOURCE_DIR}/Nano/Application/NUndoSource.h"
		"${NN_SOURCE_DIR}/Nano/Core/NanoConstants.h"
		"${NN_SOURCE_DIR}/Nano/Core/NanoMacros.h"
		"${NN_SOURCE_DIR}/Nano/Core/NanoTargets.h"
		"${NN_SOURCE_DIR}/Nano/Core/NanoTypes.h"
		"${NN_SOURCE_DIR}/Nano/Data/NData.cpp"
		"${NN_SOURCE_DIR}/Nano/Data/NData.h"
		"${NN_SOURCE_DIR}/Nano/Data/NData.inl"
		"${NN_SOURCE_DIR}/Nano/Data/NDataCipher.cpp"
		"${NN_SOURCE_DIR}/Nano/Data/NDataCipher.h"
		"${NN_SOURCE_DIR}/Nano/Data/NDataCompressor.cpp"
		"${NN_SOURCE_DIR}/Nano/Data/NDataCompressor.h"
		"${NN_SOURCE_DIR}/Nano/Data/NDataDigest.cpp"
		"${NN_SOURCE_DIR}/Nano/Data/NDataDigest.h"
		"${NN_SOURCE_DIR}/Nano/Data/NDataDigest.inl"
		"${NN_SOURCE_DIR}/Nano/Data/NDataEncoder.cpp"
		"${NN_SOURCE_DIR}/Nano/Data/NDataEncoder.h"
		"${NN_SOURCE_DIR}/Nano/Debugging/NDebug.cpp"
		"${NN_SOURCE_DIR}/Nano/Debugging/NDebug.h"
		"${NN_SOURCE_DIR}/Nano/Debugging/NDebug.inl"
		"${NN_SOURCE_DIR}/Nano/Debugging/NDebugger.cpp"
		"${NN_SOURCE_DIR}/Nano/Debugging/NDebugger.h"
		"${NN_SOURCE_DIR}/Nano/Debugging/NLog.cpp"
		"${NN_SOURCE_DIR}/Nano/Debugging/NLog.h"
		"${NN_SOURCE_DIR}/Nano/Debugging/NLogOutput.cpp"
		"${NN_SOURCE_DIR}/Nano/Debugging/NLogOutput.h"
		"${NN_SOURCE_DIR}/Nano/Debugging/NUsageBucket.cpp"
		"${NN_SOURCE_DIR}/Nano/Debugging/NUsageBucket.h"
		"${NN_SOURCE_DIR}/Nano/Debugging/NUsageLog.cpp"
		"${NN_SOURCE_DIR}/Nano/Debugging/NUsageLog.h"
		"${NN_SOURCE_DIR}/Nano/Events/NBroadcaster.cpp"
		"${NN_SOURCE_DIR}/Nano/Events/NBroadcaster.h"
		"${NN_SOURCE_DIR}/Nano/Events/NListener.cpp"
		"${NN_SOURCE_DIR}/Nano/Events/NListener.h"
		"${NN_SOURCE_DIR}/Nano/Events/NProgressable.cpp"
		"${NN_SOURCE_DIR}/Nano/Events/NProgressable.h"
		"${NN_SOURCE_DIR}/Nano/Files/NFile.cpp"
		"${NN_SOURCE_DIR}/Nano/Files/NFile.h"
		"${NN_SOURCE_DIR}/Nano/Files/NFileHandle.cpp"
		"${NN_SOURCE_DIR}/Nano/Files/NFileHandle.h"
		"${NN_SOURCE_DIR}/Nano/Files/NFileInfo.cpp"
		"${NN_SOURCE_DIR}/Nano/Files/NFileInfo.h"
		"${NN_SOURCE_DIR}/Nano/Files/NFileMap.cpp"
		"${NN_SOURCE_DIR}/Nano/Files/NFileMap.h"
		"${NN_SOURCE_DIR}/Nano/Files/NFilePath.cpp"
		"${NN_SOURCE_DIR}/Nano/Files/NFilePath.h"
		"${NN_SOURCE_DIR}/Nano/Files/NFileScanner.cpp"
		"${NN_SOURCE_DIR}/Nano/Files/NFileScanner.h"
		"${NN_SOURCE_DIR}/Nano/Files/NFileScanner.inl"
		"${NN_SOURCE_DIR}/Nano/Files/NFileUtils.cpp"
		"${NN_SOURCE_DIR}/Nano/Files/NFileUtils.h"
		"${NN_SOURCE_DIR}/Nano/Graphics/NColor.cpp"
		"${NN_SOURCE_DIR}/Nano/Graphics/NColor.h"
		"${NN_SOURCE_DIR}/Nano/Graphics/NColor.inl"
		"${NN_SOURCE_DIR}/Nano/Graphics/NPoint.h"
		"${NN_SOURCE_DIR}/Nano/Graphics/NPoint.inl"
		"${NN_SOURCE_DIR}/Nano/Graphics/NRectangle.h"
		"${NN_SOURCE_DIR}/Nano/Graphics/NRectangle.inl"
		"${NN_SOURCE_DIR}/Nano/Graphics/NSize.h"
		"${NN_SOURCE_DIR}/Nano/Graphics/NSize.inl"
		"${NN_SOURCE_DIR}/Nano/Graphics/NVector.h"
		"${NN_SOURCE_DIR}/Nano/Graphics/NVector.inl"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/expat-2.2.10/ascii.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/expat-2.2.10/asciitab.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/expat-2.2.10/expat_external.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/expat-2.2.10/expat.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/expat-2.2.10/iasciitab.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/expat-2.2.10/internal.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/expat-2.2.10/latin1tab.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/expat-2.2.10/nametab.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/expat-2.2.10/Nano_expat_config.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/expat-2.2.10/siphash.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/expat-2.2.10/utf8tab.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/expat-2.2.10/winconfig.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/expat-2.2.10/xmlparse.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/expat-2.2.10/xmlrole.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/expat-2.2.10/xmlrole.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/expat-2.2.10/xmltok_impl.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/expat-2.2.10/xmltok.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/expat-2.2.10/xmltok.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/fast_float-0.8.0/ascii_number.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/fast_float-0.8.0/decimal_to_binary.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/fast_float-0.8.0/fast_float.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/fast_float-0.8.0/fast_table.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/fast_float-0.8.0/float_common.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/fast_float-0.8.0/Nano_fast_float.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/fast_float-0.8.0/parse_number.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/fast_float-0.8.0/simple_decimal_conversion.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/fmt-7.1.3/fmt_core.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/fmt-7.1.3/fmt_format-inl.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/fmt-7.1.3/fmt_format.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/fmt-7.1.3/fmt_ostream.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/fmt-7.1.3/fmt_printf.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/fmt-7.1.3/Nano_fmt_format.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/json-parser-1.1.0/json.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/json-parser-1.1.0/json.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/libb64-2.0.0/ccommon.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/libb64-2.0.0/cdecode.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/libb64-2.0.0/cdecode.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/libb64-2.0.0/cencode.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/libb64-2.0.0/cencode.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/libb64-2.0.0/Nano_libb64.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/config.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_auto_possess.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_chartables.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_compile.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_config.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_context.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_convert.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_dfa_match.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_error.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_extuni.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_find_bracket.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_fuzzsupport.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_internal.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_intmodedep.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_jit_compile.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_jit_neon_inc.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_jit_simd_inc.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_maketables.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_match_data.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_match.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_newline.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_ord2utf.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_pattern_info.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_script_run.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_serialize.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_string_utils.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_study.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_substitute.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_substring.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_tables.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_ucd.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_ucp.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_valid_utf.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2_xclass.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36/pcre2.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/sqlite-3.34.0/Nano_sqlite3.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/sqlite-3.34.0/sqlite3.34.0.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/sqlite-3.34.0/sqlite3.34.0.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/WjCryptLib-2.3.0/WjCryptLib_Aes.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/WjCryptLib-2.3.0/WjCryptLib_Aes.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/WjCryptLib-2.3.0/WjCryptLib_Md5.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/WjCryptLib-2.3.0/WjCryptLib_Md5.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/WjCryptLib-2.3.0/WjCryptLib_Sha1.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/WjCryptLib-2.3.0/WjCryptLib_Sha1.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/WjCryptLib-2.3.0/WjCryptLib_Sha256.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/WjCryptLib-2.3.0/WjCryptLib_Sha256.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/xxHash-0.8.0/Nano_xxhash.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/xxHash-0.8.0/xxhash.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/zlib-1.2.11/adler32.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/zlib-1.2.11/compress.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/zlib-1.2.11/crc32.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/zlib-1.2.11/crc32.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/zlib-1.2.11/deflate.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/zlib-1.2.11/deflate.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/zlib-1.2.11/gzclose.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/zlib-1.2.11/gzguts.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/zlib-1.2.11/gzlib.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/zlib-1.2.11/gzread.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/zlib-1.2.11/gzwrite.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/zlib-1.2.11/infback.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/zlib-1.2.11/inffast.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/zlib-1.2.11/inffast.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/zlib-1.2.11/inffixed.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/zlib-1.2.11/inflate.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/zlib-1.2.11/inflate.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/zlib-1.2.11/inftrees.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/zlib-1.2.11/inftrees.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/zlib-1.2.11/Nano_zlib.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/zlib-1.2.11/trees.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/zlib-1.2.11/trees.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/zlib-1.2.11/uncompr.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/zlib-1.2.11/zconf.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/zlib-1.2.11/zlib.h"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/zlib-1.2.11/zutil.c"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/zlib-1.2.11/zutil.h"
		"${NN_SOURCE_DIR}/Nano/Maths/NMathUtils.cpp"
		"${NN_SOURCE_DIR}/Nano/Maths/NMathUtils.h"
		"${NN_SOURCE_DIR}/Nano/Maths/NMathUtils.inl"
		"${NN_SOURCE_DIR}/Nano/Maths/NRandom.cpp"
		"${NN_SOURCE_DIR}/Nano/Maths/NRandom.h"
		"${NN_SOURCE_DIR}/Nano/Maths/NRandom.inl"
		"${NN_SOURCE_DIR}/Nano/Maths/NRandomGenerator.h"
		"${NN_SOURCE_DIR}/Nano/Maths/NRandomGenerator.inl"
		"${NN_SOURCE_DIR}/Nano/Maths/NUInt128.h"
		"${NN_SOURCE_DIR}/Nano/Maths/NUInt128.inl"
		"${NN_SOURCE_DIR}/Nano/Std/NStdAlgorithm.h"
		"${NN_SOURCE_DIR}/Nano/Std/NStdContainer.h"
		"${NN_SOURCE_DIR}/Nano/Std/NStdMath.h"
		"${NN_SOURCE_DIR}/Nano/Std/NStdMemory.h"
		"${NN_SOURCE_DIR}/Nano/Std/NStdUtility.h"
		"${NN_SOURCE_DIR}/Nano/System/NByteSwap.cpp"
		"${NN_SOURCE_DIR}/Nano/System/NByteSwap.h"
		"${NN_SOURCE_DIR}/Nano/System/NByteSwap.inl"
		"${NN_SOURCE_DIR}/Nano/System/NMachine.cpp"
		"${NN_SOURCE_DIR}/Nano/System/NMachine.h"
		"${NN_SOURCE_DIR}/Nano/System/NPOSIX.cpp"
		"${NN_SOURCE_DIR}/Nano/System/NPOSIX.h"
		"${NN_SOURCE_DIR}/Nano/System/NProcess.cpp"
		"${NN_SOURCE_DIR}/Nano/System/NProcess.h"
		"${NN_SOURCE_DIR}/Nano/System/NSystem.cpp"
		"${NN_SOURCE_DIR}/Nano/System/NSystem.h"
		"${NN_SOURCE_DIR}/Nano/System/NVersion.cpp"
		"${NN_SOURCE_DIR}/Nano/System/NVersion.h"
		"${NN_SOURCE_DIR}/Nano/System/NVersion.inl"
		"${NN_SOURCE_DIR}/Nano/Text/NFormat.cpp"
		"${NN_SOURCE_DIR}/Nano/Text/NFormat.h"
		"${NN_SOURCE_DIR}/Nano/Text/NFormat.inl"
		"${NN_SOURCE_DIR}/Nano/Text/NString.cpp"
		"${NN_SOURCE_DIR}/Nano/Text/NString.h"
		"${NN_SOURCE_DIR}/Nano/Text/NString.inl"
		"${NN_SOURCE_DIR}/Nano/Text/NStringComparator.cpp"
		"${NN_SOURCE_DIR}/Nano/Text/NStringComparator.h"
		"${NN_SOURCE_DIR}/Nano/Text/NStringEncoder.cpp"
		"${NN_SOURCE_DIR}/Nano/Text/NStringEncoder.h"
		"${NN_SOURCE_DIR}/Nano/Text/NStringEncoder.inl"
		"${NN_SOURCE_DIR}/Nano/Text/NStringEncodings.h"
		"${NN_SOURCE_DIR}/Nano/Text/NStringScanner.cpp"
		"${NN_SOURCE_DIR}/Nano/Text/NStringScanner.h"
		"${NN_SOURCE_DIR}/Nano/Text/NStringTransformer.cpp"
		"${NN_SOURCE_DIR}/Nano/Text/NStringTransformer.h"
		"${NN_SOURCE_DIR}/Nano/Text/NTextUtils.cpp"
		"${NN_SOURCE_DIR}/Nano/Text/NTextUtils.h"
		"${NN_SOURCE_DIR}/Nano/Text/NTextUtils.inl"
		"${NN_SOURCE_DIR}/Nano/Text/NUnicodeView.cpp"
		"${NN_SOURCE_DIR}/Nano/Text/NUnicodeView.h"
		"${NN_SOURCE_DIR}/Nano/Text/NUnicodeView.inl"
		"${NN_SOURCE_DIR}/Nano/Threads/NLockable.h"
		"${NN_SOURCE_DIR}/Nano/Threads/NLockable.inl"
		"${NN_SOURCE_DIR}/Nano/Threads/NMutex.cpp"
		"${NN_SOURCE_DIR}/Nano/Threads/NMutex.h"
		"${NN_SOURCE_DIR}/Nano/Threads/NMutex.inl"
		"${NN_SOURCE_DIR}/Nano/Threads/NRunLoop.cpp"
		"${NN_SOURCE_DIR}/Nano/Threads/NRunLoop.h"
		"${NN_SOURCE_DIR}/Nano/Threads/NScopedLock.h"
		"${NN_SOURCE_DIR}/Nano/Threads/NScopedLock.inl"
		"${NN_SOURCE_DIR}/Nano/Threads/NSemaphore.cpp"
		"${NN_SOURCE_DIR}/Nano/Threads/NSemaphore.h"
		"${NN_SOURCE_DIR}/Nano/Threads/NSpinLock.h"
		"${NN_SOURCE_DIR}/Nano/Threads/NSpinLock.inl"
		"${NN_SOURCE_DIR}/Nano/Threads/NThread.cpp"
		"${NN_SOURCE_DIR}/Nano/Threads/NThread.h"
		"${NN_SOURCE_DIR}/Nano/Threads/NThread.inl"
		"${NN_SOURCE_DIR}/Nano/Threads/NThreadID.h"
		"${NN_SOURCE_DIR}/Nano/Threads/NThreadID.inl"
		"${NN_SOURCE_DIR}/Nano/Time/NClock.cpp"
		"${NN_SOURCE_DIR}/Nano/Time/NClock.h"
		"${NN_SOURCE_DIR}/Nano/Time/NDate.cpp"
		"${NN_SOURCE_DIR}/Nano/Time/NDate.h"
		"${NN_SOURCE_DIR}/Nano/Time/NDate.inl"
		"${NN_SOURCE_DIR}/Nano/Time/NDateFormatter.cpp"
		"${NN_SOURCE_DIR}/Nano/Time/NDateFormatter.h"
		"${NN_SOURCE_DIR}/Nano/Time/NTime.h"
		"${NN_SOURCE_DIR}/Nano/Time/NTime.inl"
		"${NN_SOURCE_DIR}/Nano/Time/NTimeUtils.cpp"
		"${NN_SOURCE_DIR}/Nano/Time/NTimeUtils.h"
		"${NN_SOURCE_DIR}/Nano/Types/NAny.cpp"
		"${NN_SOURCE_DIR}/Nano/Types/NAny.h"
		"${NN_SOURCE_DIR}/Nano/Types/NAny.inl"
		"${NN_SOURCE_DIR}/Nano/Types/NArray.cpp"
		"${NN_SOURCE_DIR}/Nano/Types/NArray.h"
		"${NN_SOURCE_DIR}/Nano/Types/NArray.inl"
		"${NN_SOURCE_DIR}/Nano/Types/NBitVector.cpp"
		"${NN_SOURCE_DIR}/Nano/Types/NBitVector.h"
		"${NN_SOURCE_DIR}/Nano/Types/NBitVector.inl"
		"${NN_SOURCE_DIR}/Nano/Types/NDictionary.cpp"
		"${NN_SOURCE_DIR}/Nano/Types/NDictionary.h"
		"${NN_SOURCE_DIR}/Nano/Types/NDictionary.inl"
		"${NN_SOURCE_DIR}/Nano/Types/NFunction.h"
		"${NN_SOURCE_DIR}/Nano/Types/NMixinAppendable.h"
		"${NN_SOURCE_DIR}/Nano/Types/NMixinAppendable.inl"
		"${NN_SOURCE_DIR}/Nano/Types/NMixinAttributes.h"
		"${NN_SOURCE_DIR}/Nano/Types/NMixinAttributes.inl"
		"${NN_SOURCE_DIR}/Nano/Types/NMixinComparable.h"
		"${NN_SOURCE_DIR}/Nano/Types/NMixinComparable.inl"
		"${NN_SOURCE_DIR}/Nano/Types/NMixinContainer.h"
		"${NN_SOURCE_DIR}/Nano/Types/NMixinContainer.inl"
		"${NN_SOURCE_DIR}/Nano/Types/NMixinHashable.h"
		"${NN_SOURCE_DIR}/Nano/Types/NMixinHashable.inl"
		"${NN_SOURCE_DIR}/Nano/Types/NNumber.cpp"
		"${NN_SOURCE_DIR}/Nano/Types/NNumber.h"
		"${NN_SOURCE_DIR}/Nano/Types/NNumber.inl"
		"${NN_SOURCE_DIR}/Nano/Types/NRange.h"
		"${NN_SOURCE_DIR}/Nano/Types/NRange.inl"
		"${NN_SOURCE_DIR}/Nano/Types/NVariant.h"
		"${NN_SOURCE_DIR}/Nano/Types/NVariant.inl"
)

nano_project_include_paths(
	PUBLIC
		"${NN_SOURCE_DIR}/Nano/Application"
		"${NN_SOURCE_DIR}/Nano/Core"
		"${NN_SOURCE_DIR}/Nano/Data"
		"${NN_SOURCE_DIR}/Nano/Debugging"
		"${NN_SOURCE_DIR}/Nano/Files"
		"${NN_SOURCE_DIR}/Nano/Graphics"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/fmt-7.1.3"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/xxHash-0.8.0"
		"${NN_SOURCE_DIR}/Nano/Maths"
		"${NN_SOURCE_DIR}/Nano/Std"
		"${NN_SOURCE_DIR}/Nano/System"
		"${NN_SOURCE_DIR}/Nano/Text"
		"${NN_SOURCE_DIR}/Nano/Threads"
		"${NN_SOURCE_DIR}/Nano/Time"
		"${NN_SOURCE_DIR}/Nano/Types"
	PRIVATE
		"${NN_SOURCE_DIR}/Nano/Internal/Components/expat-2.2.10"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/fast_float-0.8.0"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/json-parser-1.1.0"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/libb64-2.0.0"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/pcre2-10.36"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/sqlite-3.34.0"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/WjCryptLib-2.3.0"
		"${NN_SOURCE_DIR}/Nano/Internal/Components/zlib-1.2.11"
		"${NN_SOURCE_DIR}/Nano/Internal/Targets/Shared"
)

nano_project_warnings(
	NONE
		"Internal/Components")





#==============================================================================
#		Android
#------------------------------------------------------------------------------
if (NN_TARGET_ANDROID)
	nano_project_sources(
		PRIVATE
			"${ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Android/AndroidNDebugger.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Android/AndroidNFileHandle.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Android/AndroidNFileInfo.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Android/AndroidNFileMap.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Android/AndroidNFileUtils.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Android/AndroidNMachine.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Android/AndroidNProcess.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Android/AndroidNRandom.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Android/AndroidNRunLoop.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Android/AndroidNSemaphore.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Android/AndroidNSystem.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Android/AndroidNThread.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Android/AndroidNTimeUtils.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Shared/NSharedLinux.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Shared/NSharedLinux.h"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Shared/NSharedPOSIX.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Shared/NSharedPOSIX.h"
	)

	nano_project_include_paths(
		PRIVATE
			"${ANDROID_NDK}/sources/android"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Android"
	)

	nano_project_warnings(
		NONE
			"${ANDROID_NDK}/sources/android"
	)
endif()





#==============================================================================
#		iOS
#------------------------------------------------------------------------------
if (NN_TARGET_IOS)
	nano_project_sources(
		PRIVATE
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/iOS/iOSNDebugger.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/iOS/iOSNFileHandle.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/iOS/iOSNFileInfo.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/iOS/iOSNFileMap.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/iOS/iOSNFileUtils.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/iOS/iOSNMachine.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/iOS/iOSNProcess.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/iOS/iOSNRandom.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/iOS/iOSNRunLoop.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/iOS/iOSNSemaphore.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/iOS/iOSNSystem.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/iOS/iOSNThread.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/iOS/iOSNTimeUtils.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Shared/NSharedDarwin.h"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Shared/NSharedDarwin.mm"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Shared/NSharedPOSIX.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Shared/NSharedPOSIX.h"
	)

	nano_project_include_paths(
		PRIVATE
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/iOS"
	)
endif()





#==============================================================================
#		Linux
#------------------------------------------------------------------------------
if (NN_TARGET_LINUX)
	nano_project_sources(
		PRIVATE
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Linux/LinuxNDebugger.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Linux/LinuxNFileHandle.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Linux/LinuxNFileInfo.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Linux/LinuxNFileMap.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Linux/LinuxNFileUtils.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Linux/LinuxNMachine.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Linux/LinuxNProcess.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Linux/LinuxNRandom.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Linux/LinuxNRunLoop.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Linux/LinuxNSemaphore.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Linux/LinuxNSystem.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Linux/LinuxNThread.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Linux/LinuxNTimeUtils.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Shared/NSharedLinux.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Shared/NSharedLinux.h"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Shared/NSharedPOSIX.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Shared/NSharedPOSIX.h"
	)

	nano_project_include_paths(
		PRIVATE
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Linux"
	)
endif()





#==============================================================================
#		macOS
#------------------------------------------------------------------------------
if (NN_TARGET_MACOS)
	nano_project_sources(
		PRIVATE
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/macOS/macOSNDebugger.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/macOS/macOSNFileHandle.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/macOS/macOSNFileInfo.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/macOS/macOSNFileMap.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/macOS/macOSNFileUtils.mm"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/macOS/macOSNMachine.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/macOS/macOSNProcess.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/macOS/macOSNRandom.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/macOS/macOSNRunLoop.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/macOS/macOSNSemaphore.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/macOS/macOSNSystem.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/macOS/macOSNThread.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/macOS/macOSNTimeUtils.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Shared/NSharedDarwin.h"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Shared/NSharedDarwin.mm"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Shared/NSharedPOSIX.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Shared/NSharedPOSIX.h"
	)

	nano_project_include_paths(
		PRIVATE
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/macOS"
	)
endif()





#==============================================================================
#		tvOS
#------------------------------------------------------------------------------
if (NN_TARGET_TVOS)
	nano_project_sources(
		PRIVATE
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Shared/NSharedDarwin.h"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Shared/NSharedDarwin.mm"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Shared/NSharedPOSIX.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Shared/NSharedPOSIX.h"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/tvOS/tvOSNDebugger.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/tvOS/tvOSNFileHandle.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/tvOS/tvOSNFileInfo.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/tvOS/tvOSNFileMap.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/tvOS/tvOSNFileUtils.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/tvOS/tvOSNMachine.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/tvOS/tvOSNProcess.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/tvOS/tvOSNRandom.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/tvOS/tvOSNRunLoop.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/tvOS/tvOSNSemaphore.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/tvOS/tvOSNSystem.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/tvOS/tvOSNThread.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/tvOS/tvOSNTimeUtils.cpp"
		)

	nano_project_include_paths(
		PRIVATE
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/tvOS"
		)
endif()





#==============================================================================
#		Windows
#------------------------------------------------------------------------------
if (NN_TARGET_WINDOWS)
	nano_project_sources(
		PRIVATE
			"${PROJECT_SOURCE_DIR}/../Support/Visual Studio/Nano.natvis"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Shared/NSharedWindows.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Shared/NSharedWindows.h"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Windows/WindowsNDebugger.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Windows/WindowsNFileHandle.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Windows/WindowsNFileInfo.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Windows/WindowsNFileMap.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Windows/WindowsNFileUtils.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Windows/WindowsNMachine.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Windows/WindowsNProcess.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Windows/WindowsNRandom.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Windows/WindowsNRunLoop.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Windows/WindowsNSemaphore.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Windows/WindowsNSystem.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Windows/WindowsNThread.cpp"
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Windows/WindowsNTimeUtils.cpp"
	)

	nano_project_include_paths(
		PRIVATE
			"${NN_SOURCE_DIR}/Nano/Internal/Targets/Windows"
	)
endif()





#==============================================================================
#		Build
#------------------------------------------------------------------------------
nano_build_project()





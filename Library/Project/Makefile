CXX      = gcc
CXXFLAGS = -Wall -Wno-unknown-pragmas -include "../Prefix/Nano Prefix.h" -DNN_SEEN_RELEASE_NOTES_FOR=0xFFFFFFFF
LDFLAGS  =

AR       = ar
ARFLAGS  = rcs

INC = -I"../Prefix"                                           \
      -I"../Source/Nano/Application"                          \
      -I"../Source/Nano/Core"                                 \
      -I"../Source/Nano/Database"                             \
      -I"../Source/Nano/Events"                               \
      -I"../Source/Nano/Files"                                \
      -I"../Source/Nano/Graphics"                             \
      -I"../Source/Nano/Internal"                             \
      -I"../Source/Nano/Internal/3rd_Party/expat_2.0.1"       \
      -I"../Source/Nano/Internal/3rd_Party/fourmilab.ch"      \
      -I"../Source/Nano/Internal/3rd_Party/libb64_1.1"        \
      -I"../Source/Nano/Internal/3rd_Party/pcre_7.8"          \
      -I"../Source/Nano/Internal/3rd_Party/schneier.com"      \
      -I"../Source/Nano/Internal/3rd_Party/sha2_1.1a"         \
      -I"../Source/Nano/Internal/3rd_Party/sqlite_3.7.15.2"   \
      -I"../Source/Nano/Internal/3rd_Party/tero.co.uk"        \
      -I"../Source/Nano/Internal/3rd_Party/udp.github.com"    \
      -I"../Source/Nano/Internal/3rd_Party/unicode.org"       \
      -I"../Source/Nano/Internal/3rd_Party/zlib_1.2.3"        \
      -I"../Source/Nano/Internal/Targets"                     \
      -I"../Source/Nano/Internal/Targets/Linux"               \
      -I"../Source/Nano/Network"                              \
      -I"../Source/Nano/System"                               \
      -I"../Source/Nano/Threads"                              \
      -I"../Source/Nano/Types"                                \
      -I"../Source/Nano/Utilities"


SRCS = ../Source/Nano/Application/NCommandLine.cpp                                  \
       ../Source/Nano/Application/NPreferences.cpp                                  \
       ../Source/Nano/Application/NUndoManager.cpp                                  \
       ../Source/Nano/Application/NUndoSource.cpp                                   \
       ../Source/Nano/Core/NDebug.cpp                                               \
       ../Source/Nano/Database/NDBHandle.cpp                                        \
       ../Source/Nano/Database/NDBHandlePool.cpp                                    \
       ../Source/Nano/Database/NDBQuery.cpp                                         \
       ../Source/Nano/Database/NDBResult.cpp                                        \
       ../Source/Nano/Events/NBroadcaster.cpp                                       \
       ../Source/Nano/Events/NListener.cpp                                          \
       ../Source/Nano/Events/NTimer.cpp                                             \
       ../Source/Nano/Files/NBundle.cpp                                             \
       ../Source/Nano/Files/NFile.cpp                                               \
       ../Source/Nano/Files/NFileIterator.cpp                                       \
       ../Source/Nano/Files/NFileMap.cpp                                            \
       ../Source/Nano/Files/NPropertyList.cpp                                       \
       ../Source/Nano/Graphics/NColor.cpp                                           \
       ../Source/Nano/Graphics/NPoint.cpp                                           \
       ../Source/Nano/Graphics/NRectangle.cpp                                       \
       ../Source/Nano/Graphics/NShape.cpp                                           \
       ../Source/Nano/Graphics/NSize.cpp                                            \
       ../Source/Nano/Graphics/NTransform.cpp                                       \
       ../Source/Nano/Graphics/NVector.cpp                                          \
       ../Source/Nano/Internal/3rd_Party/expat_2.0.1/xmlrole.c                      \
       ../Source/Nano/Internal/3rd_Party/expat_2.0.1/xmltok_impl.c                  \
       ../Source/Nano/Internal/3rd_Party/expat_2.0.1/xmltok_ns.c                    \
       ../Source/Nano/Internal/3rd_Party/expat_2.0.1/xmltok.c                       \
       ../Source/Nano/Internal/3rd_Party/fourmilab.ch/fm_md5.c                      \
       ../Source/Nano/Internal/3rd_Party/libb64_1.1/cdecode.c                       \
       ../Source/Nano/Internal/3rd_Party/libb64_1.1/cencode.c                       \
       ../Source/Nano/Internal/3rd_Party/pcre_7.8/pcre_chartables.c                 \
       ../Source/Nano/Internal/3rd_Party/pcre_7.8/pcre_compile.c                    \
       ../Source/Nano/Internal/3rd_Party/pcre_7.8/pcre_config.c                     \
       ../Source/Nano/Internal/3rd_Party/pcre_7.8/pcre_dfa_exec.c                   \
       ../Source/Nano/Internal/3rd_Party/pcre_7.8/pcre_exec.c                       \
       ../Source/Nano/Internal/3rd_Party/pcre_7.8/pcre_fullinfo.c                   \
       ../Source/Nano/Internal/3rd_Party/pcre_7.8/pcre_get.c                        \
       ../Source/Nano/Internal/3rd_Party/pcre_7.8/pcre_globals.c                    \
       ../Source/Nano/Internal/3rd_Party/pcre_7.8/pcre_info.c                       \
       ../Source/Nano/Internal/3rd_Party/pcre_7.8/pcre_maketables.c                 \
       ../Source/Nano/Internal/3rd_Party/pcre_7.8/pcre_newline.c                    \
       ../Source/Nano/Internal/3rd_Party/pcre_7.8/pcre_ord2utf8.c                   \
       ../Source/Nano/Internal/3rd_Party/pcre_7.8/pcre_refcount.c                   \
       ../Source/Nano/Internal/3rd_Party/pcre_7.8/pcre_study.c                      \
       ../Source/Nano/Internal/3rd_Party/pcre_7.8/pcre_tables.c                     \
       ../Source/Nano/Internal/3rd_Party/pcre_7.8/pcre_try_flipped.c                \
       ../Source/Nano/Internal/3rd_Party/pcre_7.8/pcre_ucd.c                        \
       ../Source/Nano/Internal/3rd_Party/pcre_7.8/pcre_valid_utf8.c                 \
       ../Source/Nano/Internal/3rd_Party/pcre_7.8/pcre_version.c                    \
       ../Source/Nano/Internal/3rd_Party/pcre_7.8/pcre_xclass.c                     \
       ../Source/Nano/Internal/3rd_Party/schneier.com/blowfish_dat.c                \
       ../Source/Nano/Internal/3rd_Party/schneier.com/blowfish.c                    \
       ../Source/Nano/Internal/3rd_Party/sha2_1.1a/sha2.c                           \
       ../Source/Nano/Internal/3rd_Party/sqlite_3.7.15.2/sqlite_3.7.15.2.c          \
       ../Source/Nano/Internal/3rd_Party/tero.co.uk/tero_des.cpp                    \
       ../Source/Nano/Internal/3rd_Party/udp.github.com/json.c                      \
       ../Source/Nano/Internal/3rd_Party/unicode.org/ConvertUTF.c                   \
       ../Source/Nano/Internal/3rd_Party/zlib_1.2.3/adler32.c                       \
       ../Source/Nano/Internal/3rd_Party/zlib_1.2.3/compress.c                      \
       ../Source/Nano/Internal/3rd_Party/zlib_1.2.3/crc32.c                         \
       ../Source/Nano/Internal/3rd_Party/zlib_1.2.3/deflate.c                       \
       ../Source/Nano/Internal/3rd_Party/zlib_1.2.3/gzio.c                          \
       ../Source/Nano/Internal/3rd_Party/zlib_1.2.3/infback.c                       \
       ../Source/Nano/Internal/3rd_Party/zlib_1.2.3/inffast.c                       \
       ../Source/Nano/Internal/3rd_Party/zlib_1.2.3/inflate.c                       \
       ../Source/Nano/Internal/3rd_Party/zlib_1.2.3/inftrees.c                      \
       ../Source/Nano/Internal/3rd_Party/zlib_1.2.3/trees.c                         \
       ../Source/Nano/Internal/3rd_Party/zlib_1.2.3/uncompr.c                       \
       ../Source/Nano/Internal/3rd_Party/zlib_1.2.3/zutil.c                         \
       ../Source/Nano/Internal/Targets/Linux/NLinuxFile.cpp                         \
       ../Source/Nano/Internal/Targets/Linux/NLinuxMath.cpp                         \
       ../Source/Nano/Internal/Targets/Linux/NLinuxNetwork.cpp                      \
       ../Source/Nano/Internal/Targets/Linux/NLinuxPOSIX.cpp                        \
       ../Source/Nano/Internal/Targets/Linux/NLinuxPreferences.cpp                  \
       ../Source/Nano/Internal/Targets/Linux/NLinuxSystem.cpp                       \
       ../Source/Nano/Internal/Targets/Linux/NLinuxTarget.cpp                       \
       ../Source/Nano/Internal/Targets/Linux/NLinuxThread.cpp                       \
       ../Source/Nano/Internal/Targets/Linux/NLinuxTime.cpp                         \
       ../Source/Nano/Network/NMessageClient.cpp                                    \
       ../Source/Nano/Network/NMessageEntity.cpp                                    \
       ../Source/Nano/Network/NMessageServer.cpp                                    \
       ../Source/Nano/Network/NNetworkBrowser.cpp                                   \
       ../Source/Nano/Network/NNetworkManager.cpp                                   \
       ../Source/Nano/Network/NNetworkMessage.cpp                                   \
       ../Source/Nano/Network/NNetworkService.cpp                                   \
       ../Source/Nano/Network/NSocket.cpp                                           \
       ../Source/Nano/Network/NSocketRequest.cpp                                    \
       ../Source/Nano/Network/NURLRequest.cpp                                       \
       ../Source/Nano/Network/NURLResponse.cpp                                      \
       ../Source/Nano/System/NByteSwap.cpp                                          \
       ../Source/Nano/System/NCache.cpp                                             \
       ../Source/Nano/System/NCacheKey.cpp                                          \
       ../Source/Nano/System/NCacheValue.cpp                                        \
       ../Source/Nano/System/NDataCipher.cpp                                        \
       ../Source/Nano/System/NDataCompressor.cpp                                    \
       ../Source/Nano/System/NDataDigest.cpp                                        \
       ../Source/Nano/System/NDataEncoder.cpp                                       \
       ../Source/Nano/System/NJSONEncoder.cpp                                       \
       ../Source/Nano/System/NLocale.cpp                                            \
       ../Source/Nano/System/NTask.cpp                                              \
       ../Source/Nano/System/NUnicodeParser.cpp                                     \
       ../Source/Nano/System/NUTIRegistry.cpp                                       \
       ../Source/Nano/System/NXMLEncoder.cpp                                        \
       ../Source/Nano/System/NXMLNode.cpp                                           \
       ../Source/Nano/System/NXMLParser.cpp                                         \
       ../Source/Nano/Threads/NAtomicInt.cpp                                        \
       ../Source/Nano/Threads/NAtomicList.cpp                                       \
       ../Source/Nano/Threads/NLock.cpp                                             \
       ../Source/Nano/Threads/NSemaphore.cpp                                        \
       ../Source/Nano/Threads/NThread.cpp                                           \
       ../Source/Nano/Threads/NThreadPool.cpp                                       \
       ../Source/Nano/Threads/NThreadTask.cpp                                       \
       ../Source/Nano/Types/NArray.cpp                                              \
       ../Source/Nano/Types/NAttributes.cpp                                         \
       ../Source/Nano/Types/NBitVector.cpp                                          \
       ../Source/Nano/Types/NComparable.cpp                                         \
       ../Source/Nano/Types/NContainer.cpp                                          \
       ../Source/Nano/Types/NData.cpp                                               \
       ../Source/Nano/Types/NDate.cpp                                               \
       ../Source/Nano/Types/NDateFormatter.cpp                                      \
       ../Source/Nano/Types/NDictionary.cpp                                         \
       ../Source/Nano/Types/NEncodable.cpp                                          \
       ../Source/Nano/Types/NEncoder.cpp                                            \
       ../Source/Nano/Types/NFunctor.cpp                                            \
       ../Source/Nano/Types/NHashable.cpp                                           \
       ../Source/Nano/Types/NNumber.cpp                                             \
       ../Source/Nano/Types/NProgressable.cpp                                       \
       ../Source/Nano/Types/NPropertyStore.cpp                                      \
       ../Source/Nano/Types/NRange.cpp                                              \
       ../Source/Nano/Types/NRefCounted.cpp                                         \
       ../Source/Nano/Types/NSharedValue.cpp                                        \
       ../Source/Nano/Types/NSingleton.cpp                                          \
       ../Source/Nano/Types/NString.cpp                                             \
       ../Source/Nano/Types/NStringEncoder.cpp                                      \
       ../Source/Nano/Types/NStringFormatter.cpp                                    \
       ../Source/Nano/Types/NUncopyable.cpp                                         \
       ../Source/Nano/Types/NURL.cpp                                                \
       ../Source/Nano/Types/NUTI.cpp                                                \
       ../Source/Nano/Types/NVariant.cpp                                            \
       ../Source/Nano/Utilities/NFileUtilities.cpp                                  \
       ../Source/Nano/Utilities/NGeometryUtilities.cpp                              \
       ../Source/Nano/Utilities/NMathUtilities.cpp                                  \
       ../Source/Nano/Utilities/NSTLUtilities.cpp                                   \
       ../Source/Nano/Utilities/NSystemUtilities.cpp                                \
       ../Source/Nano/Utilities/NTextUtilities.cpp                                  \
       ../Source/Nano/Utilities/NThreadUtilities.cpp                                \
       ../Source/Nano/Utilities/NTimeUtilities.cpp


TARGET = nano.a
OBJS   = $(patsubst %.c,%.o,       $(patsubst %.cpp,%.o,       $(SRCS)) )
DEPS   = $(patsubst %.c,%.depends, $(patsubst %.cpp,%.depends, $(SRCS)) )





.PHONY: clean all

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $(TARGET) $(OBJS)


.cpp.o:
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

.c.o:
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@


%.depends: %.cpp
	$(CXX) $(CXXFLAGS) $(INC) -M -c $< > $@

%.depends: %.c
	$(CXX) $(CXXFLAGS) $(INC) -M -c $< > $@


clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)

-include $(DEPS)


CXX     = gcc
LDFLAGS = -lstdc++ -lpthread -lrt -ldl -lm
AR      = ar
ARFLAGS = rcs


TARGET = nanotest
SRCS   =                                                                             \
        ../Source/CTestListener.cpp                                                  \
        ../Source/CTestUtilities.cpp                                                 \
        ../Source/Nano/Application/TCommandLine.cpp                                  \
        ../Source/Nano/Application/TPreferences.cpp                                  \
        ../Source/Nano/Application/TUndoManager.cpp                                  \
        ../Source/Nano/Application/TUndoSource.cpp                                   \
        ../Source/Nano/Core/TDebug.cpp                                               \
        ../Source/Nano/Database/TDBHandle.cpp                                        \
        ../Source/Nano/Database/TDBHandlePool.cpp                                    \
        ../Source/Nano/Events/TBroadcaster.cpp                                       \
        ../Source/Nano/Events/TListener.cpp                                          \
        ../Source/Nano/Events/TTimer.cpp                                             \
        ../Source/Nano/Files/TBundle.cpp                                             \
        ../Source/Nano/Files/TFile.cpp                                               \
        ../Source/Nano/Files/TFileIterator.cpp                                       \
        ../Source/Nano/Files/TFileMap.cpp                                            \
        ../Source/Nano/Files/TPropertyList.cpp                                       \
        ../Source/Nano/Graphics/TColor.cpp                                           \
        ../Source/Nano/Graphics/TImage.cpp                                           \
        ../Source/Nano/Graphics/TPoint.cpp                                           \
        ../Source/Nano/Graphics/TRectangle.cpp                                       \
        ../Source/Nano/Graphics/TShape.cpp                                           \
        ../Source/Nano/Graphics/TSize.cpp                                            \
        ../Source/Nano/Graphics/TTransform.cpp                                       \
        ../Source/Nano/Graphics/TVector.cpp                                          \
        ../Source/Nano/Network/TMessageClient.cpp                                    \
        ../Source/Nano/Network/TMessageServer.cpp                                    \
        ../Source/Nano/Network/TNetworkBrowser.cpp                                   \
        ../Source/Nano/Network/TNetworkManager.cpp                                   \
        ../Source/Nano/Network/TNetworkMessage.cpp                                   \
        ../Source/Nano/Network/TNetworkService.cpp                                   \
        ../Source/Nano/Network/TSocket.cpp                                           \
        ../Source/Nano/Network/TURLRequest.cpp                                       \
        ../Source/Nano/Network/TURLResponse.cpp                                      \
        ../Source/Nano/System/TByteSwap.cpp                                          \
        ../Source/Nano/System/TCache.cpp                                             \
        ../Source/Nano/System/TCacheKey.cpp                                          \
        ../Source/Nano/System/TCacheValue.cpp                                        \
        ../Source/Nano/System/TDataCipher.cpp                                        \
        ../Source/Nano/System/TDataCompressor.cpp                                    \
        ../Source/Nano/System/TDataDigest.cpp                                        \
        ../Source/Nano/System/TDataEncoder.cpp                                       \
        ../Source/Nano/System/TJSONEncoder.cpp                                       \
        ../Source/Nano/System/TLocale.cpp                                            \
        ../Source/Nano/System/TTask.cpp                                              \
        ../Source/Nano/System/TUnicodeParser.cpp                                     \
        ../Source/Nano/System/TUTIRegistry.cpp                                       \
        ../Source/Nano/System/TXMLEncoder.cpp                                        \
        ../Source/Nano/System/TXMLNode.cpp                                           \
        ../Source/Nano/System/TXMLParser.cpp                                         \
        ../Source/Nano/Threads/TAtomicInt.cpp                                        \
        ../Source/Nano/Threads/TAtomicList.cpp                                       \
        ../Source/Nano/Threads/TLock.cpp                                             \
        ../Source/Nano/Threads/TMutex.cpp                                            \
        ../Source/Nano/Threads/TReadWriteLock.cpp                                    \
        ../Source/Nano/Threads/TSemaphore.cpp                                        \
        ../Source/Nano/Threads/TSpinLock.cpp                                         \
        ../Source/Nano/Threads/TThread.cpp                                           \
        ../Source/Nano/Threads/TThreadPool.cpp                                       \
        ../Source/Nano/Types/TArray.cpp                                              \
        ../Source/Nano/Types/TAttributes.cpp                                         \
        ../Source/Nano/Types/TBitVector.cpp                                          \
        ../Source/Nano/Types/TData.cpp                                               \
        ../Source/Nano/Types/TDate.cpp                                               \
        ../Source/Nano/Types/TDateFormatter.cpp                                      \
        ../Source/Nano/Types/TDictionary.cpp                                         \
        ../Source/Nano/Types/TEncoder.cpp                                            \
        ../Source/Nano/Types/TFunctor.cpp                                            \
        ../Source/Nano/Types/TNumber.cpp                                             \
        ../Source/Nano/Types/TPropertyStore.cpp                                      \
        ../Source/Nano/Types/TRange.cpp                                              \
        ../Source/Nano/Types/TSingleton.cpp                                          \
        ../Source/Nano/Types/TString.cpp                                             \
        ../Source/Nano/Types/TStringEncoder.cpp                                      \
        ../Source/Nano/Types/TStringFormatter.cpp                                    \
        ../Source/Nano/Types/TURL.cpp                                                \
        ../Source/Nano/Types/TUTI.cpp                                                \
        ../Source/Nano/Types/TVariant.cpp                                            \
        ../Source/Nano/Utilities/TFileUtilities.cpp                                  \
        ../Source/Nano/Utilities/TGeometryUtilities.cpp                              \
        ../Source/Nano/Utilities/TMathUtilities.cpp                                  \
        ../Source/Nano/Utilities/TSTLUtilities.cpp                                   \
        ../Source/Nano/Utilities/TSystemUtilities.cpp                                \
        ../Source/Nano/Utilities/TTextUtilities.cpp                                  \
        ../Source/Nano/Utilities/TThreadUtilities.cpp                                \
        ../Source/Nano/Utilities/TTimeUtilities.cpp                                  \
        ../Source/NanoTest.cpp                                                       \


LIBS         = ../../../Library/Project/nano.a
OBJS         = $(patsubst %.c,%.o,       $(patsubst %.cpp,%.o,       $(SRCS) ))
DEPENDENCIES = $(patsubst %.c,%.depends, $(patsubst %.cpp,%.depends, $(SRCS) ))


FLAGS_NANO =                                                                         \
        -include "../Prefix/NanoTest Prefix.h"                                       \
        -DNN_DEBUG=1 -g                                                              \

FLAGS_INCLUDE =                                                                      \
        -I"../Prefix"                                                                \
        -I"../Source"                                                                \
        -I"../Source/Nano/Files"                                                     \
        -I"../Source/Nano/Application"                                               \
        -I"../Source/Nano/Graphics"                                                  \
        -I"../Source/Nano/Network"                                                   \
        -I"../Source/Nano/Utilities"                                                 \
        -I"../Source/Nano/Database"                                                  \
        -I"../Source/Nano/Core"                                                      \
        -I"../Source/Nano/Types"                                                     \
        -I"../Source/Nano/System"                                                    \
        -I"../Source/Nano/Events"                                                    \
        -I"../Source/Nano/Threads"                                                   \
        -I"../Source/Targets/Linux"                                                  \
        -I"../../../Library/Source/Nanites/Catch"                                    \
        -I"../../../Library/Source/Nano/Application"                                 \
        -I"../../../Library/Source/Nano/Core"                                        \
        -I"../../../Library/Source/Nano/Database"                                    \
        -I"../../../Library/Source/Nano/Events"                                      \
        -I"../../../Library/Source/Nano/Files"                                       \
        -I"../../../Library/Source/Nano/Graphics"                                    \
        -I"../../../Library/Source/Nano/Internal"                                    \
        -I"../../../Library/Source/Nano/Network"                                     \
        -I"../../../Library/Source/Nano/System"                                      \
        -I"../../../Library/Source/Nano/Threads"                                     \
        -I"../../../Library/Source/Nano/Types"                                       \
        -I"../../../Library/Source/Nano/Utilities"                                   \

FLAGS_WARN = -Wall -Wno-unknown-pragmas



.PHONY: clean all

all: $(TARGET)
	cd ../../../Library/Project;                                                     \
        $(MAKE)

$(TARGET): $(OBJS) $(LIBS)
	$(CXX) $(OBJS) $(LIBS) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(OBJS) $(DEPENDENCIES) $(TARGET);                                         \
	cd ../../../Library/Project;                                                     \
	$(MAKE) clean


%.a:
	cd $(dir $@) && $(MAKE)

.cpp.o:
	$(CXX) $(FLAGS_NANO) $(FLAGS_INCLUDE) $(FLAGS_WARN) -c $< -o $@

%.depends: %.cpp
	$(CXX) $(FLAGS_NANO) $(FLAGS_INCLUDE) $(FLAGS_WARN) -M -c $< > $@

-include $(DEPENDENCIES)


# CMake Version
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

# Project Name
PROJECT(RPi3OnBoard)

# Configuration
# Set Build Directories
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(
	./ 
	./Dependence/Luna/src/Library/ccCore/include
	./Dependence/Luna/src/Library/ccNetwork/include
	./Dependence/Luna/src/Library/ccWebServer/include
	./Dependence/Luna/src/FossLibrary/ccJsonParser/include
	./Dependence/Luna/src/FossLibrary/ccWebsocketClient/include
	./Dependence/Luna/src/ThirdPartyLibrary/ccMongooseServer/include)


# Check OS and Processor Information 
#
SET(CMAKE_CXX_FLAGS "-std=c++11")
ADD_DEFINITIONS(-Wall -DCALIB_CFG_OS="${CMAKE_SYSTEM_NAME}" -DCALIB_CFG_CPU="${CMAKE_SYSTEM_PROCESSOR}")

# link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

#
# Make Libraries
#
ADD_SUBDIRECTORY(./Dependence/Luna)
ADD_SUBDIRECTORY(./IoTDeviceManagerEmulator)

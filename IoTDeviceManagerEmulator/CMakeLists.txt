# CMake Version
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

# Project Name
PROJECT(IoTDeviceManagerEmulator)

# Configuration

# Check OS and Processor Information 
#
SET(CMAKE_CXX_FLAGS "-std=c++11")
ADD_DEFINITIONS(-Wall -DCALIB_CFG_OS="${CMAKE_SYSTEM_NAME}" -DCALIB_CFG_CPU="${CMAKE_SYSTEM_PROCESSOR}")

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

file(GLOB SRC_FILES 
	Impl/*.cpp
	Linux/*.cpp
)

add_executable (IoTDeviceManagerEmulator
    ${SRC_FILES}
)

TARGET_LINK_LIBRARIES(IoTDeviceManagerEmulator
	ccMongooseServer
	ccJsonParser
	ccWebServer
	ccNetwork
	ccCore
	pthread
)  # IoTDeviceManagerEmulator now depends on ccCore ... and ccMongooseServer


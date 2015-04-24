#######################################################################
#
#       General
#
########################################################################

# CMake minimum version
cmake_minimum_required(VERSION 2.8)

# Project name
project(ircbot)

# Default build type is debug
if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Setup some folders
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(RES_DIR "${CMAKE_SOURCE_DIR}/data")
set(BIN_DIR "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")

# Change CMake module path
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Change CMake output path
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BIN_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BIN_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}/bin")

# Setup CMake compilation flags
set(CMAKE_CXX_FLAGS "-std=gnu++0x")

# Link with static curl
add_definitions("-DCURL_STATICLIB")

########################################################################
#
#       Sources
#
########################################################################

# Header folder
include_directories("${SRC_DIR}")

# Executable name
set(EXECUTABLE "ircbot")

# Get sources
file(GLOB_RECURSE EXECUTABLE_SOURCES
    "${SRC_DIR}/*.h"
    "${SRC_DIR}/*.c"
    "${SRC_DIR}/*.cc"
)

# Add executable
add_executable(${EXECUTABLE} ${EXECUTABLE_SOURCES})

# If we're building on windows
if(WIN32)
	# Add the -mconsole compilation flag
    set_target_properties(${EXECUTABLE} PROPERTIES COMPILE_FLAGS "-mconsole")
endif()

########################################################################
#
#       Boost
#
########################################################################

# Custom Boost root
list(APPEND BOOST_ROOT
	"C:/Program Files/Boost/boost_1_57_0"
	"C:/Program Files/Boost/boost_1_55_0"
	"C:/Program Files (x86)/Boost/boost_1_57_0"
	"C:/Program Files (x86)/Boost/boost_1_55_0"
)

# Static libraries
set(Boost_USE_STATIC_LIBS ON)

# Multithreaded
set(Boost_USE_MULTITHREADED ON)

# Static runtime
set(Boost_USE_STATIC_RUNTIME OFF)

# Find boost installation
find_package(Boost 1.55.0 COMPONENTS
	log
	log_setup
	system
	filesystem
	date_time
	regex
	locale
	thread
)

# Not found
if(NOT Boost_FOUND)
	# Error
	message(FATAL_ERROR "Could not find Boost 1.55.0 or 1.57.0")
endif()

# Include boost headers
include_directories(${Boost_INCLUDE_DIRS})

# Link with boost libraries statically
target_link_libraries(${EXECUTABLE} ${Boost_LIBRARIES})

########################################################################
#
#       libircclient
#
########################################################################

# Custom libircclient root
list(APPEND IRCCLIENT_ROOT
	"C:/Program Files/libircclient"
	"C:/Program Files (x86)/libircclient"
)

# Find libircclient
find_package(Ircclient)

# Not found
if(NOT IRCCLIENT_FOUND)
	# Error
	message(FATAL_ERROR "Could not find libircclient")
else()
    message(STATUS "Found ircclient")
endif()

# Include headers
include_directories(${IRCCLIENT_INCLUDE_DIR})

# Link to libraries
target_link_libraries(${EXECUTABLE} ${IRCCLIENT_LIBRARY})

# If on windows
if(WIN32)
	# Copy libircclient dll to bin folder
	file(COPY ${IRCCLIENT_BINARY} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

########################################################################
#
#       hiredis
#
########################################################################

# Custom hiredis root
list(APPEND HIREDIS_ROOT
	"C:/Program Files/hiredis"
	"C:/Program Files (x86)/hiredis"
)

# Find hiredis
find_package(Hiredis)

# Not found
if(NOT HIREDIS_FOUND)
	# Error
	message(FATAL_ERROR "Could not find hiredis")
else()
	message(STATUS "Found hiredis")
endif()

# Include headers
include_directories(${HIREDIS_INCLUDE_DIR})

# Link to libraries
target_link_libraries(${EXECUTABLE} ${HIREDIS_LIBRARY})

# If on windows
if(WIN32)
	# Copy hiredis dll to bin folder
	file(COPY ${HIREDIS_BINARY} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

########################################################################
#
#       curl
#
########################################################################

# Custom search paths for curl
set(CMAKE_INCLUDE_PATH "C:/Program Files/libcurl/include")
set(CMAKE_LIBRARY_PATH "C:/Program Files/libcurl/lib")

# Find curl
find_package(CURL)

# If curl is not found
if(NOT CURL_FOUND)
	# Error
	message(FATAL_ERROR "Could not find CURL")
else()
	message(STATUS "Found CURL")
endif()

# Include curl directories
include_directories(${CURL_INCLUDE_DIRS})

# Link with curl
target_link_libraries(${EXECUTABLE} ${CURL_LIBRARIES})

########################################################################
#
#       Lua
#
########################################################################

# Find Lua
find_package(Lua51)

# If Lua is not found
if(NOT LUA_FOUND)
	# Error
	message(FATAL_ERROR "Could not find Lua")
endif()

# Include Lua directories
include_directories(${LUA_INCLUDE_DIR})

# Link with Lua libraries
target_link_libraries(${EXECUTABLE} ${LUA_LIBRARIES})

########################################################################
#
#       jsoncpp
#
########################################################################

# Custom jsoncpp root
list(APPEND JSONCPP_ROOT
	"C:/Program Files/jsoncpp"
	"C:/Program Files (x86)/jsoncpp"
)

# Find jsoncpp
find_package(JsonCpp)

# Not found
if(NOT JSONCPP_FOUND)
	# Error
	message(FATAL_ERROR "Could not find jsoncpp")
else()
	message(STATUS "Found jsoncpp")
endif()

# Include headers
include_directories(${JSONCPP_INCLUDE_DIR})

# Link to libraries
target_link_libraries(${EXECUTABLE} ${JSONCPP_LIBRARY})

########################################################################
#
#       Other libraries
#
########################################################################

# If compiling on windows
if(WIN32)
	# Link with winsock2 libraries (For libircclient and Curl)
	target_link_libraries(${EXECUTABLE} "ws2_32")

	# Link with ldap (For Curl)
	target_link_libraries(${EXECUTABLE} "wldap32")
endif()

########################################################################
#
#       Copy data things
#
########################################################################

# Copy the scripts folder
file(COPY "${RES_DIR}/scripts" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

# Copy the config.ini file
file(COPY "${RES_DIR}/config.ini" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

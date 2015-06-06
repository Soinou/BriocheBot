#######################################################################
#
#       General
#
########################################################################

# CMake minimum version
cmake_minimum_required(VERSION 2.8)

# Project name
project(BriocheBot)

# Default build type is debug
if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Setup some folders
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(RES_DIR "${CMAKE_SOURCE_DIR}/data")
set(LIB_DIR "${CMAKE_SOURCE_DIR}/lib")
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
#       hiredis
#
########################################################################

# Get hiredis directory
set(HIREDIS_DIR "${LIB_DIR}/hiredis")

# Include hiredis directory
include_directories("${HIREDIS_DIR}")

# Get hiredis sources
file(GLOB_RECURSE HIREDIS_SRC "${HIREDIS_DIR}/*.[ch]")

########################################################################
#
#       jsoncpp
#
########################################################################

# Get jsoncpp directory
set(JSONCPP_DIR "${LIB_DIR}/jsoncpp")

# Include jsoncpp directory
include_directories("${JSONCPP_DIR}")

# Get jsoncpp sources
file(GLOB_RECURSE JSONCPP_SRC "${JSONCPP_DIR}/*.h" "${JSONCPP_DIR}/*.cpp")

########################################################################
#
#       HappyHttp
#
########################################################################

# Get happyhttp directory
set(HAPPYHTTP_DIR "${LIB_DIR}/happyhttp")

# Include happyhttp directory
include_directories("${HAPPYHTTP_DIR}")

# Get happyhttp sources
file(GLOB_RECURSE HAPPYHTTP_SRC "${HAPPYHTTP_DIR}/*.cpp" "${HAPPYHTTP_DIR}/*.h")

########################################################################
#
#       Lua
#
########################################################################

# Get lua directory
set(LUA_DIR "${LIB_DIR}/lua")

# Include lua directory
include_directories("${LUA_DIR}")

# Get lua sources
file(GLOB_RECURSE LUA_SRC "${LUA_DIR}/*.[ch]")

########################################################################
#
#       Sources
#
########################################################################

# Header folder
include_directories("${SRC_DIR}")

# Executable name
set(EXECUTABLE "BriocheBot")

# Get sources
file(GLOB_RECURSE EXECUTABLE_SOURCES "${SRC_DIR}/*.[hc]" "${SRC_DIR}/*.cc")

# Add executable
add_executable(${EXECUTABLE}
    ${LIBIRCCLIENT_SRC}
    ${HIREDIS_SRC}
    ${JSONCPP_SRC}
    ${HAPPYHTTP_SRC}
    ${LUA_SRC}
    ${EXECUTABLE_SOURCES}
)

# If we're building on windows
if(WIN32)
	# Add the -mconsole compilation flag
    set_target_properties(${EXECUTABLE} PROPERTIES COMPILE_FLAGS "-mconsole")
# Else, if we're building on unix
elseif(UNIX)
	# Add the pthread flag
	set_target_properties(${EXECUTABLE} PROPERTIES COMPILE_FLAGS "-pthread")
endif()

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
# Else, if unix
elseif(UNIX)
	# Link with lpthread
	target_link_libraries(${EXECUTABLE} "pthread")
endif()

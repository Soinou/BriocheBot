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
set(CMAKE_C_FLAGS "-std=gnu89")
set(CMAKE_CXX_FLAGS "-std=c++0x")

# Add some definitions on windows
if(WIN32)
    add_definitions("-DWIN32_LEAN_AND_MEAN -D_WIN32_WINNT=0x0600")
endif()

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
#       libuv
#
########################################################################

# Get libuv directory
set(LIBUV_DIR "${LIB_DIR}/libuv")

# Include libuv directory
include_directories("${LIBUV_DIR}")

# Platform specific directives
if(WIN32)
    # Include win directory on windows
    include_directories("${LIBUV_DIR}/win")

    # Include sources in src and src/win directories
    file(GLOB LIBUV_SRC "${LIBUV_DIR}/*.[ch]" "${LIBUV_DIR}/win/*.[ch]")
else()
    # Include unix directory on unix
    include_directories("${LIBUV_DIR}/unix")

    # Include sources in src and src/unix directories
    file(GLOB LIBUV_SRC "${LIBUV_DIR}/*.[ch]" "${LIBUV_DIR}/unix/*.[ch]")
endif()

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

# Extra executable name
set(EXTRA "BriocheBotExtra")

# Get sources
file(GLOB_RECURSE EXECUTABLE_SOURCES
    "${SRC_DIR}/ini/*.[hc]"
    "${SRC_DIR}/ini/*.cc"
    "${SRC_DIR}/irc/*.h"
    "${SRC_DIR}/irc/*.cc"
    "${SRC_DIR}/lua/*.h"
    "${SRC_DIR}/lua/*.cc"
    "${SRC_DIR}/models/*.h"
    "${SRC_DIR}/models/*.cc"
    "${SRC_DIR}/osu/*.h"
    "${SRC_DIR}/osu/*.cc"
    "${SRC_DIR}/scripts/*.h"
    "${SRC_DIR}/scripts/*.cc"
    "${SRC_DIR}/server/*.h"
    "${SRC_DIR}/server/*.cc"
    "${SRC_DIR}/utils/*.h"
    "${SRC_DIR}/utils/*.cc"
    "${SRC_DIR}/uv/*.h"
    "${SRC_DIR}/uv/*.cc"
    "${SRC_DIR}/main.cc"
)

# Get extra executable sources
file(GLOB_RECURSE EXTRA_SOURCES
    "${SRC_DIR}/ini/*.[hc]"
    "${SRC_DIR}/ini/*.cc"
    "${SRC_DIR}/irc/*.h"
    "${SRC_DIR}/irc/*.cc"
    "${SRC_DIR}/lua/*.h"
    "${SRC_DIR}/lua/*.cc"
    "${SRC_DIR}/models/*.h"
    "${SRC_DIR}/models/*.cc"
    "${SRC_DIR}/osu/*.h"
    "${SRC_DIR}/osu/*.cc"
    "${SRC_DIR}/scripts/*.h"
    "${SRC_DIR}/scripts/*.cc"
    "${SRC_DIR}/server/*.h"
    "${SRC_DIR}/server/*.cc"
    "${SRC_DIR}/utils/*.h"
    "${SRC_DIR}/utils/*.cc"
    "${SRC_DIR}/uv/*.h"
    "${SRC_DIR}/uv/*.cc"
    "${SRC_DIR}/extra.cc"
)

# Add executable
add_executable(${EXECUTABLE}
    ${LIBIRCCLIENT_SRC}
    ${HIREDIS_SRC}
    ${JSONCPP_SRC}
    ${HAPPYHTTP_SRC}
    ${LIBUV_SRC}
    ${LUA_SRC}
    ${EXECUTABLE_SOURCES}
)

# Add the extra executable
add_executable(${EXTRA}
    ${LIBIRCCLIENT_SRC}
    ${HIREDIS_SRC}
    ${JSONCPP_SRC}
    ${HAPPYHTTP_SRC}
    ${LIBUV_SRC}
    ${LUA_SRC}
    ${EXTRA_SOURCES}
)

# If we're building on windows
if(WIN32)
	# Add the -mconsole compilation flag
    set_target_properties(${EXECUTABLE} PROPERTIES COMPILE_FLAGS "-mconsole")
    set_target_properties(${EXTRA} PROPERTIES COMPILE_FLAGS "-mconsole")
# Else, if we're building on unix
elseif(UNIX)
	# Add the pthread flag
	set_target_properties(${EXECUTABLE} PROPERTIES COMPILE_FLAGS "-pthread")
    set_target_properties(${EXTRA} PROPERTIES COMPILE_FLAGS "-pthread")
endif()

########################################################################
#
#       Other libraries
#
########################################################################

# If compiling on windows
if(WIN32)
    # Link with windows libraries
	target_link_libraries(${EXECUTABLE} "ws2_32")
	target_link_libraries(${EXECUTABLE} "wldap32")
    target_link_libraries(${EXECUTABLE} "iphlpapi")
    target_link_libraries(${EXECUTABLE} "psapi")
    target_link_libraries(${EXECUTABLE} "userenv")

    # Link the extra executable with windows libraries
    target_link_libraries(${EXTRA} "ws2_32")
	target_link_libraries(${EXTRA} "wldap32")
    target_link_libraries(${EXTRA} "iphlpapi")
    target_link_libraries(${EXTRA} "psapi")
    target_link_libraries(${EXTRA} "userenv")
# Else, if unix
elseif(UNIX)
    # Link with unix libraries
    target_link_libraries(${EXECUTABLE} "dl")
    target_link_libraries(${EXECUTABLE} "pthread")

    # Link the extra executable with unix libraries
    target_link_libraries(${EXTRA} "dl")
    target_link_libraries(${EXTRA} "pthread")
endif()

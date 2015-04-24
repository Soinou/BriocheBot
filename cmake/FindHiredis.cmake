# Locate hiredis library
# This module defines
#  HIREDIS_FOUND, if false, do not try to link to HIREDIS
#  HIREDIS_LIBRARY, the hiredis variant
#  HIREDIS_BINARY, the hiredis binary (dll) file. Only on Windows platforms
#  HIREDIS_INCLUDE_DIR, where to find hiredis.h and family)

FIND_PATH(HIREDIS_INCLUDE_DIR hiredis/hiredis.h
   HINTS
   $ENV{HIREDISDIR}
   PATH_SUFFIXES include/hiredis include
   PATHS
   ${HIREDIS_ROOT}
   ~/Library/Frameworks
   /Library/Frameworks
   /usr/local
   /usr
   /sw # Fink
   /opt/local # DarwinPorts
   /opt/csw # Blastwave
   /opt
)

SET(HIREDIS_FIND_LIBRARIES hiredis)

OPTION(WANT_STATIC_LIBS "builds as many static libs as possible" OFF)
OPTION(FORCE_HIREDIS_DYNAMIC_LIBS "force the use of dynamic libs for HIREDIS" OFF)

IF(WANT_STATIC_LIBS AND NOT FORCE_HIREDIS_DYNAMIC_LIBS)
   SET(HIREDIS_FIND_LIBRARIES hiredis.a)
ENDIF()

FIND_LIBRARY(HIREDIS_LIBRARY 
   NAMES ${HIREDIS_FIND_LIBRARIES}
   HINTS
   $ENV{HIREDISDIR}
   PATH_SUFFIXES lib64 lib libs64 libs libs/Win32 libs/Win64
   PATHS
   ${HIREDIS_ROOT}
   ~/Library/Frameworks
   /Library/Frameworks
   /usr/local
   /usr
   /sw
   /opt/local
   /opt/csw
   /opt
)

# If searching on Windows
IF(WIN32)
   # Find the dll file
   FIND_LIBRARY(HIREDIS_BINARY
      NAMES libhiredis.dll
      HINTS
      $ENV{HIREDISDIR}
      PATH_SUFFIXES bin bin32 bin64 bin/Win32 bin/Win64
      PATHS
      ${HIREDIS_ROOT}
      ~/Library/Frameworks
      /Library/Frameworks
      /usr/local
      /usr
      /sw
      /opt/local
      /opt/csw
      /opt
   )
ENDIF()

# handle the QUIETLY and REQUIRED arguments and set HIREDIS_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)

IF(WIN32)
   FIND_PACKAGE_HANDLE_STANDARD_ARGS(HIREDIS DEFAULT_MSG HIREDIS_LIBRARY HIREDIS_BINARY HIREDIS_INCLUDE_DIR)
ELSE()
   FIND_PACKAGE_HANDLE_STANDARD_ARGS(HIREDIS DEFAULT_MSG HIREDIS_LIBRARY HIREDIS_INCLUDE_DIR)
ENDIF()

MARK_AS_ADVANCED(HIREDIS_LIBRARY HIREDIS_INCLUDE_DIR)
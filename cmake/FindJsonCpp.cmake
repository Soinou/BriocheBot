# Locate jsoncpp library
# This module defines
#  JSONCPP_FOUND, if false, do not try to link to jsoncpp
#  JSONCPP_LIBRARY, the jsoncpp variant
#  JSONCPP_INCLUDE_DIR, where to find json.h and family)

FIND_PATH(JSONCPP_INCLUDE_DIR json/json.h
   HINTS
   $ENV{JSONCPPDIR}
   PATH_SUFFIXES include/jsoncpp include include
   PATHS
   ${JSONCPP_ROOT}
   ~/Library/Frameworks
   /Library/Frameworks
   /usr/local
   /usr
   /sw # Fink
   /opt/local # DarwinPorts
   /opt/csw # Blastwave
   /opt
)

FIND_LIBRARY(JSONCPP_LIBRARY
   NAMES libjsoncpp.a
   HINTS
   $ENV{JSONCPPDIR}
   PATH_SUFFIXES lib64 lib libs64 libs libs/Win32 libs/Win64
   PATHS
   ${JSONCPP_ROOT}
   ~/Library/Frameworks
   /Library/Frameworks
   /usr/local
   /usr
   /sw
   /opt/local
   /opt/csw
   /opt
)

# handle the QUIETLY and REQUIRED arguments and set HIREDIS_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(JSONCPP DEFAULT_MSG JSONCPP_LIBRARY JSONCPP_INCLUDE_DIR)

MARK_AS_ADVANCED(JSONCPP_LIBRARY JSONCPP_INCLUDE_DIR)

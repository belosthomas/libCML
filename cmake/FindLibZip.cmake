# Finds libzip.
#
# This module defines:
# LIBZIP_INCLUDE_DIR_ZIP
# LIBZIP_INCLUDE_DIR_ZIPCONF
# LIBZIP_LIBRARY
#

find_package(PkgConfig)
pkg_check_modules(PC_LIBZIP QUIET libzip)

find_path(LIBZIP_INCLUDE_DIR_ZIP
        NAMES zip.h
        HINTS ${PC_LIBZIP_INCLUDE_DIRS})

find_path(LIBZIP_INCLUDE_DIR_ZIPCONF
        NAMES zipconf.h
        HINTS ${PC_LIBZIP_INCLUDE_DIRS})

find_library(LIBZIP_LIBRARY
        NAMES zip)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
        LIBZIP DEFAULT_MSG
        LIBZIP_LIBRARY LIBZIP_INCLUDE_DIR_ZIP LIBZIP_INCLUDE_DIR_ZIPCONF)

set(LIBZIP_VERSION 0)

if (LIBZIP_INCLUDE_DIR_ZIPCONF)
    FILE(READ "${LIBZIP_INCLUDE_DIR_ZIPCONF}/zipconf.h" _LIBZIP_VERSION_CONTENTS)
    if (_LIBZIP_VERSION_CONTENTS)
        STRING(REGEX REPLACE ".*#define LIBZIP_VERSION \"([0-9.]+)\".*" "\\1" LIBZIP_VERSION "${_LIBZIP_VERSION_CONTENTS}")
    endif ()
endif ()

set(LIBZIP_VERSION ${LIBZIP_VERSION} CACHE STRING "Version number of libzip")

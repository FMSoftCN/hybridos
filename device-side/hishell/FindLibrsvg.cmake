# - Try to find Librsvg 2.0



find_package(PkgConfig QUIET)
pkg_check_modules(PC_LIBRSVG librsvg-2.0)

find_path(LIBRSVG_INCLUDE_DIR
    NAMES librsvg/rsvg.h
    HINTS ${PC_LIBRSVG_INCLUDEDIR} ${PC_LIBRSVG_INCLUDE_DIRS}
)

find_library(LIBRSVG_LIBRARIES
    NAMES rsvg-2
    HINTS ${PC_LIBRSVG_LIBDIR}
          ${PC_LIBRSVG_LIBRARY_DIRS}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Librsvg REQUIRED_VARS LIBRSVG_LIBRARIES
                                  FOUND_VAR LIBRSVG_FOUND)

mark_as_advanced(LIBRSVG_INCLUDE_DIR LIBRSVG_LIBRARIES)

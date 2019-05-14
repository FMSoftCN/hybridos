include(GNUInstallDirs)
include(VersioningUtils)

SET_PROJECT_VERSION(1 0 0)
set(HFCL_API_VERSION 1.0)

# These are shared variables, but we special case their definition so that we can use the
# CMAKE_INSTALL_* variables that are populated by the GNUInstallDirs macro.
set(LIB_INSTALL_DIR "${CMAKE_INSTALL_FULL_LIBDIR}" CACHE PATH "Absolute path to library installation directory")
set(EXEC_INSTALL_DIR "${CMAKE_INSTALL_FULL_BINDIR}" CACHE PATH "Absolute path to executable installation directory")
set(LIBEXEC_INSTALL_DIR "${CMAKE_INSTALL_FULL_LIBEXECDIR}/hfcl-${HFCL_API_VERSION}" CACHE PATH "Absolute path to install executables executed by the library")

set(HFCL_HEADER_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}/hfcl-${HFCL_API_VERSION}")

find_package(MiniGUI 4.0.0 REQUIRED)
find_package(Cairo 1.10.2 REQUIRED)
find_package(Freetype 2.4.2 REQUIRED)
#find_package(LibGcrypt 1.6.0 REQUIRED)
find_package(HarfBuzz 1.4.0 REQUIRED)
find_package(JPEG REQUIRED)
find_package(PNG REQUIRED)
find_package(Sqlite REQUIRED)
#find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)

#include(GStreamerDefinitions)

#SET_AND_EXPOSE_TO_BUILD(USE_CAIRO TRUE)
#SET_AND_EXPOSE_TO_BUILD(USE_XDGMIME TRUE)
#SET_AND_EXPOSE_TO_BUILD(USE_GCRYPT TRUE)

if (WTF_CPU_ARM OR WTF_CPU_MIPS)
    SET_AND_EXPOSE_TO_BUILD(USE_CAPSTONE ${DEVELOPER_MODE})
endif ()


# - Try to find MiniGUI
# Once done, this will define
#
#  MINIGUI_FOUND - system has MiniGUI
#  MINIGUI_INCLUDE_DIRS - the MiniGUI include directories
#  MINIGUI_LIBRARIES - link these to use MiniGUI
#
# Copyright (C) 2012 Raphael Kubo da Costa <rakuco@webkit.org>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1.  Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
# 2.  Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND ITS CONTRIBUTORS ``AS
# IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR ITS
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

find_package(PkgConfig)
pkg_check_modules(PC_MINIGUI QUIET minigui)

find_path(MINIGUI_INCLUDE_DIRS
    NAMES minigui.h
    HINTS ${PC_MINIGUI_INCLUDEDIR}
          ${PC_MINIGUI_INCLUDE_DIRS}
    PATH_SUFFIXES minigui
)

find_library(MINIGUI_LIBRARIES
    NAMES minigui_ths
    HINTS ${PC_MINIGUI_LIBDIR}
          ${PC_MINIGUI_LIBRARY_DIRS}
)

if (MINIGUI_INCLUDE_DIRS)
    if (EXISTS "${MINIGUI_INCLUDE_DIRS}/mgconfig.h")
        file(READ "${MINIGUI_INCLUDE_DIRS}/mgconfig.h" MINIGUI_VERSION_CONTENT)

        string(REGEX MATCH "#define +MINIGUI_MAJOR_VERSION +([0-9]+)" _dummy "${MINIGUI_VERSION_CONTENT}")
        set(MINIGUI_VERSION_MAJOR "${CMAKE_MATCH_1}")

        string(REGEX MATCH "#define +MINIGUI_MINOR_VERSION +([0-9]+)" _dummy "${MINIGUI_VERSION_CONTENT}")
        set(MINIGUI_VERSION_MINOR "${CMAKE_MATCH_1}")

        string(REGEX MATCH "#define +MINIGUI_MICRO_VERSION +([0-9]+)" _dummy "${MINIGUI_VERSION_CONTENT}")
        set(MINIGUI_VERSION_MICRO "${CMAKE_MATCH_1}")

        set(MINIGUI_VERSION "${MINIGUI_VERSION_MAJOR}.${MINIGUI_VERSION_MINOR}.${MINIGUI_VERSION_MICRO}")
    endif ()
endif ()

if ("${MiniGUI_FIND_VERSION}" VERSION_GREATER "${MINIGUI_VERSION}")
    message(FATAL_ERROR "Required version (" ${MiniGUI_FIND_VERSION} ") is higher than found version (" ${MINIGUI_VERSION} ")")
endif ()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(MiniGUI REQUIRED_VARS MINIGUI_INCLUDE_DIRS MINIGUI_LIBRARIES
                                        VERSION_VAR MINIGUI_VERSION)

mark_as_advanced(
    MINIGUI_INCLUDE_DIRS
    MINIGUI_LIBRARIES
)

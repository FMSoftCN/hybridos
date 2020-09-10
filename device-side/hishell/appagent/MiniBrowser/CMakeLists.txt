cmake_minimum_required(VERSION 3.10)
project(MiniBrowser)
aux_source_directory(. DIR_SRCS)
#add_executable(MiniBrowser ${DIR_SRCS})
include_directories(${MINIGUI_INCLUDE_DIRS})
#target_link_libraries(MiniBrowser ${MINIGUI_LIBRARIES} ${MGEFF_LIBRARIES})


set(MINIBROWSER_DIR "${PROJECT_SOURCE_DIR}")
#set(DERIVED_SOURCES_MINIBROWSER_DIR "${CMAKE_BINARY_DIR}/DerivedSources/MiniBrowser")

set(DERIVED_SOURCES_DIR "${PROJECT_SOURCE_DIR}/../../../Output/HBD/Build/hiwebkit-1.0/DerivedSources")
set(FORWARDING_HEADERS_DIR ${DERIVED_SOURCES_DIR}/ForwardingHeaders)
set(DERIVED_SOURCES_WEBKIT2HBD_DIR ${DERIVED_SOURCES_DIR}/webkit2hbd)
set(FORWARDING_HEADERS_WEBKIT2HBD_DIR ${FORWARDING_HEADERS_DIR}/webkit2hbd)
set(CMAKE_SOURCE_DIR "${PROJECT_SOURCE_DIR}/../../../Source/hiwebkit-1.0/Source")

#file(MAKE_DIRECTORY ${DERIVED_SOURCES_MINIBROWSER_DIR})

set(MiniBrowser_SOURCES
#    ${DERIVED_SOURCES_MINIBROWSER_DIR}/BrowserMarshal.c
    ${MINIBROWSER_DIR}/Common.cpp
    ${MINIBROWSER_DIR}/BrowserWindow.cpp
    ${MINIBROWSER_DIR}/WebKitBrowserWindow.cpp
    ${MINIBROWSER_DIR}/main.cpp
)

set(MiniBrowser_INCLUDE_DIRECTORIES
#    ${DERIVED_SOURCES_MINIBROWSER_DIR}
    ${DERIVED_SOURCES_WEBKIT2HBD_DIR}
    ${FORWARDING_HEADERS_WEBKIT2HBD_DIR}
    ${FORWARDING_HEADERS_DIR}
    ${CMAKE_SOURCE_DIR}/Source
)

set(MiniBrowser_SYSTEM_INCLUDE_DIRECTORIES
    ${GLIB_INCLUDE_DIRS}
    ${LIBSOUP_INCLUDE_DIRS}
#    ${GSTREAMER_INCLUDE_DIRS}
    ${MINIGUI_INCLUDE_DIRS}
)

set(MiniBrowser_LIBRARIES
    ${MINIGUI_LIBRARIES}
#    ${JavaScriptCore_LIBRARY_NAME}
    javascriptcorehbd-1.0
    WebKit
    ${GLIB_LIBRARIES}
    ${LIBSOUP_LIBRARIES}
#    ${GSTREAMER_LIBRARIES}
)

#add_custom_command(
#    OUTPUT ${DERIVED_SOURCES_MINIBROWSER_DIR}/BrowserMarshal.c
#           ${DERIVED_SOURCES_MINIBROWSER_DIR}/BrowserMarshal.h
#    MAIN_DEPENDENCY ${MINIBROWSER_DIR}/browser-marshal.list
#    COMMAND glib-genmarshal --prefix=browser_marshal ${MINIBROWSER_DIR}/browser-marshal.list --body > ${DERIVED_SOURCES_MINIBROWSER_DIR}/BrowserMarshal.c
#    COMMAND glib-genmarshal --prefix=browser_marshal ${MINIBROWSER_DIR}/browser-marshal.list --header > ${DERIVED_SOURCES_MINIBROWSER_DIR}/BrowserMarshal.h
#    VERBATIM)

#if (DEVELOPER_MODE)
#    add_definitions(-DWEBKIT_INJECTED_BUNDLE_PATH="${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
#endif ()

add_definitions(-DHBD_VERSION_MIN_REQUIRED=HBD_VERSION_1_0)

include_directories(${MiniBrowser_INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${MiniBrowser_SYSTEM_INCLUDE_DIRECTORIES})
add_executable(MiniBrowser ${MiniBrowser_SOURCES})
target_link_libraries(MiniBrowser ${MiniBrowser_LIBRARIES})

#WEBKIT_ADD_TARGET_CXX_FLAGS(MiniBrowser -Wno-unused-parameter)
target_compile_options(MiniBrowser PRIVATE -Wno-unused-parameter})
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-as-needed")

#install(TARGETS MiniBrowser DESTINATION "${LIBEXEC_INSTALL_DIR}")

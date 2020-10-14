set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

cmake_minimum_required(VERSION 3.10)
project(appagent)
aux_source_directory(. DIR_SRCS)
#add_executable(appagent ${DIR_SRCS})
include_directories(${MINIGUI_INCLUDE_DIRS})

set(MINIBROWSER_DIR "${PROJECT_SOURCE_DIR}")

if(NOT DEFINED ENV{WEBKIT_OUTPUTDIR})
    set(WEBKIT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/../../Source/hiwebkit/Source")
    set(DERIVED_SOURCES_DIR "${PROJECT_SOURCE_DIR}/../../Output/HBD/Build/hiwebkit/DerivedSources")
else()
    set(WEBKIT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/../../../../hiwebkit/Source")
    set(DERIVED_SOURCES_DIR "$ENV{WEBKIT_OUTPUTDIR}/Build/hiwebkit/DerivedSources")
endif()

MESSAGE( STATUS "WEBKIT_SOURCE_DIR=${WEBKIT_SOURCE_DIR}")
MESSAGE( STATUS "DERIVED_SOURCES_DIR=${DERIVED_SOURCES_DIR}")


set(appagent_SOURCES
    ${MINIBROWSER_DIR}/Common.cpp
    ${MINIBROWSER_DIR}/BrowserWindow.cpp
    ${MINIBROWSER_DIR}/WebKitBrowserWindow.cpp
    ${MINIBROWSER_DIR}/main.cpp
)

set(appagent_INCLUDE_DIRECTORIES
    ${MINIBROWSER_DIR}
    ${DERIVED_SOURCES_DIR}/WebKit
    ${DERIVED_SOURCES_DIR}/WebKit/include
    ${DERIVED_SOURCES_DIR}/webkit2hbd
    ${DERIVED_SOURCES_DIR}/webkit2hbd/webkit2
    ${DERIVED_SOURCES_DIR}/ForwardingHeaders
    ${DERIVED_SOURCES_DIR}/ForwardingHeaders/webkit2hbd
    ${DERIVED_SOURCES_DIR}/ForwardingHeaders/webkit2hbd-1.0
    ${DERIVED_SOURCES_DIR}/ForwardingHeaders/webkit2hbd-webextension
    ${DERIVED_SOURCES_DIR}/ForwardingHeaders/JavaScriptCore
    ${DERIVED_SOURCES_DIR}/ForwardingHeaders/JavaScriptCore/glib
    ${DERIVED_SOURCES_DIR}/JavaScriptCore/javascriptcorehbd
    ${WEBKIT_SOURCE_DIR}
    ${WEBKIT_SOURCE_DIR}/WebKit
    ${WEBKIT_SOURCE_DIR}/WebKit/GPUProcess
    ${WEBKIT_SOURCE_DIR}/WebKit/GPUProcess/graphics
    ${WEBKIT_SOURCE_DIR}/WebKit/GPUProcess/media
    ${WEBKIT_SOURCE_DIR}/WebKit/GPUProcess/webrtc
    ${WEBKIT_SOURCE_DIR}/WebKit/NetworkProcess
    ${WEBKIT_SOURCE_DIR}/WebKit/NetworkProcess/Classifier
    ${WEBKIT_SOURCE_DIR}/WebKit/NetworkProcess/Cookies
    ${WEBKIT_SOURCE_DIR}/WebKit/NetworkProcess/CustomProtocols
    ${WEBKIT_SOURCE_DIR}/WebKit/NetworkProcess/Downloads
    ${WEBKIT_SOURCE_DIR}/WebKit/NetworkProcess/FileAPI
    ${WEBKIT_SOURCE_DIR}/WebKit/NetworkProcess/IndexedDB
    ${WEBKIT_SOURCE_DIR}/WebKit/NetworkProcess/ServiceWorker
    ${WEBKIT_SOURCE_DIR}/WebKit/NetworkProcess/WebStorage
    ${WEBKIT_SOURCE_DIR}/WebKit/NetworkProcess/cache
    ${WEBKIT_SOURCE_DIR}/WebKit/NetworkProcess/watchos
    ${WEBKIT_SOURCE_DIR}/WebKit/NetworkProcess/webrtc
    ${WEBKIT_SOURCE_DIR}/WebKit/Platform
    ${WEBKIT_SOURCE_DIR}/WebKit/Platform/IPC
    ${WEBKIT_SOURCE_DIR}/WebKit/PluginProcess
    ${WEBKIT_SOURCE_DIR}/WebKit/Shared
    ${WEBKIT_SOURCE_DIR}/WebKit/Shared/API
    ${WEBKIT_SOURCE_DIR}/WebKit/Shared/API/c
    ${WEBKIT_SOURCE_DIR}/WebKit/Shared/ApplePay
    ${WEBKIT_SOURCE_DIR}/WebKit/Shared/Authentication
    ${WEBKIT_SOURCE_DIR}/WebKit/Shared/CoreIPCSupport
    ${WEBKIT_SOURCE_DIR}/WebKit/Shared/Databases
    ${WEBKIT_SOURCE_DIR}/WebKit/Shared/Databases/IndexedDB
    ${WEBKIT_SOURCE_DIR}/WebKit/Shared/FileAPI
    ${WEBKIT_SOURCE_DIR}/WebKit/Shared/Gamepad
    ${WEBKIT_SOURCE_DIR}/WebKit/Shared/Plugins
    ${WEBKIT_SOURCE_DIR}/WebKit/Shared/Plugins/Netscape
    ${WEBKIT_SOURCE_DIR}/WebKit/Shared/Plugins/Netscape/x11
    ${WEBKIT_SOURCE_DIR}/WebKit/Shared/RemoteLayerTree
    ${WEBKIT_SOURCE_DIR}/WebKit/Shared/WebsiteData
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/API
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/API/C
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/API/cpp
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/Authentication
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/Automation
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/Downloads
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/Gamepad
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/GPU
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/Inspector
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/Inspector/Agents
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/Launcher
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/Network
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/Network/CustomProtocols
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/Notifications
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/Plugins
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/RemoteLayerTree
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/UserContent
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/WebAuthentication
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/WebAuthentication/Mock
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/WebsiteData
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/ApplePay
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/ApplicationCache
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/Automation
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/Cache
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/Databases
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/Databases/IndexedDB
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/FileAPI
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/FullScreen
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/Gamepad
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/Geolocation
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/GPU
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/GPU/graphics
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/GPU/media
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/GPU/webrtc
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/IconDatabase
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/InjectedBundle
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/InjectedBundle/API
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/InjectedBundle/API/c
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/InjectedBundle/DOM
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/Inspector
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/Launching
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/MediaCache
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/MediaStream
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/Network
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/Network/webrtc
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/Notifications
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/OriginData
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/Plugins
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/Plugins/Netscape
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/ResourceCache
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/Storage
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/UserContent
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/WebCoreSupport
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/WebPage
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/WebPage/RemoteLayerTree
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/WebStorage
    ${WEBKIT_SOURCE_DIR}/WebKit/NetworkProcess/glib
    ${WEBKIT_SOURCE_DIR}/WebKit/NetworkProcess/hbd
    ${WEBKIT_SOURCE_DIR}/WebKit/NetworkProcess/soup
    ${WEBKIT_SOURCE_DIR}/WebKit/NetworkProcess/unix
    ${WEBKIT_SOURCE_DIR}/WebKit/Platform/IPC/glib
    ${WEBKIT_SOURCE_DIR}/WebKit/Platform/IPC/unix
    ${WEBKIT_SOURCE_DIR}/WebKit/Platform/classifier
    ${WEBKIT_SOURCE_DIR}/WebKit/Platform/generic
    ${WEBKIT_SOURCE_DIR}/WebKit/Shared/API/c/hbd
    ${WEBKIT_SOURCE_DIR}/WebKit/Shared/API/glib
    ${WEBKIT_SOURCE_DIR}/WebKit/Shared/CoordinatedGraphics
    ${WEBKIT_SOURCE_DIR}/WebKit/Shared/CoordinatedGraphics/threadedcompositor
    ${WEBKIT_SOURCE_DIR}/WebKit/Shared/Plugins/unix
    ${WEBKIT_SOURCE_DIR}/WebKit/Shared/glib
    ${WEBKIT_SOURCE_DIR}/WebKit/Shared/hbd
    ${WEBKIT_SOURCE_DIR}/WebKit/Shared/linux
    ${WEBKIT_SOURCE_DIR}/WebKit/Shared/soup
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/API/C/cairo
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/API/C/hbd
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/API/glib
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/API/hbd
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/CoordinatedGraphics
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/Inspector/glib
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/Inspector/hbd
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/Plugins/hbd
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/geoclue
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/glib
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/gstreamer
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/hbd
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/linux
    ${WEBKIT_SOURCE_DIR}/WebKit/UIProcess/soup
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/InjectedBundle/API/glib
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/InjectedBundle/API/glib/DOM
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/InjectedBundle/API/hbd
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/InjectedBundle/API/hbd/DOM
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/Inspector/hbd
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/Plugins/Netscape/unix
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/Plugins/Netscape/x11
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/hbd
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/soup
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/WebCoreSupport/hbd
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/WebCoreSupport/soup
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/WebPage/CoordinatedGraphics
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/WebPage/atk
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/WebPage/hbd
    ${WEBKIT_SOURCE_DIR}/WebKit/WebProcess/WebPage/libwpe
    ${WEBKIT_SOURCE_DIR}/WebCore/platform/graphics/libwpe
)

set(appagent_SYSTEM_INCLUDE_DIRECTORIES
    ${GLIB_INCLUDE_DIRS}
    ${LIBSOUP_INCLUDE_DIRS}
#    ${GSTREAMER_INCLUDE_DIRS}
    ${Cairo_INCLUDE_DIR}
    ${MINIGUI_INCLUDE_DIRS}
    ${HIWEBKIT_INCLUDE_DIRS}
)

set(appagent_LIBRARIES
    ${MINIGUI_LIBRARIES}
    ${HIWEBKIT_LIBRARIES}
#    ${JavaScriptCore_LIBRARY_NAME}
    javascriptcorehbd-1.0
    webkit2hbd-1.0
    gobject-2.0
    icui18n
    icuuc
    icudata
    ${GLIB_LIBRARIES}
    ${LIBSOUP_LIBRARIES}
#    ${GSTREAMER_LIBRARIES}
)


add_definitions(-DHBD_VERSION_MIN_REQUIRED=HBD_VERSION_1_0)
add_definitions(-DBUILDING_HBD__=1 -DBUILDING_WITH_CMAKE=1)

include_directories(${appagent_INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${appagent_SYSTEM_INCLUDE_DIRECTORIES})
add_executable(appagent ${appagent_SOURCES})
target_link_libraries(appagent ${appagent_LIBRARIES})

#WEBKIT_ADD_TARGET_CXX_FLAGS(appagent -Wno-unused-parameter)
target_compile_options(appagent PRIVATE -Wno-unused-parameter})
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-as-needed")

install(TARGETS appagent DESTINATION ${CMAKE_INSTALL_PREFIX}/libexec/)

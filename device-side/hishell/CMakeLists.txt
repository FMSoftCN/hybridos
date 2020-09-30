cmake_minimum_required(VERSION 3.10)

set(ENV{PKG_CONFIG_PATH}  "${PROJECT_SOURCE_DIR}/../Output/HBD/Root/lib/pkgconfig")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")

# get library and include path for minigui and mgeff
find_package(MiniGUI 5.0.2 REQUIRED)
find_package(mGEff 1.5.0 REQUIRED)
find_package(GLIB 2.0 REQUIRED)
find_package(Cairo REQUIRED)
find_package(Librsvg 2.0 REQUIRED)
find_package(LibSoup 2.54.0 REQUIRED)

set(MINIGUI_LIBRARIES ${MiniGUI_LIBRARIES})
set(MINIGUI_INCLUDE_DIRS ${MiniGUI_INCLUDE_DIRS})

set(MGEFF_LIBRARIES ${mGEff_LIBRARIES})
set(MGEFF_INCLUDE_DIRS ${mGEff_INCLUDE_DIRS})

link_directories("${PROJECT_SOURCE_DIR}/../Output/HBD/Root/lib")

# copy the necessary resource files to /bin/res
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/res)

list(APPEND HISHELL_RES_FILES
    "${PROJECT_SOURCE_DIR}/dybkgnd/res/wallpaper-bottom.jpg"
    "${PROJECT_SOURCE_DIR}/dybkgnd/res/wallpaper-top.jpg"
#    "${PROJECT_SOURCE_DIR}/dybkgnd/res/home.svg"
#    "${PROJECT_SOURCE_DIR}/dybkgnd/res/arrow.svg"
    "${PROJECT_SOURCE_DIR}/sysmgr/res/arrow.svg"
    "${PROJECT_SOURCE_DIR}/sysmgr/res/home.svg"
    "${PROJECT_SOURCE_DIR}/sysmgr/res/toggle.svg"
    "${PROJECT_SOURCE_DIR}/sysmgr/res/setting.svg"
    "${PROJECT_SOURCE_DIR}/sysmgr/res/shutdown.svg"
    "${PROJECT_SOURCE_DIR}/sysmgr/res/about.svg"
)

file(COPY ${HISHELL_RES_FILES} DESTINATION ${PROJECT_SOURCE_DIR}/bin/res)
file(COPY "MiniGUI.cfg" DESTINATION ${PROJECT_SOURCE_DIR}/bin/)
file(COPY "hybridos.cfg" DESTINATION ${PROJECT_SOURCE_DIR}/bin/)
file(COPY "env.sh" DESTINATION ${PROJECT_SOURCE_DIR}/bin/)

# set output path
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# set sub-directory
add_subdirectory(mginit)
add_subdirectory(sysmgr)
add_subdirectory(dybkgnd)
add_subdirectory(appagent)

install(FILES "MiniGUI.cfg" DESTINATION ${CMAKE_INSTALL_PREFIX}/libexec/)
install(FILES "hybridos.cfg" DESTINATION ${CMAKE_INSTALL_PREFIX}/libexec/)

install(DIRECTORY samples DESTINATION ${CMAKE_INSTALL_PREFIX}/libexec/)

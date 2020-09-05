cmake_minimum_required(VERSION 3.10)

set(CMAKE_MODULE_PATH .)

find_package(MiniGUI 5.0.2 REQUIRED)
find_package(mGEff 1.5.0 REQUIRED)

set(MINIGUI_LIBRARIES ${MiniGUI_LIBRARIES})
set(MINIGUI_INCLUDE_DIRS ${MiniGUI_INCLUDE_DIRS})

set(MGEFF_LIBRARIES ${mGEff_LIBRARIES})
set(MGEFF_INCLUDE_DIRS ${mGEff_INCLUDE_DIRS})

file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/res)

list(APPEND HISHELL_RES_FILES
    "${PROJECT_SOURCE_DIR}/dybkgnd/res/wallpaper-bottom.jpg"
    "${PROJECT_SOURCE_DIR}/dybkgnd/res/wallpaper-top.jpg"
)

file(COPY ${HISHELL_RES_FILES} DESTINATION ${PROJECT_SOURCE_DIR}/bin/res)


SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(mginit)
add_subdirectory(sysmgr)
add_subdirectory(dybkgnd)
add_subdirectory(appagent)

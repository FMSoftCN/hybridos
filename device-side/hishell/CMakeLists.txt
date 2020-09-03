cmake_minimum_required(VERSION 3.10)

set(CMAKE_MODULE_PATH .)

find_package(MiniGUI 5.0.2 REQUIRED)

set(MINIGUI_LIBRARIES ${MiniGUI_LIBRARIES})
set(MINIGUI_INCLUDE_DIRS ${MiniGUI_INCLUDE_DIRS})

add_subdirectory(mginit)
add_subdirectory(sysmgr)
add_subdirectory(dybkgnd)

#add_subdirectory(appagent)

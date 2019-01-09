cmake_minimum_required(VERSION 2.6)

PROJECT(ubus C)
ADD_DEFINITIONS(-Os -Wall -Werror --std=gnu99 -g3 -Wmissing-declarations)

OPTION(BUILD_LUA "build Lua plugin" ON)
OPTION(BUILD_EXAMPLES "build examples" ON)

SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
SET(UBUS_UNIX_SOCKET "/var/run/ubus.sock")
SET(UBUS_MAX_MSGLEN 1048576)

ADD_DEFINITIONS( -DUBUS_UNIX_SOCKET="${UBUS_UNIX_SOCKET}")
ADD_DEFINITIONS( -DUBUS_MAX_MSGLEN=${UBUS_MAX_MSGLEN})

IF(BUILD_STATIC)
  FIND_LIBRARY(ubox_library NAMES ubox.a)
  FIND_LIBRARY(blob_library NAMES blobmsg_json.a)
ELSE(BUILD_STATIC)
  FIND_LIBRARY(ubox_library NAMES ubox)
  FIND_LIBRARY(blob_library NAMES blobmsg_json)
ENDIF(BUILD_STATIC)

FIND_PATH(ubox_include_dir libubox/usock.h)
INCLUDE_DIRECTORIES(${ubox_include_dir})

ADD_LIBRARY(ubus SHARED libubus.c libubus-io.c libubus-obj.c libubus-sub.c libubus-req.c libubus-acl.c)
TARGET_LINK_LIBRARIES(ubus ${ubox_library})

find_library(json NAMES json-c json)

ADD_EXECUTABLE(ubusd ubusd.c ubusd_id.c ubusd_obj.c ubusd_proto.c ubusd_event.c ubusd_acl.c ubusd_monitor.c)
TARGET_LINK_LIBRARIES(ubusd ${ubox_library} ${blob_library} ${json})

ADD_EXECUTABLE(cli cli.c)
SET_TARGET_PROPERTIES(cli PROPERTIES OUTPUT_NAME ubus)
TARGET_LINK_LIBRARIES(cli ubus ${ubox_library} ${blob_library} ${json})

ADD_SUBDIRECTORY(lua)
ADD_SUBDIRECTORY(examples)

INSTALL(TARGETS ubus cli
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin
)
INSTALL(TARGETS ubusd
	RUNTIME DESTINATION sbin
)

INSTALL(FILES ubusmsg.h ubus_common.h libubus.h DESTINATION include)

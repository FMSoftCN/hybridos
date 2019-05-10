SUBDIRS=common resource css hvml view graphics activity animation services drawable mgcl
lib_LTLIBRARIES=libhfcl@LIB_SUFFIX@.la

AM_CPPFLAGS=-I../include
MYLIBADD= \
    common/libhfcl_common.la \
    css/libhfcl_css.la \
    hvml/libhfcl_hvml.la \
    activity/libhfcl_activity.la \
    graphics/libhfcl_graphics.la \
    view/libhfcl_view.la \
    animation/libhfcl_animation.la \
    services/libhfcl_services.la \
    resource/libhfcl_resource.la \
    mgcl/libhfcl_mgcl.la \
    drawable/libhfcl_drawable.la

MYSOURCES=hfcl.cc
MYLDFLAGS=-release $(LT_RELEASE) -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

libhfcl@LIB_SUFFIX@_la_LIBADD=$(MYLIBADD) @DEP_LIBS@
libhfcl@LIB_SUFFIX@_la_DEPENDENCIES=$(MYLIBADD)
libhfcl@LIB_SUFFIX@_la_SOURCES=$(MYSOURCES)
libhfcl@LIB_SUFFIX@_la_LDFLAGS=$(MYLDFLAGS)

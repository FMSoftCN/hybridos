CLEANFILES=*.a
EXTRA_DIST=

SUBDIRS=activity view
lib_LTLIBRARIES=libhfcl@LIB_SUFFIX@.la

AM_CPPFLAGS=-I../include
COMMON_LIBADD=  \
	activity/libhfcl_activiy.la \
	view/libhfcl_view.la

MYSOURCES= 
MYLDFLAGS=-release $(LT_RELEASE) -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

libhfcl@LIB_SUFFIX@_la_LIBADD=$(COMMON_LIBADD) @DEP_LIBS@
libhfcl@LIB_SUFFIX@_la_DEPENDENCIES=$(COMMON_LIBADD)
libhfcl@LIB_SUFFIX@_la_SOURCES=$(MYSOURCES)
libhfcl@LIB_SUFFIX@_la_LDFLAGS=$(MYLDFLAGS) 


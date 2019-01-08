AUTOMAKE_OPTIONS=subdir-objects

SUBDIRS=
noinst_LTLIBRARIES=libhfcl_css.la

noinst_PROGRAMS=test-cssselector

AM_CPPFLAGS=-D__HFCL_LIB__ -I../../include
libhfcl_css_la_SOURCES = \
    css.cc \
    cssbox.cc \
    cssdeclared.cc \
    cssinitial.cc \
    csscomputed.cc \
    cssdeclaredgroup.cc \
    cssstackingcontext.cc \
    cssselector.cc

test_cssselector_CPPFLAGS = -D_UNIT_TEST -I../../include
test_cssselector_SOURCES = cssselector.cc css.cc
test_cssselector_LDADD = ../common/libhfcl_common.la

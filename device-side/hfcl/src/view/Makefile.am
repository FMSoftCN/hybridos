AUTOMAKE_OPTIONS=subdir-objects

SUBDIRS=
noinst_LTLIBRARIES=libhfcl_view.la

AM_CPPFLAGS=-D__HFCL_LIB__ -I../../include
libhfcl_view_la_SOURCES = \
    viewfactory.cc \
    view.cc \
    viewcontainer.cc \
    rootview.cc \
    panelview.cc \
    paragraphview.cc \
    htmlentitiestable.inc \
    hvmlparser.cc \
    textview.cc \
    atomictextview.cc \
    animatedimageview.cc \
    staticimageview.cc \
    scrollbar.cc \
    transition.cc

EXTRA_DIST= \
    make_html_entities_table.py

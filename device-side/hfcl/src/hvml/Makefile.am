AUTOMAKE_OPTIONS=subdir-objects

SUBDIRS=
noinst_LTLIBRARIES=libhfcl_hvml.la

AM_CPPFLAGS=-D__HFCL_LIB__ -I../../include
libhfcl_hvml_la_SOURCES = \
    dom.cc \
    dom-event.cc \
    htmlentitiestable.inc \
    hvmlparser.cc

EXTRA_DIST= \
    data \
    make_html_entities_table.py \
    make_hvml_tags_table.py

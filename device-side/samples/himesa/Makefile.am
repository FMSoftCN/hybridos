AUTOMAKE_OPTIONS = subdir-objects

SUBDIRS = eglut opengl opengles2

AM_CFLAGS = \
    $(EGL_CFLAGS) \
    $(NULL)

AM_LDFLAGS = \
    $(EGL_LIBS) \
    $(NULL)

noinst_PROGRAMS = eglinfo

eglinfo_SOURCES = eglinfo.c

EXTRA_DIST=

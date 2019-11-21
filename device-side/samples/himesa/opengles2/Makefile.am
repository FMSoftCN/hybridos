AM_CFLAGS = \
    $(GLESV2_CFLAGS) \
    $(EGL_CFLAGS) \
    -I$(top_srcdir)/himesa/eglut

AM_LDFLAGS = \
    $(GLESV2_LIBS) \
    $(EGL_LIBS) \
    -lm

noinst_PROGRAMS = \
    es2info \
    $(NULL)

if HAVE_GLESV2
noinst_PROGRAMS += \
    es2gears \
    es2tri \
    $(NULL)
endif

es2info_SOURCES = es2info.c

es2gears_SOURCES = es2gears.c
es2gears_LDADD = ../eglut/libeglut_x11.la

es2tri_SOURCES = es2tri.c
es2tri_LDADD = ../eglut/libeglut_x11.la

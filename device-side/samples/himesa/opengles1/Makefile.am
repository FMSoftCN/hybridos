AM_CFLAGS = \
    $(GLESV1_CFLAGS) \
    $(EGL_CFLAGS) \
    $(X11_CFLAGS) \
    -I$(top_srcdir)/src/egl/eglut

AM_LDFLAGS = \
    $(GLESV1_LIBS) \
    $(EGL_LIBS) \
    -lm

if HAVE_GLESV1
noinst_PROGRAMS = \
    es1info \
    es1drawtex \
    es1gears \
    es1torus \
    es1tri \
    $(NULL)
endif

es1info_SOURCES = es1info.c

es1drawtex_SOURCES = es1drawtex.c
es1gears_SOURCES = es1gears.c
es1torus_SOURCES = es1torus.c
es1tri_SOURCES = es1tri.c

es1drawtex_LDADD = ../eglut/libeglut.la
es1gears_LDADD = ../eglut/libeglut.la
es1torus_LDADD = ../eglut/libeglut.la
es1tri_LDADD = ../eglut/libeglut.la

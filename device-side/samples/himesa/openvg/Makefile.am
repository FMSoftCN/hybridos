AM_CFLAGS = \
    $(VG_CFLAGS) \
    $(EGL_CFLAGS) \
    -I$(top_srcdir)/src/egl/eglut

AM_LDFLAGS = \
    $(VG_LIBS) \
    $(EGL_LIBS) \
    -lm

if HAVE_VG
noinst_PROGRAMS = \
    lion \
    sp \
    text \
    $(NULL)
endif

lion_SOURCES = lion.c lion-render.c lion-render.h
sp_SOURCES = sp.c
text_SOURCES = text.c

lion_LDADD = ../eglut/libeglut.la
sp_LDADD = ../eglut/libeglut.la
text_CFLAGS = $(AM_CFLAGS) @FREETYPE2_CFLAGS@
text_LDADD = @FREETYPE2_LIBS@ ../eglut/libeglut.la


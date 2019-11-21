AM_CFLAGS = \
    $(GL_CFLAGS) \
    $(EGL_CFLAGS) \
    -I$(top_srcdir)/himesa/eglut

AM_LDFLAGS = \
    $(GL_LIBS) \
    $(EGL_LIBS) \
    -lm

noinst_PROGRAMS =
if HAVE_GL
noinst_PROGRAMS += \
    glgears \
    gltri
endif

glgears_SOURCES = glgears.c
glgears_LDADD = ../eglut/libeglut.la

gltri_SOURCES = gltri.c
gltri_LDADD = ../eglut/libeglut.la

dnl Process this file with autoconf to produce a configure script.
AC_PREREQ(2.60)
AC_INIT(hisamples,0.1.0)
AC_CONFIG_SRCDIR(hicairo/hicairo.c)

dnl ========================================================================
dnl Need for cross-compiling
AC_CANONICAL_SYSTEM

dnl ========================================================================
dnl Init automake
AM_INIT_AUTOMAKE

dnl ========================================================================
dnl Checks for programs.
AC_PROG_MAKE_SET
AC_PROG_CC
AC_PROG_CXX

# Initialize libtool
LT_PREREQ([2.2])
LT_INIT([disable-static])

dnl ========================================================================
dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

dnl ========================================================================
dnl Checks for header files.
AC_HEADER_STDC
AC_HEADER_TIME
AC_CHECK_HEADERS(sys/time.h unistd.h)

AC_ARG_ENABLE(develmode,
[  --enable-develmode       developer mode <default=no>],
devel_mode=$enableval)

if test "x$devel_mode" = "xyes"; then
    CPPFLAGS="$CPPFLAGS -DDEBUG -D_DEBUG"
    if test "$ac_cv_prog_gcc" = "yes"; then
        CPPFLAGS="$CPPFLAGS -Wall -Werror"
    fi
fi

if test "$ac_cv_prog_gcc" = "yes"; then
    CFLAGS="$CFLAGS -Wstrict-prototypes -pipe"
fi

dnl ========================================================================
dnl check for installation of MiniGUI
dnl ========================================================================
PKG_CHECK_MODULES([MINIGUI], [minigui >= 4.0.4])
PKG_CHECK_MODULES([EGL], [egl])

AC_ARG_ENABLE([cairo],
    [AS_HELP_STRING([--enable-cairo],
        [enable support for Cairo @<:@default=auto@:>@])],
    [cairo_enabled="$enableval"],
    [cairo_enabled=auto])
if test "x$cairo_enabled" != "xno"; then
    PKG_CHECK_MODULES([HICAIRO], [cairo-minigui >= 2.17.3],
        [cairo_enabled=yes], [cairo_enabled=no])
fi

AC_ARG_ENABLE([gl],
    [AS_HELP_STRING([--enable-gl],
        [enable OpenGL library @<:@default=auto@:>@])],
    [gl_enabled="$enableval"],
    [gl_enabled=auto])
if test "x$gl_enabled" != "xno"; then
    dnl Get the pkg-config definitions for libGL.  We include a fallback
    dnl path for GL implementation that don't provide a .pc file
    PKG_CHECK_MODULES(GL, [gl], [], [
        AC_CHECK_HEADER([GL/gl.h],
            [],
            [gl_enabled=no])
        AC_CHECK_LIB([GL],
            [glBegin],
            [GL_LIBS=-lGL],
            [gl_enabled=no])
        ])
fi
if test "x$gl_enabled" != "xno"; then
    gl_enabled=yes
fi

AC_ARG_ENABLE([gles1],
    [AS_HELP_STRING([--enable-gles1],
        [enable support for OpenGL ES 1.x API @<:@default=auto@:>@])],
    [glesv1_enabled="$enableval"],
    [glesv1_enabled=auto])
if test "x$glesv1_enabled" != "xno"; then
    PKG_CHECK_MODULES(GLESV1, [glesv1_cm], [glesv1_enabled=yes], [glesv1_enabled=no])
fi

AC_ARG_ENABLE([gles2],
    [AS_HELP_STRING([--enable-gles2],
        [enable support for OpenGL ES 2.x API @<:@default=auto@:>@])],
    [glesv2_enabled="$enableval"],
    [glesv2_enabled=auto])
if test "x$glesv2_enabled" != "xno"; then
    PKG_CHECK_MODULES(GLESV2, [glesv2], [glesv2_enabled=yes], [glesv2_enabled=no])
fi

AC_ARG_ENABLE([vg],
    [AS_HELP_STRING([--enable-vg],
        [enable support for OpenVG API @<:@default=auto@:>@])],
    [vg_enabled="$enableval"],
    [vg_enabled=auto])
if test "x$vg_enabled" != "xno"; then
    PKG_CHECK_MODULES(VG, [vg], [vg_enabled=yes], [vg_enabled=no])
fi

AC_ARG_ENABLE([libdrm],
    [AS_HELP_STRING([--enable-libdrm],
        [enable support for libdrm @<:@default=auto@:>@])],
    [drm_enabled="$enableval"],
    [drm_enabled=auto])
if test "x$drm_enabled" != "xno"; then
    PKG_CHECK_MODULES(DRM, [libdrm], [drm_enabled=yes], [drm_enabled=no])
fi

dnl FreeType2 is needed by OpenVG demos
AC_ARG_ENABLE([freetype2],
    [AS_HELP_STRING([--enable-freetype2],
        [enable support for FreeType2 @<:@default=auto@:>@])],
    [freetype2_enabled="$enableval"],
    [freetype2_enabled=auto])
if test "x$freetype2_enabled" != "xno"; then
    PKG_CHECK_MODULES(FREETYPE2, [freetype2], [freetype2_enabled=yes], [freetype2_enabled=no])
fi

LIBS="$LIBS $MINIGUI_LIBS"

AC_SUBST([EGL_CFLAGS])
AC_SUBST([EGL_LIBS])
AC_SUBST([HICAIRO_CFLAGS])
AC_SUBST([HICAIRO_LIBS])
AC_SUBST([GL_CFLAGS])
AC_SUBST([GL_LIBS])
AC_SUBST([GLESV1_CFLAGS])
AC_SUBST([GLESV1_LIBS])
AC_SUBST([GLESV2_CFLAGS])
AC_SUBST([GLESV2_LIBS])
AC_SUBST([GLESV2_CFLAGS])
AC_SUBST([GLESV2_LIBS])

dnl ========================================================================
dnl Write Output

AC_CHECK_DECLS(_MGRM_PROCESSES, minigui_runmode="procs", foo=bar, [#include <minigui/common.h>])
AM_CONDITIONAL(MGRM_PROCESSES, test "x$minigui_runmode" = "xprocs")

AM_CONDITIONAL(HAVE_CAIRO, test "x$cairo_enabled" = "xyes")
AM_CONDITIONAL(HAVE_GL, test "x$gl_enabled" = "xyes")
AM_CONDITIONAL(HAVE_GLESV1, test "x$glesv1_enabled" = "xyes")
AM_CONDITIONAL(HAVE_GLESV2, test "x$glesv2_enabled" = "xyes")
AM_CONDITIONAL(HAVE_VG, test "x$vg_enabled" = "xyes")
AM_CONDITIONAL(HAVE_FREETYPE2, test "x$freetype2_enabled" = "xyes")
AM_CONDITIONAL(HAVE_DRM, test "x$drm_enabled" = "xyes")

AC_OUTPUT(
    Makefile
    m4/Makefile
    hicairo/Makefile
    himesa/Makefile
    himesa/eglut/Makefile
    himesa/opengl/Makefile
    himesa/opengles1/Makefile
    himesa/opengles2/Makefile
    himesa/openvg/Makefile
)

AC_MSG_NOTICE([

# Main building configuration:

## Features:
  * Cairo:              ${cairo_enabled}
  * OpenGL:             ${gl_enabled}
  * OpenGL ES 1.x:      ${glesv1_enabled}
  * OpenGL ES 2.x:      ${glesv2_enabled}
  * OpenVG:             ${vg_enabled}
  * EGL:                ${EGL_LIBS}
  * FreeType2:          ${freetype2_enabled}
  * DRM:                ${drm_enabled}

## Building Info:
  * CC:                 ${CCVERSION}
  * CPP flags:          ${CPPFLAGS}
  * CC  flags:          ${CFLAGS}
  * CXX flags:          ${CXXFLAGS}
  * Dep. libraries:     ${DEP_LIBS}
])


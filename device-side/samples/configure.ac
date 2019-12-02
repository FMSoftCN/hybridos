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
PKG_CHECK_MODULES([MINIGUI], [minigui >= 4.0.3])
PKG_CHECK_MODULES([HICAIRO], [cairo-minigui >= 2.17.3])
dnl PKG_CHECK_MODULES([MGEFF], [mgeff >= 1.2.0])

LIBS="$LIBS $MINIGUI_LIBS $MGEFF_LIBS $HICAIRO_LIBS"

dnl ========================================================================
dnl Write Output

AC_CHECK_DECLS(_MGRM_PROCESSES, minigui_runmode="procs", foo=bar, [#include <minigui/common.h>])
AM_CONDITIONAL(MGRM_PROCESSES, test "x$minigui_runmode" = "xprocs")

AC_OUTPUT(
    Makefile
    m4/Makefile
    hicairo/Makefile
)

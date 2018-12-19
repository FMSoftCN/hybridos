dnl Process this file with autoconf to produce a configure script.
AC_PREREQ(2.60)
AC_INIT(hfcl-samples,0.1.0)
AC_CONFIG_SRCDIR(cbplus/cbplus.cc)

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
AC_PROG_LD
AC_PROG_RANLIB
AC_PROG_LIBTOOL

dnl ========================================================================
dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

dnl ========================================================================
dnl Checks for header files.
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_HEADER_TIME
AC_CHECK_HEADERS(sys/time.h unistd.h)

AC_ARG_ENABLE(develmode,
[  --enable-develmode       developer mode <default=no>],
devel_mode=$enableval)

AC_ARG_WITH(lang,
        [  --with-lang=[en/zhcn/zhtw]])

case "$with_lang" in
  zhcn)
    AC_DEFINE(_LANG_ZHCN, 1,
            [Use text strings for locale zh_CN])
    ;;
  zhtw)
    AC_DEFINE(_LANG_ZHTW, 1,
            [Use text strings for locale zh_TW])
    ;;
  *)
    AC_DEFINE(_LANG_EN, 1,
            [Use text strings for locale en_US])
    ;;
esac

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
PKG_CHECK_MODULES([MINIGUI], [minigui >= 3.2.1])
PKG_CHECK_MODULES([MGPLUS], [mgplus >= 1.4.0])
PKG_CHECK_MODULES([MGEFF], [mgeff >= 1.2.0])
PKG_CHECK_MODULES([HFCL], [hfcl >= 0.1.0])

LIBS="$LIBS $MINIGUI_LIBS $MGPLUS_LIBS $MGEFF_LIBS $HFCL_LIBS -lefence"

dnl ========================================================================
dnl Write Output

AC_CHECK_DECLS(_MGRM_PROCESSES, minigui_runmode="procs", foo=bar, [#include <minigui/common.h>])
AM_CONDITIONAL(MGRM_PROCESSES, test "x$minigui_runmode" = "xprocs")

AC_OUTPUT(
    Makefile
    m4/Makefile
    cbplus/Makefile
    cbplus/res/Makefile
)

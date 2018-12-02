#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ(2.60)
AC_INIT(libhfcl, 0.1.0)
AC_CONFIG_SRCDIR(src/activity/appinfo.cc)

dnl Set various version strings
#
# Making releases:
# Change the version, then:
#   _MICRO_VERSION += 1;
#   _INTERFACE_AGE += 1;
#   _BINARY_AGE += 1;
# if any functions have been added, set _INTERFACE_AGE to 0.
# if backwards compatibility has been broken,
# set _BINARY_AGE and _INTERFACE_AGE to 0.
#
HFCL_MAJOR_VERSION=0
HFCL_MINOR_VERSION=1
HFCL_MICRO_VERSION=0
HFCL_INTERFACE_AGE=0
HFCL_BINARY_AGE=0
HFCL_VERSION=$HFCL_MAJOR_VERSION.$HFCL_MINOR_VERSION.$HFCL_MICRO_VERSION

AC_SUBST(HFCL_MAJOR_VERSION)
AC_SUBST(HFCL_MINOR_VERSION)
AC_SUBST(HFCL_MICRO_VERSION)
AC_SUBST(HFCL_INTERFACE_AGE)
AC_SUBST(HFCL_BINARY_AGE)
AC_SUBST(HFCL_VERSION)

# libtool versioning
LT_RELEASE=$HFCL_MAJOR_VERSION.$HFCL_MINOR_VERSION
LT_CURRENT=`expr $HFCL_MICRO_VERSION - $HFCL_INTERFACE_AGE`
LT_REVISION=$HFCL_INTERFACE_AGE
LT_AGE=`expr $HFCL_BINARY_AGE - $HFCL_INTERFACE_AGE`

AC_SUBST(LT_RELEASE)
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_REVISION)
AC_SUBST(LT_AGE)

AC_DEFINE_UNQUOTED(HFCL_MAJOR_VERSION, $HFCL_MAJOR_VERSION,
            [Major version of HFCL])
AC_DEFINE_UNQUOTED(HFCL_MINOR_VERSION, $HFCL_MINOR_VERSION,
            [Minor version of HFCL])
AC_DEFINE_UNQUOTED(HFCL_MICRO_VERSION, $HFCL_MICRO_VERSION,
            [Micro version of HFCL])
AC_DEFINE_UNQUOTED(HFCL_INTERFACE_AGE, $HFCL_INTERFACE_AGE,
            [Interface age of HFCL])
AC_DEFINE_UNQUOTED(HFCL_BINARY_AGE, $HFCL_BINARY_AGE,
            [Binary age of HFCL])

dnl ========================================================================
dnl Need for cross-compiling
AC_CANONICAL_SYSTEM

dnl ========================================================================
dnl Init automake
AM_INIT_AUTOMAKE

dnl ========================================================================
# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_LD
AC_PROG_RANLIB
AC_PROG_LIBTOOL
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

dnl ========================================================================
dnl Put the name and version of the package here

# Checks for libraries.

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_PID_T
AC_STRUCT_TM

# Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_FORK
AC_PROG_GCC_TRADITIONAL
#AC_FUNC_MALLOC
AC_FUNC_STAT
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([memmove memset putenv strchr strdup strerror strstr])

AM_CONFIG_HEADER(hfclconfig.h)

devel_mode="no"
AC_ARG_ENABLE(develmode,
[  --enable-develmode       developer mode <default=no>],
devel_mode=$enableval)

detail_debug="no"
AC_ARG_ENABLE(detail_debug,
[  --enable-detaildebug     detailed debug information <default=no>],
detail_debug=$enableval)

libsuffix=""
AC_ARG_WITH(libsuffix,
  [  --with-libsuffix=ARG    configure the suffix of the library name.])

if test "x$with_libsuffix" = "x"; then
  LIB_SUFFIX=""
else
  LIB_SUFFIX="_$with_libsuffix"
fi

AC_SUBST(LIB_SUFFIX)
AM_CONDITIONAL(HFCL_NOSUFFIX, test "x$with_libsuffix" = "x")

DEP_LIBS=
if test "x$devel_mode" = "xyes"; then
    CPPFLAGS="$CPPFLAGS -D_DEBUG"
    if test "x$detail_debug" = "xyes"; then
        CPPFLAGS="$CPPFLAGS -DDEBUG"
    fi

    if test "$ac_cv_prog_gcc" = "yes"; then
        CPPFLAGS="$CPPFLAGS -Wall -Werror"
    fi
else
    CPPFLAGS="$CPPFLAGS -DNDEBUG"
fi

if test "$ac_cv_prog_gcc" = "yes"; then
    CFLAGS="$CFLAGS -Wstrict-prototypes -pipe"
fi

CPPFLAGS="$CPPFLAGS $FT2_CFLAGS"
DEP_LIBS="$DEP_LIBS $MINIGUI_LIBS $FT2_LIBS -lpthread -lstdc++"

dnl ========================================================================
dnl check the installation of MiniGUI
dnl ========================================================================
PKG_CHECK_MODULES([MINIGUI], [minigui >= 3.2.0])
PKG_CHECK_MODULES([MGPLUS], [mgplus >= 1.4.0])
PKG_CHECK_MODULES([MGEFF], [mgeff >= 1.2.0])
PKG_CHECK_MODULES([CHIPMUNK], [chipmunk >= 5.3.1])

APP_LIBS="$APP_LIBS -lhfcl$LIB_SUFFIX $DEP_LIBS"
AC_SUBST(APP_LIBS)
AC_SUBST(DEP_LIBS)

dnl AC_CONFIG_FILES
AC_OUTPUT([
    hfcl.pc
    Makefile
    m4/Makefile
    include/Makefile
    include/common/Makefile
    include/activity/Makefile
    include/view/Makefile
    src/Makefile
    src/activity/Makefile
    src/view/Makefile
])


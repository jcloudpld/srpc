#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])

# Note that this macro removes the newline output by the M4
# "esyscmd" built-in.  Unless you understand what you're doing,
# particularly with M4, do not modify this macro definition.
define([SRPC_VERSION],
  patsubst(esyscmd(grep SRPC_VERSION include/srpc/Version.h | sed 's/.*\" *\(.*\)\".*/\1/'), [
]))# remove newline ending every `esyscmd' answer

AC_INIT([srpc], SRPC_VERSION, [kcando@gmail.com], [libsrpc])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_HEADERS([config/config.h])
AC_CONFIG_SRCDIR([./README.txt])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_LANG([C++])

AC_PROG_MAKE_SET
AC_PROG_INSTALL

AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL 

# Initialize Automake with various options. We require at least v1.9, prevent
# pedantic complaints about package files, and enable various distribution
# targets.
AM_INIT_AUTOMAKE([1.9 dist-bzip2 dist-zip foreign subdir-objects]) 

# Checks for Boost library
AC_ARG_WITH(boost,
  AC_HELP_STRING([--with-boost=DIR],[location of boost distribution]),
[
  CPPFLAGS="$CPPFLAGS -I${withval}"
],[])

AC_CHECK_HEADER([boost/noncopyable.hpp],,
[
  echo "Can't find boost headers. Please check the location of the boost"
  echo "distribution and rerun configure using the --with-boost=DIR option."
  exit 1
],[])

# Checks for GoogleTest library
AC_ARG_WITH(gtest,
  AC_HELP_STRING([--with-gtest=DIR],[location of Google Test distribution]),
[
  CPPFLAGS="$CPPFLAGS -I${withval}"
],[])

AC_CHECK_HEADER([gtest/gtest.h],,
[
  echo "Can't find Google Test headers. Please check the location of"
  echo "the Google Test distribution and rerun configure"
  echo "using the --with-gtest=DIR option."
  exit 1
],[])

# OS specific

CXXFLAGS="$CXXFLAGS -D_REENTRANT"

WINDOWS=no
case $host in
  *-*-linux*)
    CXXFLAGS="$CXXFLAGS -pthread"
    LDFLAGS="$LDFLAGS -pthread"
    ;;
  *-*-solaris*)
    if test "$GXX" = yes; then
      CXXFLAGS="$CXXFLAGS"
    else
      # We'll assume Sun's CC.
      CXXFLAGS="$CXXFLAGS -mt"
    fi
    LIBS="$LIBS -lsocket -lnsl -lpthread"
    ;;
  *-*-mingw32*)
    CXXFLAGS="$CXXFLAGS -mthreads"
    LDFLAGS="$LDFLAGS -mthreads"
    LIBS="$LIBS -lws2_32 -lmswsock"
    WINDOWS=yes
    ;;
  *-pc-cygwin*)
    ##CXXFLAGS="$CXXFLAGS -D_WIN32_WINNT=0x0501 -D__USE_W32_SOCKETS"
    ##LIBS="$LIBS -lws2_32 -lmswsock"
    LDFLAGS="$LDFLAGS -Wl,--enable-auto-import"
    WINDOWS=yes
    ;;
  *-apple-darwin*)
    CXXFLAGS="$CXXFLAGS"
    LDFLAGS="$LDFLAGS"
    ;;
  *-*-freebsd*)
    CXXFLAGS="$CXXFLAGS -pthread"
    LDFLAGS="$LDFLAGS -pthread"
    ;;
esac

if test "$GXX" = yes; then
  CXXFLAGS="$CXXFLAGS -Wall -Winvalid-pch -ftemplate-depth-256"
fi

AM_CONDITIONAL(WINDOWS_TARGET,test x$WINDOWS != xno)

# check for doxygen
AC_ARG_ENABLE(doxygen,   [  --enable-doxygen        enable documentation generation with doxygen (auto)])
AC_ARG_ENABLE(dot,       [  --enable-dot            use 'dot' to generate graphs in doxygen (auto)])
AC_ARG_ENABLE(html-docs, [  --enable-html-docs      enable HTML generation with doxygen (yes)], [], [ enable_html_docs=yes])
if test "x$enable_doxygen" = xno; then
    enable_doc=no
else 
    AC_PATH_PROG(DOXYGEN, doxygen, , $PATH)
    if test "x$DOXYGEN" = x; then
        if test "x$enable_doxygen" = xyes; then
            AC_MSG_ERROR([could not find doxygen])
        fi
        enable_doc=no
    else
        enable_doc=yes
        AC_PATH_PROG(DOT, dot, , $PATH)
    fi
fi
AM_CONDITIONAL(DOC, test x$enable_doc = xyes)

if test x$DOT = x; then
    if test "x$enable_dot" = xyes; then
        AC_MSG_ERROR([could not find dot])
    fi
    enable_dot=no
else
    enable_dot=yes
fi
AC_SUBST(enable_dot)
AC_SUBST(enable_html_docs)

# Checks for library functions.
AC_CONFIG_FILES([Makefile
    doc/Makefile
    include/Makefile
    src/Makefile
    test/Makefile
    benchmark/Makefile
    benchmark/StreamingTest/Makefile
    examples/Makefile
    examples/NamingService/Makefile])

AC_OUTPUT

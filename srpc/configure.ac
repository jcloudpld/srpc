#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])

AC_INIT([libsrpc], [1.7.33], [kcando@gmail.com], [libsprc])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADERS([include/srpc/config/config.h])

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
    CXXFLAGS="$CXXFLAGS -D__USE_W32_SOCKETS -D_WIN32_WINNT=0x0501"
    LIBS="$LIBS -lws2_32 -lmswsock"
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
  CXXFLAGS="$CXXFLAGS -Wall -ftemplate-depth-256"
fi

AM_CONDITIONAL(WINDOWS_TARGET,test x$WINDOWS != xno)

# Checks for library functions.
AC_CONFIG_FILES([Makefile
                 include/Makefile
                 src/Makefile
                 test/Makefile
                 benchmark/Makefile
                 benchmark/StreamingTest/Makefile
                 examples/Makefile
                 examples/NamingService/Makefile])

AC_OUTPUT

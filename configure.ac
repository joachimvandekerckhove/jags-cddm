dnl Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])

AC_INIT([JAGS-CDDM],[0.1],[joachim@uci.edu],[JAGS-CDDM-MODULE])
JAGS_MAJOR=4
JAGS_MINOR=0
AC_SUBST(JAGS_MAJOR)
AC_SUBST(JAGS_MINOR)

AC_CANONICAL_HOST
AC_CONFIG_SRCDIR([src/distributions/DCDDM.cc])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([subdir-objects])

dnl libtool and ltdl configuration
LT_PREREQ(2.2.6)
LT_CONFIG_LTDL_DIR([libltdl])
LT_INIT([dlopen disable-static win32-dll])
LTDL_INIT([recursive])

dnl C++
AC_PROG_CXX

dnl Rmath stuff
AC_DEFINE(MATHLIB_STANDALONE, 1, [Define if you have standalone R math library])

case "${host_os}" in
  mingw*)
    win=true ;;
  *)
    win=false ;;
esac
AM_CONDITIONAL(WINDOWS, test x$win = xtrue)

jagsmoddir=${libdir}/JAGS/modules-${JAGS_MAJOR}
AC_SUBST(jagsmoddir)

AC_CONFIG_FILES([
  Makefile
  m4/Makefile
  libltdl/Makefile
  src/Makefile
  src/distributions/Makefile	
])
AC_OUTPUT

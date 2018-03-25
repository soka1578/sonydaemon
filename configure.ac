#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([sonydaemon], [0.6.3])
AC_CONFIG_SRCDIR([display.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign -Wall -Wno-portability])

AC_ARG_ENABLE([xbmc], [AS_HELP_STRING([--enable-xbmc], [Enable Xbmc support @<:@check@:>@])], [:], [enable_xbmc=check])

AC_ARG_ENABLE([ps3], [AS_HELP_STRING([--enable-ps3], [Enable playstation 3 support @<:@check@:>@])], [:], [enable_ps3=check])

AC_ARG_ENABLE([ps4], [AS_HELP_STRING([--enable-ps4], [Enable playstation 4 support @<:@check@:>@])], [:], [enable_ps4=check])

AC_ARG_ENABLE([debug], [AS_HELP_STRING([--enable-debug], [Enable debug output @<:@check@:>@])], [:], [enable_debug=check])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_CHECK_PROG(WAKEONLAN, wakeonlan, yes, no)
if test "$WAKEONLAN" != "yes" ; then
        AC_MSG_ERROR([wakeonlan not found!])
fi

# Checks for libraries.
AC_CHECK_LIB([curl], [curl_easy_init], [], [AC_MSG_ERROR([curl library required])])
#AC_CHECK_LIB([jansson], [json_loads], [], [AC_MSG_ERROR([json library required])])
AC_CHECK_LIB([X11], [XOpenDisplay], [], [AC_MSG_ERROR([X11 library required])])
AC_CHECK_LIB([Xext], [DPMSQueryExtension], [], [AC_MSG_ERROR([Xext library required])])

# Checks for header files.
AC_PATH_X
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_PID_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([getcwd])

AC_CONFIG_FILES([Makefile])

AS_IF([test "$enable_xbmc" = "yes"], [AC_CHECK_LIB([jansson], [json_loads], [], [AC_MSG_ERROR([json library required])])])

AS_IF([test "$enable_ps4" = "yes"], [AC_CHECK_HEADERS([sys/socket.h arpa/inet.h netdb.h], [], [AC_MSG_ERROR([header required])])])

AM_CONDITIONAL([ENABLE_XBMC], [test "$enable_xbmc" = "yes"])

AM_CONDITIONAL([ENABLE_DEBUG], [test "$enable_debug" = "yes"])

AM_CONDITIONAL([ENABLE_PS3], [test "$enable_ps3" = "yes"])

AM_CONDITIONAL([ENABLE_PS4], [test "$enable_ps4" = "yes"])

AC_OUTPUT

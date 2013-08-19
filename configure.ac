#
# io::align
#
# Copyright (C) 2013 Raphael 'kena' Poss
# This software is released under the terms of the MIT license.
# See the accompanying file 'LICENSE' for more information.
#
AC_INIT([align], [1.x], [kena@vodka-pomme.net])
AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([1.11 foreign subdir-objects silent-rules])
AM_SILENT_RULES([yes])

AC_PROG_CXX

if test "x$GXX" = xyes; then
  AM_CXXFLAGS="$AM_CXXFLAGS -W -Wall -Wextra -Weffc++ -Wundef -Wshadow -Wpointer-arith -Wmissing-declarations -Wwrite-strings"
fi

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

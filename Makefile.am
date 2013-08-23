bin_PROGRAMS = align
check_PROGRAMS = test mod11

align_SOURCES = src/align.cc

test_SOURCES = src/test.cc
mod11_SOURCES = src/mod11.cc

pkginclude_HEADERS = include/ioalign.h

AM_CPPFLAGS = -I$(srcdir)/include
ACLOCAL_AMFLAGS = -I build-aux

include doxygen.inc

bin_PROGRAMS = align
check_PROGRAMS = test mod11 bench

align_SOURCES = src/align.cc

test_SOURCES = src/test.cc
mod11_SOURCES = src/mod11.cc
bench_SOURCES = src/bench.cc

pkginclude_HEADERS = include/ioalign.h

dist_doc_DATA = README.rst

AM_CPPFLAGS = -I$(srcdir)/include
ACLOCAL_AMFLAGS = -I build-aux

include doxygen.inc

bin_PROGRAMS = align
check_PROGRAMS = test mod11 bench

align_SOURCES = src/align.cc

test_SOURCES = src/test.cc
mod11_SOURCES = src/mod11.cc
bench_SOURCES = src/bench.cc

pkginclude_HEADERS = include/ioalign.h

dist_doc_DATA = README.rst

dist_man_MANS = align.1

align.1: $(pkginclude_HEADERS) $(align_SOURCES) configure
	$(HELP2MAN) --output=align.1 -N -h -h -v -V ./align$(EXEEXT)

AM_CPPFLAGS = -I$(srcdir)/include
ACLOCAL_AMFLAGS = -I build-aux

include doxygen.inc

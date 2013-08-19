=======
 Align
=======

Automatic column alignment for streamed tables
----------------------------------------------

**Align** is both a **C++ library** and a **command-line utility**.

- The C++ library **io::align** provides a simple interface to align
  columns in tabular output performed through a standard C++ stream (std::ostream).
- The utility **align** reads tabular data, aligns columns, and writes the aligned table.

Both tools perform alignment while *streaming*: alignment occurs
line-by-line without buffering the entire table. The tool **align** is based on the library **io::align**.

How to use io::align
====================

1. copy ``ioalign.h`` in your project,
2. include ``ioalign.h`` in your C++ code,
3. use ``io::table`` on your C++ output streams.

For example::

   (fixme)

How to use align
================

1. run ``configure``
2. run ``make``
3. run ``make install``
4. use ``align`` from the command line.

For example::

   (fixme)

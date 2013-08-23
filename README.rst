=======
 Align
=======

Automatic column alignment for streamed tables
----------------------------------------------

**Align** is both a **C++ library** and a **command-line utility**.

- The C++ library **io::align** provides a simple interface to align
  columns in tabular output performed through a standard C++ stream
  (std::ostream).
- The utility **align** reads tabular data, aligns columns, and writes
  the aligned table.

Both tools perform alignment while *streaming*: alignment occurs
line-by-line without buffering the entire table.

The tool **align** is based on the library **io::align**.

How to use io::align
====================

1. copy ``ioalign.h`` in your project,
2. include ``ioalign.h`` in your C++ code,
3. use ``io::align`` on your C++ output streams.

For example::

    #include "ioalign.h"
    #include <fstream>
    int main()
    {
        std::ofstream os("/dev/stdout");

        os << "Multiplication table modulo 11:" << std::endl;

        // Create the align data structure.
        io::align a;
        // Create the output proxy.
        io::align_proxy o = a.attach(os);

        // Set the column widths.
        for (int i = 1; i <= 10; ++i)
            o << io::head("", 2);
        o << io::endr;

        // Produce a horizontal rule.
        o << io::hline;

        // Fill the table.
        for (int i = 1; i <= 10; ++i)
            for (int j = 1; j <= 10; ++j)
                o << (i * j % 11) << io::next;

        // Produce another horizontal rule.
        o << io::hline;

        return 0;
    }

How to use align
================

1. run ``configure``
2. run ``make``
3. run ``make install``
4. use ``align`` from the command line.

For example::

   (fixme)

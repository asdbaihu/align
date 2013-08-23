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

The tool **align** uses the library **io::align**.

.. contents::

When to use **align**
=====================

- To quickly reformat tabular data on the command line during
  programming tasks or system administration.

- To format tabular data in C++ programs when the number of
  rows is not known in advance.

- To approximate aligned output for large tables when the amount of
  memory is limited, eg in embedded environments.

How to use **align**
====================

1. run ``configure``
2. run ``make``
3. run ``make install``
4. use ``align`` from the command line.

Example uses::

   align -t: /etc/passwd

   align -t: -T "name:pw:uid:gid:geos:home:shell" -p </etc/passwd

   env | align -t=

See the help text (``align -h``) or `Command description`_ below for more details.

Example output
==============

Suppose the file ``/etc/passwd`` contains the following::

    ##
    # User Database
    #
    # [...]
    ##
    nobody:*:-2:-2:Unprivileged User:/var/empty:/usr/bin/false
    root:*:0:0:System Administrator:/var/root:/bin/sh
    daemon:*:1:1:System Services:/var/root:/usr/bin/false
    _uucp:*:4:4:Unix to Unix Copy Protocol:/var/spool/uucp:/usr/sbin/uucico
    _lp:*:26:26:Printing Services:/var/spool/cups:/usr/bin/false
    _postfix:*:27:27:Postfix Mail Server:/var/spool/postfix:/usr/bin/false
    _mcxalr:*:54:54:MCX AppLaunch:/var/empty:/usr/bin/false
    _pcastagent:*:55:55:Podcast Producer Agent:/var/pcast/agent:/usr/bin/false
    _pcastserver:*:56:56:Podcast Producer Server:/var/pcast/server:/usr/bin/false
    _serialnumberd:*:58:58:Serial Number Daemon:/var/empty:/usr/bin/false
    _devdocs:*:59:59:Developer Documentation:/var/empty:/usr/bin/false
    _sandbox:*:60:60:Seatbelt:/var/empty:/usr/bin/false
    _mdnsresponder:*:65:65:mDNSResponder:/var/empty:/usr/bin/false
    _ard:*:67:67:Apple Remote Desktop:/var/empty:/usr/bin/false
    _www:*:70:70:World Wide Web Server:/Library/WebServer:/usr/bin/false

We can use **align** to tabulate this data as follows::

  align -t: -T "name:pw:uid:gid:geos:home:shell" -up -n 10 /etc/passwd

the above command then produces the following output::

  name pw uid gid geos home shell
  ---- -- --- --- ---- ---- -----
  nobody *  -2  -2  Unprivileged User /var/empty /usr/bin/false
  root   *  0   0   System Administrator /var/root  /bin/sh
  daemon *  1   1   System Services      /var/root  /usr/bin/false
  _uucp  *  4   4   Unix to Unix Copy Protocol /var/spool/uucp /usr/sbin/uucico
  _lp    *  26  26  Printing Services          /var/spool/cups /usr/bin/false
  _postfix *  27  27  Postfix Mail Server        /var/spool/postfix /usr/bin/false

  name     pw uid gid geos                       home               shell
  -------- -- --- --- -------------------------- ------------------ ----------------
  _mcxalr  *  54  54  MCX AppLaunch              /var/empty         /usr/bin/false
  _pcastagent *  55  55  Podcast Producer Agent     /var/pcast/agent   /usr/bin/false
  _pcastserver *  56  56  Podcast Producer Server    /var/pcast/server  /usr/bin/false
  _serialnumberd *  58  58  Serial Number Daemon       /var/empty         /usr/bin/false
  _devdocs       *  59  59  Developer Documentation    /var/empty         /usr/bin/false
  _sandbox       *  60  60  Seatbelt                   /var/empty         /usr/bin/false

  name           pw uid gid geos                       home               shell
  -------------- -- --- --- -------------------------- ------------------ ----------------
  _mdnsresponder *  65  65  mDNSResponder              /var/empty         /usr/bin/false
  _ard           *  67  67  Apple Remote Desktop       /var/empty         /usr/bin/false
  _www           *  70  70  World Wide Web Server      /Library/WebServer /usr/bin/false

.. note:: As this example shows, **align** incrementally adjusts a
   column's width until it encounters a row with maximum width. This
   progression occurs because **align** does not pre-scan the data
   before emitting its output. This is a feature, not a bug.

In this example:

- the parameter ``-t:`` indicates that the colon (``:``) separates
  columns in the input.

- the parameter ``-T ...`` provides titles to each column.

- the parameters ``-p`` and ``-u`` indicate the output is to be
  paginated, and column titles repeated at the top of each page.

- the parameter ``-n 10`` indicates the page size.

Command description
===================

Each  row  in  the  input is split into columns at each input separator
(``-t``). Columns are then formatted by padding on the right using the fill
character  (``-f``).  Columnts are separated on the output by the separator
character (``-s``).

If option ``-i`` is specified, the following input row prefixes have a spe‐
cial  meaning:  '=' produce a horizontal rule; ';' indicates new column
titles; and '#' skips the row (comment).  These  special  prefixes  are
configurable (``-R`` / ``-H`` / ``-C``).

=========== ================================================================
Option      Description
=========== ================================================================
``-t`` C    Set the input tab character to C. (default: tab)
``-f`` C    Set the output fill character to C. (default: space)
``-s`` C    Set the output separator to C. (default: space)
``-r`` C    Set the output horizontal rule character to C. (default: -)
``-p``      Paginate the output and repeat titles on each page.
``-n`` N    Set the page height in lines. (default: terminal height or 25)
``-u``      Produce a horizontal ruler after each title row.
``-T`` STR  Define column titles from STR.
``-i``      Interpret special row prefixes in the input.
``-R`` C    Set the special row prefix for rules. (default: =)
``-C`` C    Set the special row prefix for comments. (default: #)
``-H`` C    Set the special row prefix for titles. (default: ;)
``-h``      Display this help.
=========== ================================================================

How to use the **io::align** C++ library
========================================

1. copy ``ioalign.h`` in your project,
2. include ``ioalign.h`` in your C++ code,
3. use ``io::align`` on your C++ output streams.

For example:

.. code:: c++

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

This program produces the following output::

    Multiplication table modulo 11:
    -- -- -- -- -- -- -- -- -- --
    1  2  3  4  5  6  7  8  9  10
    2  4  6  8  10 1  3  5  7  9
    3  6  9  1  4  7  10 2  5  8
    4  8  1  5  9  2  6  10 3  7
    5  10 4  9  3  8  2  7  1  6
    6  1  7  2  8  3  9  4  10 5
    7  3  10 6  2  9  5  1  8  4
    8  5  2  10 7  4  1  9  6  3
    9  7  5  3  1  10 8  6  4  2
    10 9  8  7  6  5  4  3  2  1
    -- -- -- -- -- -- -- -- -- --

An API documentation is provided; check the ``doc`` subdirectory after
building the package with ``make doxygen-doc``.

Performance
===========

Using the enclosed ``bench.cc`` on a Core 2 Duo @ 2.4GHz and
optimization level ``-O3``, the following was measured:

========================= ==================== ======================= ==================
C++ toolchain             Formatter            Performance (rows/sec)  Slowdown
========================= ==================== ======================= ==================
g++ 4.8 / libstdc++       none                 1149950                 N/A
g++ 4.8 / libstdc++       io::align            566160                  50% (2x slower)

clang++ 3.3 / libc++      none                 701035                  N/A
clang++ 3.3 / libc++      io::align            411945                  41% (1.7x slower)
========================= ==================== ======================= ==================

The memory usage should be related to the length of the longest row by
a constant factor, and does not grow with the number of rows.

Licensing
=========

**align** and **io::align** are free software. See the accompanying
file ``LICENSE`` for details.

// align -- column alignment for streams -*- C++ -*-
//
// Copyright (c) 2013 Raphael 'kena' Poss
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#include "ioalign.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>

using namespace std;

void usage(std::ostream& o, const char *pname)
{
    o << "Usage: " << pname << " [OPTION...] [INPUT] [OUTPUT]\n"
        "This program formats a table by aligning column data.\n"
        "\n"
        "Each row in the input is split into columns at each input\n"
        "separator (-t). Columns are then formatted by padding on the\n"
        "right using the fill character (-f). Columnts are separated\n"
        "on the output by the separator character (-s).\n"
        "\n"
        "If option -i is specified, the following input row\n"
        "prefixes have a special meaning: '=' produce a horizontal rule;\n"
        "';' indicates new column titles; and '#' skips the row (comment).\n"
        "These special prefixes are configurable (-R/-H/-C)."
        "\n"
        "Options:\n"
        " -t C    Set the input tab character to C. (default: tab)\n"
        " -f C    Set the output fill character to C. (default: space)\n"
        " -s C    Set the output separator to C. (default: space)\n"
        " -r C    Set the output horizontal rule character to C. (default: -)\n"
        " -p      Paginate the output and repeat titles on each page.\n"
        " -n N    Set the page height in lines. (default: terminal height or 25)\n"
        " -u      Produce a horizontal ruler after each title row.\n"
        " -T STR  Define column titles from STR.\n"
        " -i      Interpret special row prefixes in the input.\n"
        " -R C    Set the special row prefix for rules. (default: =)\n"
        " -C C    Set the special row prefix for comments. (default: #)\n"
        " -H C    Set the special row prefix for titles. (default: ;)\n"
        " -h      Display this help.\n"
        " -V      Display version information and exit.\n"
        "\n"
        "Report bugs on https://github.com/knz/align/issues.\n"
      << flush;
    exit(0);
}

void version(std::ostream& o)
{
    o << PACKAGE_NAME << " version " << PACKAGE_VERSION << endl;
    exit(0);
}

int main(int argc, char **argv)
{
    char f = ' '; // fill character
    char s = ' '; // column separator
    char r = '-'; // horizontal rule
    char t = '\t'; // tab character for input
    char R = '='; // input prefix for hline
    char C = '#'; // input prefix for comments
    char H = ';'; // input prefix for titles
    bool paginate = false; // whether to repeat column headers at intervals
    int max_lines_per_page = 25; // number of lines per interval
    bool special = false; // whether to interpret special prefixes
    char *headtext = NULL; // default column headers
    bool underline_heads = false; // whether to produce a rule underneath titles
    const char *iname = "/dev/stdin"; // input file
    const char *oname = "/dev/stdout"; // output file

    // Attempt to retrieve terminal size from /dev/tty
    {
        int fd = -1;
        struct winsize sz;

        if ( -1 != (fd = open("/dev/tty", O_RDONLY)) &&
             -1 != ioctl(fd, TIOCGWINSZ, &sz) &&
             sz.ws_row > 0)
            max_lines_per_page = sz.ws_row;

        if (fd != -1) close(fd);
    }

    // Parse command-line argument and override defaults.
    char ch;
    while ((ch = getopt(argc, argv, "hipuVf:s:r:n:T:t:")) != -1)
    {
        switch (ch) {
        case 'f': f = optarg[0]; break;
        case 's': s = optarg[0]; break;
        case 'r': r = optarg[0]; break;
        case 'p': paginate = true; break;
        case 'n': max_lines_per_page = atoi(optarg); break;
        case 'i': special = true; break;
        case 'T': headtext = optarg; break;
        case 'u': underline_heads = true; break;
        case 't': t = optarg[0]; break;
        case 'h': usage(cout, argv[0]); break;
        case 'H': H = optarg[0]; break;
        case 'C': C = optarg[0]; break;
        case 'R': R = optarg[0]; break;
        case 'V': version(cout); break;
        default: exit(1); break;
        }
    }

    argc -= optind;
    argv += optind;

    // Configure the input/output streams.
    if (argc > 0)
        iname = argv[0];
    if (argc > 1)
        oname = argv[1];

    ofstream dout(oname);
    ifstream din(iname);

    // Set up aligned output.
    io::align table;
    io::align_proxy ap(table.attach(dout, f, s, r, t));

    int line_num = 1;

    // If titles are given from the command line, start with that.
    if (headtext)
    {
        ap.rawheads(headtext);
        ap << io::endr << io::heads;

        line_num += 1;

        if (underline_heads)
        {
            ap << io::hline;
            line_num += 1;
        }
    }

    // Then go through the input stream.
    while (din.good() && dout.good())
    {
        string line;
        getline(din, line);

        if (special && !line.empty() && line[0] == C)
            continue;

        bool page_boundary = paginate && line_num + 1 >= max_lines_per_page;
        bool head_prefix = special && !line.empty() && line[0] == H;

        if (page_boundary || head_prefix)
        {
            if (head_prefix)
            {
                line.push_back('\0');
                ap << io::rawheads(line) << io::endr;
            }

            if (line_num > 1)
                ap << ' ' << io::endr;

            ap << io::heads;
            line_num = 2;

            if (underline_heads)
            {
                ap << io::hline;
                line_num += 1;
            }

            if (head_prefix)
                continue;
        }

        if (special && !line.empty() && line[0] == R)
            ap << io::hline;
        else {
            line.push_back('\0');
            ap << io::raw(line) << io::endr;
        }
        line_num += 1;
    }
    return 0;
}

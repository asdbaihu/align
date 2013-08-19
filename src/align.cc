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

using namespace std;

int main(int argc, char **argv)
{
    io::align table;
    io::align_proxy s(table.attach(cout));

    while (cin.good())
    {
        string line;
        getline(cin, line);
        line.push_back('\0');

        if (!line.empty() && line[0] == ';') {
            table.reset_heads(&line[1]);
        }

        if (line.size() >= 2 && line[0] == '-' && line[1] == '-') {
            s << io::hline;
        }
        else {
            s << io::row(line) << endl;
        }
    }
    return 0;
}

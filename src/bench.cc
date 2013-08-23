#include "ioalign.h"
#include <cstring>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <unistd.h>
#include <cstdlib>
#include <csignal>

using namespace std;

volatile bool done = false;
void handler(int n)
{
    done = true;
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        cerr << "usage: " << argv[0] << " <seconds> [align | plain]" << endl;
        return 1;
    }

    int seconds = atoi(argv[1]);
    signal(SIGALRM, &handler);
    alarm(seconds);

    ostringstream os;
    int i = 0;

    if (!strcmp(argv[2], "align"))
    {
        io::align a;
        io::align_proxy ap = a.attach(os);
        for (int j = 1; j <= 10; ++j)
            ap << io::head("", 2);
        ap << io::endr;
        while (!done)
            for (int j = 1; j <= 10; ++j)
            {
                for (int k = 1; k <= 10; ++k)
                {
                    ap << (j * k % 11) << io::next;
                }
                ++i;
            }
    }
    else if (!strcmp(argv[2], "plain"))
    {
        for (int j = 1; j <= 10; ++j)
            os << "  ";
        os << endl;
        ++i;
        while (!done)
            for (int j = 1; j <= 10; ++j)
            {
                for (int k = 1; k <= 10; ++k)
                    os << (j * k % 11) << ' ';
                os << endl;
                ++i;
            }
    }
    else
    {
        cerr << "unrecognized argument: " << argv[2] << endl;
        return 1;
    }

    cout << i << " rows formatted (" << os.str().size() << " bytes) in " << seconds
         << "s = " << fixed << setprecision(1) << (float)i/(float)seconds << " rows/s" << endl;

    return 0;
}

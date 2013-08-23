#include "ioalign.h"
#include <fstream>

int main()
{
    std::ofstream os("/dev/stdout");

    os << "Multiplication table modulo 11:" << std::endl;

    io::align a;
    io::align_proxy o = a.attach(os);

    for (int i = 1; i <= 10; ++i)
        o << io::head("", 2);
    o << io::endr;

    o << io::hline;
    for (int i = 1; i <= 10; ++i)
        for (int j = 1; j <= 10; ++j)
            o << (i * j % 11) << io::next;
    o << io::hline;

    return 0;
}

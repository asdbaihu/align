#include "ioalign.h"
#include <fstream>
#include <iomanip>

using namespace std;

int main()
{
    ofstream dout("/dev/stdout");
    io::align table;
    io::align_proxy s(table.attach(dout));

    s << "some" << io::tab << "data" << io::endr;
    s << "some" << io::tab << "longer" << io::tab << "third" << io::endr;
    s << io::hline;
    s << "o" << io::tab << "hai" << io::endr;
    s << io::endr
      << io::endr;
    s << io::head("some long column") << io::head("short") << io::endr;
    s << io::hline;

    s.setfill('*');
    s.setsep('|');
    s.setrule('=');
    s << "hi" << io::tab << "again" << io::tab << "stuff" << io::endr;

    s << io::heads
      << "some" << io::heads;

    s << io::endr
      << ' '
      << io::endr
      << "oha" << io::endr;

    s << io::hline
      << "oha2" << io::endr
      << "foo" << io::hline
      << "oha3" << io::endr;

    s << io::raw("hello\tagain\n")
      << io::reset
      << io::raw("hello\tagain\tthird") << io::endr
      << io::hline
      << 123 << io::tab << hex << 456 << io::endr;
    s << flush;
    return 0;
}

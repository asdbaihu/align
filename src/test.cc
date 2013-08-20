#include "ioalign.h"
#include <iostream>

using namespace std;

int main()
{
    io::align table;
    io::align_proxy s(table.attach(cout));

    s << io::heads << "some long column" << io::tab << "short" << io::endr
      << "hello" << io::tab << "world" << io::endr;

    return 0;
}

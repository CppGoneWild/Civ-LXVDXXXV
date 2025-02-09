#include "civlog.h"
#include "TermStyle.h"

#include <iostream>


int main()
{
    using misc::TermStyle;

    CLOG << "Test";
    CLOG << (TermStyle::fg::Red) << "Test";
    CLOG << std::hex << std::showbase << 99;
    return (0);
}
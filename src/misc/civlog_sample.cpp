#include "civlog.h"
#include "TermStyle.h"

#include <iostream>

#define LOGFLAG true

int main()
{
    using misc::TermStyle;

    CLOG(LOGFLAG) << "Test";
    CLOG(LOGFLAG) << (TermStyle::fg::Red) << "Test";
    CLOG(LOGFLAG) << std::hex << std::showbase << 99;
    return (0);
}
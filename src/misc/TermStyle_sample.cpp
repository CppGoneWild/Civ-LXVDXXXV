#include "TermStyle.h"

#include <iostream>

int main()
{
    using misc::TermStyle;
    using misc::ScopeStyle;
    using misc::termStyleDef;

    {
        ScopeStyle sf;
        std::cout << (TermStyle::fg::Red) << "Test" << std::endl;
    }
    std::cout << "Test" << std::endl;

    return (0);
}
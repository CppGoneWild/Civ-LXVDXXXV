#pragma once

#include <iostream>



namespace misc
{



class ScopeFlags
{
public:
    ScopeFlags(ScopeFlags const &)             = delete;
    ScopeFlags & operator=(ScopeFlags const &) = delete;

    ScopeFlags(ScopeFlags &&);
    ScopeFlags & operator=(ScopeFlags &&);

    ScopeFlags(std::ostream & = std::cout);
    ~ScopeFlags();

    void cancel();

private:
    std::ios_base::fmtflags m_flags;
    std::ostream *          m_stream;
};



} // namespace misc

#include "ScopeFlags.h"



misc::ScopeFlags::ScopeFlags(ScopeFlags && other)
: m_flags(other.m_flags)
, m_stream(other.m_stream)
{
    other.m_stream = nullptr;
}

misc::ScopeFlags & misc::ScopeFlags::operator=(ScopeFlags && other)
{
    if (this != &other)
    {
        if (m_stream)
            m_stream->setf(m_flags);

        m_flags = other.m_flags;
        m_stream = other.m_stream;
        other.m_stream = nullptr;
    }
    return (*this);
}



misc::ScopeFlags::ScopeFlags(std::ostream & os)
: m_flags(os.flags())
, m_stream(&os)
{}

misc::ScopeFlags::~ScopeFlags()
{
    if (m_stream)
        m_stream->setf(m_flags);
}



void misc::ScopeFlags::cancel()
{
    m_stream = nullptr;
}


#include "civlog.h"

#include "TermStyle.h"
#include "ScopeFlags.h"

#include <cassert>




misc::civlog::Output::Output(std::ostream & os, bool color_mode)
: m_stream(&os)
, m_color_mode(color_mode)
, m_mutex()
#ifndef NDEBUG
, m_is_locked(false)
#endif
{}

void misc::civlog::Output::lock_and_log(Entry const & e)
{
    std::scoped_lock<std::mutex> lck(m_mutex);
#ifndef NDEBUG
    m_is_locked = true;
#endif
    log(e);
#ifndef NDEBUG
    m_is_locked = false;
#endif
}

void misc::civlog::Output::log(Entry const & e)
{
    assert(m_is_locked);
    ScopeFlags sf(*m_stream);

    *m_stream << e.m_src_loc.file_name() << ":" << std::dec << e.m_src_loc.line();
    *m_stream << " '" << e.m_src_loc.function_name() << "' ";
    *m_stream << e.m_ss.str();

    if (m_color_mode == color_mode_on)
        *m_stream << termStyleDef;

    *m_stream << std::endl;
}

void misc::civlog::Output::lock()
{
    assert(m_is_locked == false);
    m_mutex.lock();
#ifndef NDEBUG
    m_is_locked = true;
#endif
}

void misc::civlog::Output::unlock()
{
    assert(m_is_locked);
    m_mutex.unlock();
#ifndef NDEBUG
    m_is_locked = false;
#endif
}

bool misc::civlog::Output::color_mode() const
{
    return (m_color_mode);
}



misc::civlog::Output misc::civlog::cout(std::cout, Output::color_mode_on);
misc::civlog::Output misc::civlog::cerr(std::cerr, Output::color_mode_off);




misc::civlog::Entry::Entry(Entry && other)
: m_src_loc(other.m_src_loc)
, m_ss(std::move(other.m_ss))
, m_output(other.m_output)
{
    other.m_output = nullptr;
}

misc::civlog::Entry & misc::civlog::Entry::operator=(Entry && other)
{
    if (this != &other)
    {
        m_src_loc = other.m_src_loc;
        m_ss = std::move(other.m_ss);
        m_output = other.m_output;
        other.m_output = nullptr;        
    }
    return (*this);
}

misc::civlog::Entry::Entry(Output & os, std::source_location src)
: m_src_loc(src)
, m_ss()
, m_output(&os)
{}

misc::civlog::Entry::~Entry()
{
    if (m_output && m_ss.str().empty() == false)
    {
        m_output->lock_and_log(*this);
    }
}

misc::civlog::Entry & misc::civlog::Entry::operator<<(TermStyle const & value)
{
    if (m_output && m_output->color_mode() == Output::color_mode_on)
        m_ss << value;
    return (*this);
}

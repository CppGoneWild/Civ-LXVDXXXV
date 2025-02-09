#pragma once



#include <sstream>
#include <iostream>
#include <source_location>
#include <mutex>


#define CLOG          misc::civlog::Entry()
#define CLOGS(stream) misc::civlog::Entry(stream)


namespace misc
{

class TermStyle;

} // namespace misc






namespace misc
{
namespace civlog
{



class Entry;
class Output;



class Output
{
public:
    static constexpr bool color_mode_on = true;
    static constexpr bool color_mode_off = false;

    Output()                           = delete;
    Output(Output const &)             = delete;
    Output & operator=(Output const &) = delete;
    Output(Output &&)                  = delete;
    Output & operator=(Output &&)      = delete;

    ~Output() = default;

    Output(std::ostream &, bool color_mode);

    void lock_and_log(Entry const &);
    void log(Entry const &);

    void lock();
    void unlock();

    bool color_mode() const;

private:
    std::ostream * m_stream = &std::cout;
    bool           m_color_mode = color_mode_on;
    std::mutex     m_mutex;
#ifndef NDEBUG
    bool           m_is_locked = false;
#endif
};



extern Output cout;
extern Output cerr;



class Entry
{
public:
    Entry(Entry const &)             = delete;
    Entry & operator=(Entry const &) = delete;

    Entry(Entry &&);
    Entry & operator=(Entry &&);

    Entry(Output & = cout, std::source_location = std::source_location::current());
    ~Entry();

    Entry & operator<<(TermStyle const & value);
    
    template <class T>
    Entry & operator<<(T const &);

private:
    std::source_location m_src_loc;
    std::stringstream    m_ss;
    Output *             m_output;

    friend class Output;
};



} // namespace civlog
} // namespace misc



template <class T>
misc::civlog::Entry & misc::civlog::Entry::operator<<(T const & value)
{
    m_ss << value;
    return (*this);
}

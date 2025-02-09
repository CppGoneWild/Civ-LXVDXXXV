#include "civlog.h"

#include "A_GetOpt.h"

#include <cassert>


static constexpr bool logFlag = true;


misc::A_GetOpt::UnknowOpt::UnknowOpt(Opt const & found, int ac, char const * const * av)
: m_found(found)
, m_ac(ac)
, m_av(av)
, m_msg()
{
    m_msg = "UnknowOpt '";
    if (found.short_form)
        m_msg += found.short_form;
    else
        m_msg += found.long_form;
    m_msg += "'";
}

misc::A_GetOpt::Opt const & misc::A_GetOpt::UnknowOpt::found() const
{
    return (m_found);
}

int misc::A_GetOpt::UnknowOpt::ac() const
{
    return (m_ac);
}

char const * const * misc::A_GetOpt::UnknowOpt::av() const
{
    return (m_av);
}







int misc::A_GetOpt::ac() const
{
    return (m_ac);
}



char const * const * misc::A_GetOpt::av() const
{
    return (m_av);
}



char const * misc::A_GetOpt::current_av() const
{
    return (m_av[m_current_ac]);
}



int misc::A_GetOpt::current_ac() const
{
    return (m_current_ac);
}



void misc::A_GetOpt::parse(int ac, char const * const * av)
{
    m_ac = ac;
    m_av = av;
    m_current_ac = 1;

    while (m_current_ac < m_ac)
    {
        std::string short_form, long_form, long_form_param;
        bool param = is_param(short_form, long_form, long_form_param);

        CLOG(logFlag) << "Parsing '" << current_av() << "'";

        if (param == false)
        {
            #ifndef NDEBUG
                int m_ac_dbg_counter = m_current_ac;
            #endif
            CLOG(logFlag) << "Not a option '" << current_av() << "'";
            on_no_opt();
            assert(m_ac_dbg_counter < m_current_ac); // on_no_opt() must advance m_current_ac
        }
        else if (long_form.empty() == false)
        {
            parse_long_form(long_form, long_form_param);
        }
        else if (short_form.empty() == false)
        {
            parse_short_form(short_form);
        }
        else
            assert(false);
    }
}



misc::A_GetOpt::A_GetOpt(std::vector<Opt> const & opt)
: m_ac(0)
, m_current_ac(0)
, m_av(nullptr)
, m_opt_list(opt)
{}



misc::A_GetOpt::A_GetOpt(std::vector<Opt> && opt)
: m_ac(0)
, m_current_ac(0)
, m_av(nullptr)
, m_opt_list(opt)
{}



bool misc::A_GetOpt::is_param(std::string & short_form,
                              std::string & long_form,
                              std::string & long_form_param)
{
    return (is_param(m_current_ac, short_form, long_form, long_form_param));
}



bool misc::A_GetOpt::is_param(int ac,
                              std::string & short_form,
                              std::string & long_form,
                              std::string & long_form_param) const
{
    assert(ac < m_ac);
    assert(m_av);
    return (is_param(m_av[ac], short_form, long_form, long_form_param));
}



bool misc::A_GetOpt::is_param(char const * av,
                              std::string & short_form,
                              std::string & long_form,
                              std::string & long_form_param) const
{
    if (av[0] == '-')
    {
        if (av[1] == '-')
        {
            std::string tmp(av + 2);
            short_form.clear();
            std::size_t found = tmp.find_first_of('=');
            if (found == std::string::npos)
            {
                long_form = tmp;
                long_form_param.clear();
            }
            else
            {
                long_form = tmp.substr(0, found);
                long_form_param = tmp.substr(found + 1);
            }
        }
        else
        {
            short_form = (av+1);
            long_form.clear();
            long_form_param.clear();
        }
        return (true);
    }
    return (false);
}



void misc::A_GetOpt::current_ac(int v)
{
    assert(v >= 0);
    m_current_ac = v;
}



void misc::A_GetOpt::parse_long_form(std::string const & long_form, std::string const & long_form_param)
{
    CLOG(logFlag) << "Long Form found '" << long_form << "' with param '" << long_form_param << "'";
    bool found = false;
    for (auto it = m_opt_list.cbegin(); it != m_opt_list.cend(); ++it)
    {
        if (long_form == it->long_form)
        {
            CLOG(logFlag) << "OPT found '" << it->short_form << "' - '" << it->long_form << "'";
            #ifndef NDEBUG
                int m_ac_dbg_counter = m_current_ac;
            #endif
            on_opt(*it, long_form_param);
            assert(m_ac_dbg_counter < m_current_ac); // on_opt() must advance m_current_ac
            found = true;
            break;
        }
    }
    if (found == false)
        throw UnknowOpt(Opt{.short_form=0, .long_form = long_form}, m_ac, m_av);
}



void misc::A_GetOpt::parse_short_form(std::string const & short_form)
{
    for (std::size_t i = 0; i < short_form.size(); ++i)
    {
        bool found = false;
        CLOG(logFlag) << "Short Form found '" << short_form[i] << "'";
        for (auto it = m_opt_list.cbegin(); it != m_opt_list.cend(); ++it)
        {
            if (short_form[i] == it->short_form)
            {
                CLOG(logFlag) << "OPT found '" << it->short_form << "' - '" << it->long_form << "'";
                #ifndef NDEBUG
                    int m_ac_dbg_counter = m_current_ac;
                #endif
                on_opt(*it, "");
                assert(m_ac_dbg_counter < m_current_ac); // on_opt() must advance m_current_ac
                found = true;
                break;
            }                    
        }
        if (found == false)
            throw UnknowOpt(Opt{.short_form=short_form[i], .long_form = ""}, m_ac, m_av);
    }
}




static bool is_space(char c)
{
    return (c == ' ' || c == '\t');
}

misc::opt_toto misc::to_opt_string(std::string const & str)
{
    misc::opt_toto result;
    std::string tmp;

    for (unsigned int i = 0; i < str.size(); ++i)
    {
        if (is_space(str[i]))
        {
            if (tmp.empty() == false)
            {
                result.data.emplace_back(std::move(tmp));
                tmp.clear();
            }
        }
        else
            tmp += str[i];
    }
    if (tmp.empty() == false)
        result.data.emplace_back(std::move(tmp));

    result.ac = result.data.size();
    result.av.reserve(result.ac);
    for (unsigned int i = 0; i < result.data.size(); ++i)
        result.av.emplace_back(result.data[i].c_str());

    return (result);
}

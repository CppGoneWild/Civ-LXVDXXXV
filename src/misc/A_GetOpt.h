#pragma once


#include <string>
#include <vector>
#include <exception>



namespace misc
{



class A_GetOpt
{
public:
    struct Opt
    {
        char short_form;
        std::string long_form;
    };

    class UnknowOpt : public std::exception
    {
    public:
        UnknowOpt(Opt const & found, int ac, char const * const * av);

        virtual const char * what() const noexcept
        {
            return (m_msg.c_str());
        }

        Opt const & found() const;
        int ac() const;
        char const * const * av() const;

    private:
        Opt m_found;
        int m_ac;
        char const * const * m_av;
        std::string m_msg;
    };

    int ac() const;
    char const * const * av() const;
    char const * current_av() const;
    int current_ac() const;

    void parse(int ac, char const * const * av);

protected:
    A_GetOpt(std::vector<Opt> const &);
    A_GetOpt(std::vector<Opt> &&);

    bool is_param(std::string & short_form, std::string & long_form, std::string & long_form_param);
    bool is_param(int ac, std::string & short_form, std::string & long_form, std::string & long_form_param) const;
    bool is_param(char const * av, std::string & short_form, std::string & long_form, std::string & long_form_param) const;

    void current_ac(int);

    void parse_short_form(std::string const & short_form);
    void parse_long_form(std::string const & long_form, std::string const & long_form_param);

    virtual void on_opt(Opt const &, std::string const & long_form_param) = 0;
    virtual void on_no_opt() = 0;

private:
    int                  m_ac;
    int                  m_current_ac;
    char const * const * m_av;
    std::vector<Opt>     m_opt_list;
};



struct opt_toto
{
    int ac;
    std::vector<char const *> av;
    std::vector<std::string> data;
};

opt_toto to_opt_string(std::string const &);

} // namespace misc


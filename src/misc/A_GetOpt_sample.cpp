#include "civlog.h"

#include "A_GetOpt.h"



class GetOpt : public misc::A_GetOpt
{
public:
    GetOpt()
    : A_GetOpt({ Opt{.short_form = 'a', .long_form = "all"}
               , Opt{.short_form = 'b', .long_form = "before"}
               })
    {}

protected:
    virtual void on_opt(Opt const &, std::string const & long_form_param) override
    {
        if (long_form_param.empty())
            CLOG << current_av();
        else
            CLOG << current_av() << " and param is " << long_form_param;
        current_ac(current_ac() + 1);
    }
    virtual void on_no_opt() override
    {
        CLOG << current_av();
        current_ac(current_ac() + 1);
    }
};



int main(int ac, char ** av)
{
    GetOpt gopt;

    gopt.parse(ac, av);
    return (0);
}
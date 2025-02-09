#include "civlog.h"

#include "A_GetOpt.h"


#define LOGFLAG true


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
            CLOG(LOGFLAG) << current_av();
        else
            CLOG(LOGFLAG) << current_av() << " and param is " << long_form_param;
        current_ac(current_ac() + 1);
    }
    virtual void on_no_opt() override
    {
        CLOG(LOGFLAG) << current_av();
        current_ac(current_ac() + 1);
    }
};



int main(int ac, char ** av)
{

    try {
        misc::opt_toto test = misc::to_opt_string("prog test -a    --before -b --all=123456");
        GetOpt gopt;
        gopt.parse(test.ac, test.av.data());
    }
    catch (std::exception const & e) {
        std::cout << "what(): " << e.what() << std::endl;
    }

    try {
        GetOpt gopt;
        gopt.parse(ac, av);
    }
    catch (std::exception const & e) {
        std::cout << "what(): " << e.what() << std::endl;
    }
    return (0);
}
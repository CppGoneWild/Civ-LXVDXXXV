#pragma once


#include <cstdint> // uint8_t
#include <array>
#include <iostream>
#include <string>
#include <exception>


namespace misc
{


class TermStyle
{
public:
    class StyleOutOfRange : public std::exception
    {
    public:
      virtual const char * what() const noexcept
      {
        return ("Too many style, max is TermStyle::max_styles");
      }
    };

    enum class fg : std::uint8_t
    {
        Black   = 0
      , Red     = 1
      , Green   = 2
      , Yellow  = 3
      , Blue    = 4
      , Magenta = 5
      , Cyan    = 6
      , White   = 7
      , Default = 9

      , __MAX
    };



    enum class bg : std::uint8_t
    {
        Black   = 0
      , Red     = 1
      , Green   = 2
      , Yellow  = 3
      , Blue    = 4
      , Magenta = 5
      , Cyan    = 6
      , White   = 7
      , Default = 9

      , __MAX
    };



    enum class style : std::uint8_t
    {
        Reset      = 0
      , Bold       = 1
      , Faint      = 2
      , Italic     = 3
      , Underline  = 4
      , SlowBlink  = 5
      , FastBlink  = 6
      , Reverse    = 7
      , Conceal    = 8
      , CrossedOut = 9

      , BoldOff       = Bold       + 20
      , FaintOff      = Faint      + 20
      , ItalicOff     = Italic     + 20
      , UnderlineOff  = Underline  + 20
      , SlowBlinkOff  = SlowBlink  + 20
      , FastBlinkOff  = SlowBlink  + 20
      , ReverseOff    = Reverse    + 20
      , ConcealOff    = Conceal    + 20
      , CrossedOutOff = CrossedOut + 20
    };



    static constexpr std::size_t max_styles = 4;
    using styles_array_t = std::array<style, max_styles>;



    TermStyle()                  = default;
    TermStyle(TermStyle const &) = default;
    TermStyle(TermStyle &&)      = default;
    ~TermStyle()                 = default;

    TermStyle & operator=(TermStyle const &) = default;
    TermStyle & operator=(TermStyle &&)      = default;



    TermStyle(fg);
    TermStyle(bg);
    TermStyle(style);

    TermStyle & operator+=(fg);
    TermStyle & operator+=(bg);
    TermStyle & operator+=(style);
    TermStyle & operator+=(TermStyle const &);



    void clear();
    void clear_fg();
    void clear_bg();
    void clear_styles();



    fg foregroung() const;
    bg background() const;
    styles_array_t const & styles() const;
    std::string const & escape_sequance() const;



private:
#ifndef NDEBUG
    bool _is_valid(fg);
    bool _is_valid(bg);
    bool _is_valid(style);
#endif

  void _append_style(style);

  void _append_escape_sequance(fg);
  void _append_escape_sequance(bg);
  void _append_escape_sequance(style);
  void _generate_escape_sequance();

    fg m_fg = fg::__MAX;
    bg m_bg = bg::__MAX;
    styles_array_t m_styles { style::Reset };

    std::string m_escapeSequance = "\033[0m";
};



TermStyle operator+(TermStyle const &, TermStyle::fg);
TermStyle operator+(TermStyle const &, TermStyle::bg);
TermStyle operator+(TermStyle const &, TermStyle::style);
TermStyle operator+(TermStyle const &, TermStyle const &);

TermStyle operator+(TermStyle::fg, TermStyle const &);
TermStyle operator+(TermStyle::bg, TermStyle const &);
TermStyle operator+(TermStyle::style, TermStyle const &);



std::ostream & operator<<(std::ostream &, TermStyle const &);



extern const TermStyle termStyleDef;



} // namespace misc



std::string to_string(misc::TermStyle::fg);
std::string to_string(misc::TermStyle::bg);
std::string to_string(misc::TermStyle::style);
std::string to_string(misc::TermStyle::styles_array_t);




namespace misc
{



class ScopeStyle
{
public:
    ScopeStyle(ScopeStyle const &)             = delete;
    ScopeStyle & operator=(ScopeStyle const &) = delete;

    ScopeStyle(std::ostream & = std::cout, TermStyle const & = termStyleDef);
    ~ScopeStyle();
    
    ScopeStyle(ScopeStyle &&);
    ScopeStyle & operator=(ScopeStyle &&);

    void cancel();

private:
    TermStyle m_style_to_apply = termStyleDef;
    std::ostream * m_stream = &std::cout;
};



} // namespace misc

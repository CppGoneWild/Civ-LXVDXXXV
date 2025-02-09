#include "TermStyle.h"

#include <cstring> // memset
#include <cassert>



misc::TermStyle::TermStyle(fg color)
: m_fg(color)
, m_bg(bg::__MAX)
, m_styles { style::Reset }
, m_escapeSequance()
{
    assert(_is_valid(color));

    _generate_escape_sequance();
}

misc::TermStyle::TermStyle(bg color)
: m_fg(fg::__MAX)
, m_bg(color)
, m_styles { style::Reset }
, m_escapeSequance()
{
    assert(_is_valid(color));

    _generate_escape_sequance();
}

misc::TermStyle::TermStyle(style s)
: m_fg(fg::__MAX)
, m_bg(bg::__MAX)
, m_styles { style::Reset }
, m_escapeSequance()
{
    assert(_is_valid(s));

    _append_style(s);
    _generate_escape_sequance();
}


misc::TermStyle & misc::TermStyle::operator+=(fg color)
{
    assert(_is_valid(color));

    m_fg = color;
    _generate_escape_sequance();
    return (*this);
}

misc::TermStyle & misc::TermStyle::operator+=(bg color)
{
    assert(_is_valid(color));

    m_bg = color;
    _generate_escape_sequance();
    return (*this);
}

misc::TermStyle & misc::TermStyle::operator+=(style s)
{
    assert(_is_valid(s));

    _append_style(s);
    _generate_escape_sequance();
    return (*this);
}

misc::TermStyle & misc::TermStyle::operator+=(TermStyle const & other)
{
    if (other.m_fg != fg::__MAX)
        m_fg = other.m_fg;
    if (other.m_bg != bg::__MAX)
        m_bg = other.m_bg;
    for (std::size_t i = 0; i < max_styles; ++i)
    {
        if (m_styles[i] == style::Reset)
            break ;
        _append_style(m_styles[i]);
    }
    _generate_escape_sequance();
    return (*this);
}


void misc::TermStyle::clear()
{
    m_fg = fg::__MAX;
    m_bg = bg::__MAX;
    std::memset(m_styles.data(), static_cast<unsigned char>(style::Reset), max_styles);
    m_escapeSequance = "\033[0m";
}

void misc::TermStyle::clear_fg()
{
    m_fg = fg::__MAX;
    _generate_escape_sequance();
}

void misc::TermStyle::clear_bg()
{
    m_bg = bg::__MAX;
    _generate_escape_sequance();
}

void misc::TermStyle::clear_styles()
{
    std::memset(m_styles.data(), static_cast<unsigned char>(style::Reset), max_styles);
    _generate_escape_sequance();
}

misc::TermStyle::fg misc::TermStyle::foregroung() const
{
    return (m_fg);
}

misc::TermStyle::bg misc::TermStyle::background() const
{
    return (m_bg);
}

misc::TermStyle::styles_array_t const & misc::TermStyle::styles() const
{
    return (m_styles);
}

std::string const & misc::TermStyle::escape_sequance() const
{
    return (m_escapeSequance);
}



#ifndef NDEBUG

bool misc::TermStyle::_is_valid(fg color)
{
    if (static_cast<unsigned int>(color) >= 0 &&
        static_cast<unsigned int>(color) <= static_cast<unsigned int>(fg::__MAX) &&
        static_cast<unsigned int>(color) != 8)
        return (true);
    return (false);
}

bool misc::TermStyle::_is_valid(bg color)
{
    if (static_cast<unsigned int>(color) >= 0 &&
        static_cast<unsigned int>(color) <= static_cast<unsigned int>(bg::__MAX) &&
        static_cast<unsigned int>(color) != 8)
        return (true);
    return (false);
}

bool misc::TermStyle::_is_valid(style s)
{
    if ((static_cast<unsigned int>(s) >= 0 &&
         static_cast<unsigned int>(s) <= static_cast<unsigned int>(style::CrossedOut)) ||
        (static_cast<unsigned int>(s) >= static_cast<unsigned int>(style::BoldOff) &&
         static_cast<unsigned int>(s) <= static_cast<unsigned int>(style::CrossedOutOff)))
        return (true);
    return (false);
}


#endif

void misc::TermStyle::_append_style(style s)
{
    for (std::size_t i = 0; i < max_styles; ++i)
    {
        if (m_styles[i] == style::Reset)
        {
            m_styles[i] = s;
            return ;
        }
    }
    throw StyleOutOfRange();
}



void misc::TermStyle::_append_escape_sequance(fg color)
{
    m_escapeSequance += std::to_string(static_cast<int>(color) + 30);
}

void misc::TermStyle::_append_escape_sequance(bg color)
{
    m_escapeSequance += std::to_string(static_cast<int>(color) + 40);
}

void misc::TermStyle::_append_escape_sequance(style s)
{
    m_escapeSequance += std::to_string(static_cast<int>(s));
}

void misc::TermStyle::_generate_escape_sequance()
{
    if (m_styles[0] == style::Reset &&
        m_fg == fg::__MAX &&
        m_bg == bg::__MAX)
    {
        m_escapeSequance = "\033[0m";
        return;
    }

    m_escapeSequance = "\033[";

    if (m_fg != fg::__MAX)
    {
        _append_escape_sequance(m_fg);
        if (m_bg != bg::__MAX ||
            m_styles[0] != style::Reset)
            m_escapeSequance += ';';
    }

    if (m_bg != bg::__MAX)
    {
        _append_escape_sequance(m_bg);
        if (m_styles[0] != style::Reset)
            m_escapeSequance += ';';
    }

    for (std::size_t i = 0; i < max_styles; ++i)
    {
        if (m_styles[i] == style::Reset)
            break;
        _append_escape_sequance(m_styles[i]);
        if ((i+1) < max_styles && m_styles[i+1] != style::Reset)
            m_escapeSequance += ';';
    }
    m_escapeSequance += 'm';
}



misc::TermStyle misc::operator+(TermStyle const & ts, TermStyle::fg color)
{
    TermStyle result(ts);
    result += color;
    return (result);
}

misc::TermStyle misc::operator+(TermStyle const & ts, TermStyle::bg color)
{
    TermStyle result(ts);
    result += color;
    return (result);
}

misc::TermStyle misc::operator+(TermStyle const & ts, TermStyle::style s)
{
    TermStyle result(ts);
    result += s;
    return (result);
}

misc::TermStyle misc::operator+(TermStyle const & left, TermStyle const & right)
{
    TermStyle result(left);
    result += right;
    return (result);
}

misc::TermStyle misc::operator+(TermStyle::fg color, TermStyle const & ts)
{
    TermStyle result(ts);
    result += color;
    return (result);
}

misc::TermStyle misc::operator+(TermStyle::bg color, TermStyle const & ts)
{
    TermStyle result(ts);
    result += color;
    return (result);
}

misc::TermStyle misc::operator+(TermStyle::style s, TermStyle const & ts)
{
    TermStyle result(ts);
    result += s;
    return (result);
}




std::ostream & misc::operator<<(std::ostream & os, TermStyle const & ts)
{
    os << ts.escape_sequance();
    return (os);
}




const misc::TermStyle misc::termStyleDef;




std::string to_string(misc::TermStyle::fg color)
{
    if (color == misc::TermStyle::fg::Black)
        return ("Black");
    if (color == misc::TermStyle::fg::Red)
        return ("Red");
    if (color == misc::TermStyle::fg::Green)
        return ("Green");
    if (color == misc::TermStyle::fg::Yellow)
        return ("Yellow");
    if (color == misc::TermStyle::fg::Blue)
        return ("Blue");
    if (color == misc::TermStyle::fg::Magenta)
        return ("Magenta");
    if (color == misc::TermStyle::fg::Cyan)
        return ("Cyan");
    if (color == misc::TermStyle::fg::White)
        return ("White");
    if (color == misc::TermStyle::fg::Default)
        return ("Default");
    if (color == misc::TermStyle::fg::__MAX)
        return ("__MAX");
    return ("Unknow");
}

std::string to_string(misc::TermStyle::bg color)
{
    if (color == misc::TermStyle::bg::Black)
        return ("Black");
    if (color == misc::TermStyle::bg::Red)
        return ("Red");
    if (color == misc::TermStyle::bg::Green)
        return ("Green");
    if (color == misc::TermStyle::bg::Yellow)
        return ("Yellow");
    if (color == misc::TermStyle::bg::Blue)
        return ("Blue");
    if (color == misc::TermStyle::bg::Magenta)
        return ("Magenta");
    if (color == misc::TermStyle::bg::Cyan)
        return ("Cyan");
    if (color == misc::TermStyle::bg::White)
        return ("White");
    if (color == misc::TermStyle::bg::Default)
        return ("Default");
    if (color == misc::TermStyle::bg::__MAX)
        return ("__MAX");
    return ("Unknow");
}

std::string to_string(misc::TermStyle::style s)
{
    if (s == misc::TermStyle::style::Reset)
        return ("Reset");
    if (s == misc::TermStyle::style::Bold)
        return ("Bold");
    if (s == misc::TermStyle::style::Faint)
        return ("Faint");
    if (s == misc::TermStyle::style::Italic)
        return ("Italic");
    if (s == misc::TermStyle::style::Underline)
        return ("Underline");
    if (s == misc::TermStyle::style::SlowBlink)
        return ("SlowBlink");
    if (s == misc::TermStyle::style::FastBlink)
        return ("FastBlink");
    if (s == misc::TermStyle::style::Reverse)
        return ("Reverse");
    if (s == misc::TermStyle::style::Conceal)
        return ("Conceal");
    if (s == misc::TermStyle::style::CrossedOut)
        return ("CrossedOut");
    if (s == misc::TermStyle::style::BoldOff)
        return ("BoldOff");
    if (s == misc::TermStyle::style::FaintOff)
        return ("FaintOff");
    if (s == misc::TermStyle::style::ItalicOff)
        return ("ItalicOff");
    if (s == misc::TermStyle::style::UnderlineOff)
        return ("UnderlineOff");
    if (s == misc::TermStyle::style::SlowBlinkOff)
        return ("SlowBlinkOff");
    if (s == misc::TermStyle::style::FastBlinkOff)
        return ("FastBlinkOff");
    if (s == misc::TermStyle::style::ReverseOff)
        return ("ReverseOff");
    if (s == misc::TermStyle::style::ConcealOff)
        return ("ConcealOff");
    if (s == misc::TermStyle::style::CrossedOutOff)
        return ("CrossedOutOff");
    return ("Unknow");
}

std::string to_string(misc::TermStyle::styles_array_t s)
{
    std::string result;
    for (std::size_t i = 0; i < misc::TermStyle::max_styles; ++i)
    {
        if (s[i] == misc::TermStyle::style::Reset && i > 0)
            break;
        result += to_string(s[i]);
        if ((i+1) < misc::TermStyle::max_styles &&
            s[i+1] != misc::TermStyle::style::Reset)
            result += ';';
    }
    return (result);
}







misc::ScopeStyle::ScopeStyle(std::ostream & stream, TermStyle const & style)
: m_style_to_apply(style)
, m_stream(&stream)
{}

misc::ScopeStyle::~ScopeStyle()
{
    if (m_stream)
        *m_stream << m_style_to_apply;
}


misc::ScopeStyle::ScopeStyle(ScopeStyle && other)
: m_style_to_apply(other.m_style_to_apply)
, m_stream(other.m_stream)
{
    other.m_stream = nullptr;
}

misc::ScopeStyle & misc::ScopeStyle::operator=(ScopeStyle && other)
{
    if (this != &other)
    {
        m_style_to_apply = other.m_style_to_apply;
        m_stream = other.m_stream;
        other.m_stream = nullptr;
    }
    return (*this);
}


void misc::ScopeStyle::cancel()
{
    m_stream = nullptr;
}


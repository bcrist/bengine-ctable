#pragma once
#ifndef BE_CTABLE_BORDER_CONFIG_HPP_
#define BE_CTABLE_BORDER_CONFIG_HPP_

#include <be/core/console.hpp>
#include <vector>

namespace be::ct {

///////////////////////////////////////////////////////////////////////////////
struct BorderChar {
   char glyph = ' ';
   LogColor foreground = LogColor::current;
   LogColor background = LogColor::current;
};

///////////////////////////////////////////////////////////////////////////////
struct BorderConfig {
   U8 margin = 0;
   U8 padding = 0;
   LogColor foreground = LogColor::current;
   const char* pattern = " "; // used for first (inner) char of margin, if > 0
};

///////////////////////////////////////////////////////////////////////////////
std::vector<BorderChar> expand_border_pattern(const char* pattern, std::size_t width);

} // be::ct

#endif

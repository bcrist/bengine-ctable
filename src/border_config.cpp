#include "pch.hpp"
#include "border_config.hpp"
#include <gsl/string_span>
#include <numeric>

namespace be::ct {
namespace {

///////////////////////////////////////////////////////////////////////////////
bool is_hex_digit(char c) {
   return c >= '0' && c <= '9' ||
      c >= 'a' && c <= 'f' ||
      c >= 'A' && c <= 'F';
}

///////////////////////////////////////////////////////////////////////////////
U8 hex_digit(char c) {
   if (c >= '0' && c <= '9') {
      return c - '0';
   }

   if (c >= 'a' && c <= 'f') {
      return c - 'a' + 0xA;
   }

   if (c >= 'A' && c <= 'F') {
      return c - 'A' + 0xA;
   }

   return 0;
}

///////////////////////////////////////////////////////////////////////////////
void expand(std::vector<BorderChar>& results, const char* group, std::size_t group_length, std::size_t width) {
   if (width == 0) {
      return;
   }

   std::vector<BorderChar> group_chars;
   group_chars.reserve(group_length);

   BorderChar bc;
   for (std::size_t i = 0; i < group_length; ++i) {
      char ch = group[i];

      if (ch == '$') {
         if (i + 1 >= group_length) {
            break;
         }
         ++i;
         ch = group[i];
         if (ch == 'g') {
            if (i + 1 >= group_length) {
               break;
            }
            ++i;
            ch = group[i];
            U8 val = hex_digit(ch);
            bc.foreground = static_cast<LogColor>(val);
         } else if (ch == 'G') {
            if (i + 2 >= group_length) {
               break;
            }
            ++i;
            ch = group[i];
            U8 val = hex_digit(ch);
            bc.background = static_cast<LogColor>(val);
            ++i;
            ch = group[i];
            val = hex_digit(ch);
            bc.foreground = static_cast<LogColor>(val);
         } else if (is_hex_digit(ch)) {
            if (i + 1 >= group_length) {
               break;
            }
            U8 val = hex_digit(ch) << 4;
            ++i;
            ch = group[i];
            val |= hex_digit(ch);
            bc.glyph = (char)val;
            group_chars.push_back(bc);
         } else {
            bc.glyph = ch;
            group_chars.push_back(bc);
         }
      } else {
         bc.glyph = ch;
         group_chars.push_back(bc);
      }
   }

   if (group_chars.empty()) {
      bc.glyph = ' ';
      group_chars.push_back(bc);
   }

   for (std::size_t i = 0, j = 0; i < width; ++i, j = (j + 1) % group_chars.size()) {
      results.push_back(group_chars[j]);
   }
}

} // be::ct::()

///////////////////////////////////////////////////////////////////////////////
std::vector<BorderChar> expand_border_pattern(const char* pattern, std::size_t width) {
   std::vector<BorderChar> results;

   if (!pattern) {
      results.resize(width);
      return results;
   }

   struct group {
      const char* ptr;
      std::size_t length;
      std::size_t width;
      bool expandable;
      U8 nest_level;
   };

   std::vector<group> groups;

   U8 nest_level = 0;
   bool escape = false;
   U8 hex_escape = 0;
   for (std::size_t i = 0; pattern[i]; ++i) {
      char ch = pattern[i];

      if (hex_escape > 0) {
         if (is_hex_digit(ch)) {
            groups.back().length++;
            --hex_escape;
            continue;
         } else {
            hex_escape = 0;
         }
      }

      if (escape) {
         groups.back().length++;
         escape = false;
         if (ch == 'g') {
            hex_escape = 1;
            groups.back().width--;
         } else if (ch == 'G') {
            hex_escape = 2;
            groups.back().width--;
         } else if (is_hex_digit(ch)) {
            hex_escape = 1;
         }
         continue;
      }

      if (nest_level == 0) {
         switch (ch) {
            case '[':
               ++nest_level;
               groups.push_back({ pattern + i + 1, 0, 0, false, nest_level });
               break;
            case '(':
               ++nest_level;
               groups.push_back({ pattern + i + 1, 0, 0, true, nest_level });
               break;
            case '$':
               escape = true;
               [[fallthrough]];
            default:
               if (groups.empty() || groups.back().nest_level != 0) {
                  groups.push_back({ pattern + i, 1, 1, true, 0 });
               } else {
                  groups.back().length++;
                  groups.back().width++;
               }
         }
      } else {
         switch (ch) {
            case '[':
            case '(':
               ++nest_level;
               groups.back().length++;
               groups.back().width++;
               break;
            case ']':
            case ')':
               --nest_level;
               if (nest_level > 0) {
                  groups.back().length++;
                  groups.back().width++;
               }
               break;
            case '$':
               escape = true;
               [[fallthrough]];
            default:
               groups.back().length++;
               groups.back().width++;
         }
      }
   }

   std::size_t fixed_width = std::min(width, std::accumulate(groups.begin(), groups.end(), (std::size_t)0u,
      [](std::size_t a, const group& g) {
         return g.expandable ? a : a + g.width;
      }));
   std::size_t expandable_width = width - fixed_width;
   std::size_t expandable_groups = expandable_width > 0 ? std::accumulate(groups.begin(), groups.end(), (std::size_t)0u,
      [](std::size_t a, const group& g) {
         return g.expandable ? a + 1 : a;
      }) : 0;
   std::size_t expandable_dividend = expandable_width > 0 && expandable_groups > 0 ? expandable_width / expandable_groups : 0;
   std::size_t expandable_remainder = expandable_width > 0 && expandable_groups > 0 ? expandable_width % expandable_groups : 0;

   std::size_t expandable_group_num = 0;

   for (const group& g : groups) {
      if (g.expandable) {
         ++expandable_group_num;
         std::size_t w = expandable_dividend;
         if (expandable_group_num <= expandable_remainder) {
            ++w;
         }
         expand(results, g.ptr, g.length, w);
      } else {
         expand(results, g.ptr, g.length, g.width);
      }
   }

   results.resize(width);
   return results;
}

} // be::ct

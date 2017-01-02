#pragma once
#ifndef BE_CTABLE_BOX_CONFIG_HPP_
#define BE_CTABLE_BOX_CONFIG_HPP_

#include "border_config.hpp"
#include <be/core/console.hpp>
#include <array>

namespace be {
namespace ct {

///////////////////////////////////////////////////////////////////////////////
struct BoxConfig {
   enum side {
      top_side,
      right_side,
      bottom_side,
      left_side
   };

   enum alignment : U8 {
      inherit_alignment = 0,

      align_left = 1,
      align_center = 3,
      align_right = 2,

      align_top = 4,
      align_middle = 12,
      align_bottom = 8
   };

   using corner_func = BorderChar (*)(BorderChar, BorderChar, side, side);

   U8 align = inherit_alignment;
   LogColor foreground = LogColor::current;
   LogColor background = LogColor::current;
   std::array<BorderConfig, 4> sides;
   corner_func corners = nullptr;
};

void set_border(BoxConfig& config, const BorderConfig& all);
void set_border(BoxConfig& config, const BorderConfig& top_bottom, const BorderConfig& left_right);
void set_border(BoxConfig& config, const BorderConfig& top, const BorderConfig& left_right, const BorderConfig& bottom);
void set_border(BoxConfig& config, const BorderConfig& top, const BorderConfig& right, const BorderConfig& bottom, const BorderConfig& left);

void set_border_margin(BoxConfig& config, U8 all);
void set_border_margin(BoxConfig& config, U8 top_bottom, U8 left_right);
void set_border_margin(BoxConfig& config, U8 top, U8 left_right, U8 bottom);
void set_border_margin(BoxConfig& config, U8 top, U8 right, U8 bottom, U8 left);

void set_border_padding(BoxConfig& config, U8 all);
void set_border_padding(BoxConfig& config, U8 top_bottom, U8 left_right);
void set_border_padding(BoxConfig& config, U8 top, U8 left_right, U8 bottom);
void set_border_padding(BoxConfig& config, U8 top, U8 right, U8 bottom, U8 left);

void set_border_foreground(BoxConfig& config, LogColor all);
void set_border_foreground(BoxConfig& config, LogColor top_bottom, LogColor left_right);
void set_border_foreground(BoxConfig& config, LogColor top, LogColor left_right, LogColor bottom);
void set_border_foreground(BoxConfig& config, LogColor top, LogColor right, LogColor bottom, LogColor left);

void set_border_pattern(BoxConfig& config, const char* all);
void set_border_pattern(BoxConfig& config, const char* top_bottom, const char* left_right);
void set_border_pattern(BoxConfig& config, const char* top, const char* left_right, const char* bottom);
void set_border_pattern(BoxConfig& config, const char* top, const char* right, const char* bottom, const char* left);

} // be::ct
} // be

#endif


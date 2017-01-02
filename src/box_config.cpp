#include "pch.hpp"
#include "box_config.hpp"

namespace be {
namespace ct {

///////////////////////////////////////////////////////////////////////////////
void set_border(BoxConfig& config, const BorderConfig& all) {
   config.sides[BoxConfig::top_side] = all;
   config.sides[BoxConfig::right_side] = all;
   config.sides[BoxConfig::bottom_side] = all;
   config.sides[BoxConfig::left_side] = all;
}

///////////////////////////////////////////////////////////////////////////////
void set_border(BoxConfig& config, const BorderConfig& top_bottom, const BorderConfig& left_right) {
   config.sides[BoxConfig::top_side] = top_bottom;
   config.sides[BoxConfig::right_side] = left_right;
   config.sides[BoxConfig::bottom_side] = top_bottom;
   config.sides[BoxConfig::left_side] = left_right;
}

///////////////////////////////////////////////////////////////////////////////
void set_border(BoxConfig& config, const BorderConfig& top, const BorderConfig& left_right, const BorderConfig& bottom) {
   config.sides[BoxConfig::top_side] = top;
   config.sides[BoxConfig::right_side] = left_right;
   config.sides[BoxConfig::bottom_side] = bottom;
   config.sides[BoxConfig::left_side] = left_right;
}

///////////////////////////////////////////////////////////////////////////////
void set_border(BoxConfig& config, const BorderConfig& top, const BorderConfig& right, const BorderConfig& bottom, const BorderConfig& left) {
   config.sides[BoxConfig::top_side] = top;
   config.sides[BoxConfig::right_side] = right;
   config.sides[BoxConfig::bottom_side] = bottom;
   config.sides[BoxConfig::left_side] = left;
}

///////////////////////////////////////////////////////////////////////////////
void set_border_margin(BoxConfig& config, U8 all) {
   config.sides[BoxConfig::top_side].margin = all;
   config.sides[BoxConfig::right_side].margin = all;
   config.sides[BoxConfig::bottom_side].margin = all;
   config.sides[BoxConfig::left_side].margin = all;
}

///////////////////////////////////////////////////////////////////////////////
void set_border_margin(BoxConfig& config, U8 top_bottom, U8 left_right) {
   config.sides[BoxConfig::top_side].margin = top_bottom;
   config.sides[BoxConfig::right_side].margin = left_right;
   config.sides[BoxConfig::bottom_side].margin = top_bottom;
   config.sides[BoxConfig::left_side].margin = left_right;
}

///////////////////////////////////////////////////////////////////////////////
void set_border_margin(BoxConfig& config, U8 top, U8 left_right, U8 bottom) {
   config.sides[BoxConfig::top_side].margin = top;
   config.sides[BoxConfig::right_side].margin = left_right;
   config.sides[BoxConfig::bottom_side].margin = bottom;
   config.sides[BoxConfig::left_side].margin = left_right;
}

///////////////////////////////////////////////////////////////////////////////
void set_border_margin(BoxConfig& config, U8 top, U8 right, U8 bottom, U8 left) {
   config.sides[BoxConfig::top_side].margin = top;
   config.sides[BoxConfig::right_side].margin = right;
   config.sides[BoxConfig::bottom_side].margin = bottom;
   config.sides[BoxConfig::left_side].margin = left;
}

///////////////////////////////////////////////////////////////////////////////
void set_border_padding(BoxConfig& config, U8 all) {
   config.sides[BoxConfig::top_side].padding = all;
   config.sides[BoxConfig::right_side].padding = all;
   config.sides[BoxConfig::bottom_side].padding = all;
   config.sides[BoxConfig::left_side].padding = all;
}

///////////////////////////////////////////////////////////////////////////////
void set_border_padding(BoxConfig& config, U8 top_bottom, U8 left_right) {
   config.sides[BoxConfig::top_side].padding = top_bottom;
   config.sides[BoxConfig::right_side].padding = left_right;
   config.sides[BoxConfig::bottom_side].padding = top_bottom;
   config.sides[BoxConfig::left_side].padding = left_right;
}

///////////////////////////////////////////////////////////////////////////////
void set_border_padding(BoxConfig& config, U8 top, U8 left_right, U8 bottom) {
   config.sides[BoxConfig::top_side].padding = top;
   config.sides[BoxConfig::right_side].padding = left_right;
   config.sides[BoxConfig::bottom_side].padding = bottom;
   config.sides[BoxConfig::left_side].padding = left_right;
}

///////////////////////////////////////////////////////////////////////////////
void set_border_padding(BoxConfig& config, U8 top, U8 right, U8 bottom, U8 left) {
   config.sides[BoxConfig::top_side].padding = top;
   config.sides[BoxConfig::right_side].padding = right;
   config.sides[BoxConfig::bottom_side].padding = bottom;
   config.sides[BoxConfig::left_side].padding = left;
}

///////////////////////////////////////////////////////////////////////////////
void set_border_foreground(BoxConfig& config, LogColor all) {
   config.sides[BoxConfig::top_side].foreground = all;
   config.sides[BoxConfig::right_side].foreground = all;
   config.sides[BoxConfig::bottom_side].foreground = all;
   config.sides[BoxConfig::left_side].foreground = all;
}

///////////////////////////////////////////////////////////////////////////////
void set_border_foreground(BoxConfig& config, LogColor top_bottom, LogColor left_right) {
   config.sides[BoxConfig::top_side].foreground = top_bottom;
   config.sides[BoxConfig::right_side].foreground = left_right;
   config.sides[BoxConfig::bottom_side].foreground = top_bottom;
   config.sides[BoxConfig::left_side].foreground = left_right;
}

///////////////////////////////////////////////////////////////////////////////
void set_border_foreground(BoxConfig& config, LogColor top, LogColor left_right, LogColor bottom) {
   config.sides[BoxConfig::top_side].foreground = top;
   config.sides[BoxConfig::right_side].foreground = left_right;
   config.sides[BoxConfig::bottom_side].foreground = bottom;
   config.sides[BoxConfig::left_side].foreground = left_right;
}

///////////////////////////////////////////////////////////////////////////////
void set_border_foreground(BoxConfig& config, LogColor top, LogColor right, LogColor bottom, LogColor left) {
   config.sides[BoxConfig::top_side].foreground = top;
   config.sides[BoxConfig::right_side].foreground = right;
   config.sides[BoxConfig::bottom_side].foreground = bottom;
   config.sides[BoxConfig::left_side].foreground = left;
}

///////////////////////////////////////////////////////////////////////////////
void set_border_pattern(BoxConfig& config, const char* all) {
   config.sides[BoxConfig::top_side].pattern = all;
   config.sides[BoxConfig::right_side].pattern = all;
   config.sides[BoxConfig::bottom_side].pattern = all;
   config.sides[BoxConfig::left_side].pattern = all;
}

///////////////////////////////////////////////////////////////////////////////
void set_border_pattern(BoxConfig& config, const char* top_bottom, const char* left_right) {
   config.sides[BoxConfig::top_side].pattern = top_bottom;
   config.sides[BoxConfig::right_side].pattern = left_right;
   config.sides[BoxConfig::bottom_side].pattern = top_bottom;
   config.sides[BoxConfig::left_side].pattern = left_right;
}

///////////////////////////////////////////////////////////////////////////////
void set_border_pattern(BoxConfig& config, const char* top, const char* left_right, const char* bottom) {
   config.sides[BoxConfig::top_side].pattern = top;
   config.sides[BoxConfig::right_side].pattern = left_right;
   config.sides[BoxConfig::bottom_side].pattern = bottom;
   config.sides[BoxConfig::left_side].pattern = left_right;
}

///////////////////////////////////////////////////////////////////////////////
void set_border_pattern(BoxConfig& config, const char* top, const char* right, const char* bottom, const char* left) {
   config.sides[BoxConfig::top_side].pattern = top;
   config.sides[BoxConfig::right_side].pattern = right;
   config.sides[BoxConfig::bottom_side].pattern = bottom;
   config.sides[BoxConfig::left_side].pattern = left;
}

} // be::ct
} // be

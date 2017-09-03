#pragma once
#ifndef BE_CTABLE_BORDER_RENDERER_HPP_
#define BE_CTABLE_BORDER_RENDERER_HPP_

#include "base_renderer.hpp"
#include "border_config.hpp"
#include <be/core/console.hpp>

namespace be::ct {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
template <typename Inner>
class BorderRenderer final : public BaseRenderer<BorderRenderer<Inner>> {
   friend class BaseRenderer<BorderRenderer<Inner>>;
public:
   using vec_type = std::vector<BorderChar>;

   BorderRenderer(Inner& inner)
      : inner_(inner),
        top_enabled_(false),
        right_enabled_(false),
        bottom_enabled_(false),
        left_enabled_(false),
        inside_line_(0),
        fg_(LogColor::current),
        bg_(LogColor::current)
   { }

   BorderRenderer(Inner& inner, vec_type top, vec_type right, vec_type bottom, vec_type left)
      : inner_(inner),
        top_(std::move(top)),
        right_(std::move(right)),
        bottom_(std::move(bottom)),
        left_(std::move(left)),
        top_enabled_(!top_.empty()),
        right_enabled_(!right_.empty()),
        bottom_enabled_(!bottom_.empty()),
        left_enabled_(!left_.empty()),
        inside_line_(0),
        fg_(LogColor::current),
        bg_(LogColor::current)
   { }

   BorderRenderer(Inner& inner, vec_type top, vec_type right, vec_type bottom, vec_type left, LogColor fg, LogColor bg)
      : inner_(inner),
        top_(std::move(top)),
        right_(std::move(right)),
        bottom_(std::move(bottom)),
        left_(std::move(left)),
        top_enabled_(!top_.empty()),
        right_enabled_(!right_.empty()),
        bottom_enabled_(!bottom_.empty()),
        left_enabled_(!left_.empty()),
        inside_line_(0),
        fg_(fg),
        bg_(bg)
   { }

   vec_type& top() { return top_; }
   vec_type& right() { return right_; }
   vec_type& bottom() { return bottom_; }
   vec_type& left() { return left_; }

   vec_type& get(BoxConfig::side side) {
      switch (side) {
         case BoxConfig::top_side: return top_;
         case BoxConfig::right_side: return right_;
         case BoxConfig::bottom_side: return bottom_;
         case BoxConfig::left_side: return left_;
      }
      throw std::invalid_argument("Unrecognized side!");
   }

   bool enabled(BoxConfig::side side) {
      switch (side) {
         case BoxConfig::top_side: return top_enabled_;
         case BoxConfig::right_side: return right_enabled_;
         case BoxConfig::bottom_side: return bottom_enabled_;
         case BoxConfig::left_side: return left_enabled_;
      }
      throw std::invalid_argument("Unrecognized side!");
   }

   void enabled(BoxConfig::side side, bool value) {
      switch (side) {
         case BoxConfig::top_side: top_enabled_ = value; break;
         case BoxConfig::right_side: right_enabled_ = value; break;
         case BoxConfig::bottom_side: bottom_enabled_ = value; break;
         case BoxConfig::left_side: left_enabled_ = value; break;
         default: throw std::invalid_argument("Unrecognized side!");
      }
   }

   LogColor foreground() const { return fg_; }
   void foreground(LogColor fg) {
      fg_ = fg;
   }

   LogColor background() const { return bg_; }
   void background(LogColor bg) {
      bg_ = bg;
   }

private:
   I32 width_() const {
      return inner_.width()
         + (left_enabled_ ? 1 : 0)
         + (right_enabled_ ? 1 : 0);
   }

   I32 height_() const {
      return inner_.height()
         + (top_enabled_ ? 1 : 0)
         + (bottom_enabled_ ? 1 : 0);
   }

   void freeze_() {
      inner_.freeze();
      BaseRenderer<BorderRenderer<Inner>>::freeze_();
   }

   void render_(std::ostream& os) {
      auto base_color = get_color(os);

      if (this->line_ == 0 && top_enabled_) {
         render_rule_(os, top_);
      } else if (this->line_ == this->height() - 1 && bottom_enabled_) {
         render_rule_(os, bottom_);
      } else {
         render_side_(os, left_);
         inner_(os);
         render_side_(os, right_);
         ++inside_line_;
      }

      os << base_color;
   }

   void render_rule_(std::ostream& os, const vec_type& vec) {
      os << setcolor(fg_, bg_);
      auto initial_color = get_color(os);
      auto color = initial_color;

      if (left_enabled_) {
         auto bc = vec.empty() ? BorderChar() : vec.front();
         render_rule_char_(os, bc, color, initial_color);
      }

      for (int i = 0, w = inner_.width(); i < w; ++i) {
         int index = i + 1;
         if (index + 1 < vec.size()) {
            render_rule_char_(os, vec[index], color, initial_color);
         } else {
            os << setcolor(fg_, bg_) << ' ';
         }
      }

      if (right_enabled_) {
         auto bc = vec.empty() ? BorderChar() : vec.back();
         render_rule_char_(os, bc, color, initial_color);
      }
   }

   void render_rule_char_(std::ostream& os, BorderChar bc,
                          LogColorState& color,
                          LogColorState& initial_color) {
      if (bc.foreground == LogColor::current) {
         bc.foreground = initial_color.fg;
      }

      if (bc.background == LogColor::current) {
         bc.background = initial_color.bg;
      }

      if (color.fg != bc.foreground || color.bg != bc.background) {
         color.fg = bc.foreground;
         color.bg = bc.background;
         os << color;
      }

      os << bc.glyph;
   }

   void render_side_(std::ostream& os, const vec_type& vec) {
      if (!vec.empty()) {
         os << setcolor(fg_, bg_);
         I32 index = inside_line_ + 1;
         if (index < vec.size()) {
            auto bc = vec[index];
            os << setcolor(bc.foreground, bc.background) << bc.glyph;
         } else {
            os << ' ';
         }
      }
   }

   Inner& inner_;
   vec_type top_;
   vec_type right_;
   vec_type bottom_;
   vec_type left_;
   bool top_enabled_;
   bool right_enabled_;
   bool bottom_enabled_;
   bool left_enabled_;
   I32 inside_line_;
   LogColor fg_;
   LogColor bg_;
};

} // be::ct::detail
} // be::ct

#endif


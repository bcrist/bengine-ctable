#pragma once
#ifndef BE_CTABLE_PADDED_RENDERER_HPP_
#define BE_CTABLE_PADDED_RENDERER_HPP_

#include "base_renderer.hpp"
#include <be/core/console.hpp>

namespace be {
namespace ct {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
template <typename Inner>
class PaddedRenderer final : public BaseRenderer<PaddedRenderer<Inner>> {
   friend class base;
public:
   using pad_type = U16;

   PaddedRenderer(Inner& inner, pad_type top, pad_type right, pad_type bottom, pad_type left)
      : inner_(inner),
        top_(top),
        right_(right),
        bottom_(bottom),
        left_(left),
        fg_(LogColor::current),
        bg_(LogColor::current)
   { }

   PaddedRenderer(Inner& inner, pad_type top, pad_type right, pad_type bottom, pad_type left, LogColor fg, LogColor bg)
      : inner_(inner),
        top_(top),
        right_(right),
        bottom_(bottom),
        left_(left),
        fg_(fg),
        bg_(bg)
   { }

   pad_type top() const { return top_; }
   pad_type right() const { return right_; }
   pad_type bottom() const { return bottom_; }
   pad_type left() const { return left_; }

   void top(pad_type value) { top_ = value; }
   void right(pad_type value) { right_ = value; }
   void bottom(pad_type value) { bottom_ = value; }
   void left(pad_type value) { left_ = value; }

private:
   I32 width_() const {
      return inner_.width() + right_ + left_;
   }

   I32 height_() const {
      return inner_.height() + top_ + bottom_;
   }

   void freeze_() {
      inner_.freeze();
      base::freeze_();
   }

   void render_(std::ostream& os) {
      auto base_color = get_color(os);

      if (line_ < top_) {
         render_blank_(os, width());
      } else {
         render_blank_(os, left_);
         inner_(os);
         render_blank_(os, right_);
      }

      os << base_color;
   }

   using base::render_blank_;

   void render_blank_(std::ostream& os, I32 w) {
      os << setcolor(fg_, bg_) << S((size_t)w, ' ');
   }

   Inner& inner_;
   pad_type top_;
   pad_type right_;
   pad_type bottom_;
   pad_type left_;
   LogColor fg_;
   LogColor bg_;
};

} // be::ct::detail
} // be::ct
} // be

#endif


#pragma once
#ifndef BE_CTABLE_EMPTY_RENDERER_HPP_
#define BE_CTABLE_EMPTY_RENDERER_HPP_

#include "base_renderer.hpp"
#include <be/core/console.hpp>

namespace be {
namespace ct {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
class EmptyRenderer final : public BaseRenderer<EmptyRenderer> {
   friend class base;
public:
   using dim_type = U16;

   EmptyRenderer(dim_type width, dim_type height)
      : w_(width),
        h_(height),
        fg_(LogColor::current),
        bg_(LogColor::current)
   { }

   EmptyRenderer(dim_type width, dim_type height, LogColor fg, LogColor bg)
      : w_(width),
        h_(height),
        fg_(fg),
        bg_(bg)
   { }

private:
   I32 width_() const { return w_; }
   I32 height_() const { return h_; }

   void render_(std::ostream& os) {
      auto base_color = get_color(os);
      os << setcolor(fg_, bg_);
      render_blank_(os);
      os << base_color;
   }

   dim_type w_;
   dim_type h_;
   LogColor fg_;
   LogColor bg_;
};

} // be::ct::detail
} // be::ct
} // be

#endif


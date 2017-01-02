#pragma once
#ifndef BE_CTABLE_BASE_RENDERER_HPP_
#define BE_CTABLE_BASE_RENDERER_HPP_

#include "ctable_autolink.hpp"
#include <be/core/be.hpp>
#include <glm/vec2.hpp>
#include <ostream>
#include <type_traits>
#include <algorithm>
#include <limits>

namespace be {
namespace ct {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
template <typename Derived>
class BaseRenderer : Immovable {
public:
   I32 width() const {
      if (cached_width_ >= 0) {
         return cached_width_;
      } else {
         return self_().width_();
      }
   }

   I32 height() const {
      if (cached_height_ >= 0) {
         return cached_height_;
      } else {
         return self_().height_();
      }
   }

   explicit operator bool() const {
      return line_ < height();
   }

   void freeze() {
      self_().freeze_();
   }

   void operator()(std::ostream& os) {
      freeze();
      if (line_ < height()) {
         self_().render_(os);
         ++line_;
      } else {
         self_().render_blank_(os);
      }
   }

protected:
   using base = BaseRenderer<Derived>;

   BaseRenderer() { }

   I32 width_() const { return 0; }
   I32 height_() const { return 0; }

   void freeze_() {
      if (cached_width_ < 0 || cached_height_ < 0) {
         cached_width_ = width();
         cached_height_ = height();
      }
   }

   void render_(std::ostream& os) {
      render_blank_(os);
   }

   void render_blank_(std::ostream& os, I32 w) {
      os << S((size_t)w, ' ');
   }

   void render_blank_(std::ostream& os) {
      os << S((std::size_t)width(), ' ');
   }

   template <typename T>
   I32 clamp_(T val) const {
      return clamp_impl_(val, typename std::integral_constant<bool, sizeof(I32) <= sizeof(T)>::type());
   }

   I32 line_ = 0;
   I32 cached_width_ = -1;
   I32 cached_height_ = -1;

private:
   Derived& self_() {
      return *static_cast<Derived*>(this);
   }

   const Derived& self_() const {
      return *static_cast<const Derived*>(this);
   }

   template <typename T>
   I32 clamp_impl_(T val, True) const {
      return (I32)std::min(std::max(val, (T)0), (T)std::numeric_limits<I32>::max());
   }
   template <typename T>
   I32 clamp_impl_(T val, False) const {
      return (I32)val;
   }
};

} // be::ct::detail
} // be::ct
} // be

#endif


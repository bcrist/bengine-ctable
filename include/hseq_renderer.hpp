#pragma once
#ifndef BE_CTABLE_HSEQ_RENDERER_HPP_
#define BE_CTABLE_HSEQ_RENDERER_HPP_

#include "base_renderer.hpp"
#include <be/core/console.hpp>
#include <vector>
#include <numeric>

namespace be::ct {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
template <typename Inner>
class HSeqRenderer final : public BaseRenderer<HSeqRenderer<Inner>> {
   friend class BaseRenderer<HSeqRenderer<Inner>>;
public:
   using vec_type = std::vector<Inner*>;

   HSeqRenderer() { }

   HSeqRenderer(vec_type inner)
      : inner_(std::move(inner))
   { }

   template <typename I>
   HSeqRenderer(I begin, I end)
      : inner_(begin, end)
   { }

   void add(Inner* inner) {
      inner_.push_back(inner);
   }

private:
   I32 width_() const {
      return std::accumulate(inner_.begin(), inner_.end(), (I32)0,
         [](I32 sum, Inner* ptr) {
            return sum + ptr->width();
         });
   }

   I32 height_() const {
      return std::accumulate(inner_.begin(), inner_.end(), (I32)0,
         [](I32 m, Inner* ptr) {
            return std::max(m, ptr->height());
         });
   }

   void freeze_() {
      for (auto ptr : inner_) {
         ptr->freeze();
      }
      BaseRenderer<HSeqRenderer<Inner>>::freeze_();
   }

   void render_(std::ostream& os) {
      for (auto ptr : inner_) {
         (*ptr)(os);
      }
   }

   vec_type inner_;
};

} // be::ct::detail
} // be::ct

#endif


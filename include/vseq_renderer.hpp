#pragma once
#ifndef BE_CTABLE_VSEQ_RENDERER_HPP_
#define BE_CTABLE_VSEQ_RENDERER_HPP_

#include "base_renderer.hpp"
#include <be/core/console.hpp>
#include <vector>
#include <numeric>

namespace be::ct {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
template <typename Inner>
class VSeqRenderer final : public BaseRenderer<VSeqRenderer<Inner>> {
   friend class BaseRenderer<VSeqRenderer<Inner>>;
public:
   using vec_type = std::vector<Inner*>;

   VSeqRenderer() : index_(0) { }

   VSeqRenderer(vec_type inner)
      : inner_(std::move(inner)),
        index_(0)
   { }

   template <typename I>
   VSeqRenderer(I begin, I end)
      : inner_(begin, end),
        index_(0)
   { }

   void add(Inner* inner) {
      inner_.push_back(inner);
   }

private:
   I32 width_() const {
      return std::accumulate(inner_.begin(), inner_.end(), (I32)0,
         [](I32 m, Inner* ptr) {
            return std::max(m, ptr->width());
         });
   }

   I32 height_() const {
      return std::accumulate(inner_.begin(), inner_.end(), (I32)0,
         [](I32 sum, Inner* ptr) {
            return sum + ptr->height();
         });
   }

   void freeze_() {
      for (auto ptr : inner_) {
         ptr->freeze();
      }
      BaseRenderer<VSeqRenderer<Inner>>::freeze_();
   }

   void render_(std::ostream& os) {
      for (;;) {
         if (index_ >= inner_.size()) {
            this->render_blank_(os);
            return;
         }

         Inner& in = *inner_[index_];

         if (in) {
            in(os);

            if (in.width() < this->width()) {
               this->render_blank_(os, this->width() - in.width());
            }
            break;
         }

         ++index_;
      }
   }

   vec_type inner_;
   I32 index_;
};

} // be::ct::detail
} // be::ct

#endif


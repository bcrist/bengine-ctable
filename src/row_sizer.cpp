#include "pch.hpp"
#include "row_sizer.hpp"
#include <be/core/alg.hpp>

namespace be::ct {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
RowSizer::RowSizer(RowRenderer& row)
   : row_(&row),
     top_external_height_(0),
     bottom_external_height_(0),
     pref_internal_height_(0),
     min_internal_height_(0),
     max_internal_height_(std::numeric_limits<I32>::max())
{
   for (auto& ptr : row.cells_) {
      CellRenderer& cell = *ptr;

      I32 padding = cell.padding.top() + cell.padding.bottom();

      U16 top_external = cell.margin.top() + (cell.border.enabled(BoxConfig::top_side) ? 1 : 0);
      U16 bottom_external = cell.margin.bottom() + (cell.border.enabled(BoxConfig::bottom_side) ? 1 : 0);
      I32 pref_internal = padding + cell.text.pref_height(cell.text.width());
      I32 min_internal = padding + cell.text.min_height();
      I32 max_internal = padding + cell.text.max_height();

      top_external_height_ = max(top_external_height_, top_external);
      bottom_external_height_ = max(bottom_external_height_, bottom_external);
      pref_internal_height_ = max(pref_internal_height_, pref_internal);
      min_internal_height_ = max(min_internal_height_, min_internal);
      max_internal_height_ = min(max_internal_height_, max_internal);
      max_internal_height_ = max(max_internal_height_, min_internal_height_);
   }
}

///////////////////////////////////////////////////////////////////////////////
void RowSizer::set_heights() {
   I32 internal_height = clamp(pref_internal_height_, min_internal_height_, max_internal_height_);

   for (auto& ptr : row_->cells_) {
      CellRenderer& cell = *ptr;

      I32 text_height = internal_height - (cell.padding.top() + cell.padding.bottom());

      while (text_height < 0) {
         auto& p = cell.padding;
         if (p.bottom() > p.top()) {
            p.bottom(p.bottom() - 1);
         } else if (p.top() > 0) {
            p.top(p.top() - 1);
         }
         ++text_height;
      }
      cell.text.height(text_height);

      if (top_external_height_ == 0) {
         cell.margin.top(0);
         cell.border.enabled(BoxConfig::top_side, false);
      } else {
         U16 new_margin = top_external_height_;
         if (cell.border.enabled(BoxConfig::top_side)) {
            --new_margin;
         }
         cell.margin.top(new_margin);
      }

      if (bottom_external_height_ == 0) {
         cell.margin.bottom(0);
         cell.border.enabled(BoxConfig::bottom_side, false);
      } else {
         U16 new_margin = bottom_external_height_;
         if (cell.border.enabled(BoxConfig::bottom_side)) {
            --new_margin;
         }
         cell.margin.bottom(new_margin);
      }
   }
}

} // be::ct::detail
} // be::ct

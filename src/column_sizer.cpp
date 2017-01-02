#include "pch.hpp"
#include "column_sizer.hpp"
#include <be/core/alg.hpp>

namespace be {
namespace ct {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
ColumnSizer::ColumnSizer(std::size_t n_rows)
   : left_external_width_(0),
     right_external_width_(0),
     pref_internal_width_(0),
     min_internal_width_(0),
     max_internal_width_(std::numeric_limits<I32>::max())
{
   cells_.reserve(n_rows);
}

///////////////////////////////////////////////////////////////////////////////
void ColumnSizer::add(CellRenderer& cell) {
   cells_.push_back(&cell);

   I32 padding = cell.padding.left() + cell.padding.right();

   U16 left_external = cell.margin.left() + (cell.border.enabled(BoxConfig::left_side) ? 1 : 0);
   U16 right_external = cell.margin.right() + (cell.border.enabled(BoxConfig::right_side) ? 1 : 0);
   I32 pref_internal = padding + cell.text.pref_width();
   I32 min_internal = padding + cell.text.min_width();
   I32 max_internal = padding + cell.text.max_width();

   left_external_width_ = max(left_external_width_, left_external);
   right_external_width_ = max(right_external_width_, right_external);
   pref_internal_width_ = max(pref_internal_width_, pref_internal);
   min_internal_width_ = max(min_internal_width_, min_internal);
   max_internal_width_ = min(max_internal_width_, max_internal);
   max_internal_width_ = max(max_internal_width_, min_internal_width_);
}

///////////////////////////////////////////////////////////////////////////////
void ColumnSizer::set_widths(I32 total_width) {
   I32 internal_width = total_width - external_width();
   while (internal_width < 0) {
      if (right_external_width_ > left_external_width_) {
         --right_external_width_;
      } else if (left_external_width_ > 0) {
         --left_external_width_;
      }
      ++internal_width;
   }
   
   for (CellRenderer* ptr : cells_) {
      CellRenderer& cell = *ptr;
      I32 padding = cell.padding.left() + cell.padding.right();
      I32 text_width = internal_width - padding;

      while (text_width < 0) {
         auto& p = cell.padding;
         if (p.right() > p.left()) {
            p.right(p.right() - 1);
         } else if (p.left() > 0) {
            p.left(p.left() - 1);
         }
         ++text_width;
      }
      cell.text.width(text_width);

      if (left_external_width_ == 0) {
         cell.margin.left(0);
         cell.border.enabled(BoxConfig::left_side, false);
      } else {
         U16 new_margin = left_external_width_;
         if (cell.border.enabled(BoxConfig::left_side)) {
            --new_margin;
         }
         cell.margin.left(new_margin);
      }

      if (right_external_width_ == 0) {
         cell.margin.right(0);
         cell.border.enabled(BoxConfig::right_side, false);
      } else {
         U16 new_margin = right_external_width_;
         if (cell.border.enabled(BoxConfig::right_side)) {
            --new_margin;
         }
         cell.margin.right(new_margin);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
I32 ColumnSizer::external_width() const {
   return left_external_width_ + right_external_width_;
}

///////////////////////////////////////////////////////////////////////////////
I32 ColumnSizer::pref_internal_width() const {
   return pref_internal_width_;
}

///////////////////////////////////////////////////////////////////////////////
I32 ColumnSizer::min_internal_width() const {
   return min_internal_width_;
}

///////////////////////////////////////////////////////////////////////////////
I32 ColumnSizer::max_internal_width() const {
   return max_internal_width_;
}

} // be::ct::detail
} // be::ct
} // be

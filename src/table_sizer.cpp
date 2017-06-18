#include "pch.hpp"
#include "table_sizer.hpp"
#include "row_sizer.hpp"
#include <be/core/alg.hpp>

namespace be::ct {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
TableSizer::TableSizer(std::size_t n_rows)
   : left_margin_(0),
     right_margin_(0),
     left_padding_(0),
     right_padding_(0)
{
   rows_.reserve(n_rows);
}

///////////////////////////////////////////////////////////////////////////////
void TableSizer::add(RowRenderer& row) {
   rows_.push_back(&row);

   columns_.reserve(row.cells_.size());
   for (auto i = columns_.size(), n = row.cells_.size(); i < n; ++i) {
      columns_.push_back(ColumnSizer(rows_.capacity()));
   }

   int column = 0;
   for (auto& ptr : row.cells_) {
      columns_[column].add(*ptr);
      ++column;
   }

   left_margin_ = max(left_margin_, (U16)(row.margin.left() + (row.border.enabled(BoxConfig::left_side) ? 1 : 0)));
   right_margin_ = max(right_margin_, (U16)(row.margin.right() + (row.border.enabled(BoxConfig::right_side) ? 1 : 0)));
   left_padding_ = max(left_padding_, row.padding.left());
   right_padding_ = max(right_padding_, row.padding.right());
}

///////////////////////////////////////////////////////////////////////////////
void TableSizer::set_sizes(I32 max_row_width) {
   I32 max_internal_row_width = max_row_width - (left_margin_ + right_margin_ + left_padding_ + right_padding_);
   while (max_internal_row_width < 0) {
      if (left_margin_ <= 1 && right_margin_ <= 1 && (left_padding_ > 0 || right_padding_ > 0)) {
         if (right_padding_ > left_padding_) {
            --right_padding_;
         } else {
            --left_padding_;
         }
      } else {
         if (right_margin_ > left_margin_) {
            --right_margin_;
         } else if (left_margin_ > 0) {
            --left_margin_;
         }
      }
      ++max_internal_row_width;
   }

   for (RowRenderer* row : rows_) {
      row->padding.left(left_padding_);
      row->padding.right(right_padding_);

      if (left_margin_ == 0) {
         row->margin.left(0);
         row->border.enabled(BoxConfig::left_side, false);
      } else {
         U16 new_margin = left_margin_;
         if (row->border.enabled(BoxConfig::left_side)) {
            --new_margin;
         }
         row->margin.left(new_margin);
      }

      if (right_margin_ == 0) {
         row->margin.right(0);
         row->border.enabled(BoxConfig::right_side, false);
      } else {
         U16 new_margin = right_margin_;
         if (row->border.enabled(BoxConfig::right_side)) {
            --new_margin;
         }
         row->margin.right(new_margin);
      }
   }

   // figure out how to split remaining max_row_width between columns

   I32 total_column_external = 0;
   I32 total_column_min = 0;
   I32 total_column_pref = 0;
   I32 total_column_clamppref = 0;

   for (ColumnSizer& col : columns_) {
      total_column_external += col.external_width();
      total_column_min += col.min_internal_width();
      total_column_pref += col.pref_internal_width();
      total_column_clamppref += clamp(col.pref_internal_width(), col.min_internal_width(), col.max_internal_width());
   }

   if (total_column_clamppref + total_column_external <= max_internal_row_width) {
      // max/pref width is ok
      for (ColumnSizer& col : columns_) {
         col.set_widths(col.external_width() + clamp(col.pref_internal_width(), col.min_internal_width(), col.max_internal_width()));
      }
   } else if (total_column_min + total_column_external <= max_internal_row_width) {
      // min width is ok; try to expand as much as possible
      I32 remaining = max_internal_row_width - (total_column_min + total_column_external);
      I32 total_wanted = 0;
      std::vector<std::pair<I32, I32>> extras;
      extras.reserve(columns_.size());
      for (ColumnSizer& col : columns_) {
         I32 wanted = std::max(0, col.pref_internal_width() - col.min_internal_width());
         extras.push_back({ 0, wanted });
         total_wanted += wanted;
      }

      I32 remaining_used = 0;
      for (auto& p : extras) {
         if (p.second <= 0) {
            continue;
         }
         I32 used = (I32)floor(remaining * ((F32)p.second / (F32)total_wanted));
         p.first = used;
         remaining_used += used;
      }

      while (remaining_used < remaining) {
         I32* selected_extra = nullptr;
         F32 selected_ratio = 1337; // arbitrary large initializer
         for (auto& p : extras) {
            if (p.second <= 0) {
               continue;
            }
            F32 ratio = (F32)p.first / (F32)p.second;
            if (ratio < selected_ratio) {
               selected_ratio = ratio;
               selected_extra = &p.first;
            }
         }
         if (selected_extra) {
            ++*selected_extra;
         }
         ++remaining_used;
      }

      std::size_t index = 0;
      for (ColumnSizer& col : columns_) {
         col.set_widths(col.min_internal_width() + col.external_width() + extras[index].first);
         ++index;
      }

   } else {
      // not enough room for min width; use min width for first columns until we run out, then use 0.
      I32 remaining = max_internal_row_width;
      for (ColumnSizer& col : columns_) {
         I32 total_column_width = col.min_internal_width() + col.external_width();
         if (remaining < total_column_width) {
            col.set_widths(remaining);
            remaining = 0;
         } else {
            col.set_widths(total_column_width);
            remaining -= total_column_width;
         }
      }
   }

   // Calculate heights
   for (RowRenderer* row : rows_) {
      RowSizer sizer(*row);
      sizer.set_heights();
   }
}

} // be::ct::detail
} // be::ct

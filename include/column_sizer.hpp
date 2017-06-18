#pragma once
#ifndef BE_CTABLE_COLUMN_SIZER_HPP_
#define BE_CTABLE_COLUMN_SIZER_HPP_

#include "cell_renderer.hpp"

namespace be::ct {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
class ColumnSizer final {
public:
   using vec_type = std::vector<CellRenderer*>;

   ColumnSizer(std::size_t n_rows);

   void add(CellRenderer& cell);

   void set_widths(I32 total_width);

   I32 external_width() const;
   I32 pref_internal_width() const;
   I32 min_internal_width() const;
   I32 max_internal_width() const;

private:
   vec_type cells_;
   U16 left_external_width_;
   U16 right_external_width_;
   I32 pref_internal_width_;
   I32 min_internal_width_;
   I32 max_internal_width_;
};

} // be::ct::detail
} // be::ct

#endif


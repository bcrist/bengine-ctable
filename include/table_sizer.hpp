#pragma once
#ifndef BE_CTABLE_TABLE_SIZER_HPP_
#define BE_CTABLE_TABLE_SIZER_HPP_

#include "row_renderer.hpp"
#include "column_sizer.hpp"

namespace be::ct {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
class TableSizer final {
   using row_vec_type = std::vector<RowRenderer*>;
   using column_vec_type = std::vector<ColumnSizer>;
public:
   TableSizer(std::size_t n_rows);
   
   void add(RowRenderer& row);

   void set_sizes(I32 max_row_width);

private:
   row_vec_type rows_;
   column_vec_type columns_;
   U16 left_margin_;
   U16 right_margin_;
   U16 left_padding_;
   U16 right_padding_;
};

} // be::ct::detail
} // be::ct

#endif

#pragma once
#ifndef BE_CTABLE_ROW_SIZER_HPP_
#define BE_CTABLE_ROW_SIZER_HPP_

#include "row_renderer.hpp"

namespace be {
namespace ct {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
class RowSizer final {
public:
   explicit RowSizer(RowRenderer& row);

   void set_heights();

private:
   RowRenderer* row_;
   U16 top_external_height_;
   U16 bottom_external_height_;
   I32 pref_internal_height_;
   I32 min_internal_height_;
   I32 max_internal_height_;
};

} // be::ct::detail
} // be::ct
} // be

#endif

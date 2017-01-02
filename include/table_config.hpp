#pragma once
#ifndef BE_CTABLE_TABLE_CONFIG_HPP_
#define BE_CTABLE_TABLE_CONFIG_HPP_

#include "row_config.hpp"

namespace be {
namespace ct {

///////////////////////////////////////////////////////////////////////////////
struct TableConfig {
   std::vector<RowConfig> headers;
   std::vector<RowConfig> rows;
   I16 header_repeat_modulo = 1;
   I16 row_repeat_modulo = 1;
   BoxConfig box;
};

} // be::ct
} // be

#endif

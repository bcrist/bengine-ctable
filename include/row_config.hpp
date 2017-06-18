#pragma once
#ifndef BE_CTABLE_ROW_CONFIG_HPP_
#define BE_CTABLE_ROW_CONFIG_HPP_

#include "cell_config.hpp"

namespace be::ct {

///////////////////////////////////////////////////////////////////////////////
struct RowConfig {
   std::vector<CellConfig> cells;
   I16 cell_repeat_modulo = 1;
   BoxConfig box;
};

} // be::ct

#endif

#pragma once
#ifndef BE_CTABLE_CELL_CONFIG_HPP_
#define BE_CTABLE_CELL_CONFIG_HPP_

#include "box_config.hpp"
#include <be/core/console.hpp>

namespace be::ct {

///////////////////////////////////////////////////////////////////////////////
struct CellConfig {
   OStreamConfig stream;
   BoxConfig box;
   I16 min_width = 0;
   I16 pref_width = -1;
   I16 max_width = -1;
   I16 min_height = 0;
   I16 max_height = -1;
};

} // be::ct

#endif

#pragma once
#ifndef BE_CTABLE_CELL_HPP_
#define BE_CTABLE_CELL_HPP_

#include "cell_config.hpp"
#include <be/core/be.hpp>

namespace be {
namespace ct {
namespace detail {

class TextRenderer;

} // be::ct::detail

class Table;
class Row;

///////////////////////////////////////////////////////////////////////////////
class Cell final {
   struct datum {
      S text;
      LogColor foreground;
      LogColor background;
      bool linebreak;
   };

   enum class stream_status {
      uninitialized,
      clean,
      dirty
   };

   friend Cell& operator<<(Cell&, const Cell&);
   friend Cell& operator<<(Cell&, const LogColorState&);
   friend class detail::TextRenderer;

   using data_container = std::vector<datum>;
public:
   Cell();
   explicit Cell(CellConfig config);
   Cell(const Cell& other);
   Cell(Cell&& other) = default;
   Cell& operator=(Cell other);

   bool empty() const;

   std::ostream& stream();
   
   CellConfig& config();
   const CellConfig& config() const;

   void make_dirty();
   bool dirty() const;
   void clean() const;

private:
   void init_stream_() const;

   LogColor fg_;
   LogColor bg_;
   mutable stream_status stream_status_;
   mutable std::stringstream stream_;
   mutable data_container data_;
   CellConfig config_;
};

using CellFunc = void (*)(Cell& cell);

Cell& operator<<(Cell& cell, CellFunc func);
Cell operator<<(Cell&& cell, CellFunc func);
Cell& operator<<(Cell& cell, const Cell& other);
Cell operator<<(Cell&& cell, const Cell& other);
Cell& operator<<(Cell& cell, const LogColorState& color);
Cell operator<<(Cell&& cell, const LogColorState& color);
Cell& operator<<(Cell& cell, std::ostream& (*func)(std::ostream&));
Cell operator<<(Cell&& cell, std::ostream& (*func)(std::ostream&));
Cell& operator<<(Cell& cell, std::ios& (*func)(std::ios&));
Cell operator<<(Cell&& cell, std::ios& (*func)(std::ios&));
Cell& operator<<(Cell& cell, std::ios_base& (*func)(std::ios_base&));
Cell operator<<(Cell&& cell, std::ios_base& (*func)(std::ios_base&));

template <typename T>
Cell& operator<<(Cell& cell, const T& other) {
   cell.make_dirty();
   cell.stream() << other;
   return cell;
}

template <typename T>
Cell operator<<(Cell&& cell, const T& other) {
   cell.make_dirty();
   cell.stream() << other;
   return std::move(cell);
}

std::ostream& operator<<(std::ostream& os, const Cell& cell);

} // be::ct
} // be

#endif

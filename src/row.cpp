#include "pch.hpp"
#include "row.hpp"
#include "row_renderer.hpp"

namespace be::ct {

///////////////////////////////////////////////////////////////////////////////
Row::Row()
   : is_header_(false)
{ }

///////////////////////////////////////////////////////////////////////////////
Row::Row(RowConfig config)
   : config_(std::move(config)),
     is_header_(false)
{ }

///////////////////////////////////////////////////////////////////////////////
Row::Row(RowConfig config, bool is_header)
   : config_(std::move(config)),
     is_header_(is_header)
{ }

///////////////////////////////////////////////////////////////////////////////
bool Row::header() const {
   return is_header_;
}

///////////////////////////////////////////////////////////////////////////////
void Row::header(bool is_header) {
   is_header_ = is_header;
}

///////////////////////////////////////////////////////////////////////////////
Row::iterator Row::begin() {
   return cells_.begin();
}

///////////////////////////////////////////////////////////////////////////////
Row::const_iterator Row::begin() const {
   return cells_.begin();
}

///////////////////////////////////////////////////////////////////////////////
Row::iterator Row::end() {
   return cells_.end();
}

///////////////////////////////////////////////////////////////////////////////
Row::const_iterator Row::end() const {
   return cells_.end();
}

///////////////////////////////////////////////////////////////////////////////
Cell& Row::operator[](std::size_t index) {
   return cells_[index];
}

///////////////////////////////////////////////////////////////////////////////
const Cell& Row::operator[](std::size_t index) const {
   return cells_[index];
}

///////////////////////////////////////////////////////////////////////////////
bool Row::empty() const {
   return cells_.empty();
}

///////////////////////////////////////////////////////////////////////////////
std::size_t Row::size() const {
   return cells_.size();
}

///////////////////////////////////////////////////////////////////////////////
Cell& Row::back() {
   return cells_.back();
}

///////////////////////////////////////////////////////////////////////////////
const Cell& Row::back() const {
   return cells_.back();
}

///////////////////////////////////////////////////////////////////////////////
void Row::reserve(std::size_t capacity) {
   cells_.reserve(capacity);
}

///////////////////////////////////////////////////////////////////////////////
Row::iterator Row::insert(iterator where) {
   std::size_t count = config_.cells.size();
   if (count == 0) {
      return cells_.insert(where, Cell(CellConfig()));
   }

   std::size_t index = (std::size_t)(where - begin());
   if (index >= count) {
      index -= count;
      std::size_t modulo = config_.cell_repeat_modulo;
      if (modulo <= 0 || modulo > count) {
         modulo = count;
      }
      index %= modulo;
      index += count - modulo;
   }
   return cells_.insert(where, Cell(config_.cells[index]));
}

///////////////////////////////////////////////////////////////////////////////
Row::iterator Row::insert(iterator where, Cell cell) {
   return cells_.insert(where, std::move(cell));
}

///////////////////////////////////////////////////////////////////////////////
void Row::push_back() {
   std::size_t count = config_.cells.size();
   if (count == 0) {
      cells_.push_back(Cell(CellConfig()));
      return;
   }

   std::size_t index = cells_.size();
   if (index >= count) {
      index -= count;
      std::size_t modulo = config_.cell_repeat_modulo;
      if (modulo <= 0 || modulo > count) {
         modulo = count;
      }
      index %= modulo;
      index += count - modulo;
   }
   return cells_.push_back(Cell(config_.cells[index]));
}

///////////////////////////////////////////////////////////////////////////////
void Row::push_back(Cell cell) {
   cells_.push_back(std::move(cell));
}

///////////////////////////////////////////////////////////////////////////////
RowConfig& Row::config() {
   return config_;
}

///////////////////////////////////////////////////////////////////////////////
const RowConfig& Row::config() const {
   return config_;
}

///////////////////////////////////////////////////////////////////////////////
void cell(Row& row) {
   row.push_back();
}

///////////////////////////////////////////////////////////////////////////////
Row& operator<<(Row& row, RowFunc func) {
   func(row);
   return row;
}

///////////////////////////////////////////////////////////////////////////////
Row operator<<(Row&& row, RowFunc func) {
   func(row);
   return std::move(row);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief call push_back(); back() << *it for it in [other.begin(), other.end())
Row& operator<<(Row& row, const Row& other) {
   // TODO shouldn't this add a new row?
   row.reserve(row.size() + other.size());
   for (const Cell& cell : other) {
      row.push_back();
      row.back() << cell;
   }
   return row;
}

///////////////////////////////////////////////////////////////////////////////
Row operator<<(Row&& row, const Row& other) {
   row << other;
   return std::move(row);
}

///////////////////////////////////////////////////////////////////////////////
Row& operator<<(Row& row, const Cell& other) {
   row.push_back();
   row.back() << other;
   return row;
}

///////////////////////////////////////////////////////////////////////////////
Row operator<<(Row&& row, const Cell& other) {
   row << other;
   return std::move(row);
}

///////////////////////////////////////////////////////////////////////////////
Row& operator<<(Row& row, std::ostream& (*func)(std::ostream&)) {
   if (row.empty()) {
      row.push_back();
   }
   row.back() << func;
   return row;
}

///////////////////////////////////////////////////////////////////////////////
Row operator<<(Row&& row, std::ostream& (*func)(std::ostream&)) {
   row << func;
   return std::move(row);
}

///////////////////////////////////////////////////////////////////////////////
Row& operator<<(Row& row, std::ios& (*func)(std::ios&)) {
   if (row.empty()) {
      row.push_back();
   }
   row.back() << func;
   return row;
}

///////////////////////////////////////////////////////////////////////////////
Row operator<<(Row&& row, std::ios& (*func)(std::ios&)) {
   row << func;
   return std::move(row);
}

///////////////////////////////////////////////////////////////////////////////
Row& operator<<(Row& row, std::ios_base& (*func)(std::ios_base&)) {
   if (row.empty()) {
      row.push_back();
   }
   row.back() << func;
   return row;
}

///////////////////////////////////////////////////////////////////////////////
Row operator<<(Row&& row, std::ios_base& (*func)(std::ios_base&)) {
   row << func;
   return std::move(row);
}

///////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& os, const Row& row) {
   detail::RowRenderer r(row);
   r.auto_size(console_width(os) - 1);
   r.combine_border_corners();
   while (r) {
      os << nl;
      r(os);
   }
   return os;
}

} // be::ct

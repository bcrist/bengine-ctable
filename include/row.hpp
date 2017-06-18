#pragma once
#ifndef BE_CTABLE_ROW_HPP_
#define BE_CTABLE_ROW_HPP_

#include "row_config.hpp"
#include "cell.hpp"

namespace be::ct {

class Table;

///////////////////////////////////////////////////////////////////////////////
class Row final {
   using cell_container = std::vector<Cell>;
public:
   using iterator = cell_container::iterator;
   using const_iterator = cell_container::const_iterator;

   Row();
   explicit Row(RowConfig config);
   Row(RowConfig config, bool is_header);

   bool header() const;
   void header(bool is_header);

   iterator begin();
   const_iterator begin() const;

   iterator end();
   const_iterator end() const;

   Cell& operator[](std::size_t index);
   const Cell& operator[](std::size_t index) const;

   bool empty() const;
   std::size_t size() const;

   Cell& back();
   const Cell& back() const;

   void reserve(std::size_t capacity);

   iterator insert(iterator where);
   iterator insert(iterator where, Cell cell);
   void push_back();
   void push_back(Cell cell);

   RowConfig& config();
   const RowConfig& config() const;

private:
   cell_container cells_;
   RowConfig config_;
   bool is_header_;
};

using RowFunc = void (*)(Row& row);

void cell(Row& row);

Row& operator<<(Row& row, RowFunc func);
Row operator<<(Row&& row, RowFunc func);
Row& operator<<(Row& row, const Row& other);
Row operator<<(Row&& row, const Row& other);
Row& operator<<(Row& row, const Cell& other);
Row operator<<(Row&& row, const Cell& other);
Row& operator<<(Row& row, std::ostream& (*func)(std::ostream&));
Row operator<<(Row&& row, std::ostream& (*func)(std::ostream&));
Row& operator<<(Row& row, std::ios& (*func)(std::ios&));
Row operator<<(Row&& row, std::ios& (*func)(std::ios&));
Row& operator<<(Row& row, std::ios_base& (*func)(std::ios_base&));
Row operator<<(Row&& row, std::ios_base& (*func)(std::ios_base&));

template <typename T>
Row& operator<<(Row& row, const T& other) {
   if (row.empty()) {
      row.push_back();
   }
   row.back() << other;
   return row;
}

template <typename T>
Row operator<<(Row&& row, const T& other) {
   if (row.empty()) {
      row.push_back();
   }
   row.back() << other;
   return std::move(row);
}

std::ostream& operator<<(std::ostream& os, const Row& row);

} // be::ct

#endif

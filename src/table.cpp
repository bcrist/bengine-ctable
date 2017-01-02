#include "pch.hpp"
#include "table.hpp"
#include "table_renderer.hpp"

namespace be {
namespace ct {

///////////////////////////////////////////////////////////////////////////////
Table::Table() { }

///////////////////////////////////////////////////////////////////////////////
Table::Table(TableConfig config) :
   config_(std::move(config))
{ }

///////////////////////////////////////////////////////////////////////////////
Table::iterator Table::begin() {
   return rows_.begin();
}

///////////////////////////////////////////////////////////////////////////////
Table::const_iterator Table::begin() const {
   return rows_.begin();
}

///////////////////////////////////////////////////////////////////////////////
Table::iterator Table::end() {
   return rows_.end();
}

///////////////////////////////////////////////////////////////////////////////
Table::const_iterator Table::end() const {
   return rows_.end();
}

///////////////////////////////////////////////////////////////////////////////
Row& Table::operator[](std::size_t index) {
   return rows_[index];
}

///////////////////////////////////////////////////////////////////////////////
const Row& Table::operator[](std::size_t index) const {
   return rows_[index];
}

///////////////////////////////////////////////////////////////////////////////
bool Table::empty() const {
   return rows_.empty();
}

///////////////////////////////////////////////////////////////////////////////
std::size_t Table::size() const {
   return rows_.size();
}

///////////////////////////////////////////////////////////////////////////////
Row& Table::back() {
   return rows_.back();
}

///////////////////////////////////////////////////////////////////////////////
const Row& Table::back() const {
   return rows_.back();
}

///////////////////////////////////////////////////////////////////////////////
void Table::reserve(std::size_t capacity) {
   rows_.reserve(capacity);
}

///////////////////////////////////////////////////////////////////////////////
Table::iterator Table::insert_header(iterator where) {
   std::size_t count = config_.headers.size();
   if (count == 0) {
      return rows_.insert(where, Row(RowConfig(), true));
   }

   std::size_t index = (std::size_t)(where - begin());
   if (index >= count) {
      index -= count;
      std::size_t modulo = config_.header_repeat_modulo;
      if (modulo <= 0 || modulo > count) {
         modulo = count;
      }
      index %= modulo;
      index += count - modulo;
   }
   return rows_.insert(where, Row(config_.headers[index], true));
}

///////////////////////////////////////////////////////////////////////////////
Table::iterator Table::insert(iterator where) {
   std::size_t count = config_.rows.size();
   if (count == 0) {
      return rows_.insert(where, Row(RowConfig(), false));
   }

   std::size_t index = (std::size_t)(where - begin());   
   if (index >= count) {
      index -= count;
      std::size_t modulo = config_.row_repeat_modulo;
      if (modulo <= 0 || modulo > count) {
         modulo = count;
      }
      index %= modulo;
      index += count - modulo;
   }
   return rows_.insert(where, Row(config_.rows[index], false));
}

///////////////////////////////////////////////////////////////////////////////
Table::iterator Table::insert(iterator where, Row row) {
   return rows_.insert(where, std::move(row));
}

///////////////////////////////////////////////////////////////////////////////
void Table::push_back_header() {
   std::size_t count = config_.headers.size();
   if (count == 0) {
      rows_.push_back(Row(RowConfig(), true));
      return;
   }

   std::size_t index = rows_.size();
   if (index >= count) {
      index -= count;
      std::size_t modulo = config_.header_repeat_modulo;
      if (modulo <= 0 || modulo > count) {
         modulo = count;
      }
      index %= modulo;
      index += count - modulo;
   }
   rows_.push_back(Row(config_.headers[index], true));
}

///////////////////////////////////////////////////////////////////////////////
void Table::push_back() {
   std::size_t count = config_.rows.size();
   if (count == 0) {
      rows_.push_back(Row(RowConfig(), false));
      return;
   }

   std::size_t index = rows_.size();
   if (index >= count) {
      index -= count;
      std::size_t modulo = config_.row_repeat_modulo;
      if (modulo <= 0 || modulo > count) {
         modulo = count;
      }
      index %= modulo;
      index += count - modulo;
   }
   rows_.push_back(Row(config_.rows[index], false));
}

///////////////////////////////////////////////////////////////////////////////
void Table::push_back(Row row) {
   rows_.push_back(std::move(row));
}

///////////////////////////////////////////////////////////////////////////////
TableConfig& Table::config() {
   return config_;
}

///////////////////////////////////////////////////////////////////////////////
const TableConfig& Table::config() const {
   return config_;
}

///////////////////////////////////////////////////////////////////////////////
void row(Table& table) {
   table.push_back();
}

///////////////////////////////////////////////////////////////////////////////
void header(Table& table) {
   table.push_back_header();
}

///////////////////////////////////////////////////////////////////////////////
Table& operator<<(Table& table, TableFunc func) {
   func(table);
   return table;
}

///////////////////////////////////////////////////////////////////////////////
Table operator<<(Table&& table, TableFunc func) {
   func(table);
   return std::move(table);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Add all rows from the provided table to the provided one.
Table& operator<<(Table& table, const Table& other) {
   table.reserve(table.size() + other.size());
   for (const Row& row : other) {
      if (row.header()) {
         table.push_back_header();
      } else {
         table.push_back();
      }
      table.back() << row;
   }
   return table;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Add all rows from the provided table to the provided one.
Table operator<<(Table&& table, const Table& other) {
   table << other;
   return std::move(table);
}

///////////////////////////////////////////////////////////////////////////////
Table& operator<<(Table& table, const Row& other) {
   if (other.header()) {
      table.push_back_header();
   } else {
      table.push_back();
   }
   table.back() << other;
   return table;
}

///////////////////////////////////////////////////////////////////////////////
Table operator<<(Table&& table, const Row& other) {
   table << other;
   return std::move(table);
}

///////////////////////////////////////////////////////////////////////////////
Table& operator<<(Table& table, std::ostream& (*func)(std::ostream&)) {
   if (table.empty()) {
      table.push_back();
   }
   table.back() << func;
   return table;
}

///////////////////////////////////////////////////////////////////////////////
Table operator<<(Table&& table, std::ostream& (*func)(std::ostream&)) {
   table << func;
   return std::move(table);
}

///////////////////////////////////////////////////////////////////////////////
Table& operator<<(Table& table, std::ios& (*func)(std::ios&)) {
   if (table.empty()) {
      table.push_back();
   }
   table.back() << func;
   return table;
}

///////////////////////////////////////////////////////////////////////////////
Table operator<<(Table&& table, std::ios& (*func)(std::ios&)) {
   table << func;
   return std::move(table);
}

///////////////////////////////////////////////////////////////////////////////
Table& operator<<(Table& table, std::ios_base& (*func)(std::ios_base&)) {
   if (table.empty()) {
      table.push_back();
   }
   table.back() << func;
   return table;
}

///////////////////////////////////////////////////////////////////////////////
Table operator<<(Table&& table, std::ios_base& (*func)(std::ios_base&)) {
   table << func;
   return std::move(table);
}

///////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& os, const Table& table) {
   detail::TableRenderer r(table);
   r.auto_size(console_width(os) - 1);
   r.combine_border_corners();
   while (r) {
      os << nl;
      r(os);
   }
   return os;
}

} // be::ct
} // be

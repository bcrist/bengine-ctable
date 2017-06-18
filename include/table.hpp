#pragma once
#ifndef BE_CTABLE_TABLE_HPP_
#define BE_CTABLE_TABLE_HPP_

#include "table_config.hpp"
#include "row.hpp"

namespace be::ct {

///////////////////////////////////////////////////////////////////////////////
class Table final {
   using row_container = std::vector<Row>;
public:
   using iterator = row_container::iterator;
   using const_iterator = row_container::const_iterator;

   Table();
   explicit Table(TableConfig config);

   iterator begin();
   const_iterator begin() const;

   iterator end();
   const_iterator end() const;

   Row& operator[](std::size_t index);
   const Row& operator[](std::size_t index) const;

   bool empty() const;
   std::size_t size() const;

   Row& back();
   const Row& back() const;

   void reserve(std::size_t capacity);

   iterator insert_header(iterator where);
   iterator insert(iterator where);
   iterator insert(iterator where, Row row);

   void push_back_header();
   void push_back();
   void push_back(Row row);
   
   TableConfig& config();
   const TableConfig& config() const;

private:
   row_container rows_;
   TableConfig config_;
};

using TableFunc = void (*)(Table& table);
void row(Table& table);
void header(Table& table);

Table& operator<<(Table& table, TableFunc func);
Table operator<<(Table&& table, TableFunc func);
Table& operator<<(Table& table, const Table& other);
Table operator<<(Table&& table, const Table& other);
Table& operator<<(Table& table, const Row& other);
Table operator<<(Table&& table, const Row& other);
Table& operator<<(Table& table, std::ostream& (*func)(std::ostream&));
Table operator<<(Table&& table, std::ostream& (*func)(std::ostream&));
Table& operator<<(Table& table, std::ios& (*func)(std::ios&));
Table operator<<(Table&& table, std::ios& (*func)(std::ios&));
Table& operator<<(Table& table, std::ios_base& (*func)(std::ios_base&));
Table operator<<(Table&& table, std::ios_base& (*func)(std::ios_base&));

template <typename T>
Table& operator<<(Table& table, const T& other) {
   if (table.empty()) {
      table.push_back();
   }
   table.back() << other;
   return table;
}

template <typename T>
Table operator<<(Table&& table, const T& other) {
   if (table.empty()) {
      table.push_back();
   }
   table.back() << other;
   return std::move(table);
}

std::ostream& operator<<(std::ostream& os, const Table& table);

} // be::ct

#endif

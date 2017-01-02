#include "pch.hpp"
#include "table_renderer.hpp"
#include "table_sizer.hpp"
#include <memory>

namespace be {
namespace ct {
namespace detail {
namespace {

///////////////////////////////////////////////////////////////////////////////
U8 get_margin(const BorderConfig& config) {
   return config.margin > 0 ? config.margin - 1 : 0;
}

///////////////////////////////////////////////////////////////////////////////
U8 get_margin(const Table& table, BoxConfig::side side) {
   return get_margin(table.config().box.sides[side]);
}

///////////////////////////////////////////////////////////////////////////////
U8 get_padding(const Table& table, BoxConfig::side side) {
   return table .config().box.sides[side].padding;
}

} // be::ct::detail::()

  ///////////////////////////////////////////////////////////////////////////////
TableRenderer::TableRenderer(const Table& table)
   : seq(),
     padding(seq,
             get_padding(table, BoxConfig::top_side),
             get_padding(table, BoxConfig::right_side),
             get_padding(table, BoxConfig::bottom_side),
             get_padding(table, BoxConfig::left_side),
             table.config().box.foreground,
             table.config().box.background),
     border(padding),
     margin(border,
            get_margin(table, BoxConfig::top_side),
            get_margin(table, BoxConfig::right_side),
            get_margin(table, BoxConfig::bottom_side),
            get_margin(table, BoxConfig::left_side)),
     config_(table.config().box),
     align_(table.config().box.align)
{
   border.foreground(table.config().box.foreground);
   border.background(table.config().box.background);

   try_add_border_(BoxConfig::top_side);
   try_add_border_(BoxConfig::right_side);
   try_add_border_(BoxConfig::bottom_side);
   try_add_border_(BoxConfig::left_side);

   rows_.reserve(table.size());
   for (const Row& row : table) {
      rows_.push_back(std::make_unique<RowRenderer>(row));
      seq.add(rows_.back().get());
   }
}

///////////////////////////////////////////////////////////////////////////////
void TableRenderer::auto_size(I32 max_total_width) {
   I32 max_row_width = max_total_width;
   max_row_width -= margin.left() + margin.right();
   max_row_width -= padding.left() + padding.right();
   if (border.enabled(BoxConfig::left_side)) --max_row_width;
   if (border.enabled(BoxConfig::right_side)) --max_row_width;
   while (max_row_width < 0) {
      if (margin.left() <= 1u && margin.right() <= 1u && (padding.left() > 0u || padding.right() > 0u)) {
         if (padding.right() > padding.left()) {
            padding.right(padding.right() - 1u);
         } else {
            padding.left(padding.left() - 1u);
         }
      } else {
         if (margin.right() > margin.left()) {
            margin.right(margin.right() - 1u);
         } else if (margin.left() > 0u) {
            margin.left(margin.left() - 1u);
         }
      }
      ++max_row_width;
   }

   TableSizer sizer(rows_.size());
   for (auto& row : rows_) {
      sizer.add(*row);
   }
   sizer.set_sizes(max_row_width);
}

///////////////////////////////////////////////////////////////////////////////
void TableRenderer::combine_border_corners() {
   freeze();
   for (auto& row : rows_) {
      row->combine_border_corners();
   }
   if (config_.corners) {
      if (!border.left().empty()) {
         if (!border.top().empty()) {
            auto& bc = border.top().front();
            bc = config_.corners(bc, border.left().front(), BoxConfig::top_side, BoxConfig::left_side);
         }

         if (!border.bottom().empty()) {
            auto& bc = border.bottom().front();
            bc = config_.corners(bc, border.left().back(), BoxConfig::bottom_side, BoxConfig::left_side);
         }
      }

      if (!border.right().empty()) {
         if (!border.top().empty()) {
            auto& bc = border.top().back();
            bc = config_.corners(bc, border.right().front(), BoxConfig::top_side, BoxConfig::right_side);
         }

         if (!border.bottom().empty()) {
            auto& bc = border.bottom().back();
            bc = config_.corners(bc, border.right().back(), BoxConfig::bottom_side, BoxConfig::right_side);
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
I32 TableRenderer::width_() const {
   return margin.width();
}

///////////////////////////////////////////////////////////////////////////////
I32 TableRenderer::height_() const {
   return margin.height();
}

///////////////////////////////////////////////////////////////////////////////
void TableRenderer::freeze_() {
   bool first_freeze = cached_width_ == -1;
   if (first_freeze) {
      constexpr const U8 halign_mask = BoxConfig::align_left | BoxConfig::align_center | BoxConfig::align_right;
      constexpr const U8 valign_mask = BoxConfig::align_top | BoxConfig::align_middle | BoxConfig::align_bottom;

      for (auto& row : rows_) {
         if ((row->align() & halign_mask) == BoxConfig::inherit_alignment) {
            row->align(row->align() | (align_ & halign_mask));
         }
         if ((row->align() & valign_mask) == BoxConfig::inherit_alignment) {
            row->align(row->align() | (align_ & valign_mask));
         }
      }
   }
   margin.freeze();
   base::freeze_();
   if (first_freeze) {
      generate_border_(BoxConfig::top_side);
      generate_border_(BoxConfig::right_side);
      generate_border_(BoxConfig::bottom_side);
      generate_border_(BoxConfig::left_side);
   }
}

///////////////////////////////////////////////////////////////////////////////
void TableRenderer::render_(std::ostream& os) {
   margin(os);
}

///////////////////////////////////////////////////////////////////////////////
void TableRenderer::try_add_border_(BoxConfig::side side) {
   if (config_.sides[side].margin > 0) {
      border.enabled(side, true);
      border.top().emplace_back();
   }
}

///////////////////////////////////////////////////////////////////////////////
void TableRenderer::generate_border_(BoxConfig::side side) {
   auto& b = border.get(side);
   if (!b.empty() || border.enabled(side)) {
      auto pattern = config_.sides[side].pattern;
      std::size_t size = 2 + (side == BoxConfig::top_side || side == BoxConfig::bottom_side ?
                              padding.width() : padding.height());

      b = expand_border_pattern(pattern, size);
      resolve_border_colors_(side);
   }
}

///////////////////////////////////////////////////////////////////////////////
void TableRenderer::resolve_border_colors_(BoxConfig::side side) {
   LogColor fg = config_.sides[side].foreground;
   LogColor bg = border.background();

   if (fg == LogColor::current) {
      fg = border.foreground();
   } else if (fg == LogColor::other) {
      fg = bg;
   }

   for (auto& bc : border.get(side)) {
      if (bc.foreground == LogColor::current) {
         bc.foreground = fg;
      } else if (bc.foreground == LogColor::other) {
         bc.foreground = bg;
      }

      if (bc.background == LogColor::current) {
         bc.background = bg;
      } else if (bc.background == LogColor::other) {
         bc.background = fg;
      }
   }
}

} // be::ct::detail
} // be::ct
} // be

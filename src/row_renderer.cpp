#include "pch.hpp"
#include "row_renderer.hpp"
#include "table_sizer.hpp"
#include <memory>

namespace be::ct {
namespace detail {
namespace {

///////////////////////////////////////////////////////////////////////////////
U8 get_margin(const BorderConfig& config) {
   return config.margin > 0 ? config.margin - 1 : 0;
}

///////////////////////////////////////////////////////////////////////////////
U8 get_margin(const Row& row, BoxConfig::side side) {
   return get_margin(row.config().box.sides[side]);
}

///////////////////////////////////////////////////////////////////////////////
U8 get_padding(const Row& row, BoxConfig::side side) {
   return row.config().box.sides[side].padding;
}

} // be::ct::detail::()

///////////////////////////////////////////////////////////////////////////////
RowRenderer::RowRenderer(const Row& row)
   : seq(),
     padding(seq,
             get_padding(row, BoxConfig::top_side),
             get_padding(row, BoxConfig::right_side),
             get_padding(row, BoxConfig::bottom_side),
             get_padding(row, BoxConfig::left_side),
             row.config().box.foreground,
             row.config().box.background),
     border(padding),
     margin(border,
            get_margin(row, BoxConfig::top_side),
            get_margin(row, BoxConfig::right_side),
            get_margin(row, BoxConfig::bottom_side),
            get_margin(row, BoxConfig::left_side)),
     config_(row.config().box),
     align_(row.config().box.align)
{
   border.foreground(row.config().box.foreground);
   border.background(row.config().box.background);

   try_add_border_(BoxConfig::top_side);
   try_add_border_(BoxConfig::right_side);
   try_add_border_(BoxConfig::bottom_side);
   try_add_border_(BoxConfig::left_side);

   cells_.reserve(row.size());
   for (const Cell& cell : row) {
      cells_.push_back(std::make_unique<CellRenderer>(cell));
      seq.add(cells_.back().get());
   }
}

///////////////////////////////////////////////////////////////////////////////
void RowRenderer::auto_size(I32 max_total_width) {
   TableSizer sizer(1);
   sizer.add(*this);
   sizer.set_sizes(max_total_width);
}

///////////////////////////////////////////////////////////////////////////////
void RowRenderer::combine_border_corners() {
   freeze();
   for (auto& cell : cells_) {
      cell->combine_border_corners();
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
void RowRenderer::align(U8 align) {
   align_ = align;
}

///////////////////////////////////////////////////////////////////////////////
U8 RowRenderer::align() const {
   return align_;
}

///////////////////////////////////////////////////////////////////////////////
I32 RowRenderer::width_() const {
   return margin.width();
}

///////////////////////////////////////////////////////////////////////////////
I32 RowRenderer::height_() const {
   return margin.height();
}

///////////////////////////////////////////////////////////////////////////////
void RowRenderer::freeze_() {
   bool first_freeze = cached_width_ == -1;
   if (first_freeze) {
      constexpr const U8 halign_mask = BoxConfig::align_left | BoxConfig::align_center | BoxConfig::align_right;
      constexpr const U8 valign_mask = BoxConfig::align_top | BoxConfig::align_middle | BoxConfig::align_bottom;

      for (auto& cell : cells_) {
         if ((cell->text.align() & halign_mask) == BoxConfig::inherit_alignment) {
            cell->text.align(cell->text.align() | (align_ & halign_mask));
         }
         if ((cell->text.align() & valign_mask) == BoxConfig::inherit_alignment) {
            cell->text.align(cell->text.align() | (align_ & valign_mask));
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
void RowRenderer::render_(std::ostream& os) {
   margin(os);
}

///////////////////////////////////////////////////////////////////////////////
void RowRenderer::try_add_border_(BoxConfig::side side) {
   if (config_.sides[side].margin > 0) {
      border.enabled(side, true);
      border.top().emplace_back();
   }
}

///////////////////////////////////////////////////////////////////////////////
void RowRenderer::generate_border_(BoxConfig::side side) {
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
void RowRenderer::resolve_border_colors_(BoxConfig::side side) {
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

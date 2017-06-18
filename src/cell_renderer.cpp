#include "pch.hpp"
#include "cell_renderer.hpp"

namespace be::ct {
namespace detail {
namespace {

///////////////////////////////////////////////////////////////////////////////
U8 get_margin(const BorderConfig& config) {
   return config.margin > 0 ? config.margin - 1 : 0;
}

///////////////////////////////////////////////////////////////////////////////
U8 get_margin(const Cell& cell, BoxConfig::side side) {
   return get_margin(cell.config().box.sides[side]);
}

///////////////////////////////////////////////////////////////////////////////
U8 get_padding(const Cell& cell, BoxConfig::side side) {
   return cell.config().box.sides[side].padding;
}

} // be::ct::detail::()

///////////////////////////////////////////////////////////////////////////////
CellRenderer::CellRenderer(const Cell& cell)
   : text(cell),
     padding(text,
             get_padding(cell, BoxConfig::top_side),
             get_padding(cell, BoxConfig::right_side),
             get_padding(cell, BoxConfig::bottom_side),
             get_padding(cell, BoxConfig::left_side),
             cell.config().box.foreground,
             cell.config().box.background),
     border(padding),
     margin(border,
            get_margin(cell, BoxConfig::top_side),
            get_margin(cell, BoxConfig::right_side),
            get_margin(cell, BoxConfig::bottom_side),
            get_margin(cell, BoxConfig::left_side)),
     config_(cell.config().box)
{
   border.foreground(cell.config().box.foreground);
   border.background(cell.config().box.background);

   try_add_border_(BoxConfig::top_side);
   try_add_border_(BoxConfig::right_side);
   try_add_border_(BoxConfig::bottom_side);
   try_add_border_(BoxConfig::left_side);
}

///////////////////////////////////////////////////////////////////////////////
void CellRenderer::auto_size(I32 max_total_width) {
   text.width(0);
   I32 fixed_width = width();
   I32 pref_width = text.pref_width();
   I32 min_width = text.min_width();
   I32 max_width = text.max_width();

   I32 width = std::min(std::max(pref_width, min_width), max_width);
   width = std::min(width, max_total_width - fixed_width);
   text.width(width);

   I32 pref_height = text.pref_height(width);
   I32 min_height = text.min_height();
   I32 max_height = text.max_height();

   I32 height = std::min(std::max(pref_height, min_height), max_height);
   text.height(height);
}

///////////////////////////////////////////////////////////////////////////////
void CellRenderer::combine_border_corners() {
   freeze();
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
I32 CellRenderer::width_() const {
   return margin.width();
}

///////////////////////////////////////////////////////////////////////////////
I32 CellRenderer::height_() const {
   return margin.height();
}

///////////////////////////////////////////////////////////////////////////////
void CellRenderer::freeze_() {
   bool generate_borders = cached_width_ == -1;
   margin.freeze();
   base::freeze_();
   if (generate_borders) {
      generate_border_(BoxConfig::top_side);
      generate_border_(BoxConfig::right_side);
      generate_border_(BoxConfig::bottom_side);
      generate_border_(BoxConfig::left_side);
   }
}

///////////////////////////////////////////////////////////////////////////////
void CellRenderer::render_(std::ostream& os) {
   margin(os);
}

///////////////////////////////////////////////////////////////////////////////
void CellRenderer::try_add_border_(BoxConfig::side side) {
   if (config_.sides[side].margin > 0) {
      border.enabled(side, true);
      border.top().emplace_back();
   }
}

///////////////////////////////////////////////////////////////////////////////
void CellRenderer::generate_border_(BoxConfig::side side) {
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
void CellRenderer::resolve_border_colors_(BoxConfig::side side) {
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

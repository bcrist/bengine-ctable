#pragma once
#ifndef BE_CTABLE_CELL_RENDERER_HPP_
#define BE_CTABLE_CELL_RENDERER_HPP_

#include "text_renderer.hpp"
#include "border_renderer.hpp"
#include "padded_renderer.hpp"

namespace be::ct {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
class CellRenderer final : public BaseRenderer<CellRenderer> {
   friend class base;
public:
   using text_renderer_type = TextRenderer;
   using padding_renderer_type = PaddedRenderer<text_renderer_type>;
   using border_renderer_type = BorderRenderer<padding_renderer_type>;
   using margin_renderer_type = PaddedRenderer<border_renderer_type>;

   CellRenderer(const Cell& cell);

   text_renderer_type text;
   padding_renderer_type padding;
   border_renderer_type border;
   margin_renderer_type margin;

   void auto_size(I32 max_total_width = -1);
   void combine_border_corners();

private:
   I32 width_() const;
   I32 height_() const;

   void freeze_();
   void render_(std::ostream& os);

   void try_add_border_(BoxConfig::side side);
   void generate_border_(BoxConfig::side side);
   void resolve_border_colors_(BoxConfig::side side);

   const BoxConfig& config_;
};

} // be::ct::detail
} // be::ct

#endif

#pragma once
#ifndef BE_CTABLE_ROW_RENDERER_HPP_
#define BE_CTABLE_ROW_RENDERER_HPP_

#include "cell_renderer.hpp"
#include "hseq_renderer.hpp"
#include "row.hpp"

namespace be::ct {
namespace detail {

class RowSizer;
class TableSizer;

///////////////////////////////////////////////////////////////////////////////
class RowRenderer final : public BaseRenderer<RowRenderer> {
   friend class base;
   friend class RowSizer;
   friend class TableSizer;
   using border_iterator = std::vector<BorderChar>::iterator;
public:
   using seq_renderer_type = HSeqRenderer<CellRenderer>;
   using padding_renderer_type = PaddedRenderer<seq_renderer_type>;
   using border_renderer_type = BorderRenderer<padding_renderer_type>;
   using margin_renderer_type = PaddedRenderer<border_renderer_type>;

   RowRenderer(const Row& row);

   seq_renderer_type seq;
   padding_renderer_type padding;
   border_renderer_type border;
   margin_renderer_type margin;

   void auto_size(I32 max_total_width = -1);
   void combine_border_corners();

   void align(U8 align);
   U8 align() const;

private:
   I32 width_() const;
   I32 height_() const;

   void freeze_();
   void render_(std::ostream& os);

   void try_add_border_(BoxConfig::side side);
   void generate_border_(BoxConfig::side side);
   void resolve_border_colors_(BoxConfig::side side);

   const BoxConfig& config_;
   U8 align_;
   std::vector<std::unique_ptr<CellRenderer>> cells_;
};

} // be::ct::detail
} // be::ct

#endif


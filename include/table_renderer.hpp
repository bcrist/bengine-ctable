#pragma once
#ifndef BE_CTABLE_TABLE_RENDERER_HPP_
#define BE_CTABLE_TABLE_RENDERER_HPP_

#include "row_renderer.hpp"
#include "vseq_renderer.hpp"
#include "table.hpp"

namespace be {
namespace ct {
namespace detail {

class RowSizer;
class TableSizer;

///////////////////////////////////////////////////////////////////////////////
class TableRenderer final : public BaseRenderer<TableRenderer> {
   friend class base;
public:
   using seq_renderer_type = VSeqRenderer<RowRenderer>;
   using padding_renderer_type = PaddedRenderer<seq_renderer_type>;
   using border_renderer_type = BorderRenderer<padding_renderer_type>;
   using margin_renderer_type = PaddedRenderer<border_renderer_type>;

   TableRenderer(const Table& table);

   seq_renderer_type seq;
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
   U8 align_;
   std::vector<std::unique_ptr<RowRenderer>> rows_;
};

} // be::ct::detail
} // be::ct
} // be

#endif


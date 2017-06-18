#pragma once
#ifndef BE_CTABLE_TEXT_RENDERER_HPP_
#define BE_CTABLE_TEXT_RENDERER_HPP_

#include "base_renderer.hpp"
#include "cell.hpp"
#include <be/core/console_color.hpp>
#include <vector>

namespace be::ct {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
class TextRenderer final : public BaseRenderer<TextRenderer> {
   friend class base;

   struct datum {
      S text;
      LogColor foreground;
      LogColor background;
   };

   using line_type = std::vector<datum>;
   using vec_type = std::vector<line_type>;

public:   
   TextRenderer(const Cell& cell);

   I32 min_width() const;
   I32 min_height() const;

   I32 max_width() const;
   I32 max_height() const;

   I32 pref_width() const;
   I32 pref_height(I32 width) const;

   using base::width;
   using base::height;

   void width(I32 w);
   void height(I32 h);

   void align(U8 align);
   U8 align() const;

private:
   I32 width_() const { return w_; }
   I32 height_() const { return h_; }

   I32 calc_pref_width_() const;
   vec_type calc_data_(I32 width) const;
   void add_datum_(vec_type& data, I32 width, std::size_t& remaining, Cell::datum& d) const;

   void render_(std::ostream& os);
   void render_line_(std::ostream& os, I32 index);

   const Cell& cell_;
   vec_type lines_;
   I32 pref_w_;
   I32 w_;
   I32 h_;
   U8 align_;
   LogColorState color_;
};

} // be::ct::detail
} // be::ct

#endif


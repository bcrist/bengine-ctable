#include "pch.hpp"
#include "text_renderer.hpp"
#include <numeric>

namespace be::ct {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
TextRenderer::TextRenderer(const Cell& cell)
   : cell_(cell),
     pref_w_(calc_pref_width_()),
     w_(0),
     h_(0),
     align_(cell.config().box.align)
{ }

///////////////////////////////////////////////////////////////////////////////
I32 TextRenderer::min_width() const {
   auto raw = cell_.config().min_width;
   return raw < 0 ? 0 : raw;
}

///////////////////////////////////////////////////////////////////////////////
I32 TextRenderer::min_height() const {
   auto raw = cell_.config().min_height;
   return raw < 0 ? 0 : raw;
}

///////////////////////////////////////////////////////////////////////////////
I32 TextRenderer::max_width() const {
   auto raw = cell_.config().max_width;
   return raw < 0 ? std::numeric_limits<I16>::max() : raw;
}

///////////////////////////////////////////////////////////////////////////////
I32 TextRenderer::max_height() const {
   auto raw = cell_.config().max_height;
   return raw < 0 ? std::numeric_limits<I16>::max() : raw;
}

///////////////////////////////////////////////////////////////////////////////
I32 TextRenderer::pref_width() const {
   I32 val = cell_.config().pref_width;
   if (val < 0) {
      val = pref_w_;
   }
   return val;
}

///////////////////////////////////////////////////////////////////////////////
I32 TextRenderer::pref_height(I32 width) const {
   std::size_t h = 0;
   if (width == 0) {
      return 0;
   } else if (width == w_) {
      h = lines_.size();
   } else {
      h = calc_data_(width).size();
   }
   return clamp_(h);
}

///////////////////////////////////////////////////////////////////////////////
void TextRenderer::width(I32 width) {
   w_ = width;
   lines_ = calc_data_(w_);
}

///////////////////////////////////////////////////////////////////////////////
void TextRenderer::height(I32 height) {
   h_ = height;
}

///////////////////////////////////////////////////////////////////////////////
void TextRenderer::align(U8 align) {
   align_ = align;
}

///////////////////////////////////////////////////////////////////////////////
U8 TextRenderer::align() const {
   return align_;
}

///////////////////////////////////////////////////////////////////////////////
I32 TextRenderer::calc_pref_width_() const {
   std::size_t pref_width = 0;
   std::size_t current_width = 0;

   cell_.clean();

   for (Cell::datum d : cell_.data_) {
      current_width += d.text.size();
      if (current_width > pref_width) {
         pref_width = current_width;
      }
      if (d.linebreak) {
         current_width = 0;
      }
   }

   return clamp_(pref_width);
}

///////////////////////////////////////////////////////////////////////////////
TextRenderer::vec_type TextRenderer::calc_data_(I32 width) const {
   vec_type data;
   if (width > 0 && !cell_.data_.empty()) {
      data.emplace_back();
      std::size_t remaining = width;

      for (Cell::datum d : cell_.data_) {
         add_datum_(data, width, remaining, d);
      }
   }
   return data;
}

///////////////////////////////////////////////////////////////////////////////
void TextRenderer::add_datum_(vec_type& data, I32 width, std::size_t& remaining, Cell::datum& d) const {
   if (remaining >= d.text.size()) {
      remaining -= d.text.size();
      data.back().push_back({ d.text, d.foreground, d.background });
      if (d.linebreak) {
         data.emplace_back();
         remaining = width;
      }
      return;
   }

   auto breakpoint = d.text.begin();
   for (auto begin = d.text.begin();;) {
      auto it = std::find(begin, d.text.end(), ' ');
      if (it != d.text.end() && (int)remaining >= it + 1 - d.text.begin()) {
         breakpoint = it + 1;
         begin = it + 1;
      } else {
         break;
      }
   }

   if (breakpoint != d.text.begin()) {
      data.back().push_back({ S(d.text.begin(), breakpoint), d.foreground, d.background });
      data.emplace_back();
      remaining = width;

      // find first non-space location after breakpoint
      while (breakpoint != d.text.end() && *breakpoint == ' ') ++breakpoint;

      Cell::datum newd { S(breakpoint, d.text.end()), d.foreground, d.background, d.linebreak };
      add_datum_(data, width, remaining, newd);
      return;
   }

   if (width >= d.text.size()) {
      data.emplace_back();
      remaining = width - d.text.size();
      data.back().push_back({ d.text, d.foreground, d.background });
      if (d.linebreak) {
         data.emplace_back();
         remaining = width;
      }
      return;
   }

   breakpoint += remaining;
   data.back().push_back({ S(d.text.begin(), breakpoint), d.foreground, d.background });
   data.emplace_back();
   remaining = width;
   Cell::datum newd { S(breakpoint, d.text.end()), d.foreground, d.background, d.linebreak };
   add_datum_(data, width, remaining, newd);
}

///////////////////////////////////////////////////////////////////////////////
void TextRenderer::render_(std::ostream& os) {
   I32 index = line_;

   if (lines_.size() < h_) {
      // determine where extra padding for vertical alignment should go
      U8 valign = align_ & (BoxConfig::align_top | BoxConfig::align_bottom | BoxConfig::align_middle);

      if (valign == BoxConfig::align_middle) {
         index -= (h_ - clamp_(lines_.size())) >> 1;
      } else if (valign == BoxConfig::align_bottom) {
         index -= h_ - clamp_(lines_.size());
      }
   }

   if (index < 0 || index >= lines_.size()) {
      render_blank_(os);
   } else {
      render_line_(os, index);
   }
}

///////////////////////////////////////////////////////////////////////////////
void TextRenderer::render_line_(std::ostream& os, I32 index) {
   line_type& data = lines_[index];

   std::size_t data_length = std::accumulate(data.begin(), data.end(), (std::size_t)0,
      [](std::size_t v, const datum& d) {
         return v + d.text.length();
      });

   U8 halign = align_ & (BoxConfig::align_left | BoxConfig::align_right | BoxConfig::align_center);

   std::size_t output_length = 0;
   if (data_length < w_) {
      // determine where extra padding for alignment should go
      if (halign == BoxConfig::align_center) {
         std::size_t w = (w_ - data_length) >> 1;
         os << S(w, ' ');
         output_length += w;
      } else if (halign == BoxConfig::align_right) {
         std::size_t w = w_ - data_length;
         os << S(w, ' ');
         output_length += w;
      }
   }

   auto initial = get_color(os);
   if (index == 0) {
      color_ = initial;
   }

   for (datum& d : data) {
      auto color = setcolor(d.foreground, d.background);
      if (color.fg == LogColor::initial) {
         color.fg = initial.fg;
      }
      if (color.bg == LogColor::initial) {
         color.bg = initial.bg;
      }

      os << color_ << color;
      if (output_length + d.text.length() <= w_) {
         os << d.text;
         output_length += d.text.length();
      } else {
         os << S(d.text.begin(), d.text.begin() + (w_ - output_length));
         return;
      }
   }

   color_ = get_color(os);

   while (output_length < w_) {
      os << ' ';
      ++output_length;
   }
}

} // be::ct::detail
} // be::ct

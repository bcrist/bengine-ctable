#include "pch.hpp"
#include "cell.hpp"
#include "cell_renderer.hpp"

namespace be::ct {

///////////////////////////////////////////////////////////////////////////////
Cell::Cell()
   : fg_(LogColor::current),
     bg_(LogColor::current),
     stream_status_(stream_status::uninitialized)
{ }

///////////////////////////////////////////////////////////////////////////////
Cell::Cell(CellConfig config)
   : fg_(LogColor::current),
     bg_(LogColor::current),
     stream_status_(stream_status::uninitialized),
     config_(std::move(config))
{ }

///////////////////////////////////////////////////////////////////////////////
Cell::Cell(const Cell& other)
   : fg_(other.fg_),
     bg_(other.bg_),
     stream_status_(stream_status::clean),
     config_(other.config_)
{
   other.clean();
   data_.assign(other.data_.begin(), other.data_.end());
   set_ostream_config(stream_, get_ostream_config(other.stream_));
}

///////////////////////////////////////////////////////////////////////////////
Cell& Cell::operator=(Cell other) {
   using std::swap;
   swap(fg_, other.fg_);
   swap(bg_, other.bg_);
   swap(stream_status_, other.stream_status_);
   swap(stream_, other.stream_);
   swap(data_, other.data_);
   swap(config_, other.config_);
   return *this;
}

///////////////////////////////////////////////////////////////////////////////
bool Cell::empty() const {
   clean();
   return data_.empty();
}

///////////////////////////////////////////////////////////////////////////////
std::ostream& Cell::stream() {
   return stream_;
}

///////////////////////////////////////////////////////////////////////////////
CellConfig& Cell::config() {
   return config_;
}

///////////////////////////////////////////////////////////////////////////////
const CellConfig& Cell::config() const {
   return config_;
}

///////////////////////////////////////////////////////////////////////////////
void Cell::make_dirty() {
   init_stream_();
   stream_status_ = stream_status::dirty;
}

///////////////////////////////////////////////////////////////////////////////
bool Cell::dirty() const {
   return stream_status_ != stream_status::clean;
}

///////////////////////////////////////////////////////////////////////////////
void Cell::clean() const {
   init_stream_();
   if (stream_status_ != stream_status::clean) {
      S str = stream_.str();
      if (!str.empty()) {
         auto start = str.begin();
         bool ignore_lf = false;
         for (auto it = start, end = str.end(); it != end; ++it) {
            char c = *it;
            bool found_break = false;

            if (c == '\n') {
               if (!ignore_lf) {
                  found_break = true;
               } else {
                  ++start;
               }
            }

            if (c == '\r') {
               ignore_lf = true;
               found_break = true;
            } else {
               ignore_lf = false;
            }

            if (found_break) {
               data_.push_back({ S(start, it), fg_, bg_, true });
               start = it + 1;
            }
         }

         if (start != str.end()) {
            data_.push_back({ S(start, str.end()), fg_, bg_, false });
         }
      }
      stream_.str(S());
      stream_.clear();
      stream_status_ = stream_status::clean;
   }
}

///////////////////////////////////////////////////////////////////////////////
void Cell::init_stream_() const {
   if (stream_status_ == stream_status::uninitialized) {
      set_ostream_config(stream_, config_.stream);
      stream_status_ = stream_status::clean;
   }
}

///////////////////////////////////////////////////////////////////////////////
void clean(Cell& cell) {
   cell.clean();
}

///////////////////////////////////////////////////////////////////////////////
Cell& operator<<(Cell& cell, CellFunc func) {
   func(cell);
   return cell;
}

///////////////////////////////////////////////////////////////////////////////
Cell operator<<(Cell&& cell, CellFunc func) {
   func(cell);
   return std::move(cell);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief copy all data from another cell (but not config)
Cell& operator<<(Cell& cell, const Cell& other) {
   cell.clean();
   cell.data_.reserve(cell.data_.size() + other.data_.size());
   for (const Cell::datum& d : other.data_) {
      cell.data_.push_back(d);
   }
   set_ostream_config(cell.stream_, get_ostream_config(other.stream_));

   if (other.stream_status_ == Cell::stream_status::dirty) {
      cell.stream_ << other.stream_.rdbuf();
      cell.stream_.clear();
      cell.stream_status_ = Cell::stream_status::dirty;
   }
   
   return cell;
}

///////////////////////////////////////////////////////////////////////////////
Cell operator<<(Cell&& cell, const Cell& other) {
   cell << other;
   return std::move(cell);
}

///////////////////////////////////////////////////////////////////////////////
Cell& operator<<(Cell& cell, const LogColorState& color) {
   if (color.fg != LogColor::current || color.bg != LogColor::current) {
      cell.clean();
      cell.fg_ = color.fg;
      cell.bg_ = color.bg;
      cell.stream() << color;
   }
   return cell;
}

///////////////////////////////////////////////////////////////////////////////
Cell operator<<(Cell&& cell, const LogColorState& color) {
   cell << color;
   return std::move(cell);
}

///////////////////////////////////////////////////////////////////////////////
Cell& operator<<(Cell& cell, std::ostream& (*func)(std::ostream&)) {
   cell.make_dirty();
   cell.stream() << func;
   return cell;
}

///////////////////////////////////////////////////////////////////////////////
Cell operator<<(Cell&& cell, std::ostream& (*func)(std::ostream&)) {
   cell << func;
   return std::move(cell);
}

///////////////////////////////////////////////////////////////////////////////
Cell& operator<<(Cell& cell, std::ios& (*func)(std::ios&)) {
   cell.make_dirty();
   cell.stream() << func;
   return cell;
}

///////////////////////////////////////////////////////////////////////////////
Cell operator<<(Cell&& cell, std::ios& (*func)(std::ios&)) {
   cell << func;
   return std::move(cell);
}

///////////////////////////////////////////////////////////////////////////////
Cell& operator<<(Cell& cell, std::ios_base& (*func)(std::ios_base&)) {
   cell.make_dirty();
   cell.stream() << func;
   return cell;
}

///////////////////////////////////////////////////////////////////////////////
Cell operator<<(Cell&& cell, std::ios_base& (*func)(std::ios_base&)) {
   cell << func;
   return std::move(cell);
}

///////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& os, const Cell& cell) {
   cell.clean();
   detail::CellRenderer r(cell);
   r.auto_size(console_width(os) - 1);
   r.combine_border_corners();
   while (r) {
      os << nl;
      r(os);
   }
   return os;
}

} // be::ct

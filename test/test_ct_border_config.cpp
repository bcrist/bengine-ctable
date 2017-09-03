#ifdef BE_TEST

#include "border_config.hpp"
#include <catch/catch.hpp>

#define BE_CATCH_TAGS "[ct][ct:BorderConfig]"

using namespace be;
using namespace be::ct;
using namespace std::rel_ops;

namespace be {
namespace ct {

///////////////////////////////////////////////////////////////////////////////
bool operator==(const BorderChar& a, const BorderChar& b) {
   return a.glyph == b.glyph && a.foreground == b.foreground && a.background == b.background;
}

} // be::ct
} // be

namespace Catch {

///////////////////////////////////////////////////////////////////////////////
S toString(const std::vector<BorderChar>& vec) {
   std::ostringstream oss;
   bool found_color = false;

   oss << '"';
   for (auto& bc : vec) {
      if (bc.glyph == '"' || bc.glyph == '\\') {
         oss << '\\';
      }
      oss << bc.glyph;
      if (bc.foreground != LogColor::current || bc.background != LogColor::current) {
         found_color = true;
      }
   }
   oss << '"';

   if (found_color) {
      oss << " ";
      for (auto& bc : vec) {
         switch (bc.foreground) {
            case LogColor::current:
               oss << '_';
               break;
            case LogColor::other:
               oss << '^';
               break;
            default:
               oss << "krgybpcwKRGYBPCW"[static_cast<std::size_t>(bc.foreground)];
               break;
         }
      }

      oss << " ";
      for (auto& bc : vec) {
         switch (bc.background) {
            case LogColor::current:
               oss << '_';
               break;
            case LogColor::other:
               oss << '^';
               break;
            default:
               oss << "krgybpcwKRGYBPCW"[static_cast<std::size_t>(bc.foreground)];
               break;
         }
      }
   }


   return oss.str();
}

} // Catch

///////////////////////////////////////////////////////////////////////////////
std::vector<BorderChar> simple(const S str) {
   std::vector<BorderChar> vec;
   vec.reserve(str.length());
   BorderChar ch;
   ch.foreground = LogColor::current;
   ch.background = LogColor::current;

   for (char c : str) {
      ch.glyph = c;
      vec.push_back(ch);
   }

   return vec;
}

///////////////////////////////////////////////////////////////////////////////
std::vector<BorderChar> colored(const S str, S fg = S(), S bg = S()) {
   std::vector<BorderChar> vec;
   vec.reserve(str.length());
   BorderChar bc;
   bc.foreground = LogColor::current;
   bc.background = LogColor::current;

   for (char c : str) {
      bc.glyph = c;
      vec.push_back(bc);
   }

   if (fg.size() > vec.size()) {
      fg.resize(vec.size());
   }

   if (bg.size() > vec.size()) {
      bg.resize(vec.size());
   }

   auto it = vec.begin();
   for (char ch : fg) {
      LogColor c = LogColor::current;
      switch (ch) {
         case '^': c = LogColor::other; break;
         case 'k': c = LogColor::black; break;
         case 'r': c = LogColor::red; break;
         case 'g': c = LogColor::green; break;
         case 'y': c = LogColor::yellow; break;
         case 'b': c = LogColor::blue; break;
         case 'p': c = LogColor::purple; break;
         case 'c': c = LogColor::cyan; break;
         case 'w': c = LogColor::gray; break;
         case 'K': c = LogColor::dark_gray; break;
         case 'R': c = LogColor::bright_red; break;
         case 'G': c = LogColor::bright_green; break;
         case 'Y': c = LogColor::bright_yellow; break;
         case 'B': c = LogColor::bright_blue; break;
         case 'P': c = LogColor::bright_purple; break;
         case 'C': c = LogColor::bright_cyan; break;
         case 'W': c = LogColor::white; break;
      }
      it->foreground = c;
      ++it;
   }

   it = vec.begin();
   for (char ch : bg) {
      LogColor c = LogColor::current;
      switch (ch) {
         case '^': c = LogColor::other; break;
         case 'k': c = LogColor::black; break;
         case 'r': c = LogColor::red; break;
         case 'g': c = LogColor::green; break;
         case 'y': c = LogColor::yellow; break;
         case 'b': c = LogColor::blue; break;
         case 'p': c = LogColor::purple; break;
         case 'c': c = LogColor::cyan; break;
         case 'w': c = LogColor::gray; break;
         case 'K': c = LogColor::dark_gray; break;
         case 'R': c = LogColor::bright_red; break;
         case 'G': c = LogColor::bright_green; break;
         case 'Y': c = LogColor::bright_yellow; break;
         case 'B': c = LogColor::bright_blue; break;
         case 'P': c = LogColor::bright_purple; break;
         case 'C': c = LogColor::bright_cyan; break;
         case 'W': c = LogColor::white; break;
      }
      it->background = c;
      ++it;
   }

   return vec;
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("expand_border_pattern()", BE_CATCH_TAGS) {
   SECTION("Single implicit expandable group") {
      REQUIRE(simple("-") == expand_border_pattern("-", 1));
      REQUIRE(simple("-") == expand_border_pattern("---", 1));
      REQUIRE(simple("-") == expand_border_pattern("-============", 1));
      REQUIRE(simple("-----") == expand_border_pattern("-", 5));
      REQUIRE(simple("-=-=-") == expand_border_pattern("-=", 5));
      REQUIRE(simple("-=-=-=-=-=-=-=-=") == expand_border_pattern("-=", 16));
      REQUIRE(simple("--==-") == expand_border_pattern("--==", 5));
      REQUIRE(simple("-====") == expand_border_pattern("-=============", 5));
   }

   SECTION("Explicit expandable groups") {
      REQUIRE(simple("||||----") == expand_border_pattern("(|)(-)", 8));
      REQUIRE(simple("||||----") == expand_border_pattern("(|)-", 8));
      REQUIRE(simple("||||----") == expand_border_pattern("|(-)", 8));
      REQUIRE(simple("||||---") == expand_border_pattern("(|)(-)", 7));
      REQUIRE(simple("--==--=-=") == expand_border_pattern("(-)=(-)=(-)=", 9));
      REQUIRE(simple("----====---===---===") == expand_border_pattern("(-)=(-)=(-)=", 20));

      REQUIRE(simple("| | |./*./") == expand_border_pattern("(| )(./*)", 10));
      REQUIRE(simple("| | |./*./") == expand_border_pattern("(| )./*", 10));
      REQUIRE(simple("| | | | | ./*./*./*.") == expand_border_pattern("| (./*)", 20));

      REQUIRE(simple("| |------") == expand_border_pattern("| | | |(-)-", 9));
      REQUIRE(simple("| | | ||----------------") == expand_border_pattern("| | | |(-)-", 24));

      REQUIRE(simple("-----=-=-=-----") == expand_border_pattern("-(=-)-", 15));
   }

   SECTION("Non-expandable groups") {
      REQUIRE(simple("+---") == expand_border_pattern("[+]-", 4));
      REQUIRE(simple("---+") == expand_border_pattern("-[+]", 4));
      REQUIRE(simple("+-------") == expand_border_pattern("[+]-", 8));
      REQUIRE(simple("-------+") == expand_border_pattern("-[+]", 8));
      REQUIRE(simple("+------+") == expand_border_pattern("[+]-[+]", 8));
      REQUIRE(simple("+------+") == expand_border_pattern("[+](-)[+]", 8));
      REQUIRE(simple("|>----<|") == expand_border_pattern("[|>]-[<|]", 8));
      REQUIRE(simple("|><") == expand_border_pattern("[|>]-[<|]", 3));
      REQUIRE(simple("---+---+") == expand_border_pattern("-[+]-[+]", 8));
      REQUIRE(simple("=====---=====") == expand_border_pattern("=[---]=", 13));
      REQUIRE(simple("------- TITLE ------") == expand_border_pattern("(-)[ TITLE ](-)", 20));
      REQUIRE(simple(" - TITLE -=-=-=-=-= ") == expand_border_pattern("[ - TITLE ](-=)[ ]", 20));
      REQUIRE(simple(" - TITLE -=--=--=-- ") == expand_border_pattern("[ - TITLE ](-=-)[ ]", 20));
   }

   SECTION("Escapes") {
      REQUIRE(simple("(--------------") == expand_border_pattern("[$(]-", 15));
      REQUIRE(simple("(--------------") == expand_border_pattern("[$28]-", 15));
      REQUIRE(simple("$-------------$") == expand_border_pattern("[$24]-[$$]", 15));
      REQUIRE(simple("[-------------]") == expand_border_pattern("[$[]-[$]]", 15));
      REQUIRE(simple("-------------[]") == expand_border_pattern("-[[]]", 15));
      REQUIRE(simple("----<[asdf]>---") == expand_border_pattern("-[<[asdf]>]-", 15));
      REQUIRE(simple("[][][][][][][][") == expand_border_pattern("([])", 15));
      REQUIRE(simple("[()][()][()][()") == expand_border_pattern("([()])", 15));
      REQUIRE(simple("()-------------") == expand_border_pattern("[()]-", 15));
      REQUIRE(simple("([])-----------") == expand_border_pattern("[([])]-", 15));
      REQUIRE(simple("(()())()-------") == expand_border_pattern("[(()())()]-", 15));
   }

   SECTION("Colors") {
      REQUIRE(colored("--------", "____kkkk") == expand_border_pattern("-($g0-)", 8));
      REQUIRE(colored("--------", "____rrrr") == expand_border_pattern("-($g1-)", 8));
      REQUIRE(colored("--------", "____gggg") == expand_border_pattern("-($g2-)", 8));
      REQUIRE(colored("--------", "____yyyy") == expand_border_pattern("-($g3-)", 8));
      REQUIRE(colored("--------", "____bbbb") == expand_border_pattern("-($g4-)", 8));
      REQUIRE(colored("--------", "____pppp") == expand_border_pattern("-($g5-)", 8));
      REQUIRE(colored("--------", "____cccc") == expand_border_pattern("-($g6-)", 8));
      REQUIRE(colored("--------", "____wwww") == expand_border_pattern("-($g7-)", 8));
      REQUIRE(colored("--------", "____KKKK") == expand_border_pattern("-($g8-)", 8));
      REQUIRE(colored("--------", "____RRRR") == expand_border_pattern("-($g9-)", 8));
      REQUIRE(colored("--------", "____GGGG") == expand_border_pattern("-($ga-)", 8));
      REQUIRE(colored("--------", "____YYYY") == expand_border_pattern("-($gb-)", 8));
      REQUIRE(colored("--------", "____BBBB") == expand_border_pattern("-($gc-)", 8));
      REQUIRE(colored("--------", "____PPPP") == expand_border_pattern("-($gd-)", 8));
      REQUIRE(colored("--------", "____CCCC") == expand_border_pattern("-($ge-)", 8));
      REQUIRE(colored("--------", "____WWWW") == expand_border_pattern("-($gf-)", 8));
      REQUIRE(colored("--------", "____GGGG") == expand_border_pattern("-($gA-)", 8));
      REQUIRE(colored("--------", "____YYYY") == expand_border_pattern("-($gB-)", 8));
      REQUIRE(colored("--------", "____BBBB") == expand_border_pattern("-($gC-)", 8));
      REQUIRE(colored("--------", "____PPPP") == expand_border_pattern("-($gD-)", 8));
      REQUIRE(colored("--------", "____CCCC") == expand_border_pattern("-($gE-)", 8));
      REQUIRE(colored("--------", "____WWWW") == expand_border_pattern("-($gF-)", 8));

      REQUIRE(colored("--------", "____gggg", "____rrrr") == expand_border_pattern("-($G12-)", 8));
      REQUIRE(colored("--------", "_r_r_r_r") == expand_border_pattern("-$g1-", 8));
      REQUIRE(colored("--------", "____r___") == expand_border_pattern("-[-$g1-]-", 8));
      REQUIRE(colored("--------", "rgrg____", "_W_W____") == expand_border_pattern("($g1-$GF2-)-", 8));
   }

   SECTION("Patterns with no expandable groups result in appending spaces at the end if necessary") {
      REQUIRE(simple("     ") == expand_border_pattern("", 5));
      REQUIRE(simple("          ") == expand_border_pattern("", 10));
      REQUIRE(simple("asdf      ") == expand_border_pattern("[asdf]", 10));
      REQUIRE(simple("asdf      ") == expand_border_pattern("[a][s][d][f]", 10));
   }

   SECTION("Zero width patterns return an empty result") {
      REQUIRE(simple("") == expand_border_pattern("", 0));
      REQUIRE(simple("") == expand_border_pattern("-", 0));
      REQUIRE(simple("") == expand_border_pattern("------", 0));
      REQUIRE(simple("") == expand_border_pattern("(-)-[---]- (-)", 0));
   }
}

#endif


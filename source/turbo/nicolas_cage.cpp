#include "scintilla/include/Scintilla.h"
// #include "scintilla/lexlib/LexerModule.h"
#include <array>
#include <cstdlib>
#include <fstream>
#include <turbo/nicolas_cage.h>
#include <iostream>

#include <algorithm>
#include <cassert>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <string>
#include <turbo/app.hpp>
#include <turbo/editor.hpp>
#include <turbo/explorerWindow.hpp>
#include <tvision/tv.h>
#include <tvision/util.h>
#include <vector>
#include <iostream>

#include <turbo/nicolas_cage.h>
// #include <scintilla/include/ILex.h>
// namespace cpp_namespace {
// #include "../source/scintilla/lexers/LexCPP.cxx"
//}
// namespace {
// #include "../source/scintilla/lexers/LexAsm.cxx"
//}

// #include <scintilla/include/ILexer.h>
// #include <scintilla/Lexilla.h>
extern const Scintilla::LexerModule lmCPP, lmPython;
extern const Scintilla::LexerModule lmAsm;
extern const Scintilla::LexerModule lmBash;
extern const Scintilla::LexerModule lmNull;

std::map<std::string, int> extension2lexerId = {
    {".cpp", SCLEX_CPP},  {".c", SCLEX_CPP},   {".h", SCLEX_CPP},   {".hpp", SCLEX_CPP},
    {".cc", SCLEX_CPP},   {".asm", SCLEX_ASM}, {".s", SCLEX_ASM},   {".py", SCLEX_PYTHON},
    {".txt", SCLEX_NULL}, {"", SCLEX_NULL},    {".sh", SCLEX_BASH},
};
std::vector<const Scintilla::LexerModule *> lexers = {&lmCPP, &lmPython, &lmNull, &lmAsm,
                                                      &lmBash};

const Scintilla::LexerModule *get_lexer(std::string filename) {
  auto ext = std::filesystem::path(filename).extension().string();
  if (extension2lexerId.find(ext) == extension2lexerId.end()) {
    return &lmCPP;
  }
  auto ind = extension2lexerId[ext];
  for (auto lexer : lexers) {
    if (lexer->GetLanguage() == ind) {
      return lexer;
    }
  }
  return &lmCPP;
}

#define let auto

void NicolasCage::load_file(std::string filename) {

  std::ifstream fileToView(filename);
  const int maxLines = 100;
  const int maxLineLength = 150;
  char *lines[maxLines];
  std::string all_text = "";
  int lineCount = 0;

  if (!fileToView) {
    all_text += "file not found, file was: " + filename;

  } else {
    char buf[maxLineLength];
    while (fileToView.getline(buf, maxLineLength)) {
      all_text += buf;
      all_text += "\n";
    }
  }
  WndProc(SCI_SETTEXT, 0, reinterpret_cast<sptr_t>(all_text.c_str()));
  // WndProc(SCI_SETYCARETPOLICY, CARET_STRICT | CARET_EVEN, 10);
  // WndProc(SCI_SETVIEWEOL, 0, 0);

  // WndProc(SCI_SETMARGINS, 1, 0);
  // WndProc(SCI_SETMARGINWIDTHN, 0, 10);
  WndProc(SCI_SETMARGINTYPEN, 0, SC_MARGIN_NUMBER);
  WndProc(SC_MARGIN_TEXT, 0, 0);
  WndProc(SCI_MARGINSETTEXT, 1, reinterpret_cast<sptr_t>("0"));

  this->prefered_column = 0;
  //   WndProc(SCI_SETWRAPMODE, SC_WRAP_WORD, 0);
  //   WndProc(SCI_SETMARGINLEFT, 0, 0);
  ////WndProc(SCI_SETMARGINRIGHT, 0, 0);

  // WndProc(SCI_SETEDGEMODE, EDGE_BACKGROUND, 0);
  // WndProc(SCI_SETEDGECOLUMN, 20, 0);
  // WndProc(SCI_SETEDGECOLOUR, 0xff00, 0);
  //
  //        cell s[5] = {
  //{'H', 0}, {'e', 1}, {'l', 2}, {'l', 3}, {'o', 4},
  //       };
  //        WndProc(SCI_ADDSTYLEDTEXT, 10, reinterpret_cast<sptr_t>(s));

  let lexer = get_lexer(filename)->Create();

  WndProc(SCI_SETILEXER, 0, reinterpret_cast<sptr_t>(lexer));

  auto ext = std::filesystem::path(filename).extension().string();

  // WndProc(SCI_ADDTEXT, msg.size(), reinterpret_cast<sptr_t>(msg.c_str()));
  WndProc(SCI_COLOURISE, 0, -1);
  return;
}


void NicolasCage::set_carret_pos(std::array<int, 2> pos, bool set_prefered_column) {
  // let pos_real = WndProc(SCI_POSITIONFROMLINE, pos[1], 0);
  auto [x, y] = pos;
  let line_len = WndProc(SCI_GETLINE, y, 0);
  auto lines_count = WndProc(SCI_GETLINECOUNT, 0, 0);
  x = min(line_len - 1, x);
  if (y >= lines_count) {
    y = lines_count - 1;
    x = line_len - 1;
  }
  if (set_prefered_column) this->prefered_column = x;
  int line_start = WndProc(SCI_POSITIONFROMLINE, y, 0);
  // let pos_final = WndProc(SCI_POSITIONFROMPOINT, pos[0], pos[1]);
  WndProc(SCI_SETCURRENTPOS, line_start + x, 0);
  // WndProc(SCI_SCROLLCARET, 0, 0);
}
void NicolasCage::undo() {
  WndProc(SCI_UNDO, 0, 0);
}
void NicolasCage::redo() {
  WndProc(SCI_REDO, 0, 0);
}

void NicolasCage::paste() {
  WndProc(SCI_PASTE, 0, 0);
}

void NicolasCage::copy() {
  // std::cout << "copy called (better)" << std::endl;
  auto [x, y] = get_carret_pos();
  let line_start = WndProc(SCI_POSITIONFROMLINE, y, 0);
  let line_end = WndProc(SCI_GETLINEENDPOSITION, y, 0);
  // std::cout << "line_start: " << line_start << " line_end: " << line_end << std::endl;
  WndProc(SCI_COPYRANGE, line_start, line_end);
  // WndProc(SCI_COPYALLOWLINE, 0, 0);
}

void NicolasCage::cut() {
  auto [x, y] = get_carret_pos();
  let line_start = WndProc(SCI_POSITIONFROMLINE, y, 0);
  let line_end = WndProc(SCI_GETLINEENDPOSITION, y, 0);
  copy();
  WndProc(SCI_DELETERANGE, line_start, line_end - line_start + 1);
  // std::cout << "  cut called (better)" << std::endl;
  //  WndProc(SCI_CUTALLOWLINE, 0, 0);
}

void NicolasCage::new_file() {
  auto text = "Garter, if you're gonna act like a baby,\n you might as well crawl back up "
              "your giant __ where you belong.\n\nYou're the ones acting irresponsible "
              "\nso you crawl into my __!";

  WndProc(SCI_SETTEXT, 0, reinterpret_cast<sptr_t>(text));
  // WndProc(SCI_SETCARETSTYLE, 32, 0);

  let line_count = WndProc(SCI_GETLINECOUNT, 0, 0);
  for (int line = 0; line < line_count; line++) {
    let len = WndProc(SCI_GETLINE, line, 0);
    let buffer = (char *)calloc(len + 1, sizeof(char));
    buffer[0] = 0;
    WndProc(SCI_GETLINE, line, reinterpret_cast<sptr_t>(buffer));
    buffer[len] = 0;
    std::string res(buffer);
    free(buffer);
  }
}

std::string NicolasCage::get_margin_text(int line) {
  char buf[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int len = WndProc(SCI_MARGINGETTEXT, line, reinterpret_cast<sptr_t>(buf));
  buf[9] = 0;
  std::string res((char *)buf);
  return res;
}

std::vector<cell> NicolasCage::get_styled_line(int line) {
  let line_start = WndProc(SCI_POSITIONFROMLINE, line, 0);
  let line_end = WndProc(SCI_GETLINEENDPOSITION, line, 0);

  Sci_CharacterRange cr = {line_start, line_end};
  let len = line_end - line_start + 1;
  auto buf = (cell *)calloc(len, sizeof(cell));
  Sci_TextRange tr = {cr, reinterpret_cast<char *>(buf)};

  WndProc(SCI_GETSTYLEDTEXT, 0, reinterpret_cast<sptr_t>(&tr));
  std::vector<cell> res;
  for (int i = 0; i < len; i++) {
    res.push_back(buf[i]);
  }
  free(buf);
  if (res.back().character == 0) res.pop_back();
  return res;
}

std::string NicolasCage::get_line(int line) {
  let len = WndProc(SCI_GETLINE, line, 0);
  let buffer = (char *)calloc(len + 1, sizeof(char));
  WndProc(SCI_GETLINE, line, reinterpret_cast<sptr_t>(buffer));
  buffer[len] = 0;
  if (buffer[len - 1] == '\n') buffer[len - 1] = 0;
  std::string res(buffer);
  free(buffer);
  return res;
}

void NicolasCage::move_caret_h(int amount) {
  let pos = WndProc(SCI_GETCURRENTPOS, 0, 0);
  WndProc(SCI_SETCURRENTPOS, pos + amount, 0);
  WndProc(SCI_SCROLLCARET, 0, 0);
  this->prefered_column = get_carret_pos()[0];
}

void NicolasCage::move_caret_h_word(int amount) {
  if (amount > 0) {
    for (int i = 0; i < amount; i++) {
      KeyCommand(SCI_WORDRIGHT);
    }
  } else {
    for (int i = 0; i < -amount; i++) {
      KeyCommand(SCI_WORDLEFT);
    }
  }
}
// if (amount > 0)
// WndProc(SCK_RIGHT, 0, 0);
// else
// WndProc(SCK_LEFT, 0, 0);

void NicolasCage::move_caret_v(int amount) {
  auto [_, y] = get_carret_pos();

  y = y + amount;
  int line_count = WndProc(SCI_GETLINECOUNT, 0, 0);
  y = std::clamp(y, 0, line_count - 1);
  int line_width = WndProc(SCI_GETLINE, y, 0);
  int x = min(this->prefered_column, line_width - 1);
  set_carret_pos({x, y}, 0);
  return;
  let pos = WndProc(SCI_POSITIONFROMPOINT, x, y + amount);
  WndProc(SCI_SETCURRENTPOS, pos, pos);
  return;
  // let pos = WndProc(SCI_GETCURRENTPOS, 0, 0);

  if (amount < 0) {
    for (int i = 0; i < -amount; i++)
      KeyCommand(SCI_LINEUP);
  } else {
    for (int i = 0; i < amount; i++)
      KeyCommand(SCI_LINEDOWN);
  }
  WndProc(SCI_SCROLLCARET, 0, 0);
}

void NicolasCage::colorize() {
  WndProc(SCI_COLOURISE, 0, -1);
}
char NicolasCage::get_char_at(std::array<int, 2> pos) {
  let pos_real = WndProc(SCI_POSITIONFROMLINE, pos[1], 0);
  let ch = WndProc(SCI_GETCHARAT, pos_real + pos[0], 0);
  return ch;
}

void NicolasCage::insert_char(char ch) {
  char text[2] = {ch, 0};
  WndProc(SCI_ADDTEXT, 1, reinterpret_cast<sptr_t>(text));

  WndProc(SCI_COLOURISE, 0, -1);
  WndProc(SCI_SCROLLCARET, 0, 0);
}
void NicolasCage::delete_char() {
  let pos = WndProc(SCI_GETCURRENTPOS, 0, 0);
  WndProc(SCI_DELETERANGE, pos - 1, 1);
  WndProc(SCI_SCROLLCARET, 0, 0);
}

std::array<int, 2> NicolasCage::get_carret_pos() {
  let pos = WndProc(SCI_GETCURRENTPOS, 0, 0);
  let x = WndProc(SCI_GETCOLUMN, pos, pos);
  let y = WndProc(SCI_LINEFROMPOSITION, pos, pos);
  return {(int)x, (int)y};

  // int pos = WndProc(SCI_GETCURRENTPOS, 0, 0);
  // pos = 20;
  //// std::cout << "pos: " << pos << std::endl;
  // int x = WndProc(SCI_POINTXFROMPOSITION, pos, pos);
  // int y = WndProc(SCI_POINTYFROMPOSITION, pos, pos);
  // return {x, y};
}

int NicolasCage::get_line_count() {
  return WndProc(SCI_GETLINECOUNT, 0, 0);
}

int NicolasCage::get_first_line() {
  int line_height = WndProc(SCI_TEXTHEIGHT, 0, 0);
  // int lines_on_screen = WndProc(SCI_LINESONSCREEN, 0, 0);
  // std::cout << "lines on screen: " << lines_on_screen << std::endl;
  //  std::cout << "line height: " << line_height << std::endl;
  WndProc(SCI_SCROLLCARET, 0, 0);
  int first_line = WndProc(SCI_GETFIRSTVISIBLELINE, 0, 0);
  // std::cout << "first line: " << first_line << std::endl;
  return first_line;
}


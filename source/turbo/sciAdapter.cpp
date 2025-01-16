#include "scintilla/include/Scintilla.h"
// #include "scintilla/lexlib/LexerModule.h"
#include <array>
#include <cstdlib>
#include <fstream>

#include <algorithm>
#include <cassert>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>
#include <turbo/app.hpp>
#include <turbo/editorWindow.hpp>
#include <turbo/explorerWindow.hpp>
#include <tvision/tv.h>
#include <tvision/util.h>
#include <vector>

#include <turbo/sciAdapter.hpp>

extern const Scintilla::LexerModule lmCPP, lmPython;
extern const Scintilla::LexerModule lmAsm;
extern const Scintilla::LexerModule lmBash;
extern const Scintilla::LexerModule lmNull;

std::map<std::string, int> SciAdapter::extension_to_lexerId = {
    {".cpp", SCLEX_CPP},  {".c", SCLEX_CPP},   {".h", SCLEX_CPP},   {".hpp", SCLEX_CPP},
    {".cc", SCLEX_CPP},   {".asm", SCLEX_ASM}, {".s", SCLEX_ASM},   {".py", SCLEX_PYTHON},
    {".txt", SCLEX_NULL}, {"", SCLEX_NULL},    {".sh", SCLEX_BASH},
};
const std::vector<const Scintilla::LexerModule *> SciAdapter::lexers = {
    &lmCPP, &lmPython, &lmNull, &lmAsm, &lmBash};


void SciAdapter::init_scintilla() {
  // WndProc(SCI_SETYCARETPOLICY, CARET_STRICT | CARET_EVEN, 10);
  // WndProc(SCI_SETVIEWEOL, 0, 0);

  // WndProc(SCI_SETMARGINS, 1, 0);
  // WndProc(SCI_SETMARGINWIDTHN, 0, 10);
  WndProc(SCI_SETMARGINTYPEN, 0, SC_MARGIN_NUMBER);
  WndProc(SC_MARGIN_TEXT, 0, 0);
  WndProc(SCI_MARGINSETTEXT, 1, reinterpret_cast<sptr_t>("0"));

  this->m_prefered_column = 0;
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
}

void SciAdapter::load_file(const path &filename) {

  std::ifstream fileToView(filename);

  const int maxLineLength = 150;
  std::string all_text = "";
  int lineCount = 0;

  if (fileToView) {
    char buf[maxLineLength];
    while (fileToView.getline(buf, maxLineLength)) {
      all_text += buf;
      all_text += "\n";
    }
  }

  WndProc(SCI_SETTEXT, 0, reinterpret_cast<sptr_t>(all_text.c_str()));

  auto lexer = get_lexer(filename)->Create();
  WndProc(SCI_SETILEXER, 0, reinterpret_cast<sptr_t>(lexer));
  WndProc(SCI_COLOURISE, 0, -1);
}

const Scintilla::LexerModule *SciAdapter::get_lexer(const path &filename) {
  auto ext = filename.extension().string();
  if (extension_to_lexerId.find(ext) == extension_to_lexerId.end()) {
    return &lmCPP;
  }
  auto ind = extension_to_lexerId[ext];
  for (auto lexer : lexers) {
    if (lexer->GetLanguage() == ind) {
      return lexer;
    }
  }
  return &lmCPP;
}


void SciAdapter::set_cursor_pos(std::array<int, 2> pos, bool set_m_prefered_column) {

  auto [x, y] = pos;
  auto line_len = WndProc(SCI_GETLINE, y, 0);
  auto lines_count = WndProc(SCI_GETLINECOUNT, 0, 0);

  x = min(line_len - 1, x);
  if (y >= lines_count) {
    y = lines_count - 1;
    x = line_len - 1;
  }

  if (set_m_prefered_column) m_prefered_column = x;

  int line_start = WndProc(SCI_POSITIONFROMLINE, y, 0);
  WndProc(SCI_SETCURRENTPOS, line_start + x, 0);
}

std::array<int, 2> SciAdapter::get_cursor_pos() {
  auto pos = WndProc(SCI_GETCURRENTPOS, 0, 0);
  int x = WndProc(SCI_GETCOLUMN, pos, pos);
  int y = WndProc(SCI_LINEFROMPOSITION, pos, pos);
  return {x, y};
}

char SciAdapter::get_char_at(std::array<int, 2> pos) {
  auto pos_real = WndProc(SCI_POSITIONFROMLINE, pos[1], 0);
  auto ch = WndProc(SCI_GETCHARAT, pos_real + pos[0], 0);
  return ch;
}

void SciAdapter::insert_char(char ch) {
  char text[2] = {ch, 0};
  WndProc(SCI_ADDTEXT, 1, reinterpret_cast<sptr_t>(text));
  WndProc(SCI_COLOURISE, 0, -1);
}

void SciAdapter::delete_char() {
  auto pos = WndProc(SCI_GETCURRENTPOS, 0, 0);
  WndProc(SCI_DELETERANGE, pos - 1, 1);
}

int SciAdapter::get_line_count() {
  return WndProc(SCI_GETLINECOUNT, 0, 0);
}

int SciAdapter::get_first_line_idx() {
  int first_line = WndProc(SCI_GETFIRSTVISIBLELINE, 0, 0);
  return first_line;
}

std::vector<cell> SciAdapter::get_styled_line(int line) {
  auto line_start = WndProc(SCI_POSITIONFROMLINE, line, 0);
  auto line_end = WndProc(SCI_GETLINEENDPOSITION, line, 0);

  Sci_CharacterRange cr = {line_start, line_end};
  auto len = line_end - line_start + 1;
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

std::string SciAdapter::get_line(int line) {
  auto len = WndProc(SCI_GETLINE, line, 0);
  auto buffer = (char *)calloc(len + 1, sizeof(char));
  WndProc(SCI_GETLINE, line, reinterpret_cast<sptr_t>(buffer));
  buffer[len] = 0;
  if (buffer[len - 1] == '\n') buffer[len - 1] = 0;
  std::string res(buffer);
  free(buffer);
  return res;
}


void SciAdapter::undo() {
  WndProc(SCI_UNDO, 0, 0);
}
void SciAdapter::redo() {
  WndProc(SCI_REDO, 0, 0);
}

void SciAdapter::paste() {
  std::string file = "nicolas_temp_cage";
  std::string command = "xsel --clipboard > " + file;
  system(command.c_str());

  std::ifstream fileToView(file);
  std::string all_text = "";

  if (fileToView) {
    char buf[1000];
    while (fileToView.getline(buf, 1000)) {
      all_text += buf;
      all_text += "\n";
    }
  }

  WndProc(SCI_ADDTEXT, all_text.size(), reinterpret_cast<sptr_t>(all_text.c_str()));
}

void SciAdapter::copy() {
  auto [x, y] = get_cursor_pos();
  auto line_start = WndProc(SCI_POSITIONFROMLINE, y, 0);
  auto line_end = WndProc(SCI_GETLINEENDPOSITION, y, 0);

  std::string line = get_line(y);
  std::string command = "echo \"" + line + "\" | xsel --clipboard";
  system(command.c_str());
}

void SciAdapter::cut() {
  auto [x, y] = get_cursor_pos();
  auto line_start = WndProc(SCI_POSITIONFROMLINE, y, 0);
  auto line_end = WndProc(SCI_GETLINEENDPOSITION, y, 0);
  copy();
  WndProc(SCI_DELETERANGE, line_start, line_end - line_start + 1);
}


void SciAdapter::move_cursor_h(int direction) {
  auto pos = WndProc(SCI_GETCURRENTPOS, 0, 0);
  WndProc(SCI_SETCURRENTPOS, pos + direction, 0);
  WndProc(SCI_SCROLLCARET, 0, 0);
  this->m_prefered_column = get_cursor_pos()[0];
}

void SciAdapter::move_cursor_h_word(int direction) {
  if (direction > 0) {
    KeyCommand(SCI_WORDRIGHT);
  } else {
    KeyCommand(SCI_WORDLEFT);
  }
  this->m_prefered_column = get_cursor_pos()[0];
}

void SciAdapter::move_cursor_v(int direction) {
  auto [_, y] = get_cursor_pos();

  int line_count = WndProc(SCI_GETLINECOUNT, 0, 0);
  int line_width = WndProc(SCI_GETLINE, y, 0);

  y = std::clamp(y + direction, 0, line_count - 1);
  int x = min(this->m_prefered_column, line_width - 1);
  set_cursor_pos({x, y}, 0);
}

void SciAdapter::colorize() {
  WndProc(SCI_COLOURISE, 0, -1);
}

std::string SciAdapter::get_margin_text(int line) {
  char buf[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int len = WndProc(SCI_MARGINGETTEXT, line, reinterpret_cast<sptr_t>(buf));
  buf[9] = 0;
  std::string res((char *)buf);
  return res;
}

// void SciAdapter::new_file() {
// auto text = "Garter, if you're gonna act like a baby,\n you might as well crawl back up
// "
//"your giant __ where you belong.\n\nYou're the ones acting irresponsible "
//"\nso you crawl into my __!";

// WndProc(SCI_SETTEXT, 0, reinterpret_cast<sptr_t>(text));
//// WndProc(SCI_SETCARETSTYLE, 32, 0);

// auto line_count = WndProc(SCI_GETLINECOUNT, 0, 0);
// for (int line = 0; line < line_count; line++) {
// auto len = WndProc(SCI_GETLINE, line, 0);
// auto buffer = (char *)calloc(len + 1, sizeof(char));
// buffer[0] = 0;
// WndProc(SCI_GETLINE, line, reinterpret_cast<sptr_t>(buffer));
// buffer[len] = 0;
// std::string res(buffer);
// free(buffer);
//}
//}


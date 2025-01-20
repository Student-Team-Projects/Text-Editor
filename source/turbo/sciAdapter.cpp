#include "scintilla/include/Scintilla.h"
// #include "scintilla/lexlib/LexerModule.h"
#include <array>
#include <cstdlib>
#include <fstream>

#include <debug.hpp>
#include <algorithm>
#include <cassert>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
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


SciAdapter::SciAdapter() {
  init_scintilla();
}

void SciAdapter::init_scintilla() {
  // WndProc(SCI_SETYCARETPOLICY, CARET_STRICT | CARET_EVEN, 10);
  // WndProc(SCI_SETVIEWEOL, 0, 0);

  // WndProc(SCI_SETMARGINS, 1, 0);
  // WndProc(SCI_SETMARGINWIDTHN, 0, 10);
  WndProc(SCI_SETMARGINTYPEN, 0, SC_MARGIN_NUMBER);
  WndProc(SC_MARGIN_TEXT, 0, 0);
  WndProc(SCI_MARGINSETTEXT, 1, reinterpret_cast<sptr_t>("0"));
  WndProc(SCI_STYLERESETDEFAULT, 0, 0);

  this->m_prefered_column = 0;
  this->m_selection_mode = 0;
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

  WndProc(SCI_STYLESETFORE, 0, 0x0000ff);
  WndProc(SCI_STYLESETBACK, 0, 0xaa0000);
  WndProc(SCI_STYLESETFORE, 117, 0x00ff00);
  WndProc(SCI_STYLESETBACK, 117, 0x00aa00);

  // WndProc(SCI_SETE, SC_ELEMENT_CURET, 0x00ff00);

  WndProc(SCI_SETCARETSTYLE, CARETSTYLE_BLOCK, 0);
  // WndProc(SCI_SETANCHOR, 0, 0);
  // int buf_size = 1 + WndProc(SCI_GETSELTEXT, 0, 0);
  // char *buf = (char *)calloc(buf_size, sizeof(char));
  // WndProc(SCI_GETSELTEXT, 0, reinterpret_cast<sptr_t>(buf));
  // WndProc(SCI_SETSELBACK, true, 20);
  // WndProc(SCI_SETCARETFORE, 0x9900ff, 0);
  // std::cout << " selected_text: " << buf << "-end" << std::endl;
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
  debug("COLOR");
  // TODO when I tell it to color the WHOLE text it vomits colors all over
  WndProc(SCI_COLOURISE, 0, min(all_text.size() - 1, 3000));
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

  this->m_changed_lines.insert(get_cursor_pos()[1]);

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

  this->m_changed_lines.insert(y);
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

  remove_seleted_and_exit();
  char text[2] = {ch, 0};
  WndProc(SCI_ADDTEXT, 1, reinterpret_cast<sptr_t>(text));
  // WndProc(SCI_COLOURISE, 0, -1);
}

void SciAdapter::delete_char() {
  if (this->m_selection_mode) {
    remove_seleted_and_exit();
    return;
  }
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
  remove_seleted_and_exit();
  WndProc(SCI_ADDTEXT, all_text.size(), reinterpret_cast<sptr_t>(all_text.c_str()));
}

void SciAdapter::copy() {
  auto [x, y] = get_cursor_pos();
  auto line_start = WndProc(SCI_POSITIONFROMLINE, y, 0);
  auto line_end = WndProc(SCI_GETLINEENDPOSITION, y, 0);

  std::string line = get_line(y);

  if (this->m_selection_mode == 1) {
    int buf_size = 1 + WndProc(SCI_GETSELTEXT, 0, 0);
    char *buf = (char *)calloc(buf_size, sizeof(char));
    WndProc(SCI_GETSELTEXT, 0, reinterpret_cast<sptr_t>(buf));
    line = std::string(buf);
  }
  std::string command = "echo \"" + line + "\" | xsel --clipboard";
  system(command.c_str());
}

void SciAdapter::cut() {

  auto [x, y] = get_cursor_pos();
  auto line_start = WndProc(SCI_POSITIONFROMLINE, y, 0);
  auto line_end = WndProc(SCI_GETLINEENDPOSITION, y, 0);
  copy();

  if (this->m_selection_mode == 1) {
    remove_seleted_and_exit();
    return;
  }

  WndProc(SCI_DELETERANGE, line_start, line_end - line_start + 1);
}


void SciAdapter::move_cursor_h(int direction) {
  this->m_changed_lines.insert(get_cursor_pos()[1]);
  auto pos = WndProc(SCI_GETCURRENTPOS, 0, 0);
  if (this->m_selection_mode) {
    WndProc(SCI_SETCURRENTPOS, pos + direction, 0);
  } else {
    WndProc(SCI_GOTOPOS, pos + direction, 0);
  }
  WndProc(SCI_SCROLLCARET, 0, 0);
  this->m_prefered_column = get_cursor_pos()[0];
  this->m_changed_lines.insert(get_cursor_pos()[1]);
}

void SciAdapter::move_cursor_h_word(int direction) {
  this->m_changed_lines.insert(get_cursor_pos()[1]);
  if (direction > 0) {
    KeyCommand(SCI_WORDRIGHT);
  } else {
    KeyCommand(SCI_WORDLEFT);
  }
  if (this->m_selection_mode == 0) {
    int pos = WndProc(SCI_GETCURRENTPOS, 0, 0);
    WndProc(SCI_SETSEL, -1, pos);
  }
  this->m_prefered_column = get_cursor_pos()[0];
  this->m_changed_lines.insert(get_cursor_pos()[1]);
}

void SciAdapter::move_cursor_v(int direction) {
  auto [_, y] = get_cursor_pos();
  this->m_changed_lines.insert(y);

  int line_count = WndProc(SCI_GETLINECOUNT, 0, 0);

  y = std::clamp(y + direction, 0, line_count - 1);
  int line_width = WndProc(SCI_GETLINE, y, 0);

  int x = min(this->m_prefered_column, line_width - 1);
  set_cursor_pos({x, y}, 0);

  if (this->m_selection_mode == 0) {
    int pos = WndProc(SCI_GETCURRENTPOS, 0, 0);
    WndProc(SCI_SETSEL, -1, pos);
  }
  this->m_changed_lines.insert(y);
}

void SciAdapter::colorize() {
  // debug("COLORIZE");
  // WndProc(SCI_COLOURISE, 0, -1);

  // WndProc(SCI_SCROLLCARET, 0, 0);
  // WndProc(SCI_SETXOFFSET, 3, 0);
  // int first_vis_line = WndProc(SCI_GETFIRSTVISIBLELINE, 0, 0);
  // WndProc(SCI_LINESCROLL, 0, 1);
  // WndProc(SCI_SETSCROLLWIDTH, 50, 0);
  // int width = WndProc(SCI_GETSCROLLWIDTH, 0, 0);

  // std::cout << "first_vis_line: " << first_vis_line << " width: " << width << std::endl;
}

std::string SciAdapter::get_margin_text(int line) {
  char buf[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int len = WndProc(SCI_MARGINGETTEXT, line, reinterpret_cast<sptr_t>(buf));
  buf[9] = 0;
  std::string res((char *)buf);
  return res;
}

TColorAttr SciAdapter::style_to_color(int style) {
  TColorDesired fg = (int)WndProc(SCI_STYLEGETFORE, style, 0);
  TColorDesired bg = (int)WndProc(SCI_STYLEGETBACK, style, 0);
  return TColorAttr(fg, bg);
}

std::array<int, 2> SciAdapter::selection_range() {
  int start = WndProc(SCI_GETSELECTIONSTART, 0, 0);
  int end = WndProc(SCI_GETSELECTIONEND, 0, 0);
  return {start, end};
}

std::array<std::array<int, 2>, 2> SciAdapter::selection_points() {
  auto start = WndProc(SCI_GETSELECTIONSTART, 0, 0);
  auto end = WndProc(SCI_GETSELECTIONEND, 0, 0);
  int x_start = WndProc(SCI_GETCOLUMN, start, 0);
  int y_start = WndProc(SCI_LINEFROMPOSITION, start, 0);
  int x_end = WndProc(SCI_GETCOLUMN, end, 0);
  int y_end = WndProc(SCI_LINEFROMPOSITION, end, 0);
  return {std::array<int, 2>{x_start, y_start}, {x_end, y_end}};
}

int SciAdapter::toggle_selection_mode() {
  this->m_selection_mode = !this->m_selection_mode;

  if (this->m_selection_mode == 0) {
    auto [pos_start, pos_end] = selection_points();
    for (int i = pos_start[1]; i <= pos_end[1]; i++) {
      this->m_changed_lines.insert(i);
    }
    int pos = WndProc(SCI_GETCURRENTPOS, 0, 0);
    WndProc(SCI_SETSEL, -1, pos);
  }
  return this->m_selection_mode;
}
void SciAdapter::set_selection_mode(int mode) {
  this->m_selection_mode = mode;
  if (this->m_selection_mode == 0) {
    auto [pos_start, pos_end] = selection_points();
    for (int i = pos_start[1]; i <= pos_end[1]; i++) {
      this->m_changed_lines.insert(i);
    }
    int pos = WndProc(SCI_GETCURRENTPOS, 0, 0);
    WndProc(SCI_SETSEL, -1, pos);
  }
}

void SciAdapter::remove_seleted_and_exit() {
  if (this->m_selection_mode) {
    auto [start, end] = selection_range();
    WndProc(SCI_DELETERANGE, start, end - start);
    this->m_selection_mode = 0;
  }
}


void SciAdapter::NotifyParent(SCNotification scn) {
  auto header = scn.nmhdr;
  if (header.code == SCN_MODIFIED) {
    int pos_start = scn.position;
    int line_start = WndProc(SCI_LINEFROMPOSITION, pos_start, 0);

    int len = scn.length;
    if (len == 0) return;
    int line_end = WndProc(SCI_LINEFROMPOSITION, pos_start + len, 0);
    int pos_end = WndProc(SCI_GETLINEENDPOSITION, line_end, 0);
    debug(line_start, line_end);

    if (scn.modificationType != SC_MOD_CHANGESTYLE) {
      WndProc(SCI_COLOURISE, pos_start, pos_end);
    }

    for (int i = line_start; i <= line_end; i++) {
      m_changed_lines.insert(i);
    }
    if (scn.linesAdded != -0) {
      // TODO remove 100
      for (int i = line_end; i <= line_end + 100; i++) {
        m_changed_lines.insert(i);
      }
    }
  }
}

std::set<int> SciAdapter::get_changed_lines() {
  auto res = m_changed_lines;
  m_changed_lines.clear();
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


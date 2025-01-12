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
extern Scintilla::LexerModule lmCPP;
extern Scintilla::LexerModule lmPython;
extern Scintilla::LexerModule lmAsm;
extern Scintilla::LexerModule lmBash;
extern Scintilla::LexerModule lmNull;

std::map<std::string, int> extension2lexerId = {
    {".cpp", SCLEX_CPP},  {".c", SCLEX_CPP},   {".h", SCLEX_CPP},   {".hpp", SCLEX_CPP},
    {".cc", SCLEX_CPP},   {".asm", SCLEX_ASM}, {".s", SCLEX_ASM},   {".py", SCLEX_PYTHON},
    {".txt", SCLEX_NULL}, {"", SCLEX_NULL},    {".sh", SCLEX_BASH},
};
std::vector<Scintilla::LexerModule *> lexers = {&lmCPP, &lmPython, &lmNull, &lmAsm,
                                                &lmBash};

Scintilla::LexerModule *get_lexer(std::string filename) {
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

  // cell s[5] = {
  //{'H', 0}, {'e', 1}, {'l', 2}, {'l', 3}, {'o', 4},
  //};
  // WndProc(SCI_ADDSTYLEDTEXT, 10, reinterpret_cast<sptr_t>(s));

  let lexer = get_lexer(filename)->Create();
  // const LexerModule *lm = Scintilla::Catalogue::Find(SCLEX_PYTHON);
  // CreateLexer int old_lex = WndProc(SCI_GETLEXER, 0, 0);
  // int new_lex = 0;

  // for (auto i = 0; i < 10; i++) {
  WndProc(SCI_SETILEXER, 0, reinterpret_cast<sptr_t>(lexer));
  // new_lex = WndProc(SCI_GETLEXER, 0, 0);
  // if (new_lex != old_lex) {
  // break;
  // }
  // }
  auto ext = std::filesystem::path(filename).extension().string();

  // std::string msg = "ext: " + ext;

  // WndProc(SCI_ADDTEXT, msg.size(), reinterpret_cast<sptr_t>(msg.c_str()));
  WndProc(SCI_COLOURISE, 0, -1);
  return;
  // for (int i = 0; i < lineCount; i++)
  //// for each line:
  //{
  // TDrawBuffer b;
  // b.moveChar(0, ' ', color, size.x);
  //// fill line buffer with spaces
  // int j = delta.y + i; // delta is scroller offset
  // if (lines[j]) {
  // char s[1000];
  // if (delta.x > (int)strlen(lines[j]))
  // s[0] = EOS;
  // else {
  // strncpy(s, lines[j] + delta.x, size.x);
  // s[size.x] = EOS;
  //}
  // b.moveStr(0, s, color);
  //}
  // writeLine(0, i, size.x, 1, b);
  //}
}

void NicolasCage::undo() { WndProc(SCI_UNDO, 0, 0); }
void NicolasCage::redo() { WndProc(SCI_REDO, 0, 0); }

void NicolasCage::paste() { WndProc(SCI_PASTE, 0, 0); }
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
  return res;
}

std::string NicolasCage::get_line(int line) {
  let len = WndProc(SCI_GETLINE, line, 0);
  let buffer = (char *)calloc(len, sizeof(char));
  WndProc(SCI_GETLINE, line, reinterpret_cast<sptr_t>(buffer));
  // buffer[len] = 0;
  if (buffer[len - 1] == '\n') buffer[len - 1] = 0;
  std::string res(buffer);
  free(buffer);
  return res;
}

void NicolasCage::move_caret_h(int amount) {
  let pos = WndProc(SCI_GETCURRENTPOS, 0, 0);
  WndProc(SCI_SETCURRENTPOS, pos + amount, 0);
}
// if (amount > 0)
// WndProc(SCK_RIGHT, 0, 0);
// else
// WndProc(SCK_LEFT, 0, 0);

void NicolasCage::move_caret_v(int amount) {
  auto [x, y] = get_carret_pos();
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
}
void NicolasCage::delete_char() {
  let pos = WndProc(SCI_GETCURRENTPOS, 0, 0);
  WndProc(SCI_DELETERANGE, pos - 1, 1);
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

int NicolasCage::get_line_count() { return WndProc(SCI_GETLINECOUNT, 0, 0); }

// std::string NicolasCage::act() {
// auto text = "Garter, if you're gonna act like a baby,\n you might as well crawl back up
// " "your giant __ where you belong.\n\nYou're the ones acting irresponsible "
//"\nso you crawl into my __!";
// WndProc(SCI_SETTEXT, 0, reinterpret_cast<sptr_t>(text));

// let line_count = WndProc(SCI_GETLINECOUNT, 0, 0);
// for (int line = 0; line < line_count; line++) {
// let len = WndProc(SCI_GETLINE, line, 0);
// let buffer = (char *)calloc(len + 1, sizeof(char));
// WndProc(SCI_GETLINE, line, reinterpret_cast<sptr_t>(buffer));
// buffer[len] = 0;
// std::string res(buffer);
// free(buffer);
//}
// auto line = 2;

// int len = WndProc(SCI_GETLINE, line, 0);
// let buffer = (char *)calloc(len + 1, sizeof(char));
// WndProc(SCI_GETLINE, line, reinterpret_cast<sptr_t>(buffer));
// buffer[len] = 0;
// std::string res(buffer);
//// res = res.substr(0, len);
// return res;
//}

// std::string foo() { return "420"; }

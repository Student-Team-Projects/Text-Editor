
#include "scintilla/include/Scintilla.h"
#include <turbo/nicolas_cage.h>
#define let auto

void NicolasCage::load_file(const char *filename) {}

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

void NicolasCage::insert_char(char ch) {
  char text[2] = {ch, 0};
  WndProc(SCI_ADDTEXT, 1, reinterpret_cast<sptr_t>(text));
  WndProc(SCI_MOVECARETINSIDEVIEW, 0, 0);
}
void NicolasCage::delete_char() {
  let pos = WndProc(SCI_GETCURRENTPOS, 0, 0);
  WndProc(SCI_DELETERANGE, pos - 1, 1);
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

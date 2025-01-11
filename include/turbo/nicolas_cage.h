#pragma once
#include "scintilla/include/Scintilla.h"
#include "scintilla/internals.h"
#include "scintilla/lexlib/LexerModule.h"
#include "scintilla/src/ScintillaBase.h"
#include <array>
#include <iostream>
#include <type_traits>

struct cell {
  char character;
  unsigned char style;
};
/*Scintilla::LexerModule lmCPP(0, nullptr);*/

namespace Scintilla {

class NicolasCage : public ScintillaBase {
  public:
  void SetVerticalScrollPos() override {}
  void SetHorizontalScrollPos() override {}
  bool ModifyScrollBars(Sci::Line nMax, Sci::Line nPage) override { return false; }
  void Copy() override {
    // std::cout << "copy called" << std::endl;
  }
  void Paste() override { // std::cout << "paste called" << std::endl;
  }
  void ClaimSelection() override {}
  void NotifyChange() override {}
  void NotifyParent(SCNotification scn) override {}
  void CopyToClipboard(const SelectionText &selectedText) override {
    // TODO clipboard logic
    /*std::cout << "copy to clipboard called" << std::endl;*/
  }
  void SetMouseCapture(bool on) override {}
  bool HaveMouseCapture() override { return false; }
  sptr_t DefWndProc(unsigned int iMessage, uptr_t wParam, sptr_t lParam) override {
    return 0;
  }
  void CreateCallTipWindow(PRectangle rc) override {}
  void AddToPopUp(const char *label, int cmd = 0, bool enabled = true) override {}
  /// below ade not needed to compile but are needed to link WTF
  void NotifyModifyAttempt(Scintilla::Document *a, void *b) override {
    ScintillaBase::NotifyModifyAttempt(a, b);
  }
  void NotifySavePoint(Scintilla::Document *a, void *b, bool c) override {
    ScintillaBase::NotifySavePoint(a, b, c);
  }
  void NotifyModified(Scintilla::Document *a, Scintilla::DocModification b,
                      void *c) override {
    ScintillaBase::NotifyModified(a, b, c);
  }
  void NotifyDeleted(Scintilla::Document *a, void *b) noexcept override {
    ScintillaBase::NotifyDeleted(a, b);
  }
  void NotifyStyleNeeded(Scintilla::Document *a, void *b, long c) override {
    ScintillaBase::NotifyStyleNeeded(a, b, c);
  }
  void NotifyLexerChanged(Scintilla::Document *a, void *b) override {
    ScintillaBase::NotifyLexerChanged(a, b);
  }
  void NotifyErrorOccurred(Scintilla::Document *a, void *b, int c) override {
    ScintillaBase::NotifyErrorOccurred(a, b, c);
  }
  void ListNotify(Scintilla::ListBoxEvent *a) override { ScintillaBase::ListNotify(a); }
  /**/
  // WTF 2.0
  bool FineTickerRunning(TickReason reason) override { return false; }
  void FineTickerStart(TickReason reason, int millis, int tolerance) override {}
  void FineTickerCancel(TickReason reason) override {}

  // THer following is written by human being (matra)
  public:
  // additions:
  void load_file(std::string);
  void new_file();
  // Write a char `ch` into the document at the current position
  void insert_char(char ch);
  // Returns the line with number `line` in the document
  std::string get_line(int line);
  // Removes one character from the current position
  void delete_char();
  // Returns the number of lines in the document
  int get_line_count();
  // Moves the cursor horizontally by `amount` characters
  void move_caret_h(int amount);
  // Moves the cursor vertically by `amount` lines
  void move_caret_v(int amount);
  // Returns the current position of the cursor
  std::array<int, 2> get_carret_pos();
  // Returns the character at the position `pos`
  char get_char_at(std::array<int, 2>);

  void paste();
  void copy();
  void cut();

  void undo();
  void redo();
  std::vector<cell> get_styled_line(int);
};
} // namespace Scintilla

using NicolasCage = Scintilla::NicolasCage;


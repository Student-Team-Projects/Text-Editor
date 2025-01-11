#pragma once
#include "scintilla/include/Scintilla.h"
#include "scintilla/internals.h"
#include <array>
#include <iostream>
#include <type_traits>

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
  void NotifyModifyAttempt(Scintilla::Document *, void *) override {}
  void NotifySavePoint(Scintilla::Document *, void *, bool) override {}
  void NotifyModified(Scintilla::Document *, Scintilla::DocModification, void *) override {}
  void NotifyDeleted(Scintilla::Document *, void *) noexcept override {}
  void NotifyStyleNeeded(Scintilla::Document *, void *, long) override {}
  void NotifyLexerChanged(Scintilla::Document *, void *) override {}
  void NotifyErrorOccurred(Scintilla::Document *, void *, int) override {}
  void ListNotify(Scintilla::ListBoxEvent *) override {}
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
};
} // namespace Scintilla

using NicolasCage = Scintilla::NicolasCage;


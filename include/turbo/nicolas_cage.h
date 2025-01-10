#pragma once
#include "scintilla/include/Scintilla.h"
#include "scintilla/internals.h"
#include <array>
#include <type_traits>

namespace Scintilla {

class NicolasCage : public ScintillaBase {
  public:
  void SetVerticalScrollPos() override {}
  void SetHorizontalScrollPos() override {}
  bool ModifyScrollBars(Sci::Line nMax, Sci::Line nPage) override { return false; }
  void Copy() override {}
  void Paste() override {}
  void ClaimSelection() override {}
  void NotifyChange() override {}
  void NotifyParent(SCNotification scn) override {}
  void CopyToClipboard(const SelectionText &selectedText) override {}
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

  // WTF 2.0
  bool FineTickerRunning(TickReason reason) override { return false; }
  void FineTickerStart(TickReason reason, int millis, int tolerance) override {}
  void FineTickerCancel(TickReason reason) override {}

  public:
  // additions:
  void load_file(const char *filename);
  void new_file();
  void insert_char(char ch);
  std::string get_line(int line);
  void delete_char();
  int get_line_count();
  void move_caret_h(int amount);
  void move_caret_v(int amount);
  std::array<int, 2> get_carret_pos();
};
} // namespace Scintilla

using NicolasCage = Scintilla::NicolasCage;


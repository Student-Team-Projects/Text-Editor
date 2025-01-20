#pragma once
#include "scintilla/include/Scintilla.h"
#include "scintilla/internals.h"
#include "scintilla/lexlib/LexerModule.h"
#include "scintilla/src/ScintillaBase.h"
#include <array>
#include <filesystem>
#include <iostream>
#include <set>

// represeents a character with style(color)
struct cell {
  char character;
  unsigned char style;
};

namespace Scintilla {

class SciAdapter : private ScintillaBase {

  using path = std::filesystem::path;


  private:
  void SetVerticalScrollPos() override {}
  void SetHorizontalScrollPos() override {}
  bool ModifyScrollBars(Sci::Line nMax, Sci::Line nPage) override {
    return false;
  }
  void Copy() override {}
  void Paste() override {}
  void ClaimSelection() override {}
  void NotifyChange() override {}
  void NotifyParent(SCNotification scn) override;
  void CopyToClipboard(const SelectionText &selectedText) override {}
  void SetMouseCapture(bool on) override {}
  bool HaveMouseCapture() override {
    return false;
  }
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
  void ListNotify(Scintilla::ListBoxEvent *a) override {
    ScintillaBase::ListNotify(a);
  }

  // WTF 2.0
  bool FineTickerRunning(TickReason reason) override {
    return false;
  }
  void FineTickerStart(TickReason reason, int millis, int tolerance) override {}
  void FineTickerCancel(TickReason reason) override {}


  // THer following is written by human being (matra)

  public:
  SciAdapter();
  // Reads the content of the file with path `filename` and loads it into the document
  void load_file(const path &filename);

  // Sets the position of the cursor to `pos`. When the `override_column`  is true, the
  // `m_prefered_column` will be set to the column of the cursor
  void set_cursor_pos(std::array<int, 2>, bool override_column = true);
  // Returns the current position of the cursor
  [[nodiscard]] std::array<int, 2> get_cursor_pos();

  // Write a char `ch` into the document at the current position
  void insert_char(char ch);
  // Removes one character from the current position
  void delete_char();
  // Returns the character at the position `pos`
  [[nodiscard]] char get_char_at(std::array<int, 2>);

  // reurnst the index of the first line to be print on the screen
  [[nodiscard]] int get_first_line_idx();
  // Returns the number of lines in the document
  [[nodiscard]] int get_line_count();
  // Returns the line with number `line` in the document
  [[nodiscard]] std::string get_line(int line);
  // The same as get line but each character has a style
  [[nodiscard]] std::vector<cell> get_styled_line(int);

  void paste();
  void copy();
  void cut();
  void undo();
  void redo();

  // Moves the cursor horizontally to the left if direction -1 to the right
  // otherwise if 1
  void move_cursor_h(int direction);
  // Moves the cursor horizontally, by words. If `direction` is positive, it moves to the
  // right
  void move_cursor_h_word(int direction);
  // Moves the cursor vertically by. Up if direction is -1, down if 1
  void move_cursor_v(int direction);

  // Returns the text of the margin of the line `line`
  [[nodiscard]] std::string get_margin_text(int line);

  // Toggles the selection mode. On means that any cursor move will update the selection,
  // Off means there currently is no selection. Also returns the set mode
  int toggle_selection_mode();
  // Similar to `toggle_selection_mode` but sets the selection mode
  void set_selection_mode(int mode);
  // Returns pair of points - beginning and end of the selection
  [[nodiscard]] std::array<std::array<int, 2>, 2> selection_points();


  // updates the lexer
  void colorize();
  // Given scintilla style index returns turobo TColorAttr representing this style
  [[nodiscard]] TColorAttr style_to_color(int style);

  std::set<int> get_changed_lines();

  static std::map<std::string, int> extension_to_lexerId;
  static const std::vector<const Scintilla::LexerModule *> lexers;


  private:
  // Removes the selected text and exits the selection mode
  void remove_seleted_and_exit();
  // initializes all the settrings
  void init_scintilla();
  // given a file name, returns the lexer that should be used to colorize the file
  [[nodiscard]] const Scintilla::LexerModule *get_lexer(const path &filename);
  // Similar to `selection_points` but returns pair of scintilla positions
  [[nodiscard]] std::array<int, 2> selection_range();

  // The column that the cursor before when moved to the line with less characters. When it
  // moves to a line with more characters, it will be the same as the column of the cursor
  int m_prefered_column;
  int m_selection_mode;
  std::set<int> m_changed_lines;
};
} // namespace Scintilla

using SciAdapter = Scintilla::SciAdapter;


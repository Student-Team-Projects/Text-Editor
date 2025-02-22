#pragma once

#define Uses_TKeys
#define Uses_TApplication
#define Uses_TEvent
#define Uses_TRect
#define Uses_TDialog
#define Uses_TStaticText
#define Uses_TButton
#define Uses_TMenuBar
#define Uses_TSubMenu
#define Uses_TApplication
#define Uses_TEditWindow
#define Uses_TDeskTop
#define Uses_TFileDialog
#define Uses_THistory
#define Uses_TInputLine
#define Uses_TLabel
#define Uses_TCheckBoxes
#define Uses_TSItem
#define Uses_TRect
#define Uses_TEditor
#define Uses_TFileEditor
#define Uses_TFileDialog
#define Uses_TChDirDialog
#define Uses_TMenuItem
#define Uses_MsgBox
#define Uses_TStatusLine
#define Uses_TStatusItem
#define Uses_TStatusDef
#define Uses_TDeskTop
#define Uses_TScrollBar
#include <turbo/clockView.hpp>
#include <turbo/explorerWindow.hpp>
#include <tvision/tv.h>
#include <optional>

const int cm_chdir = 102;
const int cm_toggle_tree = 103;

class t_hello_app : public TApplication {
  public:
  t_hello_app(int argc, char **argv);
  static t_hello_app *app;

  auto handleEvent(TEvent &event) -> void override;
  auto idle() -> void override;
  static auto initMenuBar(TRect rect) -> TMenuBar *;
  static auto initStatusLine(TRect rect) -> TStatusLine *;

  auto newEditor(std::optional<char *> path) -> void;

  auto fileOpen() -> void;
  auto fileNew() -> void;
  auto chdir() -> void;

  static auto createFindDialog() -> TDialog *;
  static auto createReplaceDialog() -> TDialog *;

  private:
  static void greeting_box();
  t_clock_view *m_clock;
  t_explorer_window *m_explorer;
  tc_file_watcher m_editor_watcher;
};

auto mujDialog(int dialog, ...) -> ushort;

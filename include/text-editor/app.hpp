
// Copyright (c) 2025 Piotr Białek
// Copyright (c) 2025 Mateusz Rajs
// Copyright (c) 2025 Mikołaj Rams
// Copyright (c) 2025 Antoni Długosz
//
// Licensed under the MIT license

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
#include <optional>
#include <text-editor/clockView.hpp>
#include <text-editor/explorerWindow.hpp>
#include <tvision/tv.h>

constexpr const int cm_chdir = 102;
constexpr const int cm_toggle_tree = 103;
constexpr const int cm_open_guy = 104;
constexpr const int open_help = 105;
constexpr const int cm_open_term = 106;

// Represents the whole thing
class App : public TApplication {
  public:
  App(int argc, char **argv);
  static App *app;

  auto handleEvent(TEvent &event) -> void override;
  auto idle() -> void override;
  static auto initMenuBar(TRect rect) -> TMenuBar *;
  static auto initStatusLine(TRect rect) -> TStatusLine *;

  // Spawns widow that allows editing of a file. if `path` is enpty, opens new file
  // othrewise opens file at `path`
  auto newEditor(std::optional<char *> path) -> void;

  // Open a popup dialog to open a existing file
  auto fileOpen() -> void;
  // Open a empty file
  auto fileNew() -> void;
  // Changes the root of the file explorer
  auto chdir() -> void;

  // Self explanatory
  static auto createHelpDialog() -> TDialog *;
  static auto createFindDialog() -> TDialog *;
  static auto createReplaceDialog() -> TDialog *;

  private:
  t_clock_view *m_clock;
  t_explorer_window *m_explorer;
  tc_file_watcher m_editor_watcher;
};


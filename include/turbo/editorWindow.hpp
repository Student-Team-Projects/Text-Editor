#pragma once

#include <filesystem>
#define Uses_TApplication
#define Uses_TOutline
#define Uses_TWindow

#include <turbo-core/fileWatcher.hpp>
#include <tvision/tv.h>
#include <turbo/sciAdapter.hpp>


class editorView : public TScroller {

  using path = std::filesystem::path;

  public:
  editorView(const TRect &bounds, TScrollBar *aHScrollBar, TScrollBar *aVScrollBar);

  void draw() override;
  void m_draw();

  void handleEvent(TEvent &event) override;
  // TPalette &getPalette() const override;

  // Openes a file with path `filename`, if it is empty, opens a empty file
  void open_file(path const &filename);

  // The same as draw but only one line
  void draw_line(int line);
  // sets the first visible line so that cursor is visible on the screen
  void scroll_to_cursor();

  // private:
  SciAdapter *m_adapter;
  TPalette m_palette;
};


class editorWindow : public TWindow {

  using path = std::filesystem::path;

  public:
  editorWindow(const TRect &bounds, const path &filename);
  void handleEvent(TEvent &event) override;

  private:
  editorView *m_interior;
};

#pragma once

#include <string>
#define Uses_TApplication
#define Uses_TOutline
#define Uses_TWindow

#include <turbo-core/fileWatcher.hpp>
#include <tvision/tv.h>
#include <turbo/nicolas_cage.h>

class editor_interior : public TScroller {

  public:
  editor_interior(const TRect &bounds, TScrollBar *aHScrollBar,
                  TScrollBar *aVScrollBar); // constructor
  virtual void draw() override;             // override TView::draw
  virtual void handleEvent(TEvent &event) override;
  void open_file(std::string);
  std::string m_text;
  NicolasCage *m_nick;
};

class editor : public TWindow {
  public:
  editor(const TRect &bounds, std::string text);
  virtual void handleEvent(TEvent &event) override;

  private:
  editor_interior *m_interior;
};

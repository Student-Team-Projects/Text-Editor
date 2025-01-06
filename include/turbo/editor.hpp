#pragma once

#include <string>
#define Uses_TApplication
#define Uses_TOutline
#define Uses_TWindow

#include <turbo-core/fileWatcher.hpp>
#include <tvision/tv.h>

class editor_interior : public TScroller {

  public:
  editor_interior(const TRect &bounds, TScrollBar *aHScrollBar,
                  TScrollBar *aVScrollBar); // constructor
  virtual void draw();                      // override TView::draw
  std::string m_text;
};

class editor : public TWindow {
  public:
  editor(const TRect &bounds, std::string text);

  private:
  editor_interior *m_interior;
};

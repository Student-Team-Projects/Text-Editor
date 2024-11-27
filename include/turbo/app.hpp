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
#define Uses_TMenuItem
#define Uses_TStatusLine
#define Uses_TStatusItem
#define Uses_TStatusDef
#define Uses_TDeskTop
#include <turbo/clockView.hpp>
#include <tvision/tv.h>

const int greet_them_cmd = 100;

class t_hello_app : public TApplication {
  public:
  t_hello_app();

  void handleEvent(TEvent &event) override;
  void idle() override;
  static auto initMenuBar(TRect rect) -> TMenuBar *;
  static auto initStatusLine(TRect rect) -> TStatusLine *;

  private:
  static void greeting_box();
  t_clock_view *m_clock;
};

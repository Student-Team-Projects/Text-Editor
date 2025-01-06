#pragma once

#include "turbo/terminal.hpp"
#include "tvterm/termwnd.h"
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
#define Uses_TScrollBar
#include <turbo/clockView.hpp>
#include <turbo/explorerWindow.hpp>
#include <tvision/tv.h>
#include <tvterm/consts.h>

const int greet_them_cmd = 100;
const int cm_toggle_tree = 101;
const int cm_toggle_term = 102;

//terminal commands :P



const ushort cmCheckTerminalUpdates = 203;
const ushort cmTerminalUpdated      = 204;
const ushort cmGrabInput            = 205;
const ushort cmReleaseInput         = 206;
const ushort hcInputGrabbed         = 207;

class t_hello_app : public TApplication {
  public:
  t_hello_app();
  static t_hello_app *app;

  auto handleEvent(TEvent &event) -> void override;
  auto idle() -> void override;
  static auto initMenuBar(TRect rect) -> TMenuBar *;
  static auto initStatusLine(TRect rect) -> TStatusLine *;

  private:
  static void greeting_box();
  t_clock_view *m_clock;
  t_explorer_window *m_explorer;
  tvterm::BasicTerminalWindow *m_term;
};

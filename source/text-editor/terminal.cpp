
// Copyright (c) 2025 Piotr Białek
// Copyright (c) 2025 Mateusz Rajs
// Copyright (c) 2025 Mikołaj Rams
// Copyright (c) 2025 Antoni Długosz
//
// Licensed under the MIT license

#include <text-editor/clockView.hpp>
#include <text-editor/terminal.hpp>
#include <tvterm/consts.h>
#include <tvterm/termctrl.h>
#include <tvterm/termwnd.h>
#include <tvterm/vtermemu.h>

static void onTermError(const char *reason) { exit(1); }

const tvterm::TVTermConstants term_coms = {
    cmCheckTerminalUpdates, cmTerminalUpdated, cmGrabInput, cmReleaseInput,
    7 // hcInputGrabbed,
};

void term_idle(TApplication *app) {
  message(app, evBroadcast, cmCheckTerminalUpdates, nullptr);
}

TRect default_bounds(TRect rect) {
  const int default_h = 12;

  if (rect.b.y - rect.a.y > default_h) {
    rect.a.y = rect.b.y - default_h;
  }

  // this is to match the explorer;
  if (rect.b.x > 22) {
    rect.a.x = std::min(std::max(rect.b.x - 82, 22), 30);
  }

  return rect;
}

tvterm::TerminalController *get_ctrl(const TRect &bounds) {
  using namespace tvterm;
  VTermEmulatorFactory factory;
  return TerminalController::create(BasicTerminalWindow::viewSize(bounds), factory,
                                    onTermError);
}

t_terminal::t_terminal(const TRect &bounds)
    : TWindowInit(&BasicTerminalWindow::initFrame),
      tvterm::BasicTerminalWindow(bounds, *get_ctrl(bounds), term_coms) {}

void t_terminal::handleEvent(TEvent &ev) {
  tvterm::BasicTerminalWindow::handleEvent(ev);
  if (isDisconnected() && !closing) {
    closing = true;
    ev.what = evCommand;
    ev.message.command = cmClose;
    ev.message.infoPtr = this;
    putEvent(ev);
    clearEvent(ev);
  }
}


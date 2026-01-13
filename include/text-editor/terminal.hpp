
// Copyright (c) 2025 Piotr Białek
// Copyright (c) 2025 Mateusz Rajs
// Copyright (c) 2025 Mikołaj Rams
// Copyright (c) 2025 Antoni Długosz
//
// Licensed under the MIT license

#pragma once

#define Uses_TRect
#define Uses_TApplication
#include <tvision/tv.h>
#include <tvterm/termctrl.h>
#include <tvterm/termwnd.h>
#include <tvterm/vtermemu.h>

constexpr const ushort cmCheckTerminalUpdates = 203;
constexpr const ushort cmTerminalUpdated = 204;
constexpr const ushort cmGrabInput = 205;
constexpr const ushort cmReleaseInput = 206;
constexpr const ushort hcInputGrabbed = 207;

void term_idle(TApplication *app);
TRect default_bounds(TRect deskTop_extent);

class t_terminal : public tvterm::BasicTerminalWindow {

  public:
  explicit t_terminal(const TRect &bounds);
  void handleEvent(TEvent &ev) override;
};


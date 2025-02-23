
// Copyright (c) 2025 Piotr Białek
// Copyright (c) 2025 Mateusz Rajs
// Copyright (c) 2025 Mikołaj Rams
// Copyright (c) 2025 Antoni Długosz
//
// Licensed under the MIT license

#pragma once

#define Uses_TView
#include <string>
#include <tvision/tv.h>

// Represents a clock display in the app
class t_clock_view : public TView {
  public:
  static constexpr std::string_view time_format = "%H:%M:%S";
  static const int time_size = (2 + 1) + (2 + 1) + (2) + 1;

  explicit t_clock_view(TRect &rect);
  auto draw() -> void override;
  auto update() -> void;

  private:
  std::string m_last_time;
  std::string m_cur_time;
};

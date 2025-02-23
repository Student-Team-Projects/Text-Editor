
// Copyright (c) 2025 Piotr Białek
// Copyright (c) 2025 Mateusz Rajs
// Copyright (c) 2025 Mikołaj Rams
// Copyright (c) 2025 Antoni Długosz
//
// Licensed under the MIT license

#include <turbo/app.hpp>
#include <tvision/tv.h>

struct EditWindow : TEditWindow {

  EditWindow(const TRect &rect, TStringView view, int val) noexcept;

  void handleEvent(TEvent &event) override;

  private:
  // last modification this editor made to the file
  time_t open_time;
};


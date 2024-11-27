#pragma once

#define Uses_TView
#include <string>
#include <tvision/tv.h>

class t_clock_view : public TView {
  public:
  static constexpr std::string_view time_format = "%H:%M:%S";
  static const int time_size = (2 + 1) + (2 + 1) + (2) + 1;

  explicit t_clock_view(TRect &rect);
  void draw() override;
  virtual void update();

  private:
  std::string m_last_time;
  std::string m_cur_time;
};

#pragma once

#define Uses_TView
#include <string>
#include <tvision/tv.h>

class t_clock_view : public TView {
  public:
  static const int time_size = 9;

  explicit t_clock_view(TRect &rect);
  void draw() override;
  virtual void update();

  private:
  std::string m_last_time;
  std::string m_cur_time;
};

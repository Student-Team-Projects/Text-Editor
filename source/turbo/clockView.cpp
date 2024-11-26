#include "clockView.hpp"
#include <ctime>
#include <string>

t_clock_view::t_clock_view(TRect &rect) : TView(rect) {
  m_last_time = std::string(t_clock_view::time_size, ' ');
  m_cur_time = std::string(t_clock_view::time_size, ' ');
}

void t_clock_view::draw() {
  TDrawBuffer buf;
  uchar col = getColor(2);

  buf.moveChar(0, ' ', col, size.x);
  buf.moveStr(0, m_cur_time, col);
  writeLine(0, 0, size.x, 1, buf);
}

void t_clock_view::update() {
  time_t cur_t = time(0);
  char *date = ctime(&cur_t);

  std::string date_str(date);
  const int time_start = 11;
  m_cur_time = date_str.substr(time_start, t_clock_view::time_size);

  if (m_last_time != m_cur_time) {
    drawView();
    m_last_time = m_cur_time;
  }
}
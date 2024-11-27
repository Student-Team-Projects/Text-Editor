#include <cassert>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <string>
#include <turbo/clockView.hpp>

t_clock_view::t_clock_view(TRect &rect) : TView(rect) {
  m_last_time = std::string(t_clock_view::time_size, ' ');
  m_cur_time = std::string(t_clock_view::time_size, ' ');
}

void t_clock_view::draw() {
  TDrawBuffer buf;
  auto col = getColor(2);

  buf.moveChar(0, ' ', col, static_cast<uint16_t>(size.x));
  buf.moveStr(0, m_cur_time, col);
  writeLine(0, 0, static_cast<int16_t>(size.x), 1, buf);
}

void t_clock_view::update() {
  auto now = std::chrono::system_clock::now();
  auto cur_t = std::chrono::system_clock::to_time_t(now);
  auto *ptm = std::localtime(&cur_t);

  auto result =
      std::strftime(m_cur_time.data(), t_clock_view::time_size, time_format.data(), ptm);
  assert(result == t_clock_view::time_size - 1);

  if (m_last_time != m_cur_time) {
    drawView();
    m_last_time = m_cur_time;
  }
}
#include <algorithm>
#include <cassert>
#include <cstring>
// #include <debug.hpp>
// #include <efsw/efsw.hpp>
#include <filesystem>
#include <fstream>
#include <functional>
#include <string>
#include <turbo/app.hpp>
#include <turbo/editor.hpp>
#include <turbo/explorerWindow.hpp>
#include <tvision/tv.h>
#include <tvision/util.h>
#include <vector>
#include <iostream>

#include <turbo/nicolas_cage.h>
#define Uses_IfStreamGetLine

// TPalette &editor_interior::getPalette() const {
// auto p = m_palette;
// return p;
//}
editor::editor(const TRect &bounds, std::string text)
    : TWindowInit(&editor::initFrame), TWindow(bounds, "EDIROT", wnNoNumber) {
  // auto *hsb = standardScrollBar(sbHorizontal);
  // auto *vsb = standardScrollBar(sbVertical);
  // TView::
  // insert("AAA");
  TScrollBar *vScrollBar = standardScrollBar(sbVertical | sbHandleKeyboard);
  TScrollBar *hScrollBar = standardScrollBar(sbHorizontal | sbHandleKeyboard);
  // vScrollBar->setRange(0, 400);
  // vScrollBar->setValue(100);
  //  vScrollBar->setStep(1, 1);
  TRect r = getClipRect(); // get exposed view bounds
  r.grow(-1, -1);          // shrink to fit inside window frame
  m_interior = new editor_interior(r, hScrollBar, vScrollBar);
  m_interior->open_file(text);
  // m_interior->m_text = text;

  insert(m_interior);
}
void editor::handleEvent(TEvent &event) {
  if (event.what == evKeyDown) {

    // std::cout << "editor::handleEvent: " << event.keyDown.keyCode << std::endl;
    if (event.keyDown.keyCode == 7181) {
      // 7181
      m_interior->m_nick->insert_char('\n');
    }
    if (event.keyDown.keyCode == kbBack) {
      m_interior->m_nick->delete_char();
    }
    if (event.keyDown.keyCode == kbLeft) {
      m_interior->m_nick->move_caret_h(-1);
    }
    if (event.keyDown.keyCode == kbRight) {
      m_interior->m_nick->move_caret_h(1);
    }
    if (event.keyDown.keyCode == kbUp) {
      m_interior->m_nick->move_caret_v(-1);
    }
    if (event.keyDown.keyCode == kbDown) {
      m_interior->m_nick->move_caret_v(1);
    }
    for (auto ch : event.keyDown.getText()) {
      if (isprint(ch)) m_interior->m_nick->insert_char(ch);
      // if (ch == '\n') m_interior->m_nick->insert_char('\n');
    }
  }
  // std::cout << "editor::handleEvent: " << event.what << std::endl;
  if (m_interior->m_nick->get_carret_pos()[1] > size.y) {
    m_interior->delta.y++;
    // m_nick->handleEvent(event);
  }
  m_interior->draw();
  m_interior->handleEvent(event);
}

editor_interior::editor_interior(const TRect &bounds, TScrollBar *aHScrollBar,
                                 TScrollBar *aVScrollBar)
    : TScroller(bounds, aHScrollBar, aVScrollBar), m_palette((char *)0, 0) {
  growMode = gfGrowHiX | gfGrowHiY;
  options = options | ofFramed;
  m_nick = new NicolasCage();
  TColorAttr color(27, 25);
  TColorAttr color2[5] = {3, color, color, color, color};
  TPalette p(color2);
  m_palette = p;
  // m_nick->load_file(m_text);
  //  m_nick->new_file();
  //   setLimit(maxLineLength, maxLines);
}

void editor_interior::open_file(std::string filename) {
  if (filename.empty()) {
    m_nick->new_file();
    return;
  }
  m_text = filename;
  m_nick->load_file(filename);
}

void editor_interior::handleEvent(TEvent &event) {
  if (event.what == evKeyDown) {
    if (event.keyDown.keyCode == kbCtrlV) {
      m_nick->paste();
    }
    if (event.keyDown.keyCode == kbCtrlC) {
      m_nick->copy();
    }
    if (event.keyDown.keyCode == kbCtrlX) {
      m_nick->cut();
    }
    if (event.keyDown.keyCode == kbCtrlZ) {
      m_nick->undo();
    }
    if (event.keyDown.keyCode == kbCtrlY) {
      m_nick->redo();
    }
  }
  draw();

  // std::cout << "editor_interior::handleEvent" << std::endl;
}

void editor_interior::draw() // modified for scroller
{
  // return;
  constexpr int styple_size = 7;
  ushort style[styple_size] = {0, 1, 3, 5, 31, 32, 33};
  // scrollDraw();
  //
  auto line_count = m_nick->get_line_count();
  // std::cout << "size: " << size.x << " " << size.y << std::endl;
  // setLimit(100, line_count);
  auto [x, y] = m_nick->get_carret_pos();
  // scrollTo(x, y);
  // while (y >= delta.y + size.y) {
  // delta.y++;
  //}
  // while (y < delta.y) {
  // delta.y--;
  //}
  int first_line = m_nick->get_first_line();
  for (int h = 0; h < size.y; h++) {
    TDrawBuffer b;
    std::vector<cell> line_text;
    if (h + first_line < line_count) {
      line_text = m_nick->get_styled_line(h + first_line);
      auto spaces = std::vector<cell>(size.x - line_text.size(), {' ', 0});
      line_text.insert(line_text.end(), spaces.begin(), spaces.end());
    } else {
      line_text = std::vector<cell>(size.x, {' ', 0});
    }
    // b.moveStr(0, line_text, getColor(0x0301));
    for (int i = 0; i < line_text.size(); i++) {
      auto st = line_text[i].style;
      // if (st >= styple_size) st = styple_size - 1;
      b.moveChar(i, line_text[i].character, st, 1);
    }
    writeLine(0, h, line_text.size(), 1, b);
  }

  char ch = m_nick->get_char_at({x, y});
  // std::cout << "pos: " << x << " " << y << std::endl;
  writeChar(x, y - first_line, ch, getColor(0x0100), 1);
  return;
}

// void editor_interior::draw() // modified for scroller
//{
// ushort style[5] = {0x0301, 0x0302, 0x0303, 0x0304, 0x0305};
//// scrollDraw();
////
// auto line_count = m_nick->get_line_count();
//// std::cout << "size: " << size.x << " " << size.y << std::endl;
// setLimit(100, line_count);
// auto [x, y] = m_nick->get_carret_pos();
//// scrollTo(x, y);
// while (y >= delta.y + size.y) {
// delta.y++;
//}
// while (y < delta.y) {
// delta.y--;
//}
// for (int h = 0; h < size.y; h++) {
// TDrawBuffer b;
// std::string line_text;
// if (h + delta.y < line_count) {
// auto v = m_nick->get_styled_line(h + delta.y);
// for (auto c : v) {
// line_text += c.character;
//}
//// line_text = m_nick->get_line(h + delta.y);
////  line_text += std::string('x', 10);
// line_text += std::string(size.x - line_text.size(), '.');

//} else {
// line_text = std::string(size.x, '.');
//}
// b.moveStr(0, line_text, getColor(0x0301));
// writeLine(0, h, line_text.size(), 1, b);
//}

// char ch = m_nick->get_char_at({x, y});
//// std::cout << "pos: " << x << " " << y << std::endl;
// writeChar(x, y - delta.y, ch, getColor(0x0100), 1);
// return;
//}
//

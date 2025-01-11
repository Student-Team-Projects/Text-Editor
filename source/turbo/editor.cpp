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
  m_interior->m_text = text;

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
}

editor_interior::editor_interior(const TRect &bounds, TScrollBar *aHScrollBar,
                                 TScrollBar *aVScrollBar)
    : TScroller(bounds, aHScrollBar, aVScrollBar) {
  growMode = gfGrowHiX | gfGrowHiY;
  options = options | ofFramed;
  m_nick = new NicolasCage();
  m_nick->new_file();
  // setLimit(maxLineLength, maxLines);
}
void editor_interior::handleEvent(TEvent &event) {
  std::cout << "editor_interior::handleEvent" << std::endl;
}

void editor_interior::draw() // modified for scroller
{
  // scrollDraw();
  //
  auto line_count = m_nick->get_line_count();
  // std::cout << "size: " << size.x << " " << size.y << std::endl;
  setLimit(100, line_count);
  auto [x, y] = m_nick->get_carret_pos();
  // scrollTo(x, y);
  while (y >= delta.y + size.y) {
    delta.y++;
  }
  while (y < delta.y) {
    delta.y--;
  }
  for (int h = 0; h < size.y; h++) {
    TDrawBuffer b;
    std::string line_text;
    if (h + delta.y < line_count) {
      line_text = m_nick->get_line(h + delta.y);
      // line_text += std::string('x', 10);
      line_text += std::string(size.x - line_text.size(), '.');

    } else {
      line_text = std::string(size.x, '.');
    }
    b.moveStr(0, line_text, getColor(0x0301));
    writeLine(0, h, line_text.size(), 1, b);
  }

  char ch = m_nick->get_char_at({x, y});
  // std::cout << "pos: " << x << " " << y << std::endl;
  writeChar(x, y - delta.y, ch, getColor(0x0100), 1);
  return;

  m_text.pop_back();
  std::ifstream fileToView(m_text);
  const int maxLines = 100;
  const int maxLineLength = 1000;
  char *lines[maxLines];
  int lineCount = 0;

  if (!fileToView) {
    exit(1);
  } else {
    char buf[maxLineLength];
    while (lineCount < maxLines && fileToView.getline(buf, maxLineLength)) {
      lines[lineCount] = newStr(buf);
      lineCount++;
    }
  }
  // lines[lineCount++] = (char *)m_text.c_str();
  ushort color = getColor(0x0301);
  // char *lines[] = {"AALA", "BBB", 0, 0};
  // std::ifstream file(".gitignore");
  // std::vector<char *> lines;
  // lines.push_back((char *)m_text.c_str());
  // std::string s = file.is_open() ? "file is open" : "file is not open";

  // std::string line;
  // while (getline(file, line)) {
  // lines.push_back((char *)line.c_str());
  // }
  //  lines.push_back((char *)m_text.c_str());

  for (int i = 0; i < lineCount; i++)
  // for each line:
  {
    TDrawBuffer b;
    b.moveChar(0, ' ', color, size.x);
    // fill line buffer with spaces
    int j = delta.y + i; // delta is scroller offset
    if (lines[j]) {
      char s[1000];
      if (delta.x > (int)strlen(lines[j]))
        s[0] = EOS;
      else {
        strncpy(s, lines[j] + delta.x, size.x);
        s[size.x] = EOS;
      }
      b.moveStr(0, s, color);
    }
    writeLine(0, i, size.x, 1, b);
  }

  // auto nick = NicolasCage().act();
  //  auto nick = foo();
  // TDrawBuffer b;
  // b.moveStr(0, nick, color);
  // writeLine(0, lineCount, size.x, 1, b);
}


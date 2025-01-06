#include <algorithm>
#include <cassert>
#include <cstring>
#include <debug.hpp>
#include <efsw/efsw.hpp>
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
#define Uses_IfStreamGetLine

editor::editor(const TRect &bounds, std::string text)
    : TWindowInit(&editor::initFrame), TWindow(bounds, "EDIROT", wnNoNumber) {
  // auto *hsb = standardScrollBar(sbHorizontal);
  // auto *vsb = standardScrollBar(sbVertical);
  // TView::
  // insert("AAA");
  TScrollBar *vScrollBar = standardScrollBar(sbVertical | sbHandleKeyboard);
  TScrollBar *hScrollBar = standardScrollBar(sbHorizontal | sbHandleKeyboard);
  TRect r = getClipRect(); // get exposed view bounds
  r.grow(-1, -1);          // shrink to fit inside window frame
  m_interior = new editor_interior(r, hScrollBar, vScrollBar);
  m_interior->m_text = text;
  insert(m_interior);
}

editor_interior::editor_interior(const TRect &bounds, TScrollBar *aHScrollBar,
                                 TScrollBar *aVScrollBar)
    : TScroller(bounds, aHScrollBar, aVScrollBar) {
  growMode = gfGrowHiX | gfGrowHiY;
  options = options | ofFramed;
  // setLimit(maxLineLength, maxLines);
}

void editor_interior::draw() // modified for scroller
{
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
}


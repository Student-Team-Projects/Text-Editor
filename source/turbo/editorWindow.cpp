#include <cassert>
#include <cstring>
#include <iostream>
#include <turbo/app.hpp>
#include <turbo/editorWindow.hpp>
#include <turbo/explorerWindow.hpp>
#include <tvision/tv.h>
#include <tvision/util.h>

#include <turbo/sciAdapter.hpp>
#define Uses_IfStreamGetLine

editorWindow::editorWindow(const TRect &bounds, const path &filename)
    : TWindowInit(&editorWindow::initFrame), TWindow(bounds, "EDIROT", wnNoNumber) {
  TScrollBar *vScrollBar = standardScrollBar(sbVertical | sbHandleKeyboard);
  TScrollBar *hScrollBar = standardScrollBar(sbHorizontal | sbHandleKeyboard);
  TRect rect = getClipRect(); // get exposed view bounds
  rect.grow(-1, -1);          // shrink to fit inside window frame
  m_interior = new editorView(rect, hScrollBar, vScrollBar);
  m_interior->open_file(filename);

  insert(m_interior);
  m_interior->m_adapter->colorize();
}

void editorWindow::handleEvent(TEvent &event) {
  m_interior->handleEvent(event);
  TWindow::handleEvent(event);
}

editorView::editorView(const TRect &bounds, TScrollBar *aHScrollBar,
                       TScrollBar *aVScrollBar)
    : TScroller(bounds, aHScrollBar, aVScrollBar), m_palette((char *)0, 0),
      m_adapter(new SciAdapter()) {

  growMode = gfGrowHiX | gfGrowHiY;
  options = options | ofFramed;

  // TColorAttr color(27, 25);
  // TColorAttr color2[5] = {3, color, color, color, color};
  // TPalette p(color2);
  // m_palette = p;
}

void editorView::open_file(const path &filename) {
  if (filename.empty()) {
    return;
  }
  m_adapter->load_file(filename);
}

void editorView::handleEvent(TEvent &event) {

  if (event.what == evKeyDown) {
    bool handled = true;

    switch (event.keyDown.keyCode) {
    case kbCtrlV:
      m_adapter->paste();
      break;
    case kbCtrlC:
      m_adapter->copy();
      break;
    case kbCtrlX:
      m_adapter->cut();
      break;
    case kbCtrlZ:
      m_adapter->undo();
      break;
    case kbCtrlY:
      m_adapter->redo();
      break;
    case 7181:
      m_adapter->insert_char('\n');
      break;
    case kbBack:
      m_adapter->delete_char();
      break;
    case kbLeft:
      m_adapter->move_cursor_h(-1);
      break;
    case kbRight:
      m_adapter->move_cursor_h(1);
      break;
    case kbUp:
      m_adapter->move_cursor_v(-1);
      break;
    case kbDown:
      m_adapter->move_cursor_v(1);
      break;
    case kbCtrlLeft:
      m_adapter->move_cursor_h_word(-1);
      break;
    case kbCtrlRight:
      m_adapter->move_cursor_h_word(1);
      break;
    case kbCtrlM:
      m_adapter->toggle_selection_mode();
      break;
    default:
      handled = false;

      if (event.keyDown.getText().size() > 0) {
        handled = true;
      }
      for (auto ch : event.keyDown.getText()) {
        if (isprint(ch)) m_adapter->insert_char(ch);
      }
      break;
    }
    if (handled) {
      clearEvent(event);
    }
  }
  if (event.what == evMouseDown) {
    auto y = m_adapter->get_first_line_idx();
    m_adapter->set_cursor_pos({event.mouse.where.x - 1, y + event.mouse.where.y - 2});
    m_adapter->set_selection_mode(0);
    // clearEvent(event);
  }

  if (event.what == evMouseMove) {
    if (event.mouse.buttons == 1) {
      m_adapter->set_selection_mode(1);
      auto y = m_adapter->get_first_line_idx();
      m_adapter->set_cursor_pos({event.mouse.where.x - 1, y + event.mouse.where.y - 2});
    }
  }

  if (event.what == evMouseWheel) {
    // std::cout << "Wheel: " << (int)event.mouse.wheel << std::endl;
    m_adapter->scroll(event.mouse.wheel == 1 ? -1 : 1);
  } else {
  }


  // if (m_adapter->get_cursor_pos()[1] > size.y) {
  // delta.y++;
  //// m_adapter->handleEvent(event);
  //}

  // m_adapter->colorize();
  scroll_to_cursor();
  draw();
  // for (int line : m_adapter->get_changed_lines()) {
  // draw_line(line);
  //}
}

void editorView::scroll_to_cursor() {
  auto [x, y] = m_adapter->get_cursor_pos();
  int first_line = m_adapter->get_first_line_idx();
  if (y < first_line) {
    m_adapter->set_first_line_idx(y);
  }
  if (y > first_line + size.y - 1) {
    m_adapter->set_first_line_idx(y - size.y + 1);
  }
}

bool cell_selecetd(std::array<std::array<int, 2>, 2> sel, int x, int y) {
  if (sel[0][1] == sel[1][1]) {
    if (y == sel[0][1] && x >= sel[0][0] && x < sel[1][0]) {
      return true;
    }
  } else {
    if (y == sel[0][1] && x >= sel[0][0]) {
      return true;
    }
    if (y == sel[1][1] && x < sel[1][0]) {
      return true;
    }
    if (y > sel[0][1] && y < sel[1][1]) {
      return true;
    }
  }
  return false;
}

void editorView::draw_line(int line) {
  // std::cout << "line: " << line << std::endl;
// The colors of the background in rgb
#define COL_NORMAL 0xe0'e0'e0
#define COL_CURRENT_LINE 0xe0'f0'ff
#define COL_SELECTION 0xff'ff'ff

  auto line_count = m_adapter->get_line_count();
  auto [x, y] = m_adapter->get_cursor_pos();
  int first_line = m_adapter->get_first_line_idx();

  auto [sel_start, sel_end] = m_adapter->selection_points();

  int pos = 0;
  // drawing lines
  int y_pos = line;
  std::vector<cell> line_text;

  if (y_pos + first_line < line_count) {
    line_text = m_adapter->get_styled_line(y_pos + first_line);
    int rest_line_len = max(0, size.x - line_text.size());
    auto spaces = std::vector<cell>(rest_line_len, {' ', 0});
    line_text.insert(line_text.end(), spaces.begin(), spaces.end());
  } else {
    line_text = std::vector<cell>(size.x, {' ', 0});
  }

  TDrawBuffer buf;
  int line_start_pos = 0;
  for (int i = 0; i < line_text.size(); i++) {
    auto style = line_text[i].style;
    // std::cout << "Style: " << (int)style;
    //   auto tcolor = style;
    // auto tcolor = m_adapter->style_to_color(style);
    TColorDesired fg = style;
    TColorDesired bg = y == y_pos + first_line ? COL_CURRENT_LINE : COL_NORMAL;
    if (cell_selecetd({sel_start, sel_end}, i, y_pos + first_line)) {
      bg = COL_SELECTION;
    }
    // if (sel_start[1] == sel_end[1] && y_pos == sel_start[1]) {
    // if (i >= sel_start[0] && i < sel_end[0]) bg = COL_SELECTION;
    //} else {
    // if (y_pos == sel_start[1] && i >= sel_start[0]) {
    // bg = COL_SELECTION;
    //}
    // if (y_pos == sel_end[1] && i < sel_end[0]) {
    // bg = COL_SELECTION;
    //}
    // if (y_pos > sel_start[1] && y_pos < sel_end[1]) {
    // bg = COL_SELECTION;
    //}
    //}
    // if (pos >= sel_start && pos < sel_end) {
    // bg = COL_SELECTION;
    //}
    TColorAttr tcolor(fg, bg);
    buf.moveChar(i, line_text[i].character, tcolor, 1);
    pos++;
  }

  writeLine(0, y_pos, line_text.size(), 1, buf);

  // drawing cursor
  char cursor_char = m_adapter->get_char_at({x, y});
  if (!isprint(cursor_char)) cursor_char = ' ';
  writeChar(x, y - first_line, cursor_char, getColor(0x0100), 1);
}


void editorView::draw() {

  for (int y_pos = 0; y_pos < size.y; y_pos++) {
    draw_line(y_pos);
  }
  return;

// The colors of the background in rgb
#define COL_NORMAL 0xe0'e0'e0
#define COL_CURRENT_LINE 0xe0'f0'ff
#define COL_SELECTION 0xff'ff'ff

  auto line_count = m_adapter->get_line_count();
  auto [x, y] = m_adapter->get_cursor_pos();
  int first_line = m_adapter->get_first_line_idx();

  auto [sel_start, sel_end] = m_adapter->selection_points();

  int pos = 0;
  // drawing lines
  for (short y_pos = 0; y_pos < size.y; y_pos++) {
    std::vector<cell> line_text;

    if (y_pos + first_line < line_count) {
      line_text = m_adapter->get_styled_line(y_pos + first_line);
      int rest_line_len = max(0, size.x - line_text.size());
      auto spaces = std::vector<cell>(rest_line_len, {' ', 0});
      line_text.insert(line_text.end(), spaces.begin(), spaces.end());
    } else {
      line_text = std::vector<cell>(size.x, {' ', 0});
    }

    TDrawBuffer buf;
    int line_start_pos = 0;
    for (int i = 0; i < line_text.size(); i++) {
      auto style = line_text[i].style;
      // std::cout << "Style: " << (int)style;
      //   auto tcolor = style;
      // auto tcolor = m_adapter->style_to_color(style);
      TColorDesired fg = style;
      TColorDesired bg = y == y_pos + first_line ? COL_CURRENT_LINE : COL_NORMAL;
      if (sel_start[1] == sel_end[1] && y_pos == sel_start[1]) {
        if (i >= sel_start[0] && i < sel_end[0]) bg = COL_SELECTION;
      } else {
        if (y_pos == sel_start[1] && i >= sel_start[0]) {
          bg = COL_SELECTION;
        }
        if (y_pos == sel_end[1] && i < sel_end[0]) {
          bg = COL_SELECTION;
        }
        if (y_pos > sel_start[1] && y_pos < sel_end[1]) {
          bg = COL_SELECTION;
        }
      }
      // if (pos >= sel_start && pos < sel_end) {
      // bg = COL_SELECTION;
      //}
      TColorAttr tcolor(fg, bg);
      buf.moveChar(i, line_text[i].character, tcolor, 1);
      pos++;
    }

    writeLine(0, y_pos, line_text.size(), 1, buf);
  }

  // drawing cursor
  char cursor_char = m_adapter->get_char_at({x, y});
  if (!isprint(cursor_char)) cursor_char = ' ';
  writeChar(x, y - first_line, cursor_char, getColor(0x0100), 1);
}


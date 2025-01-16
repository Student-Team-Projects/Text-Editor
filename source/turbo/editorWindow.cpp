#include <cassert>
#include <cstring>
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
    m_adapter->set_cursor_pos({event.mouse.where.x - 1, event.mouse.where.y - 2});
    clearEvent(event);
  }

  // if (m_adapter->get_cursor_pos()[1] > size.y) {
  // delta.y++;
  //// m_adapter->handleEvent(event);
  //}

  m_adapter->colorize();
  draw();
}

void editorView::draw() // modified for scroller
{
  auto line_count = m_adapter->get_line_count();
  auto [x, y] = m_adapter->get_cursor_pos();
  int first_line = m_adapter->get_first_line_idx();

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

    for (int i = 0; i < line_text.size(); i++) {
      auto style = line_text[i].style;
      buf.moveChar(i, line_text[i].character, style, 1);
    }

    writeLine(0, y_pos, line_text.size(), 1, buf);
  }

  // drawing cursor
  char cursor_char = m_adapter->get_char_at({x, y});
  if (!isprint(cursor_char)) cursor_char = ' ';
  writeChar(x, y - first_line, cursor_char, getColor(0x0100), 1);
}


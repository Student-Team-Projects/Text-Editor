#include "Scintilla.h"
#include "ScintillaMessages.h"
#include "ScintillaTypes.h"
#include <algorithm>
#include <cassert>
#include <cstring>
#include <debug.hpp>
#include <efsw/efsw.hpp>
#include <filesystem>
#include <fstream>
#include <functional>
#include <string>
#include <string_view>
#include <turbo/app.hpp>
#include <turbo/editor.hpp>
#include <turbo/explorerWindow.hpp>
#include <tvision/tv.h>
#include <tvision/util.h>
#include <vector>
#define Uses_IfStreamGetLine

#include <ScintillaHeaders.h>
#define let auto
// using namespace Scintilla::Internal;
using PRectangle = Scintilla::Internal::PRectangle;
using SelectionText = Scintilla::Internal::SelectionText;
namespace Scintilla {
class NicolasCage : public Internal::ScintillaBase {
  void CreateCallTipWindow(PRectangle rc) override {}
  void AddToPopUp(const char *label, int cmd = 0, bool enabled = true) override {}
  void SetVerticalScrollPos() override {}
  void SetHorizontalScrollPos() override {}
  bool ModifyScrollBars(Sci::Line nMax, Sci::Line nPage) override { return false; }
  void Copy() override {}
  void Paste() override {}
  void ClaimSelection() override {}
  void NotifyChange() override {}
  void NotifyParent(Scintilla::NotificationData scn) override {}
  void CopyToClipboard(const SelectionText &selectedText) override {}
  void SetMouseCapture(bool on) override {}
  bool HaveMouseCapture() override { return false; }

  protected:
  std::string UTF8FromEncoded(std::string_view encoded) override;
  // virtual std::string EncodedFromUTF8(std::string_view utf8) const = 0;

  Scintilla::sptr_t DefWndProc(Scintilla::Message iMessage, uptr_t wParam, sptr_t lParam) {}

  public:
  std::string act() {
    char *text =
        "Siokola siokola\nYou make me feeeel\nSioooookola siokola\nYou make me feeeel\n";
    auto m = static_cast<Scintilla::Message>(SCI_SETTEXT);
    WndProc(m, 0, (sptr_t)text);
    let buffer = new char[1000];

    auto m2 = static_cast<Scintilla::Message>(SCI_GETLINE);
    let line_num = 0;
    WndProc(m2, line_num, (sptr_t)buffer);
    return buffer;
  }
};
} // namespace Scintilla

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

  // let nick = NicolasCage();
  let line = new Scintilla::NicolasCage();
  // writeLine(0, lineCount, size.x, size.y - lineCount, b);
}


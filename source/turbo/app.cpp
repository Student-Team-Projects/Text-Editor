#include <algorithm>
#include <filesystem>
#include <optional>
#include <turbo/app.hpp>
#include <turbo/clockView.hpp>
#include <turbo/explorerWindow.hpp>
#include <tvision/tv.h>

t_hello_app::t_hello_app(int argc, char **argv)
    : TProgInit(&t_hello_app::initStatusLine, &t_hello_app::initMenuBar,
                &t_hello_app::initDeskTop) {
  {
    TRect rect = getExtent();
    rect.a.x = rect.b.x - t_clock_view::time_size;
    rect.b.y = rect.a.y + 1;
    m_clock = new t_clock_view(rect);
    m_clock->growMode = gfGrowLoX | gfGrowHiX;
    insert(m_clock);
  }
  {
    TRect rect = deskTop->getExtent();
    if (rect.b.x > 22) {
      rect.a.x = rect.b.x - std::min(std::max(rect.b.x - 82, 22), 30);
      rect.b.x = rect.b.x - rect.a.x;
      rect.a.x = 0;
    }
    auto current_path = std::filesystem::current_path().string() + '/';

    m_explorer = new t_explorer_window(rect, current_path);
    m_explorer->flags &= ~wfZoom;
    m_explorer->growMode = gfGrowLoX | gfGrowHiX | gfGrowHiY;
    m_explorer->setState(sfShadow, /*enable=*/false);
    deskTop->insert(m_explorer);
    if (deskTop->size.x - m_explorer->size.x < 82) {
      m_explorer->hide();
    }
  }
  {
    TCommandSet ts;
    ts.enableCmd(cmSave);
    ts.enableCmd(cmSaveAs);
    ts.enableCmd(cmCut);
    ts.enableCmd(cmCopy);
    ts.enableCmd(cmPaste);
    ts.enableCmd(cmClear);
    ts.enableCmd(cmUndo);
    ts.enableCmd(cmFind);
    ts.enableCmd(cmReplace);
    ts.enableCmd(cmSearchAgain);
    disableCommands(ts);
    cascade();
  }
  {
    while (--argc) {
      newEditor(std::optional<char *>(*++argv));
    }
  }
}

auto t_hello_app::createFindDialog() -> TDialog * {
  auto *d = new TDialog(TRect(0, 0, 38, 12), "Find");

  d->options |= ofCentered;

  auto *control = new TInputLine(TRect(3, 3, 32, 4), 80);
  d->insert(control);
  d->insert(new TLabel(TRect(2, 2, 15, 3), "~T~ext to find", control));
  d->insert(new THistory(TRect(32, 3, 35, 4), control, 10));

  d->insert(
      new TCheckBoxes(TRect(3, 5, 35, 7),
                      new TSItem("~C~ase sensitive", new TSItem("~W~hole words only", 0))));

  d->insert(new TButton(TRect(14, 9, 24, 11), "O~K~", cmOK, bfDefault));
  d->insert(new TButton(TRect(26, 9, 36, 11), "Cancel", cmCancel, bfNormal));

  d->selectNext(False);
  return d;
}

auto t_hello_app::createReplaceDialog() -> TDialog * {
  auto *d = new TDialog(TRect(0, 0, 40, 16), "Replace");

  d->options |= ofCentered;

  auto *control = new TInputLine(TRect(3, 3, 34, 4), 80);
  d->insert(control);
  d->insert(new TLabel(TRect(2, 2, 15, 3), "~T~ext to find", control));
  d->insert(new THistory(TRect(34, 3, 37, 4), control, 10));

  control = new TInputLine(TRect(3, 6, 34, 7), 80);
  d->insert(control);
  d->insert(new TLabel(TRect(2, 5, 12, 6), "~N~ew text", control));
  d->insert(new THistory(TRect(34, 6, 37, 7), control, 11));

  d->insert(new TCheckBoxes(
      TRect(3, 8, 37, 12),
      new TSItem(
          "~C~ase sensitive",
          new TSItem("~W~hole words only",
                     new TSItem("~P~rompt on replace", new TSItem("~R~eplace all", 0))))));

  d->insert(new TButton(TRect(17, 13, 27, 15), "O~K~", cmOK, bfDefault));
  d->insert(new TButton(TRect(28, 13, 38, 15), "Cancel", cmCancel, bfNormal));

  d->selectNext(False);

  return d;
}

auto t_hello_app::newEditor(std::optional<char *> path) -> void {
  auto r = deskTop->getExtent();
  r.a.x += (m_explorer->visible() ? m_explorer->size.x : 0);
  auto *editor = new TEditWindow(r, path.value_or(nullptr), wnNoNumber);
  deskTop->insert(editor);
}

auto t_hello_app::fileOpen() -> void {
  char fileName[MAXPATH];
  strcpy(fileName, "*.*");
  auto *dialog = new TFileDialog("*.*", "Open file", "~N~ame", fdOpenButton, 100);

  auto *p = validView(dialog);
  p->setData(fileName);
  if (deskTop->execView(p) != cmCancel) {
    p->getData(fileName);
  }
  destroy(p);

  newEditor(&fileName[0]);
}

auto t_hello_app::fileNew() -> void { newEditor(std::nullopt); }

auto t_hello_app::chdir() -> void {
  auto *dialog = new TChDirDialog(cdNormal, 0);
  if (deskTop->execView(dialog) != cmCancel) {
    auto current_path = std::filesystem::current_path().string() + '/';
    deskTop->remove(m_explorer);

    m_explorer = new t_explorer_window(m_explorer->getBounds(), current_path);
    m_explorer->flags &= ~wfZoom;
    m_explorer->growMode = gfGrowLoX | gfGrowHiX | gfGrowHiY;
    m_explorer->setState(sfShadow, /*enable=*/false);
    deskTop->insert(m_explorer);
    if (deskTop->size.x - m_explorer->size.x < 82) {
      m_explorer->hide();
    }
  }
}

auto t_hello_app::handleEvent(TEvent &event) -> void {
  TApplication::handleEvent(event);
  if (event.what == evCommand) {
    switch (event.message.command) {
    case cmOpen:
      fileOpen();
      clearEvent(event);
      break;
    case cmNew:
      fileNew();
      clearEvent(event);
      break;
    case cm_chdir:
      chdir();
      clearEvent(event);
      break;
    case cm_toggle_tree:
      m_explorer->toggle_tree();
      clearEvent(event);
      break;
    default:
      break;
    }
  }
}

auto t_hello_app::initMenuBar(TRect rect) -> TMenuBar * {
  TSubMenu &sub1 = *new TSubMenu("~F~ile", kbAltF) +
                   *new TMenuItem("~O~pen", cmOpen, kbF3, hcNoContext, "F3") +
                   *new TMenuItem("~N~ew", cmNew, kbCtrlN, hcNoContext, "Ctrl-N") +
                   *new TMenuItem("~S~ave", cmSave, kbF2, hcNoContext, "F2") +
                   *new TMenuItem("S~a~ve as...", cmSaveAs, kbNoKey) + newLine() +
                   *new TMenuItem("~C~hange dir...", cm_chdir, kbNoKey) +
                   *new TMenuItem("~D~OS shell", cmDosShell, kbNoKey) +
                   *new TMenuItem("E~x~it", cmQuit, kbCtrlQ, hcNoContext, "Ctrl-Q");

  TSubMenu &sub2 =
      *new TSubMenu("~E~dit", kbAltE) +
      *new TMenuItem("~U~ndo", cmUndo, kbCtrlU, hcNoContext, "Ctrl-U") + newLine() +
      *new TMenuItem("Cu~t~", cmCut, kbShiftDel, hcNoContext, "Shift-Del") +
      *new TMenuItem("~C~opy", cmCopy, kbCtrlIns, hcNoContext, "Ctrl-Ins") +
      *new TMenuItem("~P~aste", cmPaste, kbShiftIns, hcNoContext, "Shift-Ins") + newLine() +
      *new TMenuItem("~C~lear", cmClear, kbCtrlDel, hcNoContext, "Ctrl-Del");

  TSubMenu &sub3 = *new TSubMenu("~S~earch", kbAltS) +
                   *new TMenuItem("~F~ind...", cmFind, kbNoKey) +
                   *new TMenuItem("~R~eplace...", cmReplace, kbNoKey) +
                   *new TMenuItem("~S~earch again", cmSearchAgain, kbNoKey);

  TSubMenu &sub4 =
      *new TSubMenu("~W~indows", kbAltW) +
      *new TMenuItem("~S~ize/move", cmResize, kbCtrlF5, hcNoContext, "Ctrl-F5") +
      *new TMenuItem("~Z~oom", cmZoom, kbF5, hcNoContext, "F5") +
      *new TMenuItem("~T~ile", cmTile, kbNoKey) +
      *new TMenuItem("C~a~scade", cmCascade, kbNoKey) +
      *new TMenuItem("~N~ext", cmNext, kbF6, hcNoContext, "F6") +
      *new TMenuItem("~P~revious", cmPrev, kbShiftF6, hcNoContext, "Shift-F6") +
      *new TMenuItem("~C~lose", cmClose, kbCtrlW, hcNoContext, "Ctrl+W");

  rect.b.y = rect.a.y + 1;
  return new TMenuBar(rect, sub1 + sub2 + sub3 + sub4);
}

auto t_hello_app::initStatusLine(TRect rect) -> TStatusLine * {
  rect.a.y = rect.b.y - 1;
  return new TStatusLine(
      rect, *new TStatusDef(0, 0xFFFF) + *new TStatusItem(0, kbAltX, cmQuit) +
                *new TStatusItem("~F2~ Save", kbF2, cmSave) +
                *new TStatusItem("~F3~ Open", kbF3, cmOpen) +
                *new TStatusItem("~Ctrl-W~ Close", kbAltF3, cmClose) +
                *new TStatusItem("~F5~ Zoom", kbF5, cmZoom) +
                *new TStatusItem("~F6~ Next", kbF6, cmNext) +
                *new TStatusItem("~F9~ Toggle file tree", kbF9, cm_toggle_tree) +
                *new TStatusItem("~F10~ Menu", kbF10, cmMenu) +
                *new TStatusItem(0, kbShiftDel, cmCut) +
                *new TStatusItem(0, kbCtrlIns, cmCopy) +
                *new TStatusItem(0, kbShiftIns, cmPaste) +
                *new TStatusItem(0, kbCtrlF5, cmResize));
}

auto t_hello_app::idle() -> void {
  TApplication::idle();
  if (m_clock != nullptr) {
    m_clock->update();
  }
}

t_hello_app *t_hello_app::app = nullptr;
auto main(int argc, char **argv) -> int {
  t_hello_app hello_world(argc, argv);
  t_hello_app::app = &hello_world;
  hello_world.run();
  t_hello_app::app = nullptr;
  return 0;
}
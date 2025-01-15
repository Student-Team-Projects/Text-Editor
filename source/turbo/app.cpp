#include <algorithm>
#include <filesystem>
#include <iostream>
#include <semaphore>
#include <turbo/app.hpp>
#include <turbo/clockView.hpp>
#include <turbo/explorerWindow.hpp>
#include <tvision/tv.h>

t_hello_app::t_hello_app()
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
}

void t_hello_app::greeting_box() {
  auto *dialog = new TDialog(TRect(25, 5, 55, 16), "Hello, World!");

  dialog->insert(new TStaticText(TRect(3, 5, 15, 6), "How are you?"));
  dialog->insert(new TButton(TRect(16, 2, 28, 4), "Terrific", cmCancel, bfNormal));
  dialog->insert(new TButton(TRect(16, 4, 28, 6), "Ok", cmCancel, bfNormal));
  dialog->insert(new TButton(TRect(16, 6, 28, 8), "Lousy", cmCancel, bfNormal));
  dialog->insert(new TButton(TRect(16, 8, 28, 10), "Cancel", cmCancel, bfNormal));

  deskTop->execView(dialog);
  destroy(dialog);
}

auto t_hello_app::handleEvent(TEvent &event) -> void {
  TApplication::handleEvent(event);
  if (event.what == evCommand) {
    switch (event.message.command) {
    case greet_them_cmd:
      greeting_box();
      clearEvent(event);
      break;
    case cm_toggle_tree:
      m_explorer->toggle_tree();
      clearEvent(event);
      break;
    default:
      break;
    case open_file_in_editor:
      auto file = m_explorer->get_selesced_path();
      if (file.back() == '/') {
        file.pop_back();
      }
      // std::cout << "open_file_in_editor: " << file << std::endl;
      m_editor = new editor(deskTop->getExtent(), file);
      deskTop->insert(m_editor);
      break;
    }
  }
}

auto t_hello_app::initMenuBar(TRect rect) -> TMenuBar * {
  rect.b.y = rect.a.y + 1;
  return new TMenuBar(
      rect, *new TSubMenu("~H~ello", kbAltH) +
                *new TMenuItem("~G~reeting...", greet_them_cmd, kbAltG) + newLine() +
                *new TMenuItem("~T~oggle tree", cm_toggle_tree, kbF9, hcNoContext, "F9") +
                *new TMenuItem("Open in editor", open_file_in_editor, 0, hcNoContext,
                               "~nicolas cage~") +
                *new TMenuItem("E~x~it", cmQuit, cmQuit, hcNoContext, "Alt-X"));
}

auto t_hello_app::initStatusLine(TRect rect) -> TStatusLine * {
  rect.a.y = rect.b.y - 1;
  return new TStatusLine(
      rect, *new TStatusDef(0, 0xFFFF) + *new TStatusItem("~Alt-X~ Exit", kbAltX, cmQuit) +
                *new TStatusItem("~Ctrl-V~ Paste", 0, 0) +
                *new TStatusItem("~Ctrl-C~ Copy line", 0, 0) +
                *new TStatusItem("~Ctrl-X~ Cut line", 0, 0) +
                *new TStatusItem("~Ctrl-Z~ Undo", 0, 0) +
                *new TStatusItem("~Ctrl-Y~ Redo", 0, 0) +
                *new TStatusItem("~Ctrl-Left, Ctrl-Right~ Word jumps", 0, 0) +
                *new TStatusItem(nullptr, kbF10, cmMenu));
}

auto t_hello_app::idle() -> void {
  TApplication::idle();
  if (m_clock != nullptr) {
    m_clock->update();
  }
}

t_hello_app *t_hello_app::app = nullptr;
auto main(int /*argc*/, char ** /*argv*/) -> int {
  t_hello_app hello_world;
  t_hello_app::app = &hello_world;
  hello_world.run();
  t_hello_app::app = nullptr;
  return 0;
}

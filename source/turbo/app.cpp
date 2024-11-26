#include "app.hpp"
#include "some_shared_include.hpp"

t_hello_app::t_hello_app()
    : TProgInit(&t_hello_app::initStatusLine, &t_hello_app::initMenuBar,
                &t_hello_app::initDeskTop) {
  TRect rect = getExtent();
  rect.a.x = rect.b.x - t_clock_view::time_size;
  rect.b.y = rect.a.y + 1;
  m_clock = new t_clock_view(rect);
  m_clock->growMode = gfGrowLoX | gfGrowHiX;
  insert(m_clock);
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

void t_hello_app::handleEvent(TEvent &event) {
  TApplication::handleEvent(event);
  if (event.what == evCommand) {
    switch (event.message.command) {
    case greet_them_cmd:
      greeting_box();
      clearEvent(event);
      break;
    default:
      break;
    }
  }
}

auto t_hello_app::initMenuBar(TRect rect) -> TMenuBar * {
  rect.b.y = rect.a.y + 1;
  return new TMenuBar(
      rect, *new TSubMenu("~H~ello", kbAltH) +
                *new TMenuItem("~G~reeting...", greet_them_cmd, kbAltG) + newLine() +
                *new TMenuItem("E~x~it", cmQuit, cmQuit, hcNoContext, "Alt-X"));
}

auto t_hello_app::initStatusLine(TRect rect) -> TStatusLine * {
  rect.a.y = rect.b.y - 1;
  return new TStatusLine(rect, *new TStatusDef(0, 0xFFFF) +
                                   *new TStatusItem("~Alt-X~ Exit", kbAltX, cmQuit) +
                                   *new TStatusItem(0, kbF10, cmMenu));
}

auto t_hello_app::idle() -> void {
  TApplication::idle();
  if (m_clock != nullptr) {
    m_clock->update();
  }
}

auto main() -> int {
  t_hello_app hello_world;
  hello_world.run();
  return 0;
}
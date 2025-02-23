#include <turbo-core/timeUtils.hpp>
#include <turbo/dialogs.hpp>
#include <turbo/editorWindow.hpp>

EditWindow::EditWindow(const TRect &rect, TStringView view, int val) noexcept
    : TWindowInit(&TEditWindow::initFrame), TEditWindow(rect, view, val) {

  this->open_time = timeUtils::getSystemTime();
}

void EditWindow::handleEvent(TEvent &event) {

  auto p = editor->fileName;

  if (event.what == evCommand) {

    switch (event.message.command) {
    case cmSave:
      if (timeUtils::getFileLastModifTime(p) > open_time) {
        auto result = dialogs::spawnDialog(edSaveModify, p);
        if (result == cmYes) {
          TEditWindow::handleEvent(event);
          this->open_time = timeUtils::getFileLastModifTime(p);
        }
      } else {
        TEditWindow::handleEvent(event);
        this->open_time = timeUtils::getFileLastModifTime(p);
      }
      clearEvent(event);
      return;
    }
  }
  TEditWindow::handleEvent(event);
}


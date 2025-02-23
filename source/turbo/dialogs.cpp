
// Copyright (c) 2025 Piotr Białek
// Copyright (c) 2025 Mateusz Rajs
// Copyright (c) 2025 Mikołaj Rams
// Copyright (c) 2025 Antoni Długosz
//
// Licensed under the MIT license

#include <strstream>

#include <turbo/dialogs.hpp>
#include <tvision/tv.h>

#include <turbo/app.hpp>

ushort dialogs::execDialog(TDialog *d, void *data) {
  TView *p = TProgram::application->validView(d);
  if (p == 0)
    return cmCancel;
  else {
    if (data != 0) p->setData(data);
    ushort result = TProgram::deskTop->execView(p);
    if (result != cmCancel && data != 0) p->getData(data);
    TObject::destroy(p);
    return result;
  }
}

auto dialogs::spawnDialog(int dialog, ...) -> ushort {
  va_list arg;
  char buf[256] = {0};
  std::ostrstream os(buf, sizeof(buf) - 1);

  switch (dialog) {
  case edOutOfMemory:
    return messageBox("Not enough memory for this operation", mfError | mfOKButton);
  case edReadError: {
    va_start(arg, dialog);
    os << "Error reading file " << va_arg(arg, char *) << "." << std::ends;
    va_end(arg);
    return messageBox(buf, mfError | mfOKButton);
  }
  case edWriteError: {
    va_start(arg, dialog);
    os << "Error writing file " << va_arg(arg, char *) << "." << std::ends;
    va_end(arg);
    return messageBox(buf, mfError | mfOKButton);
  }
  case edCreateError: {
    va_start(arg, dialog);
    os << "Error creating file " << va_arg(arg, char *) << "." << std::ends;
    va_end(arg);
    return messageBox(buf, mfError | mfOKButton);
  }
  case edSaveModify: {
    va_start(arg, dialog);
    os << va_arg(arg, char *) << " has been modified. Save?" << std::ends;
    va_end(arg);
    return messageBox(buf, mfInformation | mfYesNoCancel);
  }
  case edSaveUntitled:
    return messageBox("Save untitled file?", mfInformation | mfYesNoCancel);
  case edSaveAs: {
    va_start(arg, dialog);
    return execDialog(new TFileDialog("*.*", "Save file as", "~N~ame", fdOKButton, 101),
                      va_arg(arg, char *));
  }
  case edFind: {
    va_start(arg, dialog);
    return execDialog(App::createFindDialog(), va_arg(arg, char *));
  }
  case edSearchFailed:
    return messageBox("Search string not found.", mfError | mfOKButton);
  case edReplace: {
    va_start(arg, dialog);
    return execDialog(App::createReplaceDialog(), va_arg(arg, char *));
  }
  case edReplacePrompt:
    //  Avoid placing the dialog on the same line as the cursor
    TRect r(0, 1, 40, 8);
    r.move((TProgram::deskTop->size.x - r.b.x) / 2, 0);
    auto t = TProgram::deskTop->makeGlobal(r.b);
    t.y++;
    va_start(arg, dialog);
    auto *pt = va_arg(arg, TPoint *);
    if (pt->y <= t.y) r.move(0, TProgram::deskTop->size.y - r.b.y - 2);
    va_end(arg);
    return messageBoxRect(r, "Replace this occurence?", mfYesNoCancel | mfInformation);
  }
  return cmCancel;
}


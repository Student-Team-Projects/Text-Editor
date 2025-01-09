
#include "scintilla/include/Scintilla.h"
#include "scintilla/internals.h"
#include <type_traits>

namespace Scintilla {

class NicolasCage : public ScintillaBase {
    public:
        std::string act();
    void SetVerticalScrollPos() override {}
    void SetHorizontalScrollPos() override{}
     bool ModifyScrollBars(Sci::Line nMax, Sci::Line nPage) override{}
     void Copy() override{}
     void Paste() override{}
     void ClaimSelection() override{}
     void NotifyChange() override{}
     void NotifyParent(SCNotification scn) override{}
      void CopyToClipboard(const SelectionText &selectedText) override{}
      void SetMouseCapture(bool on)  override{}
      bool HaveMouseCapture() override{}
      sptr_t DefWndProc(unsigned int iMessage, uptr_t wParam, sptr_t lParam) override{}
      void CreateCallTipWindow(PRectangle rc) override{}
      void AddToPopUp(const char *label, int cmd=0, bool enabled=true) override{}
      /// below ade not needed to compile but are needed to link WTF
     void  NotifyModifyAttempt(Scintilla::Document*, void*) override{}
     void NotifySavePoint(Scintilla::Document*, void*, bool) override{} 
     void NotifyModified(Scintilla::Document*, Scintilla::DocModification, void*) override{}
     void NotifyDeleted(Scintilla::Document*, void*)  noexcept override{}
     void NotifyStyleNeeded(Scintilla::Document*, void*, long) override{}
     void NotifyLexerChanged(Scintilla::Document*, void*) override{}
     void NotifyErrorOccurred(Scintilla::Document*, void*, int) override{}
     void ListNotify(Scintilla::ListBoxEvent*) override{}
};

}

using NicolasCage = Scintilla::NicolasCage;

std::string foo();

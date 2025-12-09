#define Uses_TDrawSurface
#include <text-editor/app.hpp>
#include <text-editor/ScintillaView.hpp>
#include <tvision/tv.h>
#include <tvision/views.h>
#include <scintilla/tscintilla.h>
#include <scintilla.h>

//experimental class. Tried to figure out how things work - Kacper

struct ScintillaEditWindow : TWindow, turbo::TScintillaParent{
    ScintillaEditWindow(const TRect& bounds, TStringView aTitle, short aNumber) noexcept;

    void handleEvent(TEvent &event) override;
    // virtual TPalette& getPalette() const;

    TPoint getEditorSize() noexcept;
    void invalidate(TRect area) noexcept;
    void handleNotification(const SCNotification &scn);
    void setVerticalScrollPos(int delta, int limit) noexcept;
    void setHorizontalScrollPos(int delta, int limit) noexcept;

    public:
    time_t open_time;
    int x = 0;
    int y = 0;
    int a = 10;
    int b = 10;
    ushort color = 0;
    TDrawSurface *surface;
    ScintillaView *view;
    turbo::TScintilla &scintilla;
    static TPalette myPalette;
};
#pragma once
#define Uses_TSurfaceView
#include <text-editor/app.hpp>
#include <tvision/tv.h>
#include <scintilla.h>
#include <text-editor/editor.hpp>

struct EditorView : TSurfaceView, EditorObserver{

public:
    EditorView(const TRect& bounds) noexcept;
    void resize(const TRect& bounds) noexcept;
    void paint();
    void handleEvent(TEvent& event) override;
    void attachEditor(Editor* newEditor);
    virtual void editorUpdate();

private:    
    TDrawSurface drawSurface;//maybe this should be moved to editor?
    Editor* editor = nullptr; //non-owning
};
#include <text-editor/editorView.hpp>

EditorView::EditorView(const TRect& bounds) noexcept : TSurfaceView(bounds){
    surface = &drawSurface;
    drawSurface.resize(bounds.b - bounds.a);
    options |= ofSelectable;
    eventMask |= evMouseMove | evMouseWheel;
    showCursor();
}

void EditorView::resize(const TRect& bounds) noexcept{
    drawSurface.resize(bounds.b - bounds.a);
    if(editor){
        editor->setSize(bounds.b - bounds.a);
    }
    TSurfaceView::setBounds(bounds);
    paint();
    draw();
}

void EditorView::paint(){
    if(editor){
        cursor = editor->pointMainCaret() - delta;
        showCursor();
        editor->paint(drawSurface, TRect({0, 0}, size));
    }
}

void EditorView::attachEditor(Editor* newEditor){
    if(editor){
        editor->removeObserver(this);
    }
    editor = newEditor;
    editor->setSize(size);
    editor->addObserver(this);
    paint();
    draw();
}

void EditorView::handleEvent(TEvent& event){
    if(event.what & evMouse){
        TPoint local = makeLocal(event.mouse.where);
        TEvent localEvent = event;
        localEvent.mouse.where = local;
        if(editor) editor->handleEvent(localEvent);
    }else if(event.what == evKeyboard || event.what == evCommand){
        if(editor) editor->handleEvent(event);
    }
    TSurfaceView::handleEvent(event);
}

void EditorView::editorUpdate(){
    TCommandSet s;
    s.enableCmd(cmSave);
    enableCommands(s);
    paint();
    draw();
}